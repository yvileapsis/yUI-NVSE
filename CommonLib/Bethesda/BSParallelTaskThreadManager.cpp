#include "BSParallelTaskThreadManager.hpp"

BSParallelTaskThreadManager* BSParallelTaskThreadManager::GetSingleton() {
    return *reinterpret_cast<BSParallelTaskThreadManager**>(0x1202DF4);
}
