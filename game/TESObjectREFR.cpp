#include <BaseFormComponent.h>
#include <TESObjectREFR.h>
#include <RTTI.h>
#include <InventoryChanges.h>
#include <GameTasks.h>
#include <Safewrite.hpp>
#include <NiObjects.h>
#include <unordered_set>

#include "BaseProcess.h"
#include "Setting.h"
#include <numbers>

static constexpr UInt32 s_TESObject_REFR_init						= 0x0055A2F0;	// TESObject_REFR initialization routine (first reference to s_TESObject_REFR_vtbl)
static constexpr UInt32 s_TESObjectREFR__GetContainer				= 0x0055D310;	// First call in REFR::RemoveItem
static constexpr UInt32 s_TESObjectREFR_Set3D						= 0x005702E0;	// void : (const char*)
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
	const auto refr = static_cast<TESObjectREFR*>(GameHeapAlloc(sizeof(TESObjectREFR)));
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

std::vector<InventoryChanges*> TESObjectREFR::GetAllItems(UInt32 checkLinkedRec)
{
	std::vector<InventoryChanges*> vector;
	
	InventoryChanges* entry = nullptr;
	void* iterator = nullptr;
	ThisCall(0x575510, this, 0, &iterator, &entry);
	if (iterator && entry) for (auto iter = ThisCall<InventoryChanges*>(0x4CA330, entry, iterator); iter; iter = ThisCall<InventoryChanges*>(0x4CA330, entry, iterator))
		if (iter->ShouldDisplay()) vector.push_back(iter);

	// Vanilla method is more than twice as slow but it does a lot of stuff, like removing LvlItem garbage, combining throwables, etc

	/*
	if (const auto entryList = GetContainerChangesList()) for (auto iter : *entryList) 
		if (iter->countDelta > 0) vector.push_back(iter);

	if (const auto container = GetContainer()) for (const auto iter : container->formCountList)
		if (iter->form->typeID != kFormType_TESLevItem && iter->count > 0) vector.push_back(InventoryChanges::Create(iter->form, iter->count));
	*/

	// Process linked ref for vendor containers, recursive, but hopefully no one links container on itself
	// UPD: someone linked container on itself, ffs
	if (!checkLinkedRec || checkLinkedRec > 5) {}
	else if (const auto xLinkedRef = reinterpret_cast<ExtraLinkedRef*>(this->extraDataList.GetByType(kExtraData_LinkedRef)))
		if (const auto linked = xLinkedRef->linkedRef)
		{
			const auto newvector = linked->GetAllItems(checkLinkedRec + 1);
			vector.insert(vector.end(), newvector.begin(), newvector.end());
		}

	if (this->IsActor())
		if (const auto xDropped = reinterpret_cast<ExtraDroppedItemList*>(this->extraDataList.GetByType(kExtraData_DroppedItemList)))
			for (const auto iter : xDropped->itemRefs) vector.push_back(InventoryChanges::Create(iter, 1));

	return vector;
}

void TESObjectREFR::OpenCloseContainer(bool open, bool sounds)
{
	const auto rootNode = this->GetNiNode();
	if (rootNode && (!this->IsActor() || reinterpret_cast<Actor*>(this)->lifeState != kLifeState_Dying))
	{
		const auto ctrlMgr = reinterpret_cast<NiControllerManager*>(rootNode->m_controller);
		if (ctrlMgr && IS_TYPE(ctrlMgr, NiControllerManager))
		{
			if (const auto sequence = open ? ctrlMgr->FindSequence("Open") : ctrlMgr->FindSequence("Close"))
			{
				for (const auto iter : ctrlMgr->sequences)
					if (iter->sequenceName && (*(UInt32*)iter->sequenceName != 'eldI'))
						iter->Unk_23(0, 0);
				if (sequence->Play()) this->MarkAsModified(0x10000000);
			}
		}
	}

	if (!sounds) return;
	if (baseForm->typeID == kFormType_TESObjectCONT) {
		const auto sound = open ? reinterpret_cast<TESObjectCONT*>(baseForm)->openSound : reinterpret_cast<TESObjectCONT*>(baseForm)->closeSound;
		if (sound) Sound::PlayTESSound(sound, BSAudioManager::kAudioFlags_3D | BSAudioManager::kAudioFlags100, this);
	}
	else
	{
		PlayGameSound(open ? "DRSBodyGenericOpen" : "DRSBodyGenericClose");
	}

}

const char* TESObjectREFR::GetJIPName()
{
	if (!(extraDataList.JIPExtraListFlags & BaseExtraList::kJIPExtraListFlag_AltRefName)) return "";
	return GetBaseFormFullName();
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

void Actor::EquipItem(TESForm * objType, UInt32 equipCount, ExtraDataList* itemExtraList, UInt32 shouldApplyEnchantment, UInt32 lockEquip, UInt32 noMessage)
{
	ThisStdCall(0x0088C650, this, objType, equipCount, itemExtraList, shouldApplyEnchantment, lockEquip, noMessage);
}

void Actor::UnequipItem(TESForm* objType, UInt32 unequipCount, ExtraDataList* itemExtraList, UInt32 shouldApplyEnchantment, UInt32 lockUnequip, UInt32 noMessage)
{
	ThisStdCall(0x0088C790, this, objType, unequipCount, itemExtraList, shouldApplyEnchantment, lockUnequip, noMessage);
}

std::vector<TESForm*> Actor::GetEquippedItems()
{
	std::vector<TESForm*> itemList;
	if (const auto xChanges = static_cast<ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges))) {
		InventoryChangesArray outEntryData;
		const UInt32 count = xChanges->data->GetAllEquipped(outEntryData);
		for (UInt32 i = 0; i < count ; i++) itemList.push_back(outEntryData[i]->form);
	}
	return itemList;
}

InventoryChangesArray Actor::GetEquippedEntryDataList()
{
	InventoryChangesArray itemArray;
	ExtendDataArray outExtendData;
	if(const auto xChanges = static_cast<ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges)))
		xChanges->data->GetAllEquipped(itemArray);
	return itemArray;
}

Float64 Actor::GetCalculatedSpread(UInt32 mode, InventoryChanges* entry)
{
	if (!entry) entry = reinterpret_cast<InventoryChanges*>(this->baseProcess->GetWeaponInfo());

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

			Setting* fNPCMaxGunWobbleAngle = GetGameSetting("fNPCMaxGunWobbleAngle");

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
			const Setting* fNonAttackGunWobbleMult = GetGameSetting("fNonAttackGunWobbleMult");

			totalSpread = totalSpread * fNonAttackGunWobbleMult->data.f;
		}

		totalSpread *= 0.01745329238474369;
	}
	else if (mode == 2)
	{
		totalSpread = ThisStdCall<double>(0x8B0DD0, this, 0);
		const Setting* fGunWobbleMultScope = GetGameSetting("fGunWobbleMultScope");
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

bool Actor::IsHostileCompassTarget() const
{
	if (const auto player = PlayerCharacter::GetSingleton()) for (const auto iter : *player->compassTargets)
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

void Actor::SendStealingAlarm(TESObjectREFR* container, bool checkItems)
{
	if (!checkItems) {
		const auto owner = ThisStdCall<TESForm*>(0x567790, container); // TESObjectREFR::ResolveOwnership
		if (!owner) return;
		ThisStdCall(0x8BFA40, this, container, NULL, NULL, 1, owner); // Actor::HandleStealing,
		return;
	}
	const auto owner = ThisStdCall<TESForm*>(0x567790, container); // TESObjectREFR::ResolveOwnership
	if (!owner) return;
	const auto xChanges = reinterpret_cast<ExtraContainerChanges*>(this->extraDataList.GetByType(kExtraData_ContainerChanges));
	if (!xChanges || !xChanges->data || !xChanges->data->objList) return;

	for (const auto entry : *xChanges->data->objList) {
		if (!entry->extendData || !entry->form) continue;
		for (const auto xData : *entry->extendData) {
			if (xData && xData->HasType(kExtraData_Ownership)) {
				const auto xOwn = reinterpret_cast<ExtraOwnership*>(xData->GetByType(kExtraData_Ownership));
				if (xOwn->owner && xOwn->owner->refID == owner->refID) {
					ThisStdCall(0x8BFA40, this, container, NULL, NULL, 1, owner); // Actor::HandleStealing
					return;
				}
			}
		}
	}
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
	InventoryChanges *entry;

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

bool __fastcall TESObjectREFR::GetInSameCellOrWorld(const TESObjectREFR *target) const
{
	const TESObjectCELL* parentCell = this->parentCell;
	if (!parentCell)
	{
		const auto extraCell = (ExtraPersistentCell*) extraDataList.GetByType(kExtraData_PersistentCell);

		if (!extraCell) return false;

		parentCell = extraCell->persistentCell;
	}

	const TESObjectCELL* targetParentCell = target->parentCell;
	if (!targetParentCell)
	{
		const auto extraCell = (ExtraPersistentCell*) target->extraDataList.GetByType(kExtraData_PersistentCell);

		if (!extraCell) return false;

		targetParentCell = extraCell->persistentCell;
	}

	if (parentCell == targetParentCell) return true;

	if (!parentCell->worldSpace) return false;

	if (parentCell->worldSpace == targetParentCell->worldSpace) return true;

	return false;
}

float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR* target)
{
	return GetInSameCellOrWorld(target) ? Point3Distance(*GetPos(), *target->GetPos()) : FLT_MAX;
}

float __vectorcall TESObjectREFR::GetDistance2D(TESObjectREFR* target)
{
	return GetInSameCellOrWorld(target) ? Point2Distance(*GetPos(), *target->GetPos()) : FLT_MAX;
}

Float32 TESObjectREFR::GetHeadingAngle(const TESObjectREFR* target)
{
	if (!target) return 0;
	return rot.z - atan2(target->pos.x - pos.x, target->pos.y - pos.y) - 2 * std::numbers::pi;
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
	/*
	 *  result = this->renderState;
  if ( !result )
    return result;

  result = (TESObjectREFR::RenderState *)result->niNode14;
  if ( this->refID == 0x14 && !this->isInThirdPerson )
    return (TESObjectREFR::RenderState *)this->node1stPerson;

  return result;
	 *
	 */
	__asm
	{
		mov		eax, [ecx + 0x64] // renderstate
		test	eax, eax
		jz		done
		mov		eax, [eax + 0x14]	// ninode0x14
		cmp		dword ptr[ecx + 0xC], 0x14 // player
		jnz		done
		cmp		byte ptr[ecx + 0x64A], 0 // isinthirdperson
		jnz		done
		mov		eax, [ecx + 0x694] //third person node0
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
		cmp		dword ptr [ecx+0xC], 0x14 // player
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

QuestObjectiveTargets*	PlayerCharacter::GetCurrentQuestObjectiveTargets() { return ThisStdCall<QuestObjectiveTargets*>(0x00952BA0, this); }
TESObjectREFR*			PlayerCharacter::GetPlacedMarkerOrTeleportLink() { return ThisStdCall<TESObjectREFR*>(0x77A400, this); }

bool Explosion::CanStoreAmmo()
{
	if (!source) return false;
	if (source->typeID != kFormType_Character && source->typeID != kFormType_Creature) return false;
	if (!source->IsActor()) return false;
	if (ammo && (ammo->typeID != kFormType_TESAmmo && ammo->typeID != kFormType_TESObjectWEAP)) return false; // temporary check until I resolve saving issue
	return true;
}

UInt32* g_weaponTypeToAnim = reinterpret_cast<UInt32*>(0x118A838);

UInt16 GetActorRealAnimGroup(Actor* actor, UInt8 groupID)
{
	UInt8 animHandType = 0;
	if (const auto* form = actor->GetWeaponForm())
		animHandType = g_weaponTypeToAnim[form->eWeaponType];
	const auto moveFlags = actor->actorMover->GetMovementFlags();
	UInt8 moveType = 0;
	if ((moveFlags & 0x800) != 0)
		moveType = kAnimMoveType_Swimming;
	else if ((moveFlags & 0x2000) != 0)
		moveType = kAnimMoveType_Flying;
	else if ((moveFlags & 0x400) != 0)
		moveType = kAnimMoveType_Sneaking;
	const auto isPowerArmor = ThisStdCall<bool>(0x8BA3E0, actor) || ThisStdCall<bool>(0x8BA410, actor);
	return (moveType << 12) + (isPowerArmor << 15) + (animHandType << 8) + groupID;
}

TESAmmo* ActorHitData::GetAmmo() const
{
	if (!projectile) return nullptr;
	if (projectile->IsProjectile()) return projectile->ammo;
	if (explosion->IsExplosion() && explosion->CanStoreAmmo()) return explosion->ammo;
	return nullptr;
}

TESObjectWEAP* ActorHitData::GetWeapon() const
{
	if (!projectile) return nullptr;
	if (projectile->IsProjectile()) return projectile->weapon;
	if (explosion->IsExplosion()) return explosion->weapon;
	if (weapon) return weapon;
	return nullptr;
}

Script* ActorHitData::GetAmmoScript() const
{
	const auto ammo = GetAmmo();
	if (!ammo) return nullptr;
	if (ammo->typeID == kFormType_TESAmmo) return ammo->scriptable.script;
	if (ammo->typeID == kFormType_TESObjectWEAP) return reinterpret_cast<TESObjectWEAP*>(ammo)->scritpable.script;
	return nullptr;
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
			ni3.x = abs(bounds->right.x + bounds->left.x) / 2;
			ni3.y = abs(bounds->right.y + bounds->left.y) / 2;
			ni3.z = abs(bounds->right.z + bounds->left.z) / 2;

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

bool TESObjectREFR::IsLocked() const
{
	const auto data = GetLockData();
	return data && data->IsLocked();
}


const _GetActorValueName GetActorValueName = reinterpret_cast<_GetActorValueName>(0x00066EAC0);	// See Cmd_GetActorValue_Eval

// g_baseActorValueNames is only filled in after oblivion's global initializers run
const char* GetActorValueString(UInt32 actorValue)
{
	const char* name = 0;
	if (actorValue <= kAVCode_Max)
		name = GetActorValueName(actorValue);
	if (!name)
		name = "unknown";

	return name;
}

UInt32 GetActorValueForScript(const char* avStr)
{
	for (UInt32 i = 0; i <= kAVCode_Max; i++) {
		if (_stricmp(avStr, GetActorValueName(i)) == 0)
			return i;
	}
	return kAVCode_None;
}

UInt32 GetActorValueForString(const char* strActorVal, bool bForScript)
{
	if (bForScript)
		return GetActorValueForScript(strActorVal);

	for (UInt32 n = 0; n <= kAVCode_Max; n++) {
		if (_stricmp(strActorVal, GetActorValueName(n)) == 0)
			return n;
	}
	return kAVCode_None;
}

std::unordered_map<UInt32, UInt32> ActorValueMax = {
	{ kAVCode_Aggression, 3 },
	{ kAVCode_Confidence, 4 },
	{ kAVCode_Energy, 100 },
	{ kAVCode_Responsibility, 100 },
	{ kAVCode_Mood, 8 },

	{ kAVCode_Strength, 10 },
	{ kAVCode_Perception, 10 },
	{ kAVCode_Endurance, 10 },
	{ kAVCode_Charisma, 10 },
	{ kAVCode_Intelligence, 10 },
	{ kAVCode_Agility, 10 },
	{ kAVCode_Luck, 10 },

	{ kAVCode_ActionPoints, 0 },
	{ kAVCode_CarryWeight, 0 },
	{ kAVCode_CritChance, 100 },
	{ kAVCode_HealRate, 0 },
	{ kAVCode_Health, 0 },
	{ kAVCode_MeleeDamage, 0 },
	{ kAVCode_DamageResistance, 100 },
	{ kAVCode_PoisonResist, 100 },
	{ kAVCode_RadResist, 100 },
	{ kAVCode_SpeedMult, 0 },
	{ kAVCode_Fatigue, 0 },
	{ kAVCode_Karma, 1000 },
	{ kAVCode_XP, 0 },

	{ kAVCode_PerceptionCondition, 100 },
	{ kAVCode_EnduranceCondition, 100 },
	{ kAVCode_LeftAttackCondition, 100 },
	{ kAVCode_RightAttackCondition, 100 },
	{ kAVCode_LeftMobilityCondition, 100 },
	{ kAVCode_RightMobilityCondition, 100 },
	{ kAVCode_BrainCondition, 100 },

	{ kAVCode_Barter, 100 },
	{ kAVCode_BigGuns, 100 },
	{ kAVCode_EnergyWeapons, 100 },
	{ kAVCode_Explosives, 100 },
	{ kAVCode_Lockpick, 100 },
	{ kAVCode_Medicine, 100 },
	{ kAVCode_MeleeWeapons, 100 },
	{ kAVCode_Repair, 100 },
	{ kAVCode_Science, 100 },
	{ kAVCode_Guns, 100 },
	{ kAVCode_Sneak, 100 },
	{ kAVCode_Speech, 100 },
	{ kAVCode_Survival, 100 },
	{ kAVCode_Unarmed, 100 },

	{ kAVCode_InventoryWeight, 0 },
	{ kAVCode_Paralysis, 1 },
	{ kAVCode_Invisibility, 1 },
	{ kAVCode_Chameleon, 1 },
	{ kAVCode_NightEye, 1 },
	{ kAVCode_Turbo, 1 },
	{ kAVCode_FireResist, 100 },
	{ kAVCode_WaterBreathing, 1 },
	{ kAVCode_RadiationRads, 1000 },
	{ kAVCode_BloodyMess, 1 },
	{ kAVCode_UnarmedDamage, 100 },
	{ kAVCode_Assistance, 2 },

	{ kAVCode_ElectricResist, 100 },

	{ kAVCode_FrostResist, 100 },
	{ kAVCode_EnergyResist, 100 },
	{ kAVCode_EmpResist, 100 },
	{ kAVCode_Variable01, 1 },
	{ kAVCode_Variable02, 1 },
	{ kAVCode_Variable03, 1 },
	{ kAVCode_Variable04, 1 },
	{ kAVCode_Variable05, 1 },
	{ kAVCode_Variable06, 1 },
	{ kAVCode_Variable07, 1 },
	{ kAVCode_Variable08, 1 },
	{ kAVCode_Variable09, 1 },
	{ kAVCode_Variable10, 1 },

	{ kAVCode_IgnoreCrippledLimbs, 1 },
	{ kAVCode_Dehydration, 1000 },
	{ kAVCode_Hunger, 1000 },
	{ kAVCode_SleepDeprivation, 1000 },
	{ kAVCode_DamageThreshold, 100 },
};