#include "BGSSaveLoadGame.hpp"

BGSSaveLoadGame* BGSSaveLoadGame::GetSingleton() {
	return *reinterpret_cast<BGSSaveLoadGame**>(0x11DDF38);
}

// GAME - 0x570F40
bool BGSSaveLoadGame::GetGlobalAllowChanges() const {
	return ThisStdCall<bool>(0x570F40, this);
}

// GAME - 0x42CE10
bool BGSSaveLoadGame::GetSaveGameLoading() const {
	return ThisStdCall<bool>(0x42CE10, this);
}

// GAME - 0x5621D0
bool BGSSaveLoadGame::GetSaveGameSaving() const {
	return ThisStdCall<bool>(0x5621D0, this);
}

// GAME - 0x444D20
bool BGSSaveLoadGame::GetInitingForms() const {
	return ThisStdCall<bool>(0x444D20, this);
}

// GAME - 0x546950
bool BGSSaveLoadGame::GetDeferInitForms() const {
	return ThisStdCall<bool>(0x546950, this);
}

// GAME - 0x452E90
bool BGSSaveLoadGame::GetPositioningPlayerCharacter() const {
	return ThisStdCall<bool>(0x452E90, this);
}

// GAME - 0x63EF00
bool BGSSaveLoadGame::GetPlayerLocationInvalid() const {
	return ThisStdCall<bool>(0x63EF00, this);
}

// GAME - 0x848CF0
bool BGSSaveLoadGame::GetSaveLoadFailed() const {
	return ThisStdCall<bool>(0x848CF0, this);
}

// GAME - 0x462480
bool BGSSaveLoadGame::GetThreadAllowChanges() const {
	return ThisStdCall<bool>(0x462480, this);
}

// GAME - 0x4623F0
bool BGSSaveLoadGame::SetThreadAllowChanges(bool abEnable) {
	return ThisStdCall<bool>(0x4623F0, this, abEnable);
}

// GAME - 0x469570
bool BGSSaveLoadGame::GetLoadingMovedRefs() const {
	return ThisStdCall<bool>(0x469570, this);
}

// GAME - 0x4121B0
bool BGSSaveLoadGame::GetReconstructingForms() const {
	return ThisStdCall<bool>(0x4121B0, this);
}

// GAME - 0x570F00
bool BGSSaveLoadGame::GetAllowChanges() const {
	return ThisStdCall<bool>(0x570F00, this);
}