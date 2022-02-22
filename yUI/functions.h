#pragma once
#include <GameTiles.h>
#include <GameExtraData.h>
#include <unordered_set>

inline std::unordered_set<TESForm*> g_CraftingComponents;
inline std::unordered_set<TESForm*> g_CraftingProducts;

void UIWidth();
void UIHeight();
void UIWidth2();
void UIHeight2();
void UIWidth3();
void UIHeight3();
void __fastcall AddyCMToSettingsMenu(BSSimpleArray<StartMenuOption*>*, void*, StartMenuOption**);

void FillCraftingComponents();

TESForm* GetRefFromString(char*, char*);

UInt8 ContWeaponHasAnyMod(ContChangesEntry*);
float ContGetHealthPercent(ContChangesEntry*);
bool ContGetEquipped(ContChangesEntry*);
bool IsAddictive(TESForm*);
bool IsFood(TESForm*);
bool IsMedicine(TESForm*);
bool IsPoisonous(TESForm*);
bool HasBaseEffectChangesAV(TESForm*, int);
bool IsCraftingComponent(TESForm*);
bool IsCraftingProduct(TESForm*);

bool ends_with(std::string const& value, std::string const& ending);

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

/// Try to find in the Haystack the Needle - ignore case
bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle);

// if player is in third person, returns true if anim data is the first person and vice versa
bool IsPlayersOtherAnimData(AnimData* animData);

AnimData* GetThirdPersonAnimData(AnimData* animData);

void PatchPause(UInt32 ptr);

void SetUIStringFull(const char *, const char* , UInt32);

bool TryGetTypeOfForm(TESForm* form);

void __fastcall CursorTileSetStringValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* src, char propagate);
void __fastcall CursorTileSetIntValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, int value);

char* __fastcall StrFromINI(DWORD* address);
std::string GetStringFromGameSettingFromString(const std::string& settingName);

void* __fastcall FixGetDroppedWeaponPre(ExtraDataList* extradatalist);
void* __fastcall FixGetDroppedWeaponMid();

template <UInt32 retn> __declspec(naked) void FixGetDroppedWeaponPost()
{
	static const UInt32 retnAddr = retn;
	static const UInt32 UpdateExtraHealth = 0x419970;
	static const auto GetNextWeapon = reinterpret_cast<UInt32>(FixGetDroppedWeaponMid);
	__asm
	{
		mov eax, [ecx] // dereference
		mov ecx, [eax] // this
		call UpdateExtraHealth
		call GetNextWeapon
		jmp retnAddr
	}
}

void RegisterTraitID(const char* var1, UInt32 var2);
void funpatch();

void FixTablineSelected();