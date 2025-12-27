#pragma once

#include <main.hpp>
#include <Safewrite.hpp>
#include <MemoryManager.hpp>

namespace Patch::MallocTracking
{
	extern void Init()
	{
		WriteRelJumpEx(0xAA4290, &MemoryManager::DefaultAllocate);
		WriteRelJumpEx(0xAA42C0, &MemoryManager::DefaultFree);
	}
}
