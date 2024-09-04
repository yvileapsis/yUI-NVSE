#include "NiMemManager.hpp"

NiMemManager* NiMemManager::GetSingleton() {
    return *(NiMemManager**)0x11F6080;
}
