#pragma once

#include "NiAllocator.hpp"

struct NiMemManager {
	NiAllocator* m_pkAllocator;

	static NiMemManager* GetSingleton();
};
