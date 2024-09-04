#include "AutoMemContext.hpp"
#include "MemoryManager.hpp"

AutoMemContext::AutoMemContext(UInt32 auiContext) {
	MemoryManager::EnterContext(uiPrevious, auiContext);
}

AutoMemContext::~AutoMemContext() {
	MemoryManager::LeaveContext(uiPrevious);
}
