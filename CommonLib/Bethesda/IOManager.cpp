#include "IOManager.hpp"

IOManager* IOManager::GetSingleton() {
    return *reinterpret_cast<IOManager**>(0x1202D98);
}

bool IOManager::IsUpdating() {
    return *reinterpret_cast<bool*>(0x1202DD8);
}

// GAME - 0x446DA0
UInt32 IOManager::GetPostProcessQueueCount() const {
    return ThisStdCall<UInt32>(0x446DA0, this);
}