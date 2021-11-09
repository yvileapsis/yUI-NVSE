#pragma once
#include "functions.h"

#include <unordered_set>
#include <PluginAPI.h>
#include "GameAPI.h"

void PrintAndClearQueuedConsoleMessages()
{
	for (auto iter = queuedConsoleMessages.Begin(); !iter.End(); ++iter) {
		Console_Print("%s", iter.Get());
	}
	queuedConsoleMessages.DeleteAll();
}

void FillCraftingComponents()
{
	for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter)
	{
		if (mIter.Get()->typeID != 106) continue;
		for (ListNode<RecipeComponent>* node = dynamic_cast<TESRecipe*>(mIter.Get())->inputs.Head(); node; node = node->next) {
			if (node->data && node->data->item) {
				g_CraftingComponents.emplace(node->data->item);
			}
		}
	}
}

void ConsoleQueueOrPrint(const char* str, int len)
{
	if (*reinterpret_cast<ConsoleManager**>(0x11D8CE8) || g_dataHandler) { // g_dataHandler will be non-null if Deferred init has been called
		Console_Print("%s", str);
	}
	else {
		const auto errorStr = static_cast<char*>(GameHeapAlloc(len + 1));
		strcpy(errorStr, str);
		queuedConsoleMessages.Append(errorStr);
	}
}

bool Menu::GetTemplateExists(const char* templateName)
{
	for (ListNode<TemplateData>* node = menuTemplates.Head(); node; node = node->next) {
		if (node->data && (!_strcmpi(node->data->templateName, templateName))) { return true; }
	}
	return false;
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
	MenuButton_DownloadsClick();
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


float TESObjectWEAP::GetWeaponValue(UInt32 whichVal)
{
	if (this) {
		switch (whichVal) {
		case eWeap_Type:				return this->eWeaponType;
		case eWeap_MinSpread:			return this->minSpread;
		case eWeap_Spread:				return this->spread;
		case eWeap_Proj:
		{
			if (BGSProjectile * pProj = this->projectile; pProj) {
				return pProj->refID;
			}
		}
		break;
		case eWeap_SightFOV:			return this->sightFOV;
		case eWeap_MinRange:			return this->minRange;
		case eWeap_Range:				return this->maxRange;
		case eWeap_AmmoUse:				return this->ammoUse;
		case eWeap_APCost:				return this->AP;
		case eWeap_CritDam:				return this->criticalDamage;
		case eWeap_CritChance:			return this->criticalPercent;
		case eWeap_CritEffect:
		{
			if (SpellItem * pSpell = this->criticalEffect; pSpell) {
				return pSpell->refID;
			}
		}
		break;
		case eWeap_FireRate:			return this->fireRate;
		case eWeap_AnimAttackMult:		return this->animAttackMult;
		case eWeap_RumbleLeft:			return this->rumbleLeftMotor;
		case eWeap_RumbleRight:			return this->rumbleRightMotor;
		case eWeap_RumbleDuration:		return this->rumbleDuration;
		case eWeap_RumbleWaveLength:	return this->rumbleWavelength;
		case eWeap_AnimShotsPerSec:		return this->animShotsPerSec;
		case eWeap_AnimReloadTime:		return this->animReloadTime;
		case eWeap_AnimJamTime:			return this->animJamTime;
		case eWeap_Skill:				return this->weaponSkill;
		case eWeap_ResistType:			return this->resistType;
		case eWeap_FireDelayMin:		return this->semiAutoFireDelayMin;
		case eWeap_FireDelayMax:		return this->semiAutoFireDelayMax;
		case eWeap_AnimMult:			return this->animMult;
		case eWeap_Reach:				return this->reach;
		case eWeap_IsAutomatic:			return this->IsAutomatic();
		case eWeap_HandGrip:			return this->HandGrip();
		case eWeap_ReloadAnim:			return this->reloadAnim;
		case eWeap_VATSChance:			return this->baseVATSChance;
		case eWeap_AttackAnim:			return this->AttackAnimation();
		case eWeap_NumProj:				return this->numProjectiles;
		case eWeap_AimArc:				return this->aimArc;
		case eWeap_LimbDamageMult:		return this->limbDamageMult;
		case eWeap_SightUsage:			return this->sightUsage;
		case eWeap_ReqStr:				return this->strRequired;
		case eWeap_ReqSkill:			return this->skillRequirement;
		case eWeap_LongBursts:			return this->weaponFlags2.IsSet(TESObjectWEAP::eFlag_LongBurst);
		case eWeap_Flags1:				return this->weaponFlags1.Get();
		case eWeap_Flags2:				return this->weaponFlags2.Get();
		case eWeap_HasScope:			return this->HasScope();
		case eWeap_IgnoresDTDR:			return this->IgnoresDTDR();
		case eWeap_SoundLevel:			return this->soundLevel;
		case eWeap_ClipSize:
		{
			auto pClipRounds = DYNAMIC_CAST(this, TESForm, BGSClipRoundsForm);
			if (pClipRounds) { return pClipRounds->clipRounds; }
		}
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

UInt32 TESObjectARMO::GetArmorValue(UInt32 whichVal)
{
	if (this) {
		switch (whichVal) {
		case 1: {
			TESBipedModelForm* biped = &this->bipedModel;
			return (biped->bipedFlags & eBipedFlag_HeavyArmor) ? 3 : ((biped->bipedFlags & eBipedFlag_MediumArmor) ? 2 : 1);
		}
		case 2: {
			TESBipedModelForm* biped = &this->bipedModel;
			return (biped->bipedFlags & eBipedFlag_PowerArmor) ? 1 : 0;
		}
		case 3: {
			TESBipedModelForm* biped = &this->bipedModel;
			return (biped->bipedFlags & eBipedFlag_HasBackPack) ? 1 : 0;
		}
		case 4:						return this->armorRating;
		case 5:						return static_cast<UInt32>(this->damageThreshold);
		case 6: {
			TESBipedModelForm* biped = &this->bipedModel;
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

UInt32 AlchemyItem::HasBaseEffectRestoresAV(int avCode)
{
	for (auto iter = this->magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && !(setting->effectFlags & EffectSetting::kDetrimental))
				return effect->magnitude;
		//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
	return 0;
}

UInt32 AlchemyItem::HasBaseEffectDamagesAV(const int avCode)
{
	for (auto iter = this->magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && setting->effectFlags & EffectSetting::kDetrimental)
				return effect->magnitude;
		//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
	return 0;
}

bool AlchemyItem::IsAddictive()
{
	return this->withdrawalEffect && this->withdrawalEffect->refID ? true : false;
}

bool AlchemyItem::IsFood()
{
	return (this->alchFlags & 2) ? true : false;
}

bool AlchemyItem::IsMedicine()
{
	return (this->alchFlags & 4) ? true : false;
}

bool AlchemyItem::IsPoison()
{
	EffectItem* effItem;
	EffectSetting* effSetting = nullptr;
	ListNode<EffectItem>* iter = magicItem.list.list.Head();
	do
	{
		if (!(effItem = iter->data)) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	} while (iter = iter->next);
	return effSetting != nullptr;
}

bool AlchemyItem::IsFoodAlt()
{
	return this->HasBaseEffectRestoresAV(kAVCode_Hunger) && this->HasBaseEffectRestoresAV(kAVCode_Hunger) > this->HasBaseEffectRestoresAV(kAVCode_Dehydration) ? true : false;
}

bool AlchemyItem::IsWaterAlt()
{
	return this->HasBaseEffectRestoresAV(kAVCode_Dehydration) && this->HasBaseEffectRestoresAV(kAVCode_Hunger) == 0 ? true : false;
}

bool HasBaseEffectChangesAV(TESForm* form, int avCode)
{
	auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
	TESEnchantableForm* enchantable = &armor->enchantable;
	if (!enchantable) return false;
	EnchantmentItem* enchantment = enchantable->enchantItem;
	if (!enchantment) return false;
	for (auto iter = enchantment->magicItem.list.list.Begin(); !iter.End(); ++iter)
	{
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
		{
			if (const auto setting = effect->setting; setting && (setting->effectFlags & EffectSetting::kRecover))
			{
				return true;
				//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
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
		xor edx, edx
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

TESForm* GetRefFromString(char* mod, char* id)
{
	UInt32 itemID;
	itemID = (g_dataHandler->GetModIndex(mod) << 24) | strtoul(id, NULL, 0);
	return LookupFormByRefID(itemID);
}

bool IsInListRecursive(TESForm* item, TESForm* list)
{
	if (item && list && item->refID == list->refID) return true;
	if (list->typeID != 85) return false;
//	if (dynamic_cast<BGSListForm*>(list)->GetIndexOf(item) >= 0) return true;
	for (auto iter = dynamic_cast<BGSListForm*>(list)->list.Begin(); !iter.End(); ++iter) {
		if (iter.Get() && IsInListRecursive(item, iter.Get())) return true;
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

bool IsCraftingComponent(TESForm* form)
{
	return (g_CraftingComponents.find(form) != g_CraftingComponents.end());
}

bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle)
{
	const auto it = ra::search(strHaystack, strNeedle, [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin();
	return it != strHaystack.end();
}

void Log(const std::string& msg)
{
	_MESSAGE("%s", msg.c_str());
	if (g_logLevel == 2)
		Console_Print("kNVSE: %s", msg.c_str());
}

void DebugLog(const std::string& msg)
{
#if _DEBUG
	_MESSAGE("%s", msg.c_str());
	if (g_logLevel == 2)
		Console_Print("kNVSE: %s", msg.c_str());
#endif
}

int HexStringToInt(const std::string& str)
{
	char* p;
	const auto id = strtoul(str.c_str(), &p, 16);
	if (*p == 0)
		return id;
	return -1;
}

void DebugPrint(const std::string& str)
{
	if (g_logLevel == 1)
		Console_Print("kNVSE: %s", str.c_str());
	Log(str);
}

bool IsPlayersOtherAnimData(AnimData* animData)
{
	if (g_thePlayer->IsThirdPerson() && animData == g_thePlayer->firstPersonAnimData)
		return true;
	if (!g_thePlayer->IsThirdPerson() && animData == g_thePlayer->baseProcess->GetAnimData())
		return true;
	return false;
}

AnimData* GetThirdPersonAnimData(AnimData* animData)
{
	if (animData == g_thePlayer->firstPersonAnimData)
		return g_thePlayer->baseProcess->GetAnimData();
	return animData;
}

void PatchPause(UInt32 ptr)
{
	SafeWriteBuf(ptr, "\xEB\xFE", 2);
}

extern TileMenu** g_tileMenuArray;


TileMenu* TileMenu::GetTileMenu(UInt32 menuID)
{
	return menuID ? g_tileMenuArray[menuID - kMenuType_Min] : nullptr;
}


Tile* Tile::InjectUIXML(const char* str)
{
	return this ? this->ReadXML(str) : nullptr;
}
