#include <GameObjects.h>
#include <GameRTTI.h>
#include <GameExtraData.h>
#include <GameTasks.h>
#include <GameUI.h>
#include <SafeWrite.h>
#include <NiObjects.h>
#include <unordered_set>

#include "GameProcess.h"
#include "GameSettings.h"

static constexpr UInt32 s_TESObject_REFR_init						= 0x0055A2F0;	// TESObject_REFR initialization routine (first reference to s_TESObject_REFR_vtbl)
static constexpr UInt32 s_TESObjectREFR__GetContainer				= 0x0055D310;	// First call in REFR::RemoveItem
static constexpr UInt32 s_TESObjectREFR_Set3D						= 0x005702E0;	// void : (const char*)
static constexpr UInt32 s_PlayerCharacter_GetCurrentQuestTargets	= 0x00952BA0;	// BuildedQuestObjectiveTargets* : (void)
static constexpr UInt32 s_PlayerCharacter_GenerateNiNode			= 0x0094E1D0; // Func0072
static constexpr UInt32 kPlayerUpdate3Dpatch						= 0x0094EB7A;
static constexpr UInt32 TESObjectREFR_Set3D							= 0x0094EB40;
static constexpr UInt32 ValidBip01Names_Destroy						= 0x00418E00;
static constexpr UInt32 ExtraAnimAnimation_Destroy					= 0x00418D20;
static constexpr UInt32 RefNiRefObject_ReplaceNiRefObject			= 0x0066B0D0;

static constexpr UInt32 kg_Camera1st	= 0x011E07D0;
static constexpr UInt32 kg_Camera3rd	= 0x011E07D4;
static constexpr UInt32 kg_Bip			= 0x011E07D8;
static constexpr UInt8 kPlayerUpdate3DpatchFrom	= 0x0B6;
static constexpr UInt8 kPlayerUpdate3DpatchTo	= 0x0EB;

static NiObject **	g_3rdPersonCameraNode =				(NiObject**)kg_Camera3rd;
static NiObject **	g_1stPersonCameraBipedNode =		(NiObject**)kg_Bip;
static NiObject **	g_1stPersonCameraNode =				(NiObject**)kg_Camera1st;

ScriptEventList* TESObjectREFR::GetEventList() const
{
	if (BSExtraData* xData = extraDataList.GetByType(kExtraData_Script))
		if (const ExtraScript* xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript))
			return xScript->eventList;
	return nullptr;
}

TESContainer* TESObjectREFR::GetContainer()
{
	if (IsActor()) return &static_cast<TESActorBase*>(baseForm)->container;
	if (baseForm->typeID == kFormType_TESObjectCONT)
		return &static_cast<TESObjectCONT*>(baseForm)->container;
	return nullptr;
}

bool TESObjectREFR::IsMapMarker() { return baseForm ? baseForm->refID == 0x010 : false; }

bool TESObjectREFR::Update3D_v1c()
{
	UInt8 kPlayerUpdate3DpatchFrom = 0x0B6;
	UInt8 kPlayerUpdate3DpatchTo = 0x0EB;

	if (this == PlayerCharacter::GetSingleton())
	{
		static constexpr UInt32 kPlayerUpdate3Dpatch = 0x0094EB7A;
		// Lets try to allow unloading the player3D never the less...
		SafeWrite8(kPlayerUpdate3Dpatch, kPlayerUpdate3DpatchTo);
	}

	Set3D(nullptr, true);
	ModelLoader::GetSingleton()->QueueReference(this, 1, 0);
	return true;
}

bool TESObjectREFR::Update3D()
{
	if (this == PlayerCharacter::GetSingleton()) {
		// Lets try to allow unloading the player3D never the less...
		SafeWrite8(kPlayerUpdate3Dpatch, kPlayerUpdate3DpatchTo);
	}

	Set3D(nullptr, true);
	ModelLoader::GetSingleton()->QueueReference(this, 1, false);
	return true;
}

TESActorBase* Actor::GetActorBase()
{
	if (modIndex == 0xFF)
		if (const auto xLvlCre = GetExtraType(extraDataList, LeveledCreature)) return xLvlCre->actorBase;
	return static_cast<TESActorBase*>(baseForm);
}

TESObjectREFR* TESObjectREFR::Create(bool bTemp)
{
	const auto refr = static_cast<TESObjectREFR*>(FormHeap_Allocate(sizeof(TESObjectREFR)));
	ThisStdCall(s_TESObject_REFR_init, refr);
//	if (bTemp) MarkAsTemporary();
	return refr;
}

Float64 TESObjectREFR::GetInventoryWeight()
{
	if (const auto xChanges = reinterpret_cast<ExtraContainerChanges*>(this->extraDataList.GetByType(kExtraData_ContainerChanges)))
		return xChanges->data->GetInventoryWeight();
	return 0;
}

std::vector<ContChangesEntry*> TESObjectREFR::GetAllItems()
{
	std::vector<ContChangesEntry*> vector;
	
	ContChangesEntry* entry = nullptr;
	void* iterator = nullptr;
	ThisCall(0x575510, this, 0, &iterator, &entry);
	if (iterator && entry) for (auto iter = ThisCall<ContChangesEntry*>(0x4CA330, entry, iterator); iter; iter = ThisCall<ContChangesEntry*>(0x4CA330, entry, iterator))
		if (iter->ShouldDisplay()) vector.push_back(iter);

	// Vanilla method is more than twice as slow but it does a lot of stuff, like removing LvlItem garbage, combining throwables, etc

	/*
	if (const auto entryList = GetContainerChangesList()) for (auto iter : *entryList) 
		if (iter->countDelta > 0) vector.push_back(iter);

	if (const auto container = GetContainer()) for (const auto iter : container->formCountList)
		if (iter->form->typeID != kFormType_TESLevItem && iter->count > 0) vector.push_back(ContChangesEntry::Create(iter->form, iter->count));
	*/

	// Process linked ref for vendor containers, recursive, but hopefully no one links container on itself
	if (const auto xLinkedRef = reinterpret_cast<ExtraLinkedRef*>(this->extraDataList.GetByType(kExtraData_LinkedRef)))
		if (const auto linked = xLinkedRef->linkedRef)
		{
			const auto newvector = linked->GetAllItems();
			vector.insert(vector.end(), newvector.begin(), newvector.end());
		}

	if (this->IsActor())
		if (const auto xDropped = reinterpret_cast<ExtraDroppedItemList*>(this->extraDataList.GetByType(kExtraData_DroppedItemList)))
			for (const auto iter : xDropped->itemRefs) vector.push_back(ContChangesEntry::Create(iter, 1));

	return vector;
}

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj)	// For LevelledForm, find real baseForm, not temporary one.
{
	if (thisObj)
		if (const auto pXCreatureData = DYNAMIC_CAST(thisObj->extraDataList.GetByType(kExtraData_LeveledCreature), BSExtraData, ExtraLeveledCreature); pXCreatureData && pXCreatureData->baseForm)
			return pXCreatureData->baseForm;
	if (thisObj && thisObj->baseForm)
		return thisObj->baseForm;
	return nullptr;
}

void Actor::EquipItem(TESForm * objType, UInt32 equipCount, ExtraDataList* itemExtraList, UInt32 unk3, bool lockEquip, UInt32 unk5)
{
	ThisStdCall(0x0088C650, this, objType, equipCount, itemExtraList, unk3, lockEquip, unk5);
}

void Actor::UnequipItem(TESForm* objType, UInt32 unk1, ExtraDataList* itemExtraList, UInt32 unk3, bool lockUnequip, UInt32 unk5)
{
	ThisStdCall(0x0088C790, this, objType, unk1, itemExtraList, unk3, lockUnequip, unk5);
}

EquippedItemsList Actor::GetEquippedItems()
{
	EquippedItemsList itemList;
	if (const auto xChanges = static_cast<ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges))) {
		ContChangesArray outEntryData;
		ContChangesExtendArray outExtendData;
		const UInt32 count = xChanges->GetAllEquipped(outEntryData, outExtendData);
		for (UInt32 i = 0; i < count ; i++) itemList.push_back(outEntryData[i]->form);
	}
	return itemList;
}

ContChangesArray Actor::GetEquippedEntryDataList()
{
	ContChangesArray itemArray;
	ContChangesExtendArray outExtendData;
	if(const auto xChanges = static_cast<ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges)))
		xChanges->GetAllEquipped(itemArray, outExtendData);
	return itemArray;
}

ContChangesExtendArray	Actor::GetEquippedExtendDataList()
{
	ContChangesArray itemArray;
	ContChangesExtendArray outExtendData;
	if(const auto xChanges = static_cast<ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges)))
		xChanges->GetAllEquipped(itemArray, outExtendData);
	return outExtendData;
}

Float64 Actor::GetCalculatedSpread(UInt32 mode, ContChangesEntry* entry)
{
	if (!entry) entry = reinterpret_cast<ContChangesEntry*>(this->baseProcess->GetWeaponInfo());

	if (!entry || !entry->form) return 0;

	Float64 totalSpread = 0;

	if (mode == 0)
	{
		const bool hasDecreaseSpreadEffect = ThisStdCall<bool>(0x4BDA70, entry, 3);
		const auto minSpread = ThisStdCall<double>(0x524B80, entry->form, hasDecreaseSpreadEffect);
		const double weapSpread = ThisStdCall<float>(0x524BE0, entry->form, hasDecreaseSpreadEffect);

		totalSpread = (weapSpread * ThisStdCall<double>(0x8B0DD0, this, 1) + minSpread) * 0.01745329238474369;

		const auto eqAmmo = ThisStdCall<TESAmmo*>(0x525980, entry->form, static_cast<MobileObject*>(this));
		totalSpread = CdeclCall<Float64>(0x59A030, 3, (eqAmmo ? &eqAmmo->effectList : nullptr), static_cast<Float32>(totalSpread));

		if (this != PlayerCharacter::GetSingleton())
		{
			const auto spreadPenalty = ThisStdCall<double>(0x8B0DD0, this, 2);

			Setting* fNPCMaxGunWobbleAngle;
			GameSettingCollection::GetSingleton()->GetGameSetting("fNPCMaxGunWobbleAngle", &fNPCMaxGunWobbleAngle);

			totalSpread += spreadPenalty * fNPCMaxGunWobbleAngle->data.f * 0.01745329238474369;
		}

		const auto noIdea = ThisStdCall<HighProcess*>(0x8D8520, this)->angle1D0;
		totalSpread = totalSpread + noIdea;

		if (entry->HasWeaponMod(0xC)) totalSpread *= ThisStdCall<float>(0x4BCF60, entry->form, 0xC, 1);
	}
	else if (mode == 1)
	{
		if (!entry->form || reinterpret_cast<TESObjectWEAP*>(entry->form)->IsMeleeWeapon())
			totalSpread = 1.0;
		else
			totalSpread = ThisStdCall<double>(0x8B0DD0, this, 2);

		if (!this->IsDoingAttackAnim())
		{
			Setting* fNonAttackGunWobbleMult;
			GameSettingCollection::GetSingleton()->GetGameSetting("fNonAttackGunWobbleMult", &fNonAttackGunWobbleMult);

			totalSpread = totalSpread * fNonAttackGunWobbleMult->data.f;
		}

		totalSpread *= 0.01745329238474369;
	}
	else if (mode == 2)
	{
		totalSpread = ThisStdCall<double>(0x8B0DD0, this, 0); 
		Setting* fGunWobbleMultScope;
		GameSettingCollection::GetSingleton()->GetGameSetting("fGunWobbleMultScope", &fGunWobbleMultScope);
		totalSpread = totalSpread * fGunWobbleMultScope->data.f;

	}
	return totalSpread;

}

bool Actor::IsCombatTarget(const Actor* source)
{
	if (source->isInCombat)
		for (const auto iter : *source->combatTargets)
			if (iter == this) return true;
	return false;
}

bool Actor::IsHostileCompassTarget() {
	for (const auto iter : *PlayerCharacter::GetSingleton()->compassTargets)
		if (iter->isHostile && iter->target == this) return true;
	return false;
}

__declspec(naked) double __fastcall AdjustDmgByDifficulty(ActorHitData *hitData, PlayerCharacter* g_thePlayer = PlayerCharacter::GetSingleton())
{
	__asm
	{
		mov		edx, g_thePlayer
		mov		eax, 0x119B310
		cmp		dword ptr [ecx+4], edx
		jz		isPlayer
		add		eax, 0x14
	isPlayer:
		mov		edx, [edx+0x7B8]
		mov		eax, [eax+edx*4]
		fld		dword ptr [ecx+0x14]
		fmul	dword ptr [eax+4]
		retn
	}
}

Float32 Actor::GetHitDataValue(UInt32 valueType) const
{
	if (!baseProcess || (baseProcess->processLevel > 1)) return 0;
	ActorHitData* hitData = baseProcess->GetLastHitData();
	if (!hitData) return 0;
	switch (valueType)
	{
	case 0:	return AdjustDmgByDifficulty(hitData);
	case 1:	return hitData->limbDmg;
	case 2:	return hitData->flags & 0x80000000 ? 1 : 0;
	case 3:	return hitData->wpnBaseDmg;
	case 4:	return hitData->fatigueDmg;
	case 5:	return hitData->armorDmg;
	}
	return 0;
}

Float32 Actor::GetActorValue(ActorValueCode avcode)
{
	return this->avOwner.GetActorValue(avcode);
}


bool TESObjectREFR::IsCrimeOrEnemy()
{
	const auto actor = static_cast<Actor*>(this);
	return ThisCall(0x579690, this) && (!this->IsActor() || !actor->isTeammate) || this->IsActor() && (actor->
		IsCombatTarget(PlayerCharacter::GetSingleton()) || actor->IsHostileCompassTarget());
}

bool TESObjectREFR::GetInventoryItems(InventoryItemsMap &invItems)
{
	TESContainer *container = GetContainer();
	if (!container) return false;
	const auto xChanges = static_cast<ExtraContainerChanges*>(extraDataList.GetByType(kExtraData_ContainerChanges));
	const auto entryList = (xChanges && xChanges->data) ? xChanges->data->objList : nullptr;
	if (!entryList) return false;

	TESForm *item;
	SInt32 countDelta;
	ContChangesEntry *entry;

	for (const auto contIter : container->formCountList)
	{
		item = contIter->form;
		if (item->typeID == kFormType_TESObjectCONT || invItems.contains(item)) continue;
		SInt32 contCount = container->GetCountForForm(item);
		if ((entry = entryList->FindForItem(item)))
		{
			countDelta = entry->countDelta;
			if (entry->HasExtraLeveledItem())
				contCount = countDelta;
			else contCount += countDelta;
		}
		if (contCount > 0)
			invItems.emplace(item, InventoryItemData(contCount, entry));
	}

	for (const auto entry_data : *entryList)
	{
		item = entry_data->form;
		if (invItems.contains(item)) continue;
		countDelta = entry_data->countDelta;
		if (countDelta > 0)
			invItems.emplace(item, InventoryItemData(countDelta, entry_data));
	}

	return !invItems.empty();
}

__declspec(naked) ContChangesList* TESObjectREFR::GetContainerChangesList()
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax + 0xC]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		done:
		retn
	}
}

__declspec(naked) float __vectorcall Point2Distance(const NiPoint3& pt1, const NiPoint3& pt2)
{
	__asm
	{
		movq	xmm0, qword ptr[ecx]
		movq	xmm1, qword ptr[edx]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) float __vectorcall Point3Distance(const NiPoint3& pt1, const NiPoint3& pt2)
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [edx]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		xorps	xmm1, xmm1
		haddps	xmm0, xmm1
		haddps	xmm0, xmm1
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) bool __fastcall TESObjectREFR::GetInSameCellOrWorld(TESObjectREFR *target) const
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		hasCell1
		push	edx
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		pop		edx
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	hasCell1:
		mov		ecx, [edx+0x40]
		test	ecx, ecx
		jnz		hasCell2
		push	eax
		push	kExtraData_PersistentCell
		lea		ecx, [edx+0x44]
		call	BaseExtraList::GetByType
		pop		edx
		test	eax, eax
		jz		done
		mov		ecx, [eax+0xC]
		mov		eax, edx
	hasCell2:
		cmp		eax, ecx
		jz		retnTrue
		mov		eax, [eax+0xC0]
		test	eax, eax
		jz		done
		cmp		eax, [ecx+0xC0]
	retnTrue:
		setz	al
	done:
		retn
	}
}

float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR* target)
{
	return this->GetInSameCellOrWorld(target) ? Point3Distance(*this->GetPos(), *target->GetPos()) : FLT_MAX;
}

float __vectorcall TESObjectREFR::GetDistance2D(TESObjectREFR* target)
{
	return this->GetInSameCellOrWorld(target) ? Point2Distance(*this->GetPos(), *target->GetPos()) : FLT_MAX;
}

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlockByName(const char *nameStr)	//	str of NiFixedString
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		eax, [esi]
		add		esi, 4
		test	eax, eax
		jz		iterHead
		cmp		[eax+8], edx
		jz		found
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0xC], 0x6815C0
		jnz		iterHead
		mov		ecx, eax
		call	NiNode::GetBlockByName
		test	eax, eax
		jz		iterHead
	found:
		pop		edi
		pop		esi
		retn
		ALIGN 16
	iterEnd:
		xor		eax, eax
		pop		edi
		pop		esi
	done:
		retn
	}
}
__declspec(naked) NiAVObject* __fastcall NiNode::GetBlock(const char* blockName)
{
	__asm
	{
		cmp[edx], 0
		jz		retnNULL
		push	ecx
		push	edx
		__asm mov eax, 0xA5B690 __asm call eax
		pop		ecx
		pop		ecx
		test	eax, eax
		jz		done
		lock dec dword ptr[eax - 8]
		jz		retnNULL
		cmp[ecx + 8], eax
		jz		found
		mov		edx, eax
		call	NiNode::GetBlockByName
		retn
	found :
		mov		eax, ecx
		retn
	retnNULL :
		xor eax, eax
	done :
		retn
	}
}

__declspec(naked) NiNode* __fastcall NiNode::GetNode(const char *nodeName) const
{
	__asm
	{
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		xor		edx, edx
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0xC], 0x6815C0
		cmovnz	eax, edx
	done:
		retn
	}
}

__declspec(naked) NiNode* TESObjectREFR::GetNiNode()
{
	__asm
	{
		mov		eax, [ecx + 0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax + 0x14]
		cmp		dword ptr[ecx + 0xC], 0x14
		jnz		done
		cmp		byte ptr[ecx + 0x64A], 0
		jnz		done
		mov		eax, [ecx + 0x694]
	done:
		retn
	}
}

__declspec(naked) NiNode* __fastcall TESObjectREFR::GetNode(const char* nodeName)
{
	__asm
	{
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		cmp[edx], 0
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		xor edx, edx
		mov		ecx, [eax]
		cmp		dword ptr[ecx + 0xC], 0x6815C0
		cmovnz	eax, edx
	done :
		retn
	}
}

// jazz code
__declspec(naked) NiAVObject* __fastcall GetNifBlock2(TESObjectREFR *thisObj, UInt32 pcNode, const char *blockName)
{
	__asm
	{
		test	dl, dl
		jz		notPlayer
		cmp		dword ptr [ecx+0xC], 0x14
		jnz		notPlayer
		test	dl, 1
		jz		get1stP
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		jmp		gotRoot
	get1stP:
		mov		eax, [ecx+0x694]
		jmp		gotRoot
	notPlayer:
		call	TESObjectREFR::GetNiNode
	gotRoot:
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		cmp		[edx], 0
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
	done:
		retn	4
	}
}

NiAVObject* TESObjectREFR::GetNifBlock(UInt32 pcNode, const char* blockName)
{
	return GetNifBlock2(this, pcNode, blockName);
}

ExtraDataList* ContChangesEntry::GetEquippedExtra()
{
	return GetCustomExtra(kExtraData_Worn);
}

ExtraDataList* ContChangesEntry::GetCustomExtra(UInt32 whichVal)
{
	if (!extendData) return nullptr;
	const TListNode<ExtraDataList>* xdlIter = extendData->Head();
	do if (const auto xData = xdlIter->data; xData && xData->HasType(whichVal)) return xData;
	while ((xdlIter = xdlIter->next));
	return nullptr;
}

BSExtraData* ContChangesEntry::GetExtraData(UInt32 whichVal)
{
	const auto extra = GetCustomExtra(whichVal);
	return extra ? extra->GetByType(whichVal) : nullptr;
}

UInt32 ContChangesEntry::GetWeaponNumProjectiles(Actor* owner)
{
	return ThisCall<UInt8>(0x525B20, this->form, this->HasWeaponMod(0xC), 0, owner);
}

inline bool ContChangesEntry::ShouldDisplay()
{
	return this->form->IsItemPlayable() && *this->form->GetTheName();
}

UInt8 ContChangesEntry::GetWeaponMod()
{
	const auto xModFlags = reinterpret_cast<ExtraWeaponModFlags*>(this->GetExtraData(kExtraData_WeaponModFlags));
	return xModFlags ? xModFlags->flags : 0;
}

Float64 ExtraContainerChanges::EntryData::GetHealthPercentAlt(bool axonisFix)
{
	Float64 healthPer = -1;

	if (const auto healthForm = DYNAMIC_CAST(form->TryGetREFRParent(), TESForm, TESHealthForm))
	{
		const auto xHealth = form->GetIsReference() ? reinterpret_cast<ExtraHealth*>(reinterpret_cast<TESObjectREFR*>(form)->extraDataList.GetByType(kExtraData_Health)) : reinterpret_cast<ExtraHealth*>(this->GetExtraData(kExtraData_Health));

		healthPer = xHealth ? xHealth->health / ((int)healthForm->health + (
				            HasWeaponMod(TESObjectWEAP::kWeaponModEffect_IncreaseMaxCondition)
					            ? reinterpret_cast<TESObjectWEAP*>(form)->GetEffectModValue(TESObjectWEAP::kWeaponModEffect_IncreaseMaxCondition)
					            : 0))
							: 1;
	}
	else
	{
		const auto destructible = DYNAMIC_CAST(form->TryGetREFRParent(), TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data)
		{
			const auto xObjHealth = form->GetIsReference() ? reinterpret_cast<ExtraObjectHealth*>(reinterpret_cast<TESObjectREFR*>(form)->extraDataList.GetByType(kExtraData_ObjectHealth)) : reinterpret_cast<ExtraObjectHealth*>(this->GetExtraData(kExtraData_ObjectHealth));
			healthPer = xObjHealth ? xObjHealth->health / (int) destructible->data->health : 1;
		}
	}
	return axonisFix ? healthPer >= 0.995 ? 1 : healthPer >= 95 ? 0.95 : healthPer : healthPer;
}

bool ContChangesEntry::GetEquipped()
{
	if (reinterpret_cast<ExtraWorn*>(this->GetExtraData(kExtraData_Worn))) return true;
	if (reinterpret_cast<ExtraWornLeft*>(this->GetExtraData(kExtraData_WornLeft))) return true;
	return false;
}

__declspec(naked) ContChangesEntry* ContChangesList::FindForItem(TESForm* item)
{
	__asm
	{
		mov		edx, [esp + 4]
	listIter:
		mov		eax, [ecx]
		test	eax, eax
		jz		listNext
		cmp[eax + 8], edx
		jz		done
	listNext :
		mov		ecx, [ecx + 4]
		test	ecx, ecx
		jnz		listIter
		xor eax, eax
	done :
		retn	4
	}
}

PlayerCharacter*		PlayerCharacter::GetSingleton() { return *reinterpret_cast<PlayerCharacter**>(0x011DEA3C); }
QuestObjectiveTargets*	PlayerCharacter::GetCurrentQuestObjectiveTargets() { return ThisStdCall<QuestObjectiveTargets*>(s_PlayerCharacter_GetCurrentQuestTargets, this); }
TESObjectREFR*			PlayerCharacter::GetPlacedMarkerOrTeleportLink() { return ThisStdCall<TESObjectREFR*>(0x77A400, this); }
HighProcess*			PlayerCharacter::GetHighProcess() { return reinterpret_cast<HighProcess*>(baseProcess); }

bool Explosion::CanStoreAmmo()
{
	if (!source) return false;
	if (source->typeID != kFormType_Character && source->typeID != kFormType_Creature) return false;
	if (!source->IsActor()) return false;
	if (ammo && (ammo->typeID != kFormType_TESAmmo && ammo->typeID != kFormType_TESObjectWEAP)) return false; // temporary check until I resolve saving issue
	return true;
}
/*
void Actor::FireWeapon()
{
	animGroupId110 = static_cast<UInt32>(GetAnimData()->GetNextAttackGroupID());
	this->baseProcess->SetQueuedIdleFlag(kIdleFlag_FireWeapon);
	Actor::HandleQueuedAnimFlags(this); //Actor::HandleQueuedIdleFlags
}

void Actor::EjectFromWeapon(TESObjectWEAP* weapon)
{
	if (weapon && Actor::IsDoingAttackAnimation(this) && !weapon->IsMeleeWeapon() && !weapon->IsAutomatic())
	{
		baseProcess->SetQueuedIdleFlag(kIdleFlag_AttackEjectEaseInFollowThrough);
		Actor::HandleQueuedAnimFlags(this);
	}
}
*/
TESObjectWEAP* Actor::GetWeaponForm() const
{
	const auto weaponInfo = this->baseProcess->GetWeaponInfo();
	if (!weaponInfo) return nullptr;
	return weaponInfo->weapon;
}

bool Actor::IsAnimActionReload() const
{
	const auto currentAnimAction = static_cast<AnimAction>(baseProcess->GetCurrentAnimAction());
	const static std::unordered_set s_reloads = { kAnimAction_Reload, kAnimAction_ReloadLoop, kAnimAction_ReloadLoopStart, kAnimAction_ReloadLoopEnd };
	return s_reloads.contains(currentAnimAction);
}

NiPoint3 TESObjectREFR::GetCenter() const
{
	if (this->IsActor())
		if (const auto baseProcess = ((Actor*)this)->baseProcess; baseProcess && baseProcess->processLevel != 1)
			return baseProcess->GetBoundingBox()->centre;
	if (this->renderState && this->renderState->niNode14)
		if (const auto bounds = reinterpret_cast<BSBound*>(this->renderState->niNode14->GetExtraData(kVtbl_BSBound)))
			return bounds->centre;
	if (this->baseForm && this->baseForm->IsBoundObject())
		if (const auto bounds = (reinterpret_cast<TESBoundObject*>(this->baseForm)))
		{
			NiPoint3 ni3 {};
			ni3.x = abs(bounds->right.x - bounds->left.x) / 2;
			ni3.y = abs(bounds->right.y - bounds->left.y) / 2;
			ni3.z = abs(bounds->right.z - bounds->left.z) / 2;

			const auto rot = this->rot;

			const auto sX = sin(rot.x);
			const auto sY = sin(rot.y);
			const auto sZ = sin(rot.z);
			const auto cX = cos(rot.x);
			const auto cY = cos(rot.y);
			const auto cZ = cos(rot.z);

			const Float32 newX = (ni3.x * cZ + ni3.y * sZ) * cY + ni3.z * -sY;
			const Float32 newY = (ni3.x * -sZ + ni3.y * cZ) * cX + ((ni3.x * cZ + ni3.y * sZ) * sY + ni3.z * cY) * sX;
			const Float32 newZ = (ni3.x * -sZ + ni3.y * cZ) * -sX + ((ni3.x * cZ + ni3.y * sZ) * sY + ni3.z * cY) * cX;

			ni3.x = newX;
			ni3.y = newY;
			ni3.z = newZ;

			return ni3;
		}
	const NiPoint3 ni3{};
	return ni3;
}

NiPoint3 TESObjectREFR::GetDimensions() const
{
	if (this->IsActor())
		if (const auto baseProcess = ((Actor*)this)->baseProcess; baseProcess && baseProcess->processLevel != 1)
			return baseProcess->GetBoundingBox()->dimensions;
	if (this->renderState && this->renderState->niNode14)
		if (const auto bounds = reinterpret_cast<BSBound*>(this->renderState->niNode14->GetExtraData(kVtbl_BSBound)))
			return bounds->dimensions;
	if (this->baseForm && this->baseForm->IsBoundObject())
		if (const auto bounds = (reinterpret_cast<TESBoundObject*>(this->baseForm)))
		{
			NiPoint3 ni3 {};
			ni3.x = abs(bounds->right.x - bounds->left.x) / 2;
			ni3.y = abs(bounds->right.y - bounds->left.y) / 2;
			ni3.z = abs(bounds->right.z - bounds->left.z) / 2;

			const auto rot = this->rot;

			const auto sX = sin(rot.x);
			const auto sY = sin(rot.y);
			const auto sZ = sin(rot.z);
			const auto cX = cos(rot.x);
			const auto cY = cos(rot.y);
			const auto cZ = cos(rot.z);

			const Float32 newX = (ni3.x * cZ + ni3.y * sZ) * cY + ni3.z * -sY;
			const Float32 newY = (ni3.x * -sZ + ni3.y * cZ) * cX + ((ni3.x * cZ + ni3.y * sZ) * sY + ni3.z * cY) * sX;
			const Float32 newZ = (ni3.x * -sZ + ni3.y * cZ) * -sX + ((ni3.x * cZ + ni3.y * sZ) * sY + ni3.z * cY) * cX;

			ni3.x = newX;
			ni3.y = newY;
			ni3.z = newZ;

			return ni3;
		}
	const NiPoint3 ni3{};
	return ni3;
}

TESObjectREFR* TESObjectREFR::ResolveAshpile()
{
	if (*(UInt32*)baseForm == kVtbl_TESObjectACTI)
		if (const auto xAshPileRef = reinterpret_cast<ExtraAshPileRef*>(extraDataList.GetByType(kExtraData_AshPileRef)))
			return xAshPileRef->sourceRef;
	return this;
}