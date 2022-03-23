#include <GameObjects.h>
#include <GameRTTI.h>
#include <GameExtraData.h>
#include <GameTasks.h>
#include <GameUI.h>
#include <SafeWrite.h>
#include <NiObjects.h>

static constexpr UInt32 s_TESObject_REFR_init	= 0x0055A2F0;	// TESObject_REFR initialization routine (first reference to s_TESObject_REFR_vtbl)
static constexpr UInt32	s_Actor_EquipItem		= 0x0088C650;	// maybe, also, would be: 007198E0 for FOSE	4th call from the end of TESObjectREFR::RemoveItem (func5F)
static constexpr UInt32	s_Actor_UnequipItem		= 0x0088C790;	// maybe, also, would be: 007133E0 for FOSE next sub after EquipItem
static constexpr UInt32 s_TESObjectREFR__GetContainer	= 0x0055D310;	// First call in REFR::RemoveItem
static constexpr UInt32 s_TESObjectREFR_Set3D	= 0x005702E0;	// void : (const char*)
static constexpr UInt32 s_PlayerCharacter_GetCurrentQuestTargets	= 0x00952BA0;	// BuildedQuestObjectiveTargets* : (void)
static constexpr UInt32 s_PlayerCharacter_GenerateNiNode	= 0x0094E1D0; // Func0072
static constexpr UInt32 kPlayerUpdate3Dpatch = 0x0094EB7A;
static constexpr UInt32 TESObjectREFR_Set3D = 0x0094EB40;
static constexpr UInt32 ValidBip01Names_Destroy = 0x00418E00;
static constexpr UInt32 ExtraAnimAnimation_Destroy = 0x00418D20;
static constexpr UInt32 RefNiRefObject_ReplaceNiRefObject = 0x0066B0D0;

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
	if (IsActor())
		return &static_cast<TESActorBase*>(baseForm)->container;
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
	if (bTemp) CALL_MEMBER_FN(refr, MarkAsTemporary)();
	return refr;
}

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj)	// For LevelledForm, find real baseForm, not temporary one.
{
	if (thisObj)
		if (const auto pXCreatureData = GetByTypeCast(thisObj->extraDataList, LeveledCreature); pXCreatureData && pXCreatureData->baseForm)
			return pXCreatureData->baseForm;
	if (thisObj && thisObj->baseForm)
		return thisObj->baseForm;
	return nullptr;
}

void Actor::EquipItem(TESForm * objType, UInt32 equipCount, ExtraDataList* itemExtraList, UInt32 unk3, bool lockEquip, UInt32 unk5)
{
	ThisStdCall(s_Actor_EquipItem, this, objType, equipCount, itemExtraList, unk3, lockEquip, unk5);
}

void Actor::UnequipItem(TESForm* objType, UInt32 unk1, ExtraDataList* itemExtraList, UInt32 unk3, bool lockUnequip, UInt32 unk5)
{
	ThisStdCall(s_Actor_UnequipItem, this, objType, unk1, itemExtraList, unk3, lockUnequip, unk5);
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

__declspec(naked) float __vectorcall GetDistance3D(TESObjectREFR* ref1, TESObjectREFR* ref2)
{
	__asm
	{
		movups	xmm0, [ecx + 0x2C]
		psrldq	xmm0, 4
		movups	xmm1, [edx + 0x2C]
		psrldq	xmm1, 4
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) float __vectorcall GetDistance2D(TESObjectREFR* ref1, TESObjectREFR* ref2)
{
	__asm
	{
		movq	xmm0, qword ptr[ecx + 0x30]
		movq	xmm1, qword ptr[edx + 0x30]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) bool TESObjectREFR::GetInSameCellOrWorld(TESObjectREFR *target)
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

__declspec(naked) float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR* target)
{
	__asm
	{
		push	ecx
		push	edx
		call	TESObjectREFR::GetInSameCellOrWorld
		pop		edx
		pop		ecx
		test	al, al
		jz		fltMax
		jmp		GetDistance3D
	fltMax:
		mov		eax, 0x7F7FFFFF
		movd	xmm0, eax
		retn
	}
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

ExtraDataList* ExtraContainerChanges::EntryData::GetEquippedExtra()
{
	return GetCustomExtra(kExtraData_Worn);
}

ExtraDataList* ExtraContainerChanges::EntryData::GetCustomExtra(UInt32 whichVal)
{
	if (!extendData) return nullptr;
	const TListNode<ExtraDataList>* xdlIter = extendData->Head();
	do if (const auto xData = xdlIter->data; xData && xData->HasType(whichVal)) return xData;
	while ((xdlIter = xdlIter->next));
	return nullptr;
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
HighProcess*			PlayerCharacter::GetHighProcess() { return reinterpret_cast<HighProcess*>(baseProcess); }

bool Explosion::CanStoreAmmo()
{
	if (!source) return false;
	if (source->typeID != kFormType_Character && source->typeID != kFormType_Creature) return false;
	if (!source->IsActor()) return false;
	return true;
}