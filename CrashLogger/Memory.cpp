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

	char cTickFixError[260] = {};

	bool HandleNVTF() {
		char iniDir[MAX_PATH];
		sprintf_s(iniDir, "%s\\Data\\NVSE\\Plugins\\NVTF.ini", GetRootDirectory());

		bool usesDefaultPool = true;

		if (GetFileAttributes(iniDir) != INVALID_FILE_ATTRIBUTES) {
			bool hasDXSettings = GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniDir);
			bool hasDefaultPool = GetPrivateProfileInt("DirectX", "bUseDefaultPoolForTextures", 0, iniDir);
			const char* string = "INFO: NVTF.ini has bModifyDirectXBehavior=%d %s bUseDefaultPoolForTextures=%d\n";
			
			if ((hasDXSettings && !hasDefaultPool) || (!hasDXSettings && hasDefaultPool)) {
				sprintf_s(cTickFixError, string, hasDXSettings, "but", hasDefaultPool);

				usesDefaultPool = false;
			}
			else if (!hasDXSettings && !hasDefaultPool) {
				sprintf_s(cTickFixError, string, hasDXSettings, "and", hasDefaultPool);

				usesDefaultPool = false;
			}
		}
		else if (!GetModuleHandle("nvtf.dll")) {
			strcpy_s(cTickFixError, "WARNING: New Vegas Tick Fix not found! This will cause performance issues, and crashes if using texture mods!\n");
			usesDefaultPool = false;
		}
		else {
			strcpy_s(cTickFixError, "WARNING: New Vegas Tick Fix is missing its INI file!\n");
			usesDefaultPool = false;
		}

		return usesDefaultPool;
	}

	static bool __fastcall PrintGraphicsMemory(bool usesDefaultPool) {
		try {
			ComPtr<IDXGIFactory6> spDXGIFactory;
			CreateDXGIFactory2(0, IID_PPV_ARGS(&spDXGIFactory));

			HRESULT hResult = S_OK;
			UInt32 i = 0; 
			size_t stHighestUsage = 0;
			_MESSAGE("\nGraphics Memory:");
			uint32_t uiLastDevice = 0;
			uint32_t uiGPUIndex = 0;
			bool bHighUsage = false;
			while (hResult != DXGI_ERROR_NOT_FOUND) {
				ComPtr<IDXGIAdapter4> spDXGIAdapter;
				hResult = spDXGIFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&spDXGIAdapter));
				
				if (SUCCEEDED(hResult)) {
					DXGI_ADAPTER_DESC1 kDesc;
					spDXGIAdapter->GetDesc1(&kDesc);
					if (kDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE || uiLastDevice == kDesc.DeviceId) {
						i++;
						continue;
					}

					uiLastDevice = kDesc.DeviceId;

					char cDescription[128] = {};
					wcstombs_s(nullptr, cDescription, kDesc.Description, 128);

					char cLocalMem[64] = {};
					char cNonLocalMem[64] = {};
					float fUsedPercent = 0.f;
					{
						DXGI_QUERY_VIDEO_MEMORY_INFO kInfo;
						spDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &kInfo);
						GetMemoryUsageString(kInfo.CurrentUsage, kInfo.Budget, cLocalMem, sizeof(cLocalMem));
						fUsedPercent = float(kInfo.CurrentUsage) / kInfo.Budget;
						stHighestUsage = std::max<size_t>(stHighestUsage, kInfo.CurrentUsage);
					}
					{
						DXGI_QUERY_VIDEO_MEMORY_INFO kInfo;
						spDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &kInfo);
						if (kInfo.Budget)
							GetMemoryUsageString(kInfo.CurrentUsage, kInfo.Budget, cNonLocalMem, sizeof(cNonLocalMem));
					}

					AutoIndent indent;
					_MESSAGE("GPU %i - %s", uiGPUIndex++, cDescription);
					{
						AutoIndent indent2;
						_MESSAGE("Local Memory:     %-32s", cLocalMem);
						if (cNonLocalMem[0])
							_MESSAGE("Non-Local Memory: %-32s", cNonLocalMem);

						if (fUsedPercent >= 1.f && usesDefaultPool) {
							_MESSAGE("\nWARNING: Graphics memory went over budget! This (usually) can't lead to a crash, but causes performance loss instead!\n");
						}
					}
				}
				i++;
			}
			return ConvertToGiB(stHighestUsage) > 1.f;
		}
		catch (...) {
			_MESSAGE("\nFailed to get graphics memory info.");
			return false;
		}
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			char cSmallBuffer[128];
			const auto hProcess = GetCurrentProcess();

			MemoryErrors eMemoryErrorState = MemoryErrors::NONE;

			PROCESS_MEMORY_COUNTERS_EX2 pmc = {};
			pmc.cb = sizeof(pmc);

			bool bConfirmedOutOfMem = false;

			// Get physical memory size
			MEMORYSTATUSEX kMemoryStatus;
			kMemoryStatus.dwLength = sizeof(kMemoryStatus);
			try {
				GlobalMemoryStatusEx(&kMemoryStatus);
				if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
					DWORDLONG virtUsage = kMemoryStatus.ullTotalVirtual - kMemoryStatus.ullAvailVirtual;
					DWORDLONG physUsage = pmc.PrivateUsage;
					_MESSAGE("\nProcess' Memory:");

					{
						AutoIndent indent;
						GetMemoryUsageString(physUsage, kMemoryStatus.ullTotalPhys, cSmallBuffer, sizeof(cSmallBuffer));
						_MESSAGE("Physical Usage: %s", cSmallBuffer);

						GetMemoryUsageString(virtUsage, kMemoryStatus.ullTotalVirtual, cSmallBuffer, sizeof(cSmallBuffer));
						_MESSAGE("Virtual  Usage: %s", cSmallBuffer);
					}

#if 0
					_MESSAGE("\nProcess Heaps:");
					{
						uint32_t uiHeapCount = GetProcessHeaps(0, nullptr);
						uint32_t uiTotalReserved = 0;
						if (uiHeapCount > 0) [[likely]] {
							HANDLE* pHeaps = static_cast<HANDLE*>(alloca(uiHeapCount * sizeof(HANDLE)));
							GetProcessHeaps(uiHeapCount, pHeaps);
							HEAP_SUMMARY kSummary;
							kSummary.cb = sizeof(kSummary);
							for (uint32_t i = 0; i < uiHeapCount; i++) {
								HANDLE hHeap = pHeaps[i];
								if (HeapSummary(hHeap, 0, &kSummary))
									uiTotalReserved += kSummary.cbReserved;
							}
						}
						AutoIndent indent;
						_MESSAGE("Reserved Memory: %s", FormatSize(uiTotalReserved, cSmallBuffer, sizeof(cSmallBuffer)));
					}
#endif

					float usedVirtual = (float)virtUsage / kMemoryStatus.ullTotalVirtual * 100.0f;
					if (usedVirtual >= MEMORY_USAGE_CONCERN_THRESHOLD) {
						// There's no doubt here
						if (usedVirtual >= MEMORY_USAGE_OOM_THRESHOLD) {
							bConfirmedOutOfMem = true;
						}

						if (usedVirtual >= MEMORY_USAGE_CRITICAL_THRESHOLD) {
							eMemoryErrorState = MemoryErrors::CRITICAL_USAGE;
						}
						else {
							eMemoryErrorState = MemoryErrors::HIGH_USAGE;
						}
					}
				}
			}
			catch (...) {
				_MESSAGE("\nFailed to get process memory info.");
			}

			bool bDefaultPool = HandleNVTF();

			bool bHighGraphics = PrintGraphicsMemory(bDefaultPool);

			MemoryManager* pMemMgr = MemoryManager::GetSingleton();
			// If NVHR is used, the number will be 0
			try {
				if (pMemMgr->usNumHeaps > 0) {
					UInt32 usedHeapMemory = 0;
					UInt32 totalHeapMemory = 0;

					PrintSeparator();

					{
#if PRINT_HEAPS
						_MESSAGE("\nGame's Heaps:");
#endif
						AutoIndent indent;

						for (UInt32 i = 0; i < pMemMgr->usNumHeaps; i++) {
							IMemoryHeap* heap = pMemMgr->ppHeaps[i];
							if (!heap)
								continue;

							try {
								HeapStats stats;
								if (!pMemMgr->GetHeapStats(i, true, &stats))
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
							catch (...) {
								_MESSAGE("Failed to get stats for heap %u", i);
							}
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

							try {
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
							catch (...) {
								_MESSAGE("Failed to get stats for pool %u", i);
							}
						}
					}

					_MESSAGE("\nGame's Total Memory:");

					AutoIndent indent;
					GetMemoryUsageString(usedHeapMemory, totalHeapMemory, cSmallBuffer, sizeof(cSmallBuffer));
					_MESSAGE("Total Heap Memory:   %s", cSmallBuffer);

					GetMemoryUsageString(uiPoolMemory, uiTotalPoolMemory, cSmallBuffer, sizeof(cSmallBuffer));
					_MESSAGE("Total Pool Memory:   %s", cSmallBuffer);

					GetMemoryUsageString(usedHeapMemory + uiPoolMemory, totalHeapMemory + uiTotalPoolMemory, cSmallBuffer, sizeof(cSmallBuffer));
					_MESSAGE("Total Memory:        %s", cSmallBuffer);

					if (pMemMgr->uiMallocBytes > 0) {
						FormatSize(pMemMgr->uiMallocBytes, cSmallBuffer, sizeof(cSmallBuffer));
						_MESSAGE("Malloc Memory:       %s", cSmallBuffer);
					};

					float usedHeap = (float)usedHeapMemory / totalHeapMemory * 100.0f;
					if (usedHeap < 80.f && eMemoryErrorState >= MemoryErrors::HIGH_USAGE)
						eMemoryErrorState = MemoryErrors::EXTERNAL_LEAK;
				}
				else {
					_MESSAGE("\nCustom heap replacer detected - Game's memory tracking unavailable.");
				}
			}
			catch (...) {
				_MESSAGE("\nFailed to get heap memory info.");
			}

			if (bConfirmedOutOfMem) {
				_MESSAGE("\nWARNING: Process ran out of memory - this is why you crashed! Logging may be inaccurate as well!");
			}

			// We are going to bet here
			if (!bDefaultPool && bHighGraphics && eMemoryErrorState >= MemoryErrors::HIGH_USAGE)
				eMemoryErrorState = MemoryErrors::TEXTURE_SHARING;

			if (cTickFixError[0]) {
				_MESSAGE("\n%s", cTickFixError);
			}

			switch (eMemoryErrorState) {
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
