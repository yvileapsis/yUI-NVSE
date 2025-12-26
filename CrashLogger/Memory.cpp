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
		CRITICAL_USAGE,
		EXTERNAL_LEAK,
		TEXTURE_SHARING,
	};

	// In percentages
	constexpr float MEMORY_USAGE_CONCERN_THRESHOLD	= 80.0f;
	constexpr float MEMORY_USAGE_CRITICAL_THRESHOLD = 92.5f;
	constexpr float MEMORY_USAGE_OOM_THRESHOLD		= 98.0f;

	char nvtfError[260] = {};

	bool HandleNVTF() {
		char iniDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, iniDir);
		strcat_s(iniDir, "\\Data\\NVSE\\Plugins\\NVTF.ini");

		bool usesDefaultPool = true;

		if (GetFileAttributes(iniDir) != INVALID_FILE_ATTRIBUTES) {
			bool hasDXSettings = GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniDir);
			bool hasDefaultPool = GetPrivateProfileInt("DirectX", "bUseDefaultPoolForTextures", 0, iniDir);
			static const char* string = "INFO: NVTF.ini has bModifyDirectXBehavior=%d %s bUseDefaultPoolForTextures=%d\n";
			
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
			strcpy_s(nvtfError, "WARNING: New Vegas Tick Fix not found! This will cause performance issues, and crashes if using texture mods!\n");
			usesDefaultPool = false;
		}
		else {
			strcpy_s(nvtfError, "WARNING: New Vegas Tick Fix is missing its INI file!\n");
			usesDefaultPool = false;
		}

		return usesDefaultPool;
	}

	static bool __fastcall PrintGraphicsMemory(bool usesDefaultPool) {
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

				char cDescription[128] = {};
				wcstombs_s(nullptr, cDescription, kDesc.Description, 128);

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

					_MESSAGE("\nGraphics Memory:");
					char textBuffer[128];
					GetMemoryUsageString(kInfo.CurrentUsage, kInfo.Budget, textBuffer, sizeof(textBuffer));

					AutoIndent indent;

					_MESSAGE("Budget Usage:   %s", textBuffer);

					float used = (float)kInfo.CurrentUsage / kInfo.Budget * 100.0f;
					if (used >= 99.f && usesDefaultPool) {
						_MESSAGE("\nWARNING: Graphics memory went over budget! This (usually) can't lead to a crash, but causes performance loss instead!\n");
					}
					// Not sure if there's a point in reporting issues when you go over budget with managed pool, as odds are you'll just crash anyway (unless you have a sub 1GB GPU)
					// In addition, handling of that case is elsewhere

					return ConvertToGiB(kInfo.CurrentUsage) > 1.f;
				}
			}
			i++;
		}
		return false;
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			char cSmallBuffer[128];
			const auto hProcess = GetCurrentProcess();

			MemoryErrors memoryErrorState = MemoryErrors::NONE;

			PROCESS_MEMORY_COUNTERS_EX2 pmc = {};
			pmc.cb = sizeof(pmc);

			bool confirmedOutOfMem = false;

			// Get physical memory size
			MEMORYSTATUSEX memoryStatus;
			memoryStatus.dwLength = sizeof(memoryStatus);
			GlobalMemoryStatusEx(&memoryStatus);
			if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
				DWORDLONG virtUsage = memoryStatus.ullTotalVirtual - memoryStatus.ullAvailVirtual;
				DWORDLONG physUsage = pmc.PrivateUsage;
				_MESSAGE("\nProcess' Memory:");

				AutoIndent indent;

				GetMemoryUsageString(physUsage, memoryStatus.ullTotalPhys, cSmallBuffer, sizeof(cSmallBuffer));
				_MESSAGE("Physical Usage: %s", cSmallBuffer);

				GetMemoryUsageString(virtUsage, memoryStatus.ullTotalVirtual, cSmallBuffer, sizeof(cSmallBuffer));
				_MESSAGE("Virtual  Usage: %s", cSmallBuffer);

				float usedVirtual = (float)virtUsage / memoryStatus.ullTotalVirtual * 100.0f;
				if (usedVirtual >= MEMORY_USAGE_CONCERN_THRESHOLD) {
					// There's no doubt here
					if (usedVirtual >= MEMORY_USAGE_OOM_THRESHOLD) {
						confirmedOutOfMem = true;
					}

					if (usedVirtual >= MEMORY_USAGE_CRITICAL_THRESHOLD) {
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

				{
#if PRINT_HEAPS
					_MESSAGE("\nGame's Heaps:");
#endif
					AutoIndent indent;

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

						_MESSAGE("%-16s %s  (%08X - %08X)", heap->GetName(), GetMemoryUsageString(used, total, cSmallBuffer, sizeof(cSmallBuffer)), start, end);
#endif
						usedHeapMemory += used;
						totalHeapMemory += total;
					}
				}


				SIZE_T uiPoolMemory = 0;
				SIZE_T uiTotalPoolMemory = 0;
				{
#if PRINT_POOLS
					_MESSAGE("\nGame's Pools:");

					AutoIndent indent;
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

						_MESSAGE("%-16s %s  (%08X - %08X)", pPool->pName, GetMemoryUsageString(used, total, cSmallBuffer, sizeof(cSmallBuffer)), start, end);
#endif
					}
				}

				_MESSAGE("\nGame's Total Memory:");

				AutoIndent indent;

				char textBuffer[128];
				GetMemoryUsageString(usedHeapMemory, totalHeapMemory, textBuffer, sizeof(textBuffer));
				_MESSAGE("Total Heap Memory:   %s", textBuffer);

				GetMemoryUsageString(uiPoolMemory, uiTotalPoolMemory, textBuffer, sizeof(textBuffer));
				_MESSAGE("Total Pool Memory:   %s", textBuffer);

				GetMemoryUsageString(usedHeapMemory + uiPoolMemory, totalHeapMemory + uiTotalPoolMemory, textBuffer, sizeof(textBuffer));
				_MESSAGE("Total Memory:        %s", textBuffer);

				if (memMgr->uiMallocBytes > 0) {
					FormatSize(memMgr->uiMallocBytes, textBuffer, sizeof(textBuffer));
					_MESSAGE("Malloc Memory:       %s", textBuffer);
				};

				float usedHeap = (float)usedHeapMemory / totalHeapMemory * 100.0f;
				if (usedHeap < 80.f && memoryErrorState >= MemoryErrors::HIGH_USAGE)
					memoryErrorState = MemoryErrors::EXTERNAL_LEAK;
			}
			else {
				_MESSAGE("\nNew Vegas Heap Replacer found - Game's memory tracking unavailable.");
			}

			if (confirmedOutOfMem) {
				_MESSAGE("\nWARNING: Process ran out of memory - this is why you crashed! Logging may be inaccurate as well!");
			}

			// We are going to bet here
			if (!defaultPool && highGraphics && memoryErrorState >= MemoryErrors::HIGH_USAGE)
				memoryErrorState = MemoryErrors::TEXTURE_SHARING;

			if (nvtfError[0]) {
				_MESSAGE("\n%s", nvtfError);
			}

			switch (memoryErrorState) {
				case MemoryErrors::HIGH_USAGE:
					_MESSAGE("\nWARNING: Memory usage is high!");
					break;
				case MemoryErrors::CRITICAL_USAGE:
					_MESSAGE("\nWARNING: Memory usage is very high! Is there a leak?");
					break;
				case MemoryErrors::EXTERNAL_LEAK:
					_MESSAGE("\nWARNING: Memory usage is very high, but the game's heaps are not full! Is some module leaking?");
					break;
				case MemoryErrors::TEXTURE_SHARING:
					_MESSAGE("\nWARNING: High memory usage due to texture sharing! See https://performance.moddinglinked.com/falloutnv.html#NVTF on how to resolve this issue.");
					break;
				default:
					break;
			};
		}
		catch (...) { 
			_MESSAGE("\nFailed to log memory."); 
		}
	}
}
