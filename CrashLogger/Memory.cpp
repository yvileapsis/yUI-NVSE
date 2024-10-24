#include <CrashLogger.hpp>
#include <MemoryManager.hpp>
#include <MemoryPool.hpp>
#include <psapi.h>
#include <ZeroOverheadHeap.hpp>

#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

#define PRINT_HEAPS 1
#define PRINT_POOLS 0

namespace CrashLogger::Memory
{
	enum MemoryErrors {
		NONE			= 0,
		HIGH_USAGE		= 1,
		CRITICAL_USAGE	= 2,
		EXTERNAL_LEAK	= 3,
		TEXTURE_SHARING = 4,
	};

	std::stringstream output;

	char nvtfError[260] = {};

	bool HandleNVTF() {
		char iniDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, iniDir);
		strcat_s(iniDir, "\\Data\\NVSE\\Plugins\\NVTF.ini");

		bool usesDefaultPool = true;

		if (GetFileAttributes(iniDir) != INVALID_FILE_ATTRIBUTES) {
			bool hasDXSettings = GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniDir);
			bool hasDefaultPool = GetPrivateProfileInt("DirectX", "bUseDefaultPoolForTextures", 0, iniDir);
			static const char* string = "WARNING: NVTF.ini has bModifyDirectXBehavior=%d %s bUseDefaultPoolForTextures=%d! This will cause high memory usage!\n         See https://performance.moddinglinked.com/falloutnv.html#NVTF on how to resolve this issue.";
			
			if ((hasDXSettings && !hasDefaultPool) || (!hasDXSettings && hasDefaultPool)) {
				sprintf_s(nvtfError, string, hasDXSettings, "but", hasDefaultPool);

				usesDefaultPool = false;
			}
			else if (!hasDXSettings && !hasDefaultPool) {
				sprintf_s(nvtfError, string, hasDXSettings, "and", hasDefaultPool);

				usesDefaultPool = false;
			}
		}
		else if (!GetModuleHandle("nvtf.dll")) {
			strcpy_s(nvtfError, "WARNING: New Vegas Tick Fix not found! This will cause high memory usage and performance issues!\n         See https://performance.moddinglinked.com/falloutnv.html#NVTF on how to resolve this issue.");
			usesDefaultPool = false;
		}
		else {
			strcpy_s(nvtfError, "WARNING: New Vegas Tick Fix is missing its INI file!\n");
			usesDefaultPool = false;
		}

		return usesDefaultPool;
	}

	static bool PrintGraphicsMemory(bool usesDefaultPool) {
		ComPtr<IDXGIFactory2> spDXGIFactory;
		CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)(&spDXGIFactory));

		HRESULT hResult = S_OK;
		UInt32 i = 0;
		while (hResult != DXGI_ERROR_NOT_FOUND) {
			ComPtr<IDXGIAdapter1> spDXGIAdapter;
			hResult = spDXGIFactory->EnumAdapters1(i, spDXGIAdapter.GetAddressOf());

			if (SUCCEEDED(hResult)) {
				DXGI_ADAPTER_DESC1 kDesc;
				spDXGIAdapter->GetDesc1(&kDesc);
				if (kDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
					i++;
					continue;
				}

				const char* gpu = *(const char**)0x11C72C4;

				char cDescription[128];
				wcstombs(cDescription, kDesc.Description, 128);

				// Game annoyingly wraps the name in quotes
				char cCompareTarget[130];
				cCompareTarget[0] = '"';
				strcpy_s(cCompareTarget + 1, 128, cDescription);
				strcat_s(cCompareTarget, "\"");

				if (_stricmp(cCompareTarget, gpu) == 0) {
					ComPtr<IDXGIAdapter3> spAdapter3;
					spDXGIAdapter.As<IDXGIAdapter3>(&spAdapter3);

					DXGI_QUERY_VIDEO_MEMORY_INFO kInfo;
					spAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &kInfo);

					output << "\nGraphics Memory:\n";
					output << std::format("Budget Usage:   {}", GetMemoryUsageString(kInfo.CurrentUsage, kInfo.Budget)) << '\n';

					float used = (float)kInfo.CurrentUsage / kInfo.Budget * 100.0f;
					if (used >= 99.f && usesDefaultPool) {
						output << "\nWARNING: Graphics memory went over budget! This can't lead to a crash, but causes performance loss instead!\n";
					}
					// Not sure if there's a point in reporting issues when you go over budget with managed pool, as odds are you'll just crash anyway (unless you have a sub 1GB GPU)
					// In addition, handling of that case is elsewhere

					return ConvertToGiB(kInfo.CurrentUsage) > 1.f;
				}
			}
			i++;
		}
	}

	extern void Process(EXCEPTION_POINTERS* info)
	try 
	{
		const auto hProcess = GetCurrentProcess();

		MemoryErrors memoryErrorState = MemoryErrors::NONE;

		PROCESS_MEMORY_COUNTERS_EX2 pmc = {};
		pmc.cb = sizeof(pmc);

		// Get physical memory size
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);
		GlobalMemoryStatusEx(&memoryStatus);
		if ( GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)) )
		{
			DWORDLONG virtUsage = memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual;
			DWORDLONG physUsage = pmc.PrivateUsage;
			output << "Process' Memory:" << '\n';
			output << std::format("Physical Usage: {}", GetMemoryUsageString(physUsage, memoryStatus.ullTotalPhys)) << '\n';
			output << std::format("Virtual  Usage: {}", GetMemoryUsageString(virtUsage, memoryStatus.ullTotalVirtual)) << '\n';

			float usedVirtual = (float)virtUsage / memoryStatus.ullTotalVirtual * 100.0f;
			if (usedVirtual >= 80.f) {
				if (usedVirtual >= 95.f) {
					memoryErrorState = MemoryErrors::CRITICAL_USAGE;
				}
				else {
					memoryErrorState = MemoryErrors::HIGH_USAGE;
				}
			}
		}

		bool defaultPool = HandleNVTF();

		bool highGraphics = PrintGraphicsMemory(defaultPool);

		MemoryManager* memMgr = MemoryManager::GetSingleton();
		// If NVHR is used, the number will be 0
		if (memMgr->usNumHeaps > 0) {
			UInt32 usedHeapMemory = 0;
			UInt32 totalHeapMemory = 0;

			PrintSeparator();

#if PRINT_HEAPS
			output << "\nGame's Heaps:" << '\n';
#endif
			for (UInt32 i = 0; i < memMgr->usNumHeaps; i++) {
				IMemoryHeap* heap = memMgr->ppHeaps[i];
				if (!heap)
					continue;

				HeapStats stats;
				if (!memMgr->GetHeapStats(i, true, &stats))
					continue;

				SIZE_T used = stats.uiMemUsedInBlocks;
				SIZE_T total = stats.uiMemHeapSize;
#if PRINT_HEAPS
				SIZE_T start = 0;
				SIZE_T end = 0;
				if (stats.uiHeapOverhead == sizeof(ZeroOverheadHeap)) {
					start = reinterpret_cast<SIZE_T>(static_cast<ZeroOverheadHeap*>(heap)->pHeap);
					end = start + static_cast<ZeroOverheadHeap*>(heap)->uiSize;
				}
				else {
					start = reinterpret_cast<SIZE_T>(static_cast<MemoryHeap*>(heap)->pMemHeap);
					end = start + static_cast<MemoryHeap*>(heap)->uiMemHeapSize;
				}

				output << std::format("{:16}	 {}	  ({:08X} - {:08X})", heap->GetName(), GetMemoryUsageString(used, total), start, end) << '\n';
#endif
				usedHeapMemory += used;
				totalHeapMemory += total;
			}



			SIZE_T uiPoolMemory = 0;
			SIZE_T uiTotalPoolMemory = 0;
#if PRINT_POOLS
			output << "\nGame's Pools:" << '\n';
#endif
			for (UInt32 i = 0; i < 256; i++) {
				MemoryPool* pPool = MemoryManager::GetPool(i);
				if (!pPool)
					continue;

				SIZE_T used = (pPool->uiActiveAllocations * pPool->uiBlockSize * pPool->GetBlocksPerPage());
				SIZE_T total = pPool->uiSize;

				uiPoolMemory += used;
				uiTotalPoolMemory += total;
#if PRINT_POOLS
				SIZE_T start = reinterpret_cast<SIZE_T>(pPool->pAllocBase);
				SIZE_T end = start + pPool->uiSize;
				output << std::format("{:16}	 {}	  ({:08X} - {:08X})", pPool->pName, GetMemoryUsageString(used, total), start, end) << '\n';
#endif
			}

			output << "\nGame's Total Memory:" << '\n';
			output << std::format("Total Heap Memory: {}", GetMemoryUsageString(usedHeapMemory, totalHeapMemory)) << '\n';
			output << std::format("Total Pool Memory: {}", GetMemoryUsageString(uiPoolMemory, uiTotalPoolMemory)) << '\n';
			output << std::format("Total Memory:      {}", GetMemoryUsageString(usedHeapMemory + uiPoolMemory, totalHeapMemory + uiTotalPoolMemory)) << '\n';

			float usedHeap = (float)usedHeapMemory / totalHeapMemory * 100.0f;
			if (usedHeap < 80.f && memoryErrorState >= MemoryErrors::HIGH_USAGE)
				memoryErrorState = MemoryErrors::EXTERNAL_LEAK;
		}

		output << '\n';

		// We are going to bet here
		if (!defaultPool && highGraphics && memoryErrorState == MemoryErrors::EXTERNAL_LEAK)
			memoryErrorState = MemoryErrors::TEXTURE_SHARING;

		if (nvtfError[0])
			output << nvtfError << '\n';

		switch (memoryErrorState) {
		case MemoryErrors::TEXTURE_SHARING:
			output << "\nWARNING: High memory usage due to texture sharing! See above warning about NVTF!" << '\n';
			break;
		case MemoryErrors::EXTERNAL_LEAK:
			output << "\nWARNING: Memory usage is very high, but the game's heaps are not full! Is some module leaking?" << '\n';
			break;
		case MemoryErrors::CRITICAL_USAGE:
			output << "\nWARNING: Memory usage is very high! Is there a leak?" << '\n';
			break;
		case MemoryErrors::HIGH_USAGE:
			output << "\nWARNING: Memory usage is high!" << '\n';
			break;
		default:
			break;
		};
	}
	catch (...) { output << "Failed to log memory." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}