#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"
#include "GameTasks.h"
#include "GameUI.h"
#include "SafeWrite.h"
#include "NiObjects.h"

static const UInt32 s_TESObject_REFR_init	= 0x0055A2F0;	// TESObject_REFR initialization routine (first reference to s_TESObject_REFR_vtbl)
static const UInt32	s_Actor_EquipItem		= 0x0088C650;	// maybe, also, would be: 007198E0 for FOSE	4th call from the end of TESObjectREFR::RemoveItem (func5F)
static const UInt32	s_Actor_UnequipItem		= 0x0088C790;	// maybe, also, would be: 007133E0 for FOSE next sub after EquipItem
static const UInt32 s_TESObjectREFR__GetContainer	= 0x0055D310;	// First call in REFR::RemoveItem
static const UInt32 s_TESObjectREFR_Set3D	= 0x005702E0;	// void : (const char*)
static const UInt32 s_PlayerCharacter_GetCurrentQuestTargets	= 0x00952BA0;	// BuildedQuestObjectiveTargets* : (void)
static const UInt32 s_PlayerCharacter_GenerateNiNode	= 0x0094E1D0; // Func0072
static const UInt32 kPlayerUpdate3Dpatch = 0x0094EB7A;
static const UInt32 TESObjectREFR_Set3D = 0x0094EB40;
static const UInt32 ValidBip01Names_Destroy = 0x00418E00;
static const UInt32 ExtraAnimAnimation_Destroy = 0x00418D20;
static const UInt32 RefNiRefObject_ReplaceNiRefObject = 0x0066B0D0;

static const UInt32 kg_Camera1st	= 0x011E07D0;
static const UInt32 kg_Camera3rd	= 0x011E07D4;
static const UInt32 kg_Bip			= 0x011E07D8;
static const UInt8 kPlayerUpdate3DpatchFrom	= 0x0B6;
static const UInt8 kPlayerUpdate3DpatchTo	= 0x0EB;

static NiObject **	g_3rdPersonCameraNode =				(NiObject**)kg_Camera3rd;
static NiObject **	g_1stPersonCameraBipedNode =		(NiObject**)kg_Bip;
static NiObject **	g_1stPersonCameraNode =				(NiObject**)kg_Camera1st;

ScriptEventList* TESObjectREFR::GetEventList() const
{
	BSExtraData* xData = extraDataList.GetByType(kExtraData_Script);
	if (xData)
	{
		ExtraScript* xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript);
		if (xScript)
			return xScript->eventList;
	}

	return nullptr;
}

PlayerCharacter* g_thePlayer = nullptr;

PlayerCharacter* PlayerCharacter::GetSingleton()
{
	return *(PlayerCharacter **)0x011DEA3C;
}

QuestObjectiveTargets* PlayerCharacter::GetCurrentQuestObjectiveTargets()
{
	return (QuestObjectiveTargets *)ThisStdCall(s_PlayerCharacter_GetCurrentQuestTargets, this);
}

HighProcess* PlayerCharacter::GetHighProcess()
{
	return reinterpret_cast<HighProcess*>(baseProcess);
}

TESContainer* TESObjectREFR::GetContainer()
{
	if (IsActor())
		return &((TESActorBase*)baseForm)->container;
	if (baseForm->typeID == kFormType_TESObjectCONT)
		return &((TESObjectCONT*)baseForm)->container;
	return nullptr;
}

bool TESObjectREFR::IsMapMarker()
{
	if (baseForm) {
		return baseForm->refID == 0x010;	// Read from the geck. Why is OBSE looking for a pointer ?
	}
	else {
		return false;
	}
}

// Less worse version as used by some modders 
bool PlayerCharacter::SetSkeletonPath_v1c(const char* newPath)
{
	if (!thirdPersonConsistent) {
		// ###TODO: enable in first person
		return false;
	}

	return true;
}

bool TESObjectREFR::Update3D_v1c()
{
	static const UInt32 kPlayerUpdate3Dpatch = 0x0094EB7A;

	UInt8 kPlayerUpdate3DpatchFrom = 0x0B6;
	UInt8 kPlayerUpdate3DpatchTo = 0x0EB;

	if (this == g_thePlayer) {
		// Lets try to allow unloading the player3D never the less...
		SafeWrite8(kPlayerUpdate3Dpatch, kPlayerUpdate3DpatchTo);
	}

	Set3D(NULL, true);
	ModelLoader::GetSingleton()->QueueReference(this, 1, 0);
	return true;
}

// Current basically not functioning version, but should show some progress in the end.. I hope
bool PlayerCharacter::SetSkeletonPath(const char* newPath)
{
	if (!thirdPersonConsistent) {
		// ###TODO: enable in first person
		return false;
	}

	return true;
}

bool TESObjectREFR::Update3D()
{
	if (this == g_thePlayer) {
		// Lets try to allow unloading the player3D never the less...
		SafeWrite8(kPlayerUpdate3Dpatch, kPlayerUpdate3DpatchTo);
	}

	Set3D(NULL, true);
	ModelLoader::GetSingleton()->QueueReference(this, 1, false);
	return true;
}

TESActorBase* Actor::GetActorBase()
{
	if (modIndex == 0xFF)
	{
		const auto xLvlCre = GetExtraType(extraDataList, LeveledCreature);
		if (xLvlCre) return xLvlCre->actorBase;
	}
	return static_cast<TESActorBase*>(baseForm);
}

TESObjectREFR* TESObjectREFR::Create(bool bTemp)
{
	TESObjectREFR* refr = (TESObjectREFR*)FormHeap_Allocate(sizeof(TESObjectREFR));
	ThisStdCall(s_TESObject_REFR_init, refr);
	if (bTemp)
		CALL_MEMBER_FN(refr, MarkAsTemporary)();
	return refr;
}

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj)	// For LevelledForm, find real baseForm, not temporary one.
{
	ExtraLeveledCreature * pXCreatureData = NULL;

	if (thisObj) {
		pXCreatureData = GetByTypeCast(thisObj->extraDataList, LeveledCreature);
		if (pXCreatureData && pXCreatureData->baseForm) {
			return pXCreatureData->baseForm;
		}
	}
	if (thisObj && thisObj->baseForm) {
		return thisObj->baseForm;
	}
	return NULL;
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
	ExtraContainerDataArray outEntryData;
	ExtraContainerExtendDataArray outExtendData;

	ExtraContainerChanges	* xChanges = static_cast <ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges));
	if(xChanges) {
		UInt32 count = xChanges->GetAllEquipped(outEntryData, outExtendData);
		for (UInt32 i = 0; i < count ; i++)
			itemList.push_back(outEntryData[i]->type);

	}

	return itemList;
}

ExtraContainerDataArray	Actor::GetEquippedEntryDataList()
{
	ExtraContainerDataArray itemArray;
	ExtraContainerExtendDataArray outExtendData;

	ExtraContainerChanges	* xChanges = static_cast <ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges));
	if(xChanges)
		xChanges->GetAllEquipped(itemArray, outExtendData);

	return itemArray;
}

ExtraContainerExtendDataArray	Actor::GetEquippedExtendDataList()
{
	ExtraContainerDataArray itemArray;
	ExtraContainerExtendDataArray outExtendData;

	ExtraContainerChanges	* xChanges = static_cast <ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges));
	if(xChanges)
		xChanges->GetAllEquipped(itemArray, outExtendData);

	return outExtendData;
}

bool TESObjectREFR::GetInventoryItems(InventoryItemsMap &invItems)
{
	TESContainer *container = GetContainer();
	if (!container) return false;
	ExtraContainerChanges *xChanges = (ExtraContainerChanges*)extraDataList.GetByType(kExtraData_ContainerChanges);
	ExtraContainerChanges::EntryDataList *entryList = (xChanges && xChanges->data) ? xChanges->data->objList : NULL;
	if (!entryList) return false;

	TESForm *item;
	SInt32 contCount, countDelta;
	ExtraContainerChanges::EntryData *entry;

	for (auto contIter = container->formCountList.Begin(); !contIter.End(); ++contIter)
	{
		item = contIter->form;
		if ((item->typeID == kFormType_TESObjectCONT) || invItems.find(item) != invItems.end())
			continue;
		contCount = container->GetCountForForm(item);
		if (entry = entryList->FindForItem(item))
		{
			countDelta = entry->countDelta;
			if (entry->HasExtraLeveledItem())
				contCount = countDelta;
			else contCount += countDelta;
		}
		if (contCount > 0)
			invItems.emplace(item, InventoryItemData(contCount, entry));
	}

	for (auto xtraIter = entryList->Begin(); !xtraIter.End(); ++xtraIter)
	{
		entry = xtraIter.Get();
		item = entry->type;
		if (invItems.find(item) != invItems.end())
			continue;
		countDelta = entry->countDelta;
		if (countDelta > 0)
			invItems.emplace(item, InventoryItemData(countDelta, entry));
	}

	return !invItems.empty();
}

__declspec(naked) ExtraContainerChanges::EntryDataList* TESObjectREFR::GetContainerChangesList()
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

/*__declspec(naked) ContChangesEntry* ExtraContainerChanges::EntryDataList::FindForItem(TESForm* item)
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
}*/
