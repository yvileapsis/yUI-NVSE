#include <functions.h>
#include <Utilities.h>
#include <GameProcess.h>
#include <GameUI.h>
#include <GameAPI.h>
#include <GameData.h>
#include <settings.h>
#include <GameRTTI.h>
#include <SafeWrite.h>
#include <algorithm>

extern DataHandler* g_dataHandler;

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

void FillCraftingComponents()
{
	for (auto mIter = g_dataHandler->recipeList.Begin(); !mIter.End(); ++mIter)
	{
		for (ListNode<RecipeComponent>* node = mIter.Get()->inputs.Head(); node; node = node->next)
			if (node->data && node->data->item) g_CraftingComponents.emplace(node->data->item);
		for (ListNode<RecipeComponent>* node = mIter.Get()->outputs.Head(); node; node = node->next)
			if (node->data && node->data->item) g_CraftingProducts.emplace(node->data->item);
	}
}

float TESObjectWEAP::GetWeaponValue(UInt32 whichVal)
{
	if (!this) return false;
	switch (whichVal) {
	case eWeap_Type:				return eWeaponType;
	case eWeap_MinSpread:			return minSpread;
	case eWeap_Spread:				return spread;
	case eWeap_Proj:				return projectile ? projectile->refID : false;
	case eWeap_SightFOV:			return sightFOV;
	case eWeap_MinRange:			return minRange;
	case eWeap_Range:				return maxRange;
	case eWeap_AmmoUse:				return ammoUse;
	case eWeap_APCost:				return AP;
	case eWeap_CritDam:				return criticalDamage;
	case eWeap_CritChance:			return criticalPercent;
	case eWeap_CritEffect:			return criticalEffect ? criticalEffect->refID : false;
	case eWeap_FireRate:			return fireRate;
	case eWeap_AnimAttackMult:		return animAttackMult;
	case eWeap_RumbleLeft:			return rumbleLeftMotor;
	case eWeap_RumbleRight:			return rumbleRightMotor;
	case eWeap_RumbleDuration:		return rumbleDuration;
	case eWeap_RumbleWaveLength:	return rumbleWavelength;
	case eWeap_AnimShotsPerSec:		return animShotsPerSec;
	case eWeap_AnimReloadTime:		return animReloadTime;
	case eWeap_AnimJamTime:			return animJamTime;
	case eWeap_Skill:				return weaponSkill;
	case eWeap_ResistType:			return resistType;
	case eWeap_FireDelayMin:		return semiAutoFireDelayMin;
	case eWeap_FireDelayMax:		return semiAutoFireDelayMax;
	case eWeap_AnimMult:			return animMult;
	case eWeap_Reach:				return reach;
	case eWeap_IsAutomatic:			return IsAutomatic();
	case eWeap_HandGrip:			return HandGrip();
	case eWeap_ReloadAnim:			return reloadAnim;
	case eWeap_VATSChance:			return baseVATSChance;
	case eWeap_AttackAnim:			return AttackAnimation();
	case eWeap_NumProj:				return numProjectiles;
	case eWeap_AimArc:				return aimArc;
	case eWeap_LimbDamageMult:		return limbDamageMult;
	case eWeap_SightUsage:			return sightUsage;
	case eWeap_ReqStr:				return strRequired;
	case eWeap_ReqSkill:			return skillRequirement;
	case eWeap_LongBursts:			return weaponFlags2.IsSet(eFlag_LongBurst);
	case eWeap_Flags1:				return weaponFlags1.Get();
	case eWeap_Flags2:				return weaponFlags2.Get();
	case eWeap_HasScope:			return HasScope();
	case eWeap_IgnoresDTDR:			return IgnoresDTDR();
	case eWeap_SoundLevel:			return soundLevel;
	case eWeap_ClipSize:
	{
		if (const auto pClipRounds = DYNAMIC_CAST(this, TESForm, BGSClipRoundsForm); pClipRounds) return pClipRounds->clipRounds;
		break;
	}
	default: HALT("unknown weapon value"); break;
	}
	return false;
}

UInt32 TESObjectARMO::GetArmorValue(UInt32 whichVal)
{
	if (!this) return false;
	switch (whichVal) {
	case 1:	return bipedModel.bipedFlags & eBipedFlag_HeavyArmor ? 3 : bipedModel.bipedFlags & eBipedFlag_MediumArmor ? 2 : 1;
	case 2:	return bipedModel.bipedFlags & eBipedFlag_PowerArmor ? 1 : 0;
	case 3:	return bipedModel.bipedFlags & eBipedFlag_HasBackPack ? 1 : 0;
	case 4:	return armorRating;
	case 5:	return static_cast<UInt32>(damageThreshold);
	case 6: return bipedModel.partMask;
	default: HALT("unknown armor value"); break;
	}
	return false;
}

ExtraDataList* ExtraContainerChanges::EntryData::GetCustomExtra(UInt32 whichVal)
{
	if (extendData)
	{
		ListNode<ExtraDataList>* xdlIter = extendData->Head();
		do if (const auto xData = xdlIter->data; xData && xData->HasType(whichVal)) return xData;
		while ((xdlIter = xdlIter->next));
	}
	return nullptr;
}

UInt8 ContWeaponHasAnyMod(ContChangesEntry* weaponInfo)
{
	const auto xData = weaponInfo->GetCustomExtra(kExtraData_WeaponModFlags);
	if (!xData) return 0;
	const auto xModFlags = GetExtraType((*xData), WeaponModFlags);
	if (!xModFlags) return 0;
	return xModFlags->flags;
}

float ContGetHealthPercent(ContChangesEntry* itemInfo)
{
	const auto xData = itemInfo->GetCustomExtra(kExtraData_Health);
	if (!xData) return 0;
	const auto healthForm = DYNAMIC_CAST(itemInfo->form, TESForm, TESHealthForm);
	if (!healthForm) return 0;
	const auto xHealth = GetExtraType((*xData), Health);
	const float health = xHealth ? xHealth->health : static_cast<int>(healthForm->health);
	return (health / healthForm->health);
}

bool ContGetEquipped(ContChangesEntry* weaponInfo)
{
	const auto xData = weaponInfo->GetCustomExtra(kExtraData_Worn);
	const auto xDataLeft = weaponInfo->GetCustomExtra(kExtraData_WornLeft);
	if (xData && GetExtraType((*xData), Worn)) return true;
	if (xDataLeft && GetExtraType((*xDataLeft), WornLeft)) return true;
	return false;
}

UInt32 AlchemyItem::HasBaseEffectRestoresAV(const SInt32 avCode)
{
	for (auto iter = magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && !(setting->effectFlags & EffectSetting::kDetrimental))
				return effect->magnitude;
		//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
	return 0;
}

UInt32 AlchemyItem::HasBaseEffectDamagesAV(const SInt32 avCode)
{
	for (auto iter = magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (const auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && setting->effectFlags & EffectSetting::kDetrimental)
				return effect->magnitude;
		//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
	return 0;
}

bool AlchemyItem::IsAddictive()
{
	return withdrawalEffect && withdrawalEffect->refID;
}

bool AlchemyItem::IsFood()
{
	return alchFlags & 2;
}

bool AlchemyItem::IsMedicine()
{
	return alchFlags & 4;
}

bool AlchemyItem::IsPoison()
{
	EffectItem* effItem;
	EffectSetting* effSetting = nullptr;
	const ListNode<EffectItem>* iter = magicItem.list.list.Head();
	do
	{
		if (!((effItem = iter->data))) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	} while ((iter = iter->next));
	return effSetting != nullptr;
}

bool AlchemyItem::IsFoodAlt()
{
	return HasBaseEffectRestoresAV(kAVCode_Hunger) && HasBaseEffectRestoresAV(kAVCode_Hunger) > HasBaseEffectRestoresAV(kAVCode_Dehydration) ? true : false;
}

bool AlchemyItem::IsWaterAlt()
{
	return HasBaseEffectRestoresAV(kAVCode_Dehydration) && HasBaseEffectRestoresAV(kAVCode_Hunger) == 0 ? true : false;
}

bool HasBaseEffectChangesAV(TESForm* form, const int avCode)
{
	const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
	if (!armor) return false;
	const auto enchantment = armor->enchantable.enchantItem;
	if (!enchantment) return false;
	for (auto iter = enchantment->magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (const auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && setting->effectFlags & EffectSetting::kRecover)
				return true;
	return false;
}

TESForm* GetRefFromString(char* mod, char* id)
{
	const auto itemID = (g_dataHandler->GetModIndex(mod) << 24) | strtoul(id, nullptr, 0);
	return LookupFormByRefID(itemID);
}

bool IsCraftingComponent(TESForm* form)
{
	return g_CraftingComponents.contains(form);
}

bool IsCraftingProduct(TESForm* form)
{
	return g_CraftingProducts.contains(form);
}

bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle)
{
	const auto it = ra::search(strHaystack, strNeedle, [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin();
	return it != strHaystack.end();
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

void SetUIStringFull(const char* tochange, const char* changeto, UInt32 tileValue)
{
	HUDMainMenu::GetSingleton()->tile->SetStringRecursive(tileValue, changeto, tochange);
}

bool TryGetTypeOfForm(TESForm* form)
{
	__try {
		const auto whaa = form->typeID && form->refID;
		return whaa;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}


void Tile::SetStringRecursive(const UInt32 tileValue, const char* changeto, const char* tochange) {
	if (GetValue(tileValue))// && (!tochange || !_strcmpi(GetValue(tileValue)->str, tochange)))
	{
		SetString(tileValue, changeto, true);
	}
	for (auto iter = this->children.Head(); iter; iter = iter->next)
		iter->data->SetStringRecursive(tileValue, changeto, tochange);
}

void __fastcall CursorTileSetStringValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* src, char propagate)
{
	tile->SetFloat(kTileValue_zoom, -1, propagate);
	tile->SetFloat(kTileValue_systemcolor, 1, propagate);
}

void __fastcall CursorTileSetIntValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, int value)
{
	tile->SetFloat(kTileValue_visible, value, true);
	ThisCall(0xA0B350, InterfaceManager::GetSingleton()->cursor, 1, 0);
}

char* __fastcall StrFromINI(DWORD *address)
{
	return address ? reinterpret_cast<char*>(address[1]) : nullptr;
}

void (*RegTraitID)(const char*, UInt32) = (void (*)(const char*, UInt32))0x9FF8A0;
void RegisterTraitID(const char* var1, UInt32 var2) { RegTraitID(var1, var2);  }

void purefun()
{
	ConsoleQueueOrPrint("AAAAA");
}

__declspec(naked) void funpatch()
{
	static const UInt32 retnAddr = 0xA095D8;
	static const auto purefun2 = reinterpret_cast<UInt32>(purefun);
	__asm
	{
		cmp [ebp - 0x30], 2032
		jne welp
		call purefun2
	welp :
		cmp [ebp - 0x30], 0x7E8
		jmp retnAddr
	}
}

tList<TESObjectREFR>::_Node* iterDroppedItem;

void* __fastcall FixGetDroppedWeaponPre(ExtraDataList* extradatalist)
{
	const auto xDropped = static_cast<ExtraDroppedItemList*>(extradatalist->GetByType(kExtraData_DroppedItemList));
	if (!xDropped) return nullptr;
	iterDroppedItem = xDropped->itemRefs.Head();
	if (!iterDroppedItem) return nullptr;
	return iterDroppedItem->data;
}

void* __fastcall FixGetDroppedWeaponMid()
{
	iterDroppedItem = iterDroppedItem->next;
	if (!iterDroppedItem) return nullptr;
	return iterDroppedItem->data;
}

bool fixTablineSelected = false;

void FixTablineSelected()
{
	if (CdeclCall<bool>(0x702360) && InterfaceManager::IsMenuVisible(kMenuType_Inventory))	{
		if (fixTablineSelected)	{
			fixTablineSelected = false;
			const auto tabline = InventoryMenu::GetSingleton()->tile->GetChild("GLOW_BRANCH")->GetChild("IM_Tabline");
			if (!tabline) return;
			for (auto iter = tabline->children.Head(); iter; iter = iter->next)
				iter->data->SetFloat(kTileValue_mouseover, 0, false);
		}
	} else {
		fixTablineSelected = true;
	}
}