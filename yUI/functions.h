#pragma once

#include <internal/DecodedUI.h>
#include <GameTypes.h>
#include <unordered_set>

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;
extern std::unordered_set<TESForm*> g_CraftingComponents;

void UIWidth();
void UIHeight();
void UIWidth2();
void UIHeight2();
void UIWidth3();
void UIHeight3();
void __fastcall AddyCMToSettingsMenu(BSSimpleArray<StartMenuOption*>*, void*, StartMenuOption**);


void PrintAndClearQueuedConsoleMessages();
void FillCraftingComponents();

TESForm* GetRefFromString(char*, char*);
bool IsInListRecursive(TESForm*, TESForm*);

UInt8 ContWeaponHasAnyMod(ContChangesEntry*);
float ContGetHealthPercent(ContChangesEntry*);
bool ContGetEquipped(ContChangesEntry*);
bool IsAddictive(TESForm*);
bool IsFood(TESForm*);
bool IsMedicine(TESForm*);
bool IsPoisonous(TESForm*);
bool HasBaseEffectChangesAV(TESForm*, int);
TESForm* __stdcall LookupFormByRefID(UInt32);
bool IsCraftingComponent(TESForm*);

std::string GetCurPath();

bool ends_with(std::string const& value, std::string const& ending);

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);

/// Try to find in the Haystack the Needle - ignore case
bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle);

void Log(const std::string& msg);
void DebugLog(const std::string& msg);

int HexStringToInt(const std::string& str);

void DebugPrint(const std::string& str);

// if player is in third person, returns true if anim data is the first person and vice versa
bool IsPlayersOtherAnimData(AnimData* animData);

AnimData* GetThirdPersonAnimData(AnimData* animData);

void PatchPause(UInt32 ptr);

void SetUIStringFull(char *, char* , UInt32);

void __fastcall TileSetStringValueCursor(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* src, char propagate);
void __fastcall TileSetIntValueCursor(Tile* tile, void* dummyEDX, enum TileValues tilevalue, int value);
bool __fastcall FileExists(const char* path);
char* __fastcall StrFromINI(DWORD* address);