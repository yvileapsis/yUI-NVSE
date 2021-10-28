#pragma once

#include <internal/DecodedUI.h>

#include <GameTypes.h>
#include <unordered_set>

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;
extern std::unordered_set<TESForm*> map_CraftingComponents;

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
bool IsInListRecursive(TESForm*, BGSListForm*);
bool IsInRepairListRecursive(TESForm*, BGSListForm*);
bool IsInAmmoListRecursive(TESForm*, BGSListForm*);

UInt32 GetArmorValue(TESForm*, UInt32);
UInt8 ContWeaponHasAnyMod(ContChangesEntry*);
float ContGetHealthPercent(ContChangesEntry*);
bool ContGetEquipped(ContChangesEntry*);
bool HasBaseEffectRestoresAV(TESForm*, int);
bool HasBaseEffectDamagesAV(TESForm*, int);
bool IsAddictive(TESForm*);
bool IsFood(TESForm*);
bool IsMedicine(TESForm*);
bool IsPoisonous(TESForm*);
bool HasBaseEffectChangesAV(TESForm*, int);
TESForm* __stdcall LookupFormByRefID(UInt32);
bool IsCraftingComponent(TESForm*);