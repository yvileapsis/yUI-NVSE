#pragma once

#include "MemoryContexts.hpp"

class AutoMemContext {
public:
	AutoMemContext(UInt32 auiContext);
	~AutoMemContext();

private:
	UInt32 uiPrevious;
};