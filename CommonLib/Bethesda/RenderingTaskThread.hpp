#pragma once

#include "BSLinearTaskThread.hpp"

class RenderingTaskThread : public BSLinearTaskThread {
public:
	void(__cdecl* pCallback)();
};