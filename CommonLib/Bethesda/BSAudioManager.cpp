#include "BSAudioManager.hpp"

BSAudioManager* BSAudioManager::GetSingleton() {
    return CdeclCall<BSAudioManager*>(0xAD9060);
}