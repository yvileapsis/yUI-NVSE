#include <CrashLogger.hpp>
#include <MemoryManager.hpp>
#include <MemoryPool.hpp>
#include <psapi.h>
#include <ZeroOverheadHeap.hpp>

#define PRINT_HEAPS 1
#define PRINT_POOLS 0

namespace CrashLogger::Memory
{
	std::stringstream output;

	void HandleNVTF() {
		char iniDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, iniDir);
		strcat_s(iniDir, "\\Data\\NVSE\\Plugins\\NVTF.ini");

		if (GetFileAttributes(iniDir) != INVALID_FILE_ATTRIBUTES) {
			bool hasDXSettings = GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniDir);
			bool hasDefaultPool = GetPrivateProfileInt("DirectX", "bUseDefaultPoolForTextures", 0, iniDir);
			static const char* string = "WARNING: NVTF.ini has bModifyDirectXBehavior=%d %s bUseDefaultPoolForTextures=%d! This will cause high memory usage!\n         See https://performance.moddinglinked.com/falloutnv.html#NVTF on how to resolve this issue.";
			char errorBuffer[260];
			if ((hasDXSettings && !hasDefaultPool) || (!hasDXSettings && hasDefaultPool)) {
				sprintf_s(errorBuffer, string, hasDXSettings, "but", hasDefaultPool);
				output << errorBuffer << '\n';
			}
			else if (!hasDXSettings && !hasDefaultPool) {
				sprintf_s(errorBuffer, string, hasDXSettings, "and", hasDefaultPool);
				output << errorBuffer << '\n';
			}
		}
		else if (!GetModuleHandle("nvtf.dll")) {
			output << "WARNING: New Vegas Tick Fix not found! This will cause high memory usage and performance issues!\n         See https://performance.moddinglinked.com/falloutnv.html#NVTF on how to resolve this issue." << '\n';
		}
		else {
			output << "WARNING: New Vegas Tick Fix is missing its INI file!" << '\n';
		}
	}

	extern void Process(EXCEPTION_POINTERS* info)
	try 
	{
		const auto hProcess = GetCurrentProcess();


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
			if (usedVirtual >= 80.0f) {
				output << "WARNING: Virtual memory usage is above 80%!" << '\n';

				HandleNVTF();
			}
		}


		MemoryManager* memMgr = MemoryManager::GetSingleton();
		// If NVHR is used, the number will be 0
		if (memMgr->usNumHeaps > 0) {
			UInt32 usedHeapMemory = 0;
			UInt32 totalHeapMemory = 0;

			PrintSeparator();

			output << "\nGame's Memory:" << '\n';

#if PRINT_HEAPS
			output << "\nHeaps:" << '\n';
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

				output << std::format("{:30}	 {}	  ({:08X} - {:08X})", heap->GetName(), GetMemoryUsageString(used, total), start, end) << '\n';
#endif
				usedHeapMemory += used;
				totalHeapMemory += total;
			}



			SIZE_T uiPoolMemory = 0;
			SIZE_T uiTotalPoolMemory = 0;
#if PRINT_POOLS
			output << "\nPools:" << '\n';
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
				output << std::format("{:30}	 {}	  ({:08X} - {:08X})", pPool->pName, GetMemoryUsageString(used, total), start, end) << '\n';
#endif
			}

			output << std::format("\nTotal Heap Memory: {}", GetMemoryUsageString(usedHeapMemory, totalHeapMemory)) << '\n';
			output << std::format("Total Pool Memory: {}", GetMemoryUsageString(uiPoolMemory, uiTotalPoolMemory)) << '\n';
			output << std::format("Total Memory:      {}", GetMemoryUsageString(usedHeapMemory + uiPoolMemory, totalHeapMemory + uiTotalPoolMemory)) << '\n';
		}
	}
	catch (...) { output << "Failed to log memory." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}