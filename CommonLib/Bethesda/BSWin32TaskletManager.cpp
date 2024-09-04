#include "BSWin32TaskletManager.hpp"

BSWin32TaskletManager* BSWin32TaskletManager::GetSingleton() {
	return CdeclCall<BSWin32TaskletManager*>(0xB00A00);
}