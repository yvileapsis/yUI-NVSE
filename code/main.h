#pragma once
#include <float.h>
#include <internal/param_info.h>
#include <nvse/nvse/SafeWrite.h>
#include <nvse/nvse/GameObjects.h>
#include <nvse/nvse/GameProcess.h>
#include <nvse/nvse/GameUI.h>
#include <nvse/nvse/GameEffects.h>
#include <nvse/nvse/GameTasks.h>
#include <nvse/nvse/GameSettings.h>
#include <nvse/nvse/GameExtraData.h>
#include <nvse/nvse/GameRTTI.h>
#include <nvse/nvse/Hooks_DirectInput8Create.h>
#include <nvse/nvse/PluginAPI.h>
#include <nvse/nvse_version.h>
#include <nvse/nvse/CommandTable.h>
#include <nvse/nvse/NiTypes.h>
#include <decoding.h>
#include <vec3.h>
#include <regex>
#include <settings.h>
#include <fstream>
#include <ParamInfos.h>
#include <set>
#include <unordered_set>
#include <unordered_map>


PCMiscStat** g_miscStatData = (PCMiscStat * *)0x11C6D50;
const CommandInfo* (*GetCmdByOpcode)(UInt32 opcode);

HMODULE yUIHandle;
DebugLog s_log;

DIHookControl* g_DIHook = NULL;
const NVSEInterface* g_nvse;
PlayerCharacter* g_thePlayer = NULL;
ActorValueOwner* g_playerAVOwner = NULL;
BaseProcess* g_playerProcess = NULL;
DataHandler* g_dataHandler = NULL;

ModelLoader* g_modelLoader = NULL;
TimeGlobal* timeGlobals = (TimeGlobal*)0x11F6394;

NVSEArrayVarInterface* g_arrInterface = NULL;
NVSEStringVarInterface* g_strInterface = NULL;
NVSEMessagingInterface* g_msg = NULL;
NVSEScriptInterface* g_scriptInterface = NULL;
NVSECommandTableInterface* g_cmdTableInterface = NULL;

const char* (*GetStringVar)(UInt32 stringID);
void		(*SetStringVar)(UInt32 stringID, const char* newValue);

bool (*AssignString)(COMMAND_ARGS, const char* newValue);
NVSEArrayVar* (*CreateArray)(const NVSEArrayElement* data, UInt32 size, Script* callingScript);
NVSEArrayVar* (*CreateStringMap)(const char** keys, const NVSEArrayElement* values, UInt32 size, Script* callingScript);
bool (*AssignCommandResult)(NVSEArrayVar* arr, double* dest);
void (*SetElement)(NVSEArrayVar* arr, const NVSEArrayElement& key, const NVSEArrayElement& value);
void (*AppendElement)(NVSEArrayVar* arr, const NVSEArrayElement& value);
UInt32(*GetArraySize)(NVSEArrayVar* arr);
NVSEArrayVar* (*LookupArrayByID)(UInt32 id);
bool (*GetElement)(NVSEArrayVar* arr, const NVSEArrayElement& key, NVSEArrayElement& outElement);
bool (*GetElements)(NVSEArrayVar* arr, NVSEArrayElement* elements, NVSEArrayElement* keys);
bool (*ExtractArgsEx)(COMMAND_ARGS_EX, ...);
bool (*ExtractFormatStringArgs)(UInt32 fmtStringPos, char* buffer, COMMAND_ARGS_EX, UInt32 maxParams, ...);
bool (*CallFunction)(Script* funcScript, TESObjectREFR* callingObj, TESObjectREFR* container, NVSEArrayElement* result, UInt8 numArgs, ...);

#define ExtractFormatStringArgs(...) g_scriptInterface->ExtractFormatStringArgs(__VA_ARGS__)
#define IS_TYPE(form, type) (*(UInt32*)form == kVtbl_##type)
#define NOT_ID(form, type) (form->typeID != kFormType_##type)
#define IS_ID(form, type) (form->typeID == kFormType_##type)
#define REG_CMD(name) nvse->RegisterCommand(&kCommandInfo_##name);
#define REG_CMD_FRM(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Form)
#define REG_CMD_STR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_String)
#define REG_CMD_ARR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Array)
#define REG_CMD_AMB(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Ambiguous)
#define REG_TYPED_CMD(name, type) nvse->RegisterTypedCommand(&kCommandInfo_##name,kRetnType_##type);
#define VarNameSize 64
IDebugLog ParamLog;

#define MSGBOX_ARGS 0, 0, ShowMessageBox_Callback, 0, 0x17, 0, 0, "OK", NULL

void writePatches();
void ySIFillMapsAlt();

int iDoOnce;

//static TESQuest* lastQuest;
//bool shouldRefreshQuestMenu;

Tile* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu;

tList<char> queuedConsoleMessages;
void PrintAndClearQueuedConsoleMessages()
{
	for (auto iter = queuedConsoleMessages.Begin(); !iter.End(); ++iter)
	{
		Console_Print("%s", iter.Get());
	}

	queuedConsoleMessages.DeleteAll();
}

void ConsoleQueueOrPrint(const char* str, int len)
{
	if (*(ConsoleManager * *)0x11D8CE8 || g_dataHandler) // g_dataHandler will be non-null if Deferred init has been called
	{
		Console_Print("%s", str);
	}
	else
	{
		auto errorStr = (char*)GameHeapAlloc(len + 1);
		strcpy(errorStr, str);

		queuedConsoleMessages.Append(errorStr);
	}
}


bool Menu::GetTemplateExists(const char* templateName)
{
	for (ListNode<TemplateData>* node = menuTemplates.Head(); node; node = node->next) {
		if (node->data && (StrEqualCI(node->data->templateName, templateName))) { return 1; }
	}
	return 0;
}


__declspec(naked) void UIWidth()
{
	static const UInt32 retnAddr = 0x715D8D;

	__asm
	{
		fld[ebp - 0x4]
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight()
{
	static const UInt32 retnAddr = 0x715DED;

	__asm
	{
		fld[ebp - 0x4]
		jmp		retnAddr
	}
}


double f1920 = 1920.0;
double f1080 = 1080.0;

__declspec(naked) void UIWidth2()
{
	static const UInt32 retnAddr = 0x71303D;

	__asm
	{
		fld		ds : f1920
		jmp		retnAddr
	}
}


__declspec(naked) void UIHeight2()
{
	static const UInt32 retnAddr = 0x713046;

	__asm
	{
		fld		ds : f1080
		jmp		retnAddr
	}
}


__declspec(naked) void UIWidth3()
{
	static const UInt32 retnAddr = 0x7FBBE2;

	__asm
	{
		fld		ds : f1920
		jmp		retnAddr
	}
}


__declspec(naked) void UIHeight3()
{
	static const UInt32 retnAddr = 0x7FBBEB;

	__asm
	{
		fld		ds : f1080
		jmp		retnAddr
	}
}


StartMenuOption* yCM;

void __cdecl yCMCallback()
{
	MenuButton_Downloads();
//	ThisCall(0x7D74F0, (void *)( 0x11DAAC0 + 0x0B4), 0);
//	static StartMenu* g_StartMenu = StartMenu::GetSingleton();
//	((Tile*)(&(g_StartMenu->settingsSubMenu) + 0xC))->SetFloat(TraitNameToID("_enabled"), 1);
//	g_StartMenu->GetChild("yCM")->GetChild("yCM")->SetFloat(kTileValue_visible, 0, 0);
}

void __fastcall AddyCMToSettingsMenu(BSSimpleArray<StartMenuOption*>* settingsMenuOptions, void* edx, StartMenuOption** menuDownloads)
{
//		(*HUDColorSetting)->onSelection = HUDColorSettingCallback;
//	(*menuDownloads)->displayString = "Mods";
//	(*menuDownloads)->callback = yCMCallback;
//	(*menuDownloads)->data = StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu;

	yCM = StartMenuOption::Create("Mods", yCMCallback, StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu);
	settingsMenuOptions->Append(&yCM);
	settingsMenuOptions->Append(menuDownloads);


	//yCM = StartMenuOption::Create("Mods", yCMCallback, StartMenuOption::kMainMenu);

}


NiTPointerMap<TESForm>** g_allFormsMap = (NiTPointerMap<TESForm> * *)0x11C54C0;

typedef Vector<ArrayElementL> TempElements;
__declspec(noinline) TempElements* GetTempElements()
{
	thread_local TempElements s_tempElements(0x100);
	return &s_tempElements;
}

const UInt8 kTypeListJmpTbl[] =
{
	255, 255, 255, 255, 128, 50, 28, 15, 16, 8, 9, 10, 11, 25, 26, 255, 255, 24, 13, 6, 7, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 48, 128, 39, 128, 5, 2, 58, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 1, 255, 255, 255, 32, 33, 34, 255, 0, 35, 36, 128, 51, 37, 38, 255, 40, 45, 3, 4, 49, 46, 47, 128, 255, 41, 14,
	255, 29, 30, 31, 128, 12, 52, 27, 255, 53, 54, 128, 17, 255, 19, 20, 128, 22, 55, 56, 57, 18, 21, 128, 128, 23, 42, 43, 44
};

tList<TESForm>* GetSourceList(UInt8 formType)
{
	if (formType == kFormType_TESRegion)
		return (tList<TESForm>*) & g_dataHandler->regionList->list;
	UInt8 listIdx = kTypeListJmpTbl[formType];
	if (listIdx >= 0x80) return NULL;
	return &((tList<TESForm>*) & g_dataHandler->packageList)[listIdx];
}

void GetLoadedType(UInt32 formType, int index, tList<TESForm>* outList, TempElements* tmpElements)
{
	if (formType == kFormType_TESObjectCELL)
	{
		TESObjectCELL** cells = g_dataHandler->cellArray.data;
		for (UInt32 count = g_dataHandler->cellArray.Length(); count; count--, cells++)
		{
			if ((index != -1) && (index != (*cells)->modIndex)) continue;
			if (outList) outList->Insert(*cells);
			else tmpElements->Append(*cells);
		}
	}
	else if (formType == 301)
	{
		ListNode<TESWorldSpace>* wspcIter = g_dataHandler->worldSpaceList.Head();
		TESWorldSpace* wspc;
		ListNode<TESObjectREFR>* refrIter;
		TESObjectREFR* refr;
		do
		{
			if (!(wspc = wspcIter->data)) continue;
			refrIter = wspc->cell->objectList.Head();
			do
			{
				refr = refrIter->data;
				if (!refr || !refr->extraDataList.HasType(kExtraData_MapMarker) || ((index != -1) && (index != refr->modIndex))) continue;
				if (outList) outList->Insert(refr);
				else tmpElements->Append(refr);
			} while (refrIter = refrIter->next);
		} while (wspcIter = wspcIter->next);
	}
	else if (formType == 302)
	{
		TESObjectCELL** cells = g_dataHandler->cellArray.data;
		ListNode<TESObjectREFR>* refrIter;
		TESObjectREFR* refr;
		for (UInt32 count = g_dataHandler->cellArray.Length(); count; count--, cells++)
		{
			refrIter = (*cells)->objectList.Head();
			do
			{
				refr = refrIter->data;
				if (!refr || !refr->extraDataList.HasType(kExtraData_RadioData) || ((index != -1) && (index != refr->modIndex))) continue;
				if (outList) outList->Insert(refr);
				else tmpElements->Append(refr);
			} while (refrIter = refrIter->next);
		}
	}
	else if (formType < kFormType_Max)
	{
		tList<TESForm>* sourceList = GetSourceList(formType);
		TESForm* form;
		if (sourceList)
		{
			ListNode<TESForm>* iter = sourceList->Head();
			do
			{
				form = iter->data;
				if (!form || ((index != -1) && (index != form->modIndex))) continue;
				if (outList) outList->Insert(form);
				else tmpElements->Append(form);
			} while (iter = iter->next);
		}
		else if (kTypeListJmpTbl[formType] == 0x80)
		{
			for (TESBoundObject* object = g_dataHandler->boundObjectList->first; object; object = object->next)
			{
				if ((object->typeID != formType) || ((index != -1) && (index != object->modIndex))) continue;
				if (outList) outList->Insert(object);
				else tmpElements->Append(object);
			}
		}
		else
		{
			for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter)
			{
				form = mIter.Get();
				if (!form || (form->typeID != formType) || ((index != -1) && (index != form->modIndex))) continue;
				if (outList) outList->Insert(form);
				else tmpElements->Append(form);
			}
		}
	}
}


enum EWeapValues
{
	eWeap_Type = 0,
	eWeap_MinSpread,
	eWeap_Spread,
	eWeap_Proj,
	eWeap_SightFOV,
	eWeap_MinRange,
	eWeap_Range,
	eWeap_AmmoUse,
	eWeap_APCost,
	eWeap_CritDam,
	eWeap_CritChance,
	eWeap_CritEffect,
	eWeap_FireRate,
	eWeap_AnimAttackMult,
	eWeap_RumbleLeft,
	eWeap_RumbleRight,
	eWeap_RumbleDuration,
	eWeap_RumbleWaveLength,
	eWeap_AnimShotsPerSec,
	eWeap_AnimReloadTime,
	eWeap_AnimJamTime,
	eWeap_Skill,
	eWeap_ResistType,
	eWeap_FireDelayMin,
	eWeap_FireDelayMax,
	eWeap_AnimMult,
	eWeap_Reach,
	eWeap_IsAutomatic,
	eWeap_HandGrip,
	eWeap_ReloadAnim,
	eWeap_VATSChance,
	eWeap_AttackAnim,
	eWeap_NumProj,
	eWeap_AimArc,
	eWeap_LimbDamageMult,
	eWeap_SightUsage,
	eWeap_ReqStr,
	eWeap_ReqSkill,
	eWeap_LongBursts,
	eWeap_Flags1,
	eWeap_Flags2,
	eWeap_HasScope,
	eWeap_IgnoresDTDR,
	eWeap_SoundLevel,
};

float GetWeaponValue(TESForm* pForm, UInt32 whichVal)
{
	TESObjectWEAP* pWeapon = DYNAMIC_CAST(pForm, TESForm, TESObjectWEAP);
	if (pWeapon) {
		switch (whichVal) {
		case eWeap_Type:				return pWeapon->eWeaponType;
		case eWeap_MinSpread:			return pWeapon->minSpread;
		case eWeap_Spread:				return pWeapon->spread;
		case eWeap_Proj:
		{
			BGSProjectile* pProj = pWeapon->projectile;
			if (pProj) {
				return (UInt32)pProj->refID;
			}
		}
		break;
		case eWeap_SightFOV:			return pWeapon->sightFOV;
		case eWeap_MinRange:			return pWeapon->minRange;
		case eWeap_Range:				return pWeapon->maxRange;
		case eWeap_AmmoUse:				return pWeapon->ammoUse;
		case eWeap_APCost:				return pWeapon->AP;
		case eWeap_CritDam:				return pWeapon->criticalDamage;
		case eWeap_CritChance:			return pWeapon->criticalPercent;
		case eWeap_CritEffect:
		{
			SpellItem* pSpell = pWeapon->criticalEffect;
			if (pSpell) {
				return (UInt32)pSpell->refID;
			}
		}
		break;
		case eWeap_FireRate:			return pWeapon->fireRate;
		case eWeap_AnimAttackMult:		return pWeapon->animAttackMult;
		case eWeap_RumbleLeft:			return pWeapon->rumbleLeftMotor;
		case eWeap_RumbleRight:			return pWeapon->rumbleRightMotor;
		case eWeap_RumbleDuration:		return pWeapon->rumbleDuration;
		case eWeap_RumbleWaveLength:	return pWeapon->rumbleWavelength;
		case eWeap_AnimShotsPerSec:		return pWeapon->animShotsPerSec;
		case eWeap_AnimReloadTime:		return pWeapon->animReloadTime;
		case eWeap_AnimJamTime:			return pWeapon->animJamTime;
		case eWeap_Skill:				return pWeapon->weaponSkill;
		case eWeap_ResistType:			return pWeapon->resistType;
		case eWeap_FireDelayMin:		return pWeapon->semiAutoFireDelayMin;
		case eWeap_FireDelayMax:		return pWeapon->semiAutoFireDelayMax;
		case eWeap_AnimMult:			return pWeapon->animMult;
		case eWeap_Reach:				return pWeapon->reach;
		case eWeap_IsAutomatic:			return pWeapon->IsAutomatic();
		case eWeap_HandGrip:			return pWeapon->HandGrip();
		case eWeap_ReloadAnim:			return pWeapon->reloadAnim;
		case eWeap_VATSChance:			return pWeapon->baseVATSChance;
		case eWeap_AttackAnim:			return pWeapon->AttackAnimation();
		case eWeap_NumProj:				return pWeapon->numProjectiles;
		case eWeap_AimArc:				return pWeapon->aimArc;
		case eWeap_LimbDamageMult:		return pWeapon->limbDamageMult;
		case eWeap_SightUsage:			return pWeapon->sightUsage;
		case eWeap_ReqStr:				return pWeapon->strRequired;
		case eWeap_ReqSkill:			return pWeapon->skillRequirement;
		case eWeap_LongBursts:			return pWeapon->weaponFlags2.IsSet(TESObjectWEAP::eFlag_LongBurst);
		case eWeap_Flags1:				return pWeapon->weaponFlags1.Get();
		case eWeap_Flags2:				return pWeapon->weaponFlags2.Get();
		case eWeap_HasScope:			return pWeapon->HasScope();
		case eWeap_IgnoresDTDR:			return pWeapon->IgnoresDTDR();
		case eWeap_SoundLevel:			return pWeapon->soundLevel;
		default: HALT("unknown weapon value"); break;
		}
	}
	return true;
}

enum EBipedFlags {
	eBipedFlag_HasBackPack = 0x4,
	eBipedFlag_MediumArmor = 0x8,
	eBipedFlag_PowerArmor = 0x20,
	eBipedFlag_NonPlayable = 0x40,
	eBipedFlag_HeavyArmor = 0x80,
};

UInt32 GetArmorValue(TESForm* pForm, UInt32 whichVal)
{
	TESObjectARMO* pArmor = DYNAMIC_CAST(pForm, TESForm, TESObjectARMO);
	if (pArmor) {
		switch (whichVal) {
		case 1: {
			TESBipedModelForm* biped = &pArmor->bipedModel;
			return (biped->bipedFlags & eBipedFlag_HeavyArmor) ? 3 : ((biped->bipedFlags & eBipedFlag_MediumArmor) ? 2 : 1);
		}
		case 2: {
			TESBipedModelForm* biped = &pArmor->bipedModel;
			return (biped->bipedFlags & eBipedFlag_PowerArmor) ? 1 : 0;
		}
		case 3: {
			TESBipedModelForm* biped = &pArmor->bipedModel;
			return (biped->bipedFlags & eBipedFlag_HasBackPack) ? 1 : 0;
		}
		case 4:						return pArmor->armorRating;
		case 5:						return static_cast<UInt32>(pArmor->damageThreshold);
		case 6: {
			TESBipedModelForm* biped = &pArmor->bipedModel;
			return biped->partMask;
		}
		default: HALT("unknown weapon value"); break;
		}
	}
	return true;
}

ExtraDataList* ExtraContainerChanges::EntryData::GetCustomExtra(UInt32 whichVal)
{
	if (extendData)
	{
		ExtraDataList* xData;
		ListNode<ExtraDataList>* xdlIter = extendData->Head();
		do
		{
			xData = xdlIter->data;
			if (xData && xData->HasType(whichVal))
				return xData;
		} while (xdlIter = xdlIter->next);
	}
	return NULL;
}

UInt8 ContWeaponHasAnyMod(ContChangesEntry* weaponInfo)
{
	ExtraDataList* xData = weaponInfo->GetCustomExtra(kExtraData_WeaponModFlags);
	if (!xData) return 0;
	ExtraWeaponModFlags* xModFlags = GetExtraType((*xData), WeaponModFlags);
	if (!xModFlags) return 0;
	return xModFlags->flags;
}

float ContGetHealthPercent(ContChangesEntry* itemInfo)
{
	ExtraDataList* xData = itemInfo->GetCustomExtra(kExtraData_Health);
	if (!xData) return 0;
	TESHealthForm* healthForm = DYNAMIC_CAST(itemInfo->type, TESForm, TESHealthForm);
	ExtraHealth* xHealth;
	if (healthForm)
	{
		xHealth = GetExtraType((*xData), Health);
	}
	float health = xHealth ? xHealth->health : (int)healthForm->health;
	return (health / healthForm->health);
}

bool ContGetEquipped(ContChangesEntry* weaponInfo)
{
	ExtraDataList* xData = weaponInfo->GetCustomExtra(kExtraData_Worn);
	if (!xData) return 0;
	ExtraWorn* xWorn = GetExtraType((*xData), Worn);
	if (xWorn) return 1;
	ExtraWornLeft* xWornLeft = GetExtraType((*xData), WornLeft);
	if (xWornLeft) return 1;
	return 0;
}

bool HasBaseEffectRestoresAV(TESForm* form, int avCode)
{
	AlchemyItem* item = DYNAMIC_CAST(form, TESForm, AlchemyItem);

	auto iter = item->magicItem.list.list.Begin();
	for (; !iter.End(); ++iter)
	{
		auto effect = iter.Get();
		if (effect->GetSkillCode() == avCode)
		{
			auto setting = effect->setting;
			if (setting && !(setting->effectFlags & EffectSetting::kDetrimental))
			{
				// check if player meets the conditions for the item
				bool eval;
				return true;
//				if (effect->conditions.Evaluate(g_thePlayer, nullptr, &eval, false)) return true;
			}
		}
	}
	return false;
}

bool HasBaseEffectDamagesAV(TESForm* form, int avCode)
{
	AlchemyItem* item = DYNAMIC_CAST(form, TESForm, AlchemyItem);

	auto iter = item->magicItem.list.list.Begin();
	for (; !iter.End(); ++iter)
	{
		auto effect = iter.Get();
		if (effect->GetSkillCode() == avCode)
		{
			auto setting = effect->setting;
			if (setting && setting->effectFlags & EffectSetting::kDetrimental)
			{
				// check if player meets the conditions for the item
				bool eval;
				return true;
//				if (effect->conditions.Evaluate(g_thePlayer, nullptr, &eval, false)) return true;
			}
		}
	}
	return false;
}

bool IsAddictive(TESForm* form)
{
	AlchemyItem* item = DYNAMIC_CAST(form, TESForm, AlchemyItem);
	if (item->withdrawalEffect && item->withdrawalEffect->refID) {
		return true;
	}
	return false;
}


bool IsFood(TESForm* form)
{
	AlchemyItem* item = DYNAMIC_CAST(form, TESForm, AlchemyItem);
	return (item->alchFlags & 2) ? 1 : 0;
}

bool IsMedicine(TESForm* form)
{
	AlchemyItem* item = DYNAMIC_CAST(form, TESForm, AlchemyItem);
	return (item->alchFlags & 4) ? 1 : 0;
}

bool AlchemyItem::IsPoison()
{
	EffectItem* effItem;
	EffectSetting* effSetting = NULL;
	ListNode<EffectItem>* iter = magicItem.list.list.Head();
	do
	{
		if (!(effItem = iter->data)) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	} while (iter = iter->next);
	return effSetting != NULL;
}

bool IsPoisonous(TESForm* form)
{
	AlchemyItem* item = DYNAMIC_CAST(form, TESForm, AlchemyItem);
	return (item->IsPoison()) ? 1 : 0;
}

bool HasBaseEffectChangesAV(TESForm* form, int avCode)
{

	TESObjectARMO* armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
	TESEnchantableForm* enchantable = &armor->enchantable;
	if (!enchantable) return false;
	EnchantmentItem* enchantment = enchantable->enchantItem;
	if (!enchantment) return false;
	auto iter = enchantment->magicItem.list.list.Begin();
	for (; !iter.End(); ++iter)
	{
		auto effect = iter.Get();
		if (effect->GetSkillCode() == avCode)
		{
			auto setting = effect->setting;
			if (setting && (setting->effectFlags & EffectSetting::kRecover))
			{
				bool eval;
				return true;
//				if (effect->conditions.Evaluate(g_thePlayer, nullptr, &eval, false)) return true;
			}
		}
	}
	return false;
}


__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds: [0x11C54C0]
		mov		eax, [esp + 4]
		xor		edx, edx
		div		dword ptr[ecx + 4]
		mov		eax, [ecx + 8]
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		edx, [esp + 4]
		ALIGN 16
	iterHead:
		cmp[eax + 4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		retn	4
	found:
		mov		eax, [eax + 8]
	done :
		retn	4
	}
}

TESForm* GetRefFromString(char* mod, char *id)
{
	UInt32 itemID;
	itemID = (g_dataHandler->GetModIndex(mod) << 24) | strtoul(id, NULL, 0);
	return LookupFormByRefID(itemID);
}

bool IsInListRecursive(TESForm* item, BGSListForm* list)
{
	if (list->GetIndexOf(item) >= 0) return true;
	for (tList<TESForm>::Iterator iter = list->list.Begin(); !iter.End(); ++iter) {
		if (iter.Get() && iter.Get()->typeID == 85 && IsInListRecursive(item, DYNAMIC_CAST(iter.Get(), TESForm, BGSListForm))) return true;
	}
	return false;
}

bool IsInRepairListRecursive(TESForm* item, BGSListForm* list)
{
	if (IS_TYPE(item, TESObjectWEAP) && ((TESObjectWEAP*)item)->repairItemList.listForm && list->GetIndexOf(((TESObjectWEAP*)item)->repairItemList.listForm) >= 0) return true;
	for (tList<TESForm>::Iterator iter = list->list.Begin(); !iter.End(); ++iter) {
		if (iter.Get() && iter.Get()->typeID == 85 && IsInListRecursive(item, DYNAMIC_CAST(iter.Get(), TESForm, BGSListForm))) return true;
	}
	return false;
}

bool IsInAmmoListRecursive(TESForm* item, BGSListForm* list)
{
	if (IS_TYPE(item, TESObjectWEAP) && ((TESObjectWEAP*)item)->ammo.ammo && list->GetIndexOf(((TESObjectWEAP*)item)->ammo.ammo) >= 0) return true;
	for (tList<TESForm>::Iterator iter = list->list.Begin(); !iter.End(); ++iter) {
		if (iter.Get() && iter.Get()->typeID == 85 && IsInListRecursive(item, DYNAMIC_CAST(iter.Get(), TESForm, BGSListForm))) return true;
	}
	return false;
}

std::unordered_set <TESForm*> map_CraftingComponents;


inline void FillCraftingComponent()
{
	for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter)
	{
		if (mIter.Get()->typeID != 106) continue;
		for (ListNode<RecipeComponent>* node = ((TESRecipe*)mIter.Get())->inputs.Head(); node; node = node->next) {
			if (node->data && node->data->item) {
				map_CraftingComponents.emplace(node->data->item);
			}
		}
	}
}


inline bool IsCraftingComponent(TESForm* form)
{
	return (map_CraftingComponents.find(form) != map_CraftingComponents.end());
}