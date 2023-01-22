#pragma once
#include <string>

#include <InventoryChanges.h>
#include <OSDepend.h>
#include <NiTypes.h>

namespace CraftingComponents
{
	void Fill();
	bool IsComponent(TESForm* form);
	bool IsProduct(TESForm* form);
}

namespace HideInfoPrompt
{
	void Show();
	void Hide();
	void Update();
}

TESForm* GetRefFromString(char*, char*);

bool HasBaseEffectChangesAV(TESForm*, int);

bool ends_with(std::string const& value, std::string const& ending);

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

/// Try to find in the Haystack the Needle - ignore case
bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle);

// if player is in third person, returns true if anim data is the first person and vice versa
bool IsPlayersOtherAnimData(AnimData* animData);

AnimData* GetThirdPersonAnimData(AnimData* animData);

bool TryGetTypeOfForm(TESForm* form);

char* __fastcall StrFromINI(DWORD* address);
std::string GetStringFromGameSettingFromString(const std::string& settingName);

void RegisterTraitID(const char* var1, UInt32 var2);
void funpatch();

inline Tile* g_TileReticleCenter = nullptr;
extern PlayerCharacter* g_player;

bool WorldToScreen(NiPoint3* posXYZ, NiPoint3& posOut, float offscreenHandling);

Float32 GetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 def);
Float32 SetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 value);

bool GetCannibalPrompt(TESObjectREFR* ref);

bool IsKeyPressed(UInt32 key, UInt32 flags = 0);
void DisableKey(UInt32 key, bool disable, UInt32 mask = 0);
bool IsButtonPressed(UInt32 button);
UInt32 GetControl(UInt32 whichControl, UInt32 type = OSInputGlobals::kControlType_Keyboard);
void SetNativeHandlerFunctionBool(bool trueorfalse);
bool ToggleVanityWheel(bool toggle);
std::string GetStringForButton(UInt32 button);