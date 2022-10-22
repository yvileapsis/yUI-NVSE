#include "GameExtraData.h"

#include <algorithm>

#include "GameBSExtraData.h"
#include "GameAPI.h"
#include "Objects.h"
#include "GameRTTI.h"
#include "GameScript.h"

struct GetMatchingEquipped {
	FormMatcher& m_matcher;
	EquipData m_found;

	GetMatchingEquipped(FormMatcher& matcher) : m_matcher(matcher) {
		m_found.pForm = NULL;
		m_found.pExtraData = NULL;
	}

	bool Accept(InventoryChanges* pEntryData) {
		if (pEntryData) {
			// quick check - needs an extendData or can't be equipped
			const auto pExtendList = pEntryData->extendData;
			if (pExtendList && m_matcher.Matches(pEntryData->form)) { 
				SInt32 n = 0;
				ExtraDataList* pExtraDataList = pExtendList->GetNthItem(n);
				while (pExtraDataList) {
					if (pExtraDataList->HasType(kExtraData_Worn) || pExtraDataList->HasType(kExtraData_WornLeft)) {
						m_found.pForm = pEntryData->form;
						m_found.pExtraData = pExtraDataList;
						return false;
					}
					n++;
					pExtraDataList = pExtendList->GetNthItem(n);
				}
			}
		}
		return true;
	}

	EquipData Found() {
		return m_found;
	}
};


EquipData ExtraContainerChanges::FindEquipped(FormMatcher& matcher) const
{
	FoundEquipData equipData = {};
	if (data && data->objList) {
		GetMatchingEquipped getEquipped(matcher);
		data->objList->Visit(getEquipped);
		equipData = getEquipped.Found();
	}
	return equipData;
};

#if RUNTIME
static const UInt32 s_ExtraContainerChangesVtbl					= 0x01015BB8;	//	0x0100fb78;
static const UInt32 s_ExtraWornVtbl								= 0x01015BDC;
//static const UInt32 s_ExtraWornLeftVtbl							= 0x01015BE8;
static const UInt32 s_ExtraCannotWearVtbl						= 0x01015BF4;

static const UInt32 s_ExtraOwnershipVtbl						= 0x010158B4;	//	0x0100f874;
static const UInt32 s_ExtraRankVtbl								= 0x010158CC;	//	0x0100f88c;
static const UInt32 s_ExtraActionVtbl							= 0x01015BAC;	
static const UInt32 s_ExtraFactionChangesVtbl					= 0x01015F30;
static const UInt32 s_ExtraScriptVtbl							= 0X1015914;

//static const UInt32 s_ExtraScript_init							= 0x0042C760;

static const UInt32 s_ExtraHealthVtbl = 0x010158E4;
static const UInt32 s_ExtraLockVtbl = 0x0101589C;
static const UInt32 s_ExtraCountVtbl = 0x010158D8;
static const UInt32 s_ExtraTeleportVtbl = 0x010158A8;
static const UInt32 s_ExtraWeaponModFlagsVtbl = 0x010159A4;

static const UInt32 s_ExtraHotkeyVtbl = 0x0101592C;

static const UInt32 s_ExtraSemaphore	= 0x011C3920;
static const UInt32 s_SemaphoreWait		= 0x0040FBF0;
static const UInt32 s_SemaphoreLeave	= 0x0040FBA0;

#endif

static void** g_ExtraSemaphore = (void **)s_ExtraSemaphore;

void* GetExtraSemaphore()
{
	return *g_ExtraSemaphore;
};

void CallSemaphore(void * Semaphore, UInt32 SemaphoreFunc)
{
	_asm pushad
	_asm mov ecx, Semaphore
	_asm call SemaphoreFunc
	_asm popad
};

void CallSemaphore4(void * Semaphore, UInt32 SemaphoreFunc)
{
	_asm pushad
	_asm push ecx	;	does not seem to be used at all
	_asm mov ecx, Semaphore
	_asm call SemaphoreFunc
	_asm popad
};

SInt32 ExtendDataList::AddAt(ExtraDataList* item, SInt32 index)
{
	//CallSemaphore4(GetExtraSemaphore(), s_SemaphoreWait);
	const SInt32 Result = TList<ExtraDataList>::AddAt(item, index);
	//CallSemaphore(GetExtraSemaphore(), s_SemaphoreLeave);
	return Result;
};

void ExtendDataList::RemoveAll() const
{
	//CallSemaphore4(GetExtraSemaphore(), s_SemaphoreWait);
	TList<ExtraDataList>::RemoveAll();
	//CallSemaphore(GetExtraSemaphore(), s_SemaphoreLeave);
};

ExtraDataList* ExtendDataList::RemoveNth(SInt32 n)
{
	//CallSemaphore4(GetExtraSemaphore(), s_SemaphoreWait);
	ExtraDataList* Result = TList<ExtraDataList>::RemoveNth(n);
	//CallSemaphore(GetExtraSemaphore(), s_SemaphoreLeave);
	return Result;
}

ExtendDataList* ExtraContainerChanges::Add(TESForm* form, ExtraDataList* dataList)
{
	if (!data) {
		// wtf
		PrintLog("ExtraContainerChanges::Add() encountered ExtraContainerChanges with NULL data");
		return NULL;
	}

	if (!data->objList) {
		data->objList = InventoryChangesList::Create(0, 0, nullptr);
	}

	// try to locate the form
	InventoryChanges* found = data->objList->Find(ItemInEntryDataListMatcher(form));
	if (!found) {
		// add it to the list with a count delta of 0
		found = InventoryChanges::Create(form, 0);
		data->objList->AddAt(found, eListEnd);
	}

	return found->Add(dataList);
}

ExtraContainerChanges* ExtraContainerChanges::Create()
{
	const auto xChanges = (ExtraContainerChanges*)BSExtraData::Create(kExtraData_ContainerChanges, sizeof(ExtraContainerChanges), s_ExtraContainerChangesVtbl);
	xChanges->data = nullptr;
	return xChanges;
}

ExtraContainerChangesData* ExtraContainerChangesData::Create(TESObjectREFR* owner)
{
	const auto data = static_cast<ExtraContainerChangesData*>(FormHeap_Allocate(sizeof(ExtraContainerChangesData)));
	if (data) {
		data->owner = owner;
		data->objList = nullptr;
		data->unk2 = 0.0;
		data->unk3 = 0.0;
	}
	return data;
}

Float64 ExtraContainerChangesData::GetInventoryWeight()
{
	return ThisCall<Float64>(0x4D0900, this, PlayerCharacter::GetSingleton()->isHardcore);
}

ExtraContainerChanges* ExtraContainerChanges::Create(TESObjectREFR* thisObj, UInt32 refID, UInt32 count, ExtendDataList* pExtendDataList)
{
	const auto xData = (ExtraContainerChanges*)BSExtraData::Create(kExtraData_ContainerChanges, sizeof(ExtraContainerChanges), s_ExtraContainerChangesVtbl);
	if (xData) {
		xData->data = ExtraContainerChangesData::Create(thisObj);
		if (refID) {
			xData->data->objList = InventoryChangesList::Create(refID, count, pExtendDataList);
		}
	}
	return xData;
}

void ExtraContainerChangesFree(ExtraContainerChanges* xData, bool bFreeList) {
	if (xData) {
		if (xData->data) {
			if (xData->data->objList && bFreeList) {
				xData->data->objList->Free(true);
			}
			FormHeap_Free(xData->data);
		}
		FormHeap_Free(xData);
	}
}

void ExtraContainerChanges::Cleanup()
{
	if (data && data->objList) {
		for (const auto iter : *data->objList) {
			iter->Cleanup() ;

			// make sure we don't have any NULL ExtraDataList's in extend data, game will choke when saving
			if (iter->extendData == nullptr) continue;

			for (SInt32 index = 0; index < iter->extendData->Count(); ) {
				if (const auto xtendData = iter->extendData->GetNthItem(index); xtendData && !xtendData->m_data) {
					iter->extendData->RemoveNth(index);
					FormHeap_Free(xtendData);
				}
				else index++;
			}
		}
	}
}

ExtraDataList* ExtraContainerChanges::SetEquipped(TESForm* obj, bool bEquipped, bool bForce)
{
	if (data) {
		if (const auto xData = data->objList->Find(ItemInEntryDataListMatcher(obj))) {
			ExtraDataList* Possible = nullptr;
			if (xData->extendData && (xData->extendData->Count()>0))
				for (const auto iter : *xData->extendData)
					if (iter->HasType(kExtraData_Worn) || iter->HasType(kExtraData_WornLeft)) {
						if (!bEquipped)
							if (bForce || !iter->HasType(kExtraData_WornLeft)) {
								iter->RemoveByType(kExtraData_Worn);
								iter->RemoveByType(kExtraData_WornLeft);
								Cleanup();
								return iter; 
							}

					}
					else if (bEquipped && (bForce || !iter->HasType(kExtraData_CannotWear)))
							Possible = iter;
			if (!xData->extendData)
				xData->extendData = ExtendDataList::Create(nullptr);
			if (Possible) {
				Possible->Add(ExtraWorn::Create());
				return Possible; 
			}
		}
	}
	return nullptr; 
}

bool ExtraContainerChanges::Remove(TESForm* obj, ExtraDataList* dataList, bool bFree)
{
	for (UInt32 i = 0; i < data->objList->Count(); i++)
		if (data->objList->GetNthItem(i)->form == obj) {
			if (const auto found = data->objList->GetNthItem(i); dataList && found->extendData) {
				for (UInt32 j = 0; j < found->extendData->Count(); j++)
					if (found->extendData->GetNthItem(j) == dataList)
						found->extendData->RemoveNth(j);
			}
			else if (!dataList && !found->extendData)
				data->objList->RemoveNth(i);
		}
	return false;
}

void ExtraContainerChanges::DebugDump()
{
	PrintLog("Dumping ExtraContainerChanges");
	gLog.Indent();

	if (data && data->objList)
	{
		for (const auto entry : *data->objList)
		{
			PrintLog("Type: %s CountDelta: %d [%08X]", entry->form->GetFullName()->name.CStr(), entry->countDelta, entry);
			gLog.Indent();
			if (!entry || !entry->extendData)
				PrintLog("* No extend data *");
			else
			{
				for (const auto extendData : *entry->extendData)
				{
					PrintLog("Extend Data: [%08X]", extendData);
					gLog.Indent();
					if (extendData) {
						extendData->DebugDump();
						if (const auto xCount = (ExtraCount*)extendData->GetByType(kExtraData_Count))
							PrintLog("ExtraCount value : %d", xCount->count);
					}
					else PrintLog("NULL");
					gLog.Outdent();
				}
			}
			gLog.Outdent();
		}
	}
	gLog.Outdent();
}

ExtraContainerChanges* ExtraContainerChanges::GetForRef(TESObjectREFR* refr)
{
	auto xChanges = (ExtraContainerChanges*)refr->extraDataList.GetByType(kExtraData_ContainerChanges);
	if (!xChanges) {
		xChanges = Create();
		refr->extraDataList.Add(xChanges);
	}
	return xChanges;
}

UInt32 ExtraContainerChanges::GetAllEquipped(std::vector<InventoryChanges*>& outEntryData, std::vector<ExtendDataList*>& outExtendData)
{
	if (data && data->objList)
	{
		for (const auto entry : *data->objList)
		{
			if (entry && entry->extendData)
			{
				for (const auto extendData : *entry->extendData)
				{
					if (extendData->IsWorn()) {
						outEntryData.push_back(entry);
						outExtendData.push_back(entry->extendData);
					}
				}
			}
		}
	}
	return outEntryData.size();
}

// static
BSExtraData* BSExtraData::Create(UInt8 xType, UInt32 size, UInt32 vtbl)
{
	void* memory = FormHeap_Allocate(size);
	memset(memory, 0, size);
	static_cast<UInt32*>(memory)[0] = vtbl;
	const auto xData = static_cast<BSExtraData*>(memory);
	xData->type = xType;
	return xData;
}

ExtraHealth* ExtraHealth::Create() 
{
	const auto xHealth = (ExtraHealth*)BSExtraData::Create(kExtraData_Health, sizeof(ExtraHealth), s_ExtraHealthVtbl);
	return xHealth;
}

ExtraWorn* ExtraWorn::Create() 
{
	const auto xWorn = (ExtraWorn*)BSExtraData::Create(kExtraData_Worn, sizeof(ExtraWorn), s_ExtraWornVtbl);
	return xWorn;
}

//ExtraWornLeft* ExtraWornLeft::Create() 
//{
//	ExtraWornLeft* xWornLeft = (ExtraWornLeft*)BSExtraData::Create(kExtraData_WornLeft, sizeof(ExtraWornLeft), s_ExtraWornLeftVtbl);
//	return xWornLeft;
//}

ExtraCannotWear* ExtraCannotWear::Create() 
{
	const auto xCannotWear = (ExtraCannotWear*)BSExtraData::Create(kExtraData_CannotWear, sizeof(ExtraCannotWear), s_ExtraCannotWearVtbl);
	return xCannotWear;
}

ExtraLock* ExtraLock::Create()
{
	const auto xLock = (ExtraLock*)BSExtraData::Create(kExtraData_Lock, sizeof(ExtraLock), s_ExtraLockVtbl);
	const auto lockData = (ExtraLockData*)FormHeap_Allocate(sizeof(ExtraLockData));
	memset(lockData, 0, sizeof(ExtraLockData));
	xLock->data = lockData;
	return xLock;
}

ExtraCount* ExtraCount::Create(UInt32 count)
{
	const auto xCount = (ExtraCount*)BSExtraData::Create(kExtraData_Count, sizeof(ExtraCount), s_ExtraCountVtbl);
	xCount->count = count;
	return xCount;
}

ExtraTeleport* ExtraTeleport::Create()
{
	const auto tele = (ExtraTeleport*)BSExtraData::Create(kExtraData_Teleport, sizeof(ExtraTeleport), s_ExtraTeleportVtbl);
	
	// create data
	const auto data = (Data*)FormHeap_Allocate(sizeof(Data));
	data->linkedDoor = NULL;
	data->yRot = -0.0;
	data->xRot = 0.0;
	data->x = 0.0;
	data->y = 0.0;
	data->z = 0.0;
	data->zRot = 0.0;

	tele->data = data;
	return tele;
}

ExtraWeaponModFlags* ExtraWeaponModFlags::Create()
{
	const auto xWeaponModFlags = (ExtraWeaponModFlags*)BSExtraData::Create(kExtraData_WeaponModFlags, sizeof(ExtraWeaponModFlags), s_ExtraWeaponModFlagsVtbl);

	xWeaponModFlags->flags = 0;

	return xWeaponModFlags;
}

SInt32 GetCountForExtraDataList(ExtraDataList* list)
{
	if (!list)
		return 1;

	const auto xCount = (ExtraCount*)list->GetByType(kExtraData_Count);
	return xCount ? xCount->count : 1;
}

ExtraOwnership* ExtraOwnership::Create() 
{
	const auto xOwner = (ExtraOwnership*)BSExtraData::Create(kExtraData_Ownership, sizeof(ExtraOwnership), s_ExtraOwnershipVtbl);
	return xOwner;
}

ExtraRank* ExtraRank::Create()
{
	const auto xRank = (ExtraRank*)BSExtraData::Create(kExtraData_Rank, sizeof(ExtraRank), s_ExtraRankVtbl);
	return xRank;
}

ExtraAction* ExtraAction::Create()
{
	const auto xAction = (ExtraAction*)BSExtraData::Create(kExtraData_Action, sizeof(ExtraAction), s_ExtraActionVtbl);
	return xAction;
}

const char * GetExtraDataName(UInt8 ExtraDataType) {
	switch (ExtraDataType) {			
		case	kExtraData_Havok                    	: return "Havok"; break;
		case	kExtraData_Cell3D                   	: return "Cell3D"; break;
		case	kExtraData_CellWaterType            	: return "CellWaterType"; break;
		case	kExtraData_RegionList               	: return "RegionList"; break;
		case	kExtraData_SeenData                 	: return "SeenData"; break;
		case	kExtraData_CellMusicType            	: return "CellMusicType"; break;
		case	kExtraData_CellClimate              	: return "CellClimate"; break;
		case	kExtraData_ProcessMiddleLow         	: return "ProcessMiddleLow"; break;
		case	kExtraData_CellCanopyShadowMask     	: return "CellCanopyShadowMask"; break;
		case	kExtraData_DetachTime               	: return "DetachTime"; break;
		case	kExtraData_PersistentCell           	: return "PersistentCell"; break;
		case	kExtraData_Script                   	: return "Script"; break;
		case	kExtraData_Action                   	: return "Action"; break;
		case	kExtraData_StartingPosition         	: return "StartingPosition"; break;
		case	kExtraData_Anim                     	: return "Anim"; break;
		case	kExtraData_UsedMarkers              	: return "UsedMarkers"; break;
		case	kExtraData_DistantData              	: return "DistantData"; break;
		case	kExtraData_RagdollData              	: return "RagdollData"; break;
		case	kExtraData_ContainerChanges         	: return "ContainerChanges"; break;
		case	kExtraData_Worn                     	: return "Worn"; break;
		case	kExtraData_WornLeft                 	: return "WornLeft"; break;
		case	kExtraData_PackageStartLocation     	: return "PackageStartLocation"; break;
		case	kExtraData_Package                  	: return "Package"; break;
		case	kExtraData_TrespassPackage          	: return "TrespassPackage"; break;
		case	kExtraData_RunOncePacks             	: return "RunOncePacks"; break;
		case	kExtraData_ReferencePointer         	: return "ReferencePointer"; break;
		case	kExtraData_Follower                 	: return "Follower"; break;
		case	kExtraData_LevCreaModifier          	: return "LevCreaModifier"; break;
		case	kExtraData_Ghost                    	: return "Ghost"; break;
		case	kExtraData_OriginalReference        	: return "OriginalReference"; break;
		case	kExtraData_Ownership                	: return "Ownership"; break;
		case	kExtraData_Global                   	: return "Global"; break;
		case	kExtraData_Rank                     	: return "Rank"; break;
		case	kExtraData_Count                    	: return "Count"; break;
		case	kExtraData_Health                   	: return "Health"; break;
		case	kExtraData_Uses                     	: return "Uses"; break;
		case	kExtraData_TimeLeft                 	: return "TimeLeft"; break;
		case	kExtraData_Charge                   	: return "Charge"; break;
		case	kExtraData_Light                    	: return "Light"; break;
		case	kExtraData_Lock                     	: return "Lock"; break;
		case	kExtraData_Teleport                 	: return "Teleport"; break;
		case	kExtraData_MapMarker                	: return "MapMarker"; break;
		case	kExtraData_LeveledCreature          	: return "LeveledCreature"; break;
		case	kExtraData_LeveledItem              	: return "LeveledItem"; break;
		case	kExtraData_Scale                    	: return "Scale"; break;
		case	kExtraData_Seed                     	: return "Seed"; break;
		case	kExtraData_PlayerCrimeList          	: return "PlayerCrimeList"; break;
		case	kExtraData_EnableStateParent        	: return "EnableStateParent"; break;
		case	kExtraData_EnableStateChildren      	: return "EnableStateChildren"; break;
		case	kExtraData_ItemDropper              	: return "ItemDropper"; break;
		case	kExtraData_DroppedItemList          	: return "DroppedItemList"; break;
		case	kExtraData_RandomTeleportMarker     	: return "RandomTeleportMarker"; break;
		case	kExtraData_MerchantContainer        	: return "MerchantContainer"; break;
		case	kExtraData_SavedHavokData           	: return "SavedHavokData"; break;
		case	kExtraData_CannotWear               	: return "CannotWear"; break;
		case	kExtraData_Poison                   	: return "Poison"; break;
		case	kExtraData_LastFinishedSequence     	: return "LastFinishedSequence"; break;
		case	kExtraData_SavedAnimation           	: return "SavedAnimation"; break;
		case	kExtraData_NorthRotation            	: return "NorthRotation"; break;
		case	kExtraData_XTarget                  	: return "XTarget"; break;
		case	kExtraData_FriendHits               	: return "FriendHits"; break;
		case	kExtraData_HeadingTarget            	: return "HeadingTarget"; break;
		case	kExtraData_RefractionProperty       	: return "RefractionProperty"; break;
		case	kExtraData_StartingWorldOrCell      	: return "StartingWorldOrCell"; break;
		case	kExtraData_Hotkey                   	: return "Hotkey"; break;
		case	kExtraData_EditorRefMovedData       	: return "EditorRefMovedData"; break;
		case	kExtraData_InfoGeneralTopic         	: return "InfoGeneralTopic"; break;
		case	kExtraData_HasNoRumors              	: return "HasNoRumors"; break;
		case	kExtraData_Sound                    	: return "Sound"; break;
		case	kExtraData_TerminalState            	: return "TerminalState"; break;
		case	kExtraData_LinkedRef                	: return "LinkedRef"; break;
		case	kExtraData_LinkedRefChildren        	: return "LinkedRefChildren"; break;
		case	kExtraData_ActivateRef              	: return "ActivateRef"; break;
		case	kExtraData_ActivateRefChildren      	: return "ActivateRefChildren"; break;
		case	kExtraData_TalkingActor             	: return "TalkingActor"; break;
		case	kExtraData_ObjectHealth             	: return "ObjectHealth"; break;
		case	kExtraData_DecalRefs                	: return "DecalRefs"; break;
		case	kExtraData_CellImageSpace           	: return "CellImageSpace"; break;
		case	kExtraData_NavMeshPortal            	: return "NavMeshPortal"; break;
		case	kExtraData_ModelSwap                	: return "ModelSwap"; break;
		case	kExtraData_Radius                   	: return "Radius"; break;
		case	kExtraData_Radiation                	: return "Radiation"; break;
		case	kExtraData_FactionChanges           	: return "FactionChanges"; break;
		case	kExtraData_DismemberedLimbs         	: return "DismemberedLimbs"; break;
		case	kExtraData_MultiBound               	: return "MultiBound"; break;
		case	kExtraData_MultiBoundData           	: return "MultiBoundData"; break;
		case	kExtraData_MultiBoundRef            	: return "MultiBoundRef"; break;
		case	kExtraData_ReflectedRefs            	: return "ReflectedRefs"; break;
		case	kExtraData_ReflectorRefs            	: return "ReflectorRefs"; break;
		case	kExtraData_EmittanceSource          	: return "EmittanceSource"; break;
		case	kExtraData_RadioData                	: return "RadioData"; break;
		case	kExtraData_CombatStyle              	: return "CombatStyle"; break;
		case	kExtraData_Primitive                	: return "Primitive"; break;
		case	kExtraData_OpenCloseActivateRef     	: return "OpenCloseActivateRef"; break;
		case	kExtraData_AnimNoteReciever				: return "AnimNoteReciever"; break;
		case	kExtraData_Ammo                     	: return "Ammo"; break;
		case	kExtraData_PatrolRefData            	: return "PatrolRefData"; break;
		case	kExtraData_PackageData              	: return "PackageData"; break;
		case	kExtraData_OcclusionPlane           	: return "OcclusionPlane"; break;
		case	kExtraData_CollisionData            	: return "CollisionData"; break;
		case	kExtraData_SayTopicInfoOnceADay     	: return "SayTopicInfoOnceADay"; break;
		case	kExtraData_EncounterZone            	: return "EncounterZone"; break;
		case	kExtraData_SayToTopicInfo           	: return "SayToTopicInfo"; break;
		case	kExtraData_OcclusionPlaneRefData    	: return "OcclusionPlaneRefData"; break;
		case	kExtraData_PortalRefData            	: return "PortalRefData"; break;
		case	kExtraData_Portal                   	: return "Portal"; break;
		case	kExtraData_Room                     	: return "Room"; break;
		case	kExtraData_HealthPerc               	: return "HealthPerc"; break;
		case	kExtraData_RoomRefData              	: return "RoomRefData"; break;
		case	kExtraData_GuardedRefData           	: return "GuardedRefData"; break;
		case	kExtraData_CreatureAwakeSound       	: return "CreatureAwakeSound"; break;
		case	kExtraData_WaterZoneMap             	: return "WaterZoneMap"; break;
		case	kExtraData_IgnoredBySandbox         	: return "IgnoredBySandbox"; break;
		case	kExtraData_CellAcousticSpace        	: return "CellAcousticSpace"; break;
		case	kExtraData_ReservedMarkers          	: return "ReservedMarkers"; break;
		case	kExtraData_WeaponIdleSound          	: return "WeaponIdleSound"; break;
		case	kExtraData_WaterLightRefs           	: return "WaterLightRefs"; break;
		case	kExtraData_LitWaterRefs             	: return "LitWaterRefs"; break;
		case	kExtraData_WeaponAttackSound        	: return "WeaponAttackSound"; break;
		case	kExtraData_ActivateLoopSound        	: return "ActivateLoopSound"; break;
		case	kExtraData_PatrolRefInUseData       	: return "PatrolRefInUseData"; break;
		case	kExtraData_AshPileRef               	: return "AshPileRef"; break;
		case	kExtraData_CreatureMovementSound    	: return "CreatureMovementSound"; break;
		case	kExtraData_FollowerSwimBreadcrumbs  	: return "FollowerSwimBreadcrumbs"; break;
	};
	return "unknown";
}

class TESScript;
class TESScriptableForm;

ExtraScript* ExtraScript::Create(TESScriptableForm* pScript, bool create, TESObjectREFR* container) {
	const auto xScript = reinterpret_cast<ExtraScript*>(BSExtraData::Create(kExtraData_Script, sizeof(ExtraScript), s_ExtraScriptVtbl));
	if (xScript && (pScript && pScript->script)) {
		xScript->script = pScript->script;
		if (create) {
			xScript->eventList = xScript->script->CreateEventList();
			if (container)
				xScript->EventCreate(ScriptEventList::kEvent_OnAdd, container);
		}
	}
	return xScript;
}

void ExtraScript::EventCreate(UInt32 eventCode, TESObjectREFR* container) {
	if (eventList) {
		// create Event struct
		const auto pEvent = (ScriptEventList::Event*)FormHeap_Allocate(sizeof(ScriptEventList::Event));
		if (pEvent) {
			pEvent->eventMask = eventCode;
			pEvent->object = container;
		}

		if (!eventList->m_eventList) {
			eventList->m_eventList = (ScriptEventList::EventList*)FormHeap_Allocate(sizeof(ScriptEventList::EventList));
			eventList->m_eventList->Init();
		}
		if (eventList->m_eventList && pEvent)
			eventList->m_eventList->AddAt(pEvent, 0);
	}
}

ExtraFactionChanges* ExtraFactionChanges::Create()
{
	const auto xFactionChanges = (ExtraFactionChanges*)BSExtraData::Create(kExtraData_FactionChanges, sizeof(ExtraFactionChanges), s_ExtraFactionChangesVtbl);
	const auto FactionChangesData = (FactionListEntry*)FormHeap_Allocate(sizeof(FactionListEntry));
	memset(FactionChangesData, 0, sizeof(FactionListEntry));
	xFactionChanges->data = FactionChangesData;
	return xFactionChanges;
}

ExtraFactionChanges::FactionListEntry* GetExtraFactionList(BaseExtraList& xDataList)
{
	const auto xFactionChanges = GetByTypeCast(xDataList, FactionChanges);
	if (xFactionChanges)
		return xFactionChanges->data;
	return NULL;
}

SInt8 GetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction)
{
	const auto xFactionChanges = GetByTypeCast(xDataList, FactionChanges);
	if (xFactionChanges && xFactionChanges->data) {
		ExtraFactionChangesMatcher matcher(faction, xFactionChanges);
		ExtraFactionChanges::FactionListData* pData = xFactionChanges->data->Find(matcher);
		return (pData) ? pData->rank : -1;
	}
	return -1;
}

void SetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction, SInt8 rank)
{
	ExtraFactionChanges::FactionListData* pData = NULL;
	auto xFactionChanges = GetByTypeCast(xDataList, FactionChanges);
	if (xFactionChanges && xFactionChanges->data) {
		ExtraFactionChangesMatcher matcher(faction, xFactionChanges);
		pData = xFactionChanges->data->Find(matcher);
		if (pData)
			pData->rank = rank;
	}
	if (!pData) {
		if (!xFactionChanges) {
			xFactionChanges = ExtraFactionChanges::Create();
			xDataList.Add(xFactionChanges);
		}
		pData = (ExtraFactionChanges::FactionListData*)FormHeap_Allocate(sizeof(ExtraFactionChanges::FactionListData));
		if (pData) {
			pData->faction = faction;
			pData->rank = rank;
			xFactionChanges->data->AddAt(pData, -2);
		}
	}
}

ExtraHotkey* ExtraHotkey::Create()
{
	const auto xHotkey = (ExtraHotkey*)BSExtraData::Create(kExtraData_Hotkey, sizeof(ExtraHotkey), s_ExtraHotkeyVtbl);
	xHotkey->index  = 0;
	return xHotkey;
}

