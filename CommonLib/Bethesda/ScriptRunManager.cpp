#include "ScriptRunManager.hpp"

ScriptRunManager* ScriptRunManager::GetSingleton() {
	return CdeclCall<ScriptRunManager*>(0x5E24D0);
}
