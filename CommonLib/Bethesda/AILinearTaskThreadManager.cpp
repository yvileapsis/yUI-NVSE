#include "AILinearTaskThreadManager.hpp"

// GAME - 0x713D80
AILinearTaskThreadManager* AILinearTaskThreadManager::GetSingleton() {
	return reinterpret_cast<AILinearTaskThreadManager*>(0x11DFA50);
}

bool AILinearTaskThreadManager::IsRunningThreads() {
	return *reinterpret_cast<bool*>(0x11DFA19);
}

bool AILinearTaskThreadManager::IsMainRendering() {
	return *reinterpret_cast<bool*>(0x11DFA18);
}

// GAME - 0x8C7AA0
bool AILinearTaskThreadManager::ShouldQueue3DTask() {
	return CdeclCall<bool>(0x8C7AA0);
}