#pragma once

#include <main.hpp>
#include <Safewrite.hpp>
#include <MemoryManager.hpp>

namespace Patch::MallocTracking
{

	void Patch()
	{
		WriteRelJumpEx(0xAA4290, &MemoryManager::DefaultAllocate);
		WriteRelJumpEx(0xAA42C0, &MemoryManager::DefaultFree);
	}

	extern void Init()
	{
		if (g_currentGame == kFalloutNewVegas)
			Patch();
	}
}
