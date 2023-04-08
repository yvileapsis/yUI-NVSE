#include <main.h>

#include "CrashLog.h"

#include "Form.h"
#include "Menus.h"
#include "Reference.h"
#include "SafeWrite.h"
#include "StackWalk.h"
#include "Tiles.h"

#define SYMOPT_EX_WINE_NATIVE_MODULES  1000

void StackWalk(EXCEPTION_POINTERS* info, HANDLE process, HANDLE thread);

constexpr UInt32 ce_printStackCount = 256;

void StackWalk(EXCEPTION_POINTERS* info, HANDLE process, HANDLE thread) {
	DWORD machine = IMAGE_FILE_MACHINE_I386;
	CONTEXT context = {};
	memcpy(&context, info->ContextRecord, sizeof(CONTEXT));
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);
	//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
	if (SymInitialize(process, NULL, TRUE) != TRUE) {
		Log() << FormatString("Error initializing symbol store");
	}
	//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);

	STACKFRAME frame = {};
	frame.AddrPC.Offset = info->ContextRecord->Eip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = info->ContextRecord->Ebp;
	frame.AddrFrame.Mode = AddrModeFlat;
	frame.AddrStack.Offset = info->ContextRecord->Esp;
	frame.AddrStack.Mode = AddrModeFlat;
	DWORD eip = 0;
	while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) {
		/*
	Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
	When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
		*/
		if (frame.AddrPC.Offset == eip) break;
		eip = frame.AddrPC.Offset;

		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
			   if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

		BOOL moduleAv = SymGetModuleInfo(process, frame.AddrPC.Offset, &module);
		std::string functioName;
		char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
		IMAGEHLP_SYMBOL* symbol = (IMAGEHLP_SYMBOL*)symbolBuffer;
		symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		symbol->MaxNameLength = 254;
		DWORD  offset = 0;
		if (!moduleAv) {
			//    Log() << FormatString("porcoddio2 %0X", GetLastError());
			Log() << FormatString("0x%08X ==> (0x%08X) ¯\\(°_o)/¯ (Corrupt stack or heap?)", frame.AddrPC.Offset, frame.AddrFrame.Offset);
			continue;
		}

		if (!SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol)) {
			//   Log() << FormatString("porcoddio1 %0X", GetLastError());
			//WIne dbghelp and Windows dbghelp operate totally differently
			Log() << FormatString("0x%08X ==> (0x%08X) %s : %s+0x%0X ", frame.AddrPC.Offset, frame.AddrFrame.Offset, module.ModuleName, "EntryPoint", (UInt32)-1);
			continue;
		}


		DWORD offset2 = 0;
		char lineBuffer[sizeof(IMAGEHLP_LINE) + 255];
		IMAGEHLP_LINE* line = (IMAGEHLP_LINE*)lineBuffer;
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

		functioName = symbol->Name;

		if (!SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset2, line)) {
			Log() << FormatString("0x%08X ==> (0x%08X) %s : %s+0x%0X", frame.AddrPC.Offset, frame.AddrFrame.Offset, module.ModuleName, functioName.c_str(), offset);
			continue;
		}

		Log() << FormatString("0x%08X ==> %-80s <== <%s:%d>", frame.AddrPC.Offset, FormatString("(0x%08X) %s : %s+0x%0X", frame.AddrFrame.Offset, module.ModuleName, functioName.c_str(), offset).c_str(), line->FileName, line->LineNumber);
	}
}

// take a look inside
namespace YvilesMagicBox
{



	UInt32 CanDereference1(UInt32 ptr)
	{
		__try
		{
			UInt32 vtbl = *(UInt32*)(ptr);
			return vtbl;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
		}
		return 0;
	}

	UInt32 CanDereference(UInt32 ptr)
	{
		try
		{
			return CanDereference1(ptr);
		}
		catch (...)
		{
			return 0;
		}

	}

	std::string Handle(Menu* menu)
	{
		return printTypeName(menu) + ": " + menu->tile->GetFullPath();
	}

	std::string Handle(Tile* tile)
	{
		return printTypeName(tile) + ": " + tile->GetFullPath();
	}

	std::string Handle(TESForm* form)
	{
		return printTypeName(form) + ": " + FormatString("%08X (%s)", form->refID, form->GetName());
	}

	std::string Handle(TESObjectREFR* refr)
	{
		return printTypeName(refr) + ": " + FormatString("%08X (%s), BaseForm %08X (%s)", refr->refID, refr->GetName(), refr->TryGetREFRParent()->refID, refr->TryGetREFRParent()->GetName());
	}

	std::string Handle(BSAnimGroupSequence* sequence)
	{
		return printTypeName(sequence) + ": " + FormatString("%s, %s, AnimGroup %04X", sequence->sequenceName, sequence->accumRootName, sequence->animGroup->groupID);
	}

	std::string Handle(AnimSequenceSingle* sequence)
	{
		return printTypeName(sequence) + ": " + FormatString("%s, %s, AnimGroup %04X", sequence->anim->sequenceName, sequence->anim->accumRootName, sequence->anim->animGroup->groupID);
	}

	std::string Handle(NiObjectNET* sequence)
	{
		return printTypeName(sequence) + ": " + FormatString("%s", sequence->m_pcName);
	}

	std::string Handle(BaseProcess* process)
	{
		for (const auto iter : *TESForm::GetAll())
		{
			if (iter->typeID == kFormType_Character && static_cast<Character*>(iter)->baseProcess == process)
			{
				return printTypeName(process) + ": " + FormatString("%08X (%s)", iter->refID, iter->GetName());
			}
			if (iter->typeID == kFormType_Creature && static_cast<Creature*>(iter)->baseProcess == process)
			{
				return printTypeName(process) + ": " + FormatString("%08X (%s)", iter->refID, iter->GetName());
			}
		}
		return "";
	}

	std::string GetStringForVTBL(void* ptr, UInt32 vtbl)
	{
		
		switch (vtbl)
		{
			// UI

		case 0x10472D0:				return "BSSimpleArray<char const *>:";
		case 0x10472CC:				return "BSSimpleArray<char const *>:";
		case 0x106EDEC:				return "BSSimpleArray<Tile::Value*>:";
		case 0x106CC64:				return "BSSimpleArray<float>";

		case kVtbl_Menu:
		case kVtbl_TutorialMenu:
		case kVtbl_StatsMenu:
		case kVtbl_TextEditMenu: 
		case kVtbl_SurgeryMenu: 
		case kVtbl_VATSMenu: 
		case kVtbl_SpecialBookMenu: 
		case kVtbl_LoveTesterMenu: 
		case kVtbl_QuantityMenu: 
		case kVtbl_RecipeMenu: 
		case kVtbl_BarterMenu: 
		case kVtbl_BlackjackMenu: 
		case kVtbl_BookMenu: 
		case kVtbl_CaravanMenu: 
		case kVtbl_CharGenMenu: 
		case kVtbl_CompanionWheelMenu: 
		case kVtbl_ComputersMenu: 
		case kVtbl_ContainerMenu: 
		case kVtbl_CreditsMenu: 
		case kVtbl_DialogMenu: 
		case kVtbl_HackingMenu: 
		case kVtbl_HUDMainMenu: 
		case kVtbl_InventoryMenu: 
		case kVtbl_ItemModMenu: 
		case kVtbl_LevelUpMenu: 
		case kVtbl_LoadingMenu: 
		case kVtbl_LockpickMenu: 
		case kVtbl_MapMenu: 
		case kVtbl_MessageMenu: 
		case kVtbl_PlayerNameEntryMenu: 
		case kVtbl_RaceSexMenu: 
		case kVtbl_RepairMenu: 
		case kVtbl_RepairServicesMenu: 
		case kVtbl_RouletteMenu: 
		case kVtbl_SleepWaitMenu: 
		case kVtbl_SlotMachineMenu: 
		case kVtbl_StartMenu: 
		case kVtbl_TraitMenu: 
		case kVtbl_TraitSelectMenu: return Handle(reinterpret_cast<Menu*>(ptr));

		case kVtbl_Tile:			
		case kVtbl_TileMenu:		
		case kVtbl_TileRect:		
		case kVtbl_TileImage:		return Handle(reinterpret_cast<Tile*>(ptr));



			// NPC parts
		case 0x104A284: return "TESNPC::TESActorBase:";
		case 0x104A270: return "TESNPC::container:";
		case 0x104A25C: return "TESNPC::touchSpell:";
		case 0x104A23C: return "TESNPC::spellList:";
		case 0x104A21C: return "TESNPC::ai:";
		case 0x104A204: return "TESNPC::health:";
		case 0x104A1F0: return "TESNPC::attributes:";
		case 0x104A1DC: return "TESNPC::animation:";
		case 0x104A1C8: return "TESNPC::fullName:";
		case 0x104A1A4: return "TESNPC::model:";
		case 0x104A190: return "TESNPC::scriptable:";
		case 0x104A160: return "TESNPC::avOwner:";
		case 0x104A14C: return "TESNPC::destructible:";
		case 0x104A138: return "TESNPC::race:";


			// Forms
		case kVtbl_TESClimate:
		case kVtbl_TESImageSpace:
		case kVtbl_TESImageSpaceModifier:

		case kVtbl_BGSTerminal:
		case kVtbl_BGSExplosion:
		case kVtbl_BGSProjectile:
		case kVtbl_BGSTalkingActivator:
		case kVtbl_TESObjectANIO:
		case kVtbl_TESObjectARMA:
		case kVtbl_TESObjectARMO:
		case kVtbl_TESObjectBOOK:
		case kVtbl_TESObjectCLOT:
		case kVtbl_TESObjectIMOD:
		case kVtbl_TESObjectMISC:
		case kVtbl_TESObjectSTAT:
		case kVtbl_TESObjectTREE:
		case kVtbl_TESObjectWEAP:
		case kVtbl_TESObjectCONT:
		case kVtbl_TESObjectDOOR:

		case kVtbl_EnchantmentItem:
		case kVtbl_TESLandTexture:
		case kVtbl_EffectSetting:
		case kVtbl_TESReputation:
		case kVtbl_TESSkill:
		case kVtbl_TESTopic:
		case kVtbl_TESSound:
		case kVtbl_TESChallenge:
		case kVtbl_TESClass:
		case kVtbl_BGSNote:
		case kVtbl_BGSPerk:
		case kVtbl_TESCaravanCard:
		case kVtbl_TESCaravanMoney:
		case kVtbl_TESCaravanDeck:
		case kVtbl_TESGlobal:
		case kVtbl_TESRecipe:
		case kVtbl_TESWeather:
		case kVtbl_TESWorldSpace:
		case kVtbl_BGSListForm:
		case kVtbl_TESQuest:
		case kVtbl_TESRace:
		case kVtbl_TESEyes:
		case kVtbl_TESFaction:
		case kVtbl_TESHair:
		case kVtbl_TESForm:
		case kVtbl_AlchemyItem:
		case kVtbl_BGSHeadPart:
		case kVtbl_BGSPlaceableWater:
		case kVtbl_TESWaterForm:		
		case kVtbl_TESObjectCELL:		
		case kVtbl_TESCreature:			
		case kVtbl_TESNPC:
		case kVtbl_Script:
		case kVtbl_TESObject:
		case kVtbl_TESAmmo:
		case kVtbl_TESCasinoChips:
		case kVtbl_TESCasino:
		case kVtbl_TESCombatStyle:
		case kVtbl_TESEffectShader:
		case kVtbl_TESFlora:
		case kVtbl_TESFurniture:
		case kVtbl_TESGrass:
		case kVtbl_TESKey:
		case kVtbl_TESLevCharacter:
		case kVtbl_TESLevCreature:
		case kVtbl_TESLevItem:
		case kVtbl_TESLevSpell:
		case kVtbl_TESBoundObject:
		case kVtbl_TESBoundAnimObject:
		case kVtbl_TESBoundTreeObject:
		case kVtbl_BGSMessage:

			return Handle(reinterpret_cast<TESForm*>(ptr));

			// ObjectREFR
		case kVtbl_TESObjectREFR:
		case kVtbl_TESObjectACTI:
		case kVtbl_MobileObject:	
		case kVtbl_Actor:			
		case kVtbl_Creature:		
		case kVtbl_Character:		
		case kVtbl_PlayerCharacter:		return Handle(reinterpret_cast<TESObjectREFR*>(ptr));

		case kVtbl_BaseProcess:			
		case kVtbl_LowProcess:			
		case kVtbl_MiddleLowProcess:	
		case kVtbl_MiddleHighProcess:	
		case kVtbl_HighProcess:			return Handle(reinterpret_cast<BaseProcess*>(ptr));



			// Ni

		case kVtbl_NiNode:			return Handle(reinterpret_cast<NiObjectNET*>(ptr));





			// Bound
		case kVtbl_BSFadeNode:	return Handle(reinterpret_cast<BSFadeNode*>(ptr));





			// animations
		case kVtbl_BSAnimGroupSequence: return Handle(reinterpret_cast<BSAnimGroupSequence*>(ptr));
		case kVtbl_AnimSequenceSingle: return Handle(reinterpret_cast<AnimSequenceSingle*>(ptr));




		case kVtbl_BGSDehydrationStage:
		case kVtbl_BGSHungerStage:
		case kVtbl_BGSSleepDeprevationStage:
		case kVtbl_ShadowSceneNode:

		case kVtbl_MagicItemObject:
		case kVtbl_TESIcon:
		case kVtbl_MagicItem:
		case kVtbl_TESFullName:
		case kVtbl_BaseFormComponent:
		case kVtbl_Setting:
		case kVtbl_GameSettingCollection:
		case kVtbl_EffectItemList:
		case kVtbl_MagicItemForm:
		case kVtbl_IngredientItem:
		case kVtbl_SpellItem:
		case kVtbl_ExtraCell3D:
		case kVtbl_BSExtraData:
		case kVtbl_ExtraHavok:
		case kVtbl_ExtraRegionList:
		case kVtbl_ExtraCellMusicType:
		case kVtbl_ExtraCellAcousticSpace:
		case kVtbl_ExtraCellClimate:
		case kVtbl_ExtraCellImageSpace:
		case kVtbl_ExtraCellImpactSwap:
		case kVtbl_ExtraCellWaterType:
		case kVtbl_ExtraProcessMiddleLow:
		case kVtbl_ExtraCellCanopyShadowMask:
		case kVtbl_ExtraSeenData:
		case kVtbl_ExtraNorthRotation:
		case kVtbl_ExtraDetachTime:
		case kVtbl_BaseExtraList:
		case kVtbl_ExtraDataList:
		case kVtbl_ExtraReflectedRefs:
		case kVtbl_ExtraReflectorRefs:
		case kVtbl_ExtraWaterLightRefs:
		case kVtbl_ExtraLitWaterRefs:
		case kVtbl_ExtraSpecialRenderFlags:
		case kVtbl_ExtraNavMeshPortal:
		case kVtbl_ExtraRadioData:
		case kVtbl_ExtraHealthPerc:
		case kVtbl_ExtraObjectHealth:
		case kVtbl_ExtraTerminalState:
		case kVtbl_ExtraEditorID:
		case kVtbl_ExtraOpenCloseActivateRef:
		case kVtbl_ExtraPrimitive:
		case kVtbl_ExtraPatrolRefData:
		case kVtbl_ExtraOcclusionPlaneRefData:
		case kVtbl_ExtraPortalRefData:
		case kVtbl_ExtraRoomRefData:
		case kVtbl_ExtraCollisionData:
		case kVtbl_ExtraPackageData:
		case kVtbl_ExtraRadius:
		case kVtbl_ExtraRadiation:
		case kVtbl_ExtraLock:
		case kVtbl_ExtraTeleport:
		case kVtbl_ExtraOwnership:
		case kVtbl_ExtraGlobal:
		case kVtbl_ExtraRank:
		case kVtbl_ExtraCount:
		case kVtbl_ExtraHealth:
		case kVtbl_ExtraUses:
		case kVtbl_ExtraTimeLeft:
		case kVtbl_ExtraCharge:
		case kVtbl_ExtraScript:
		case kVtbl_ExtraScale:
		case kVtbl_ExtraHotkey:
		case kVtbl_ExtraReferencePointer:
		case kVtbl_ExtraTresPassPackage:
		case kVtbl_ExtraLeveledItem:
		case kVtbl_ExtraPoison:
		case kVtbl_ExtraHeadingTarget:
		case kVtbl_ExtraHasNoRumors:
		case kVtbl_ExtraModelSwap:
		case kVtbl_ExtraActorCause:
		case kVtbl_ExtraAmmo:
		case kVtbl_ExtraWeaponModFlags:
		case kVtbl_ExtraSecuritronFace:
		case kVtbl_ExtraModdingItem:
		case kVtbl_ExtraCombatStyle:
		case kVtbl_ExtraAnim:
		case kVtbl_ExtraDismemberedLimbs:
		case kVtbl_ExtraStartingPosition:
		case kVtbl_ExtraStartingWorldOrCell:
		case kVtbl_ExtraLight:
		case kVtbl_ExtraFollower:
		case kVtbl_ExtraGuardedRefData:
		case kVtbl_ExtraLeveledCreature:
		case kVtbl_ExtraMapMarker:
		case kVtbl_ExtraAudioMarker:
		case kVtbl_ExtraAudioBuoyMarker:
		case kVtbl_ExtraAction:
		case kVtbl_ExtraContainerChanges:
		case kVtbl_ExtraOriginalReference:
		case kVtbl_ExtraGhost:
		case kVtbl_ExtraWorn:
		case kVtbl_ExtraWornLeft:
		case kVtbl_ExtraCannotWear:
		case kVtbl_ExtraInfoGeneralTopic:
		case kVtbl_ExtraSeed:
		case kVtbl_ExtraPackageStartLocation:
		case kVtbl_ExtraPackage:
		case kVtbl_ExtraPlayerCrimeList:
		case kVtbl_ExtraPersistentCell:
		case kVtbl_ExtraRagDollData:
		case kVtbl_ExtraEncounterZone:
		case kVtbl_ExtraUsedMarkers:
		case kVtbl_ExtraReservedMarkers:
		case kVtbl_ExtraRunOncePacks:
		case kVtbl_ExtraDistantData:
		case kVtbl_ExtraEnableStateParent:
		case kVtbl_ExtraEnableStateChildren:
		case kVtbl_ExtraRandomTeleportMarker:
		case kVtbl_ExtraLinkedRefChildren:
		case kVtbl_ExtraLinkedRef:
		case kVtbl_ExtraAshPileRef:
		case kVtbl_ExtraActivateRefChildren:
		case kVtbl_ExtraActivateRef:
		case kVtbl_ExtraDecalRefs:
		case kVtbl_ExtraMerchantContainer:
		case kVtbl_ExtraLevCreaModifier:
		case kVtbl_ExtraLastFinishedSequence:
		case kVtbl_ExtraXTarget:
		case kVtbl_ExtraEmittanceSource:
		case kVtbl_ExtraMultiBoundRef:
		case kVtbl_ExtraMultiBoundData:
		case kVtbl_ExtraMultiBound:
		case kVtbl_ExtraOcclusionPlane:
		case kVtbl_ExtraPortal:
		case kVtbl_ExtraRoom:
		case kVtbl_ExtraItemDropper:
		case kVtbl_ExtraDroppedItemList:
		case kVtbl_ExtraSavedAnimation:
		case kVtbl_ExtraSavedHavokData:
		case kVtbl_ExtraFriendHits:
		case kVtbl_ExtraRefractionProperty:
		case kVtbl_ExtraEditorRefMoveData:
		case kVtbl_ExtraSound:
		case kVtbl_ExtraCreatureAwakeSound:
		case kVtbl_ExtraCreatureMovementSound:
		case kVtbl_ExtraWeaponIdleSound:
		case kVtbl_ExtraWeaponAttackSound:
		case kVtbl_ExtraActivateLoopSound:
		case kVtbl_ExtraTalkingActor:
		case kVtbl_ExtraFactionChanges:
		case kVtbl_ExtraSayTopicInfoOnceADay:
		case kVtbl_ExtraSayToTopicInfo:
		case kVtbl_ExtraWaterZoneMap:
		case kVtbl_ExtraIgnoredBySandbox:
		case kVtbl_ExtraPatrolRefInUseData:
		case kVtbl_ExtraFollowerSwimBreadcrumbs:
		case kVtbl_NiAlphaProperty:
		case kVtbl_NiProperty:
		case kVtbl_QueuedTexture:
		case kVtbl_QueuedModel:
		case kVtbl_BSStream:
		case kVtbl_QueuedTreeBillboard:
		case kVtbl_QueuedTreeModel:
		case kVtbl_QueuedMagicItem:
		case kVtbl_QueuedKF:
		case kVtbl_QueuedAnimIdle:
		case kVtbl_QueuedReplacementKF:
		case kVtbl_QueuedHead:
		case kVtbl_QueuedHelmet:
		case kVtbl_QueuedReference:
		case kVtbl_AttachDistant3DTask:
		case kVtbl_IOTask:
		case kVtbl_QueuedTree:
		case kVtbl_QueuedActor:
		case kVtbl_QueuedCharacter:
		case kVtbl_QueuedCreature:
		case kVtbl_QueuedPlayer:
		case kVtbl_QueuedFileLoad:
		case kVtbl_QueuedFaceGenFile:
		case kVtbl_BackgroundCloneThread:
		case kVtbl_QueuedReplacementKFList:
		case kVtbl_InterfacedClass:
		case kVtbl_INISettingCollection:
		case kVtbl_TES:
		case kVtbl_BSTempNodeManager:
		case kVtbl_BSTempNode:
		case kVtbl_RendererSettingCollection:
		case kVtbl_BGSAmmoForm:
		case kVtbl_BGSBipedModelList:
		case kVtbl_BGSClipRoundsForm:
		case kVtbl_BGSDestructibleObjectForm:
		case kVtbl_BGSEquipType:
		case kVtbl_BGSIdleCollection:
		case kVtbl_BGSMessageIcon:
		case kVtbl_BGSPickupPutdownSounds:
		case kVtbl_BGSRepairItemList:
		case kVtbl_BGSTouchSpellForm:
		case kVtbl_TESActorBaseData:
		case kVtbl_TESAIForm:
		case kVtbl_TESAnimation:
		case kVtbl_TESAttackDamageForm:
		case kVtbl_TESAttributes:
		case kVtbl_TESBipedModelForm:
		case kVtbl_TESModelRDT:
		case kVtbl_TESContainer:
		case kVtbl_TESDescription:
		case kVtbl_TESEnchantableForm:

		case kVtbl_TESHealthForm:
		case kVtbl_TESImageSpaceModifiableForm:
		case kVtbl_TESLeveledList:
		case kVtbl_TESModel:
		case kVtbl_TESModelList:
		case kVtbl_TESModelTextureSwap:
		case kVtbl_TESProduceForm:
		case kVtbl_TESRaceForm:
		case kVtbl_TESReactionForm:
		case kVtbl_TESScriptableForm:
		case kVtbl_TESSpellList:
		case kVtbl_TESTexture:
		case kVtbl_TESValueForm:
		case kVtbl_TESWeightForm:

		case kVtbl_AnimSequenceBase:
		case kVtbl_AnimSequenceMultiple:
		case kVtbl_AnimIdle:
		case kVtbl_NiRefObject:

		case kVtbl_NiStencilProperty:
		case kVtbl_BSCullingProcess:
		case kVtbl_hkpAllRayHitCollector:
		case kVtbl_hkpRayHitCollector:
		case kVtbl_BGSPrimitive:
		case kVtbl_BGSPrimitivePlane:
		case kVtbl_BSOcclusionPlane:
		case kVtbl_BSPortal:
		case kVtbl_BGSPrimitiveBox:
		case kVtbl_BSMultiBoundAABB:
		case kVtbl_BSMultiBoundOBB:
		case kVtbl_BGSPrimitiveSphere:
		case kVtbl_BSMultiBoundSphere:
		case kVtbl_BGSSceneInfo:
		case kVtbl_BSAnimNoteReceiver:
		case kVtbl_IBSAnimNoteReceiver:
		case kVtbl_bhkMouseSpringAction:
		case kVtbl_bhkUnaryAction:
		case kVtbl_bhkAction:
		case kVtbl_bhkSerializable:
		case kVtbl_GridArray:
		case kVtbl_GridCellArray:
		case kVtbl_INIPrefSettingCollection:
		case kVtbl_BSCustomSplatterExtraData:
		case kVtbl_BSSplatterExtraData:
		case kVtbl_ImageSpaceEffectParam:
		case kVtbl_BGSDecalNode:
		case kVtbl_TESRegion:
		case kVtbl_TESRegionData:
		case kVtbl_TESRegionDataGrass:
		case kVtbl_TESRegionDataImposter:
		case kVtbl_TESRegionDataLandscape:
		case kVtbl_TESRegionDataManager:
		case kVtbl_TESRegionDataMap:
		case kVtbl_TESRegionDataSound:
		case kVtbl_TESRegionDataWeather:
		case kVtbl_TESRegionGrassObject:
		case kVtbl_TESRegionObjectBase:
		case kVtbl_TESRegionGrassObjectList:
		case kVtbl_TESRegionList:
		case kVtbl_BGSAddonNode:
		case kVtbl_BGSAddonNodeSoundHandleExtra:
		case kVtbl_BGSConstructibleObject:
		case kVtbl_BGSDebris:
		case kVtbl_BGSPreloadable:

		case kVtbl_BGSMovableStatic:


		case kVtbl_BGSStaticCollection:
		case kVtbl_PathingLocation:

		case kVtbl_BGSOpenCloseForm:

		case kVtbl_TESModelTree:
		case kVtbl_TESIconTree:
		case kVtbl_TESTexture1024:
		case kVtbl_NiBillboardNode:
		case kVtbl_BSShaderTextureSet:
		case kVtbl_BSTextureSet:
		case kVtbl_BGSEncounterZone:
		case kVtbl_BGSLightingTemplate:
		case kVtbl_LoadedReferenceMap:
		case kVtbl_ExteriorCellLoaderTask:
		case kVtbl_ImageSpaceModifierInstanceForm:
		case kVtbl_ImageSpaceModifierInstance:
		case kVtbl_ImageSpaceModifierInstanceDOF:
		case kVtbl_ImageSpaceModifierInstanceRB:
		case kVtbl_ImageSpaceModifierInstanceDRB:
		case kVtbl_ImageSpaceModifierInstanceTemp:

		case kVtbl_NiFloatData:
		case kVtbl_NiColorData:
		case kVtbl_TESObjectLAND:
		case kVtbl_TESChildCell:
		case kVtbl_BSPackedAdditionalGeometryData:
		case kVtbl_NiAdditionalGeometryData:
		case kVtbl_hkpMoppCode:
		case kVtbl_hkReferencedObject:
		case kVtbl_hkBaseObject:
		case kVtbl_NiBinaryExtraData:

		case kVtbl_CheckWithinMultiBoundTask:
		case kVtbl_hkpWorldCinfo:
		case kVtbl_TESWindListener:
		case kVtbl_hkpEntityListener:

		case kVtbl_bhkRigidBody:
		case kVtbl_bhkEntity:
		case kVtbl_bhkWorldObject:
		case kVtbl_bhkCollisionObject:
		case kVtbl_bhkWaterPhantomCallbackShape:
		case kVtbl_hkpPhantomCallbackShape:
		case kVtbl_hkpShape:
		case kVtbl_bhkSimpleShapePhantom:
		case kVtbl_bhkShapePhantom:
		case kVtbl_bhkPhantom:
		case kVtbl_bhkAabbPhantom:
		case kVtbl_bhkBoxShape:
		case kVtbl_bhkConvexShape:
		case kVtbl_bhkSphereRepShape:
		case kVtbl_bhkShape:
		case kVtbl_bhkSphereShape:
		case kVtbl_bhkAcousticSpacePhantomCallbackShape:
		case kVtbl_ExtraAnimNoteReceiver:


		case kVtbl_BGSAcousticSpace:
		case kVtbl_BGSCameraPath:
		case kVtbl_NiFormArray:
		case kVtbl_BGSCameraShot:
		case kVtbl_BGSDefaultObjectManager:
		case kVtbl_BGSImpactData:
		case kVtbl_BGSImpactDataSet:
		case kVtbl_BGSMenuIcon:
		case kVtbl_BGSMusicType:
		case kVtbl_TESSoundFile:
		case kVtbl_BGSRadiationStage:
		case kVtbl_BGSTextureSet:
		case kVtbl_MediaLocationController:
		case kVtbl_MediaSet:
		case kVtbl_TESAmmoEffect:

		case kVtbl_hkpClosestRayHitCollector:
		case kVtbl_TESLoadScreen:
		case kVtbl_TESLoadScreenType:
		case kVtbl_TESRecipeCategory:

		case kVtbl_MobIterOperator:
		case kVtbl_BGSBodyPart:
		case kVtbl_TESModelPSA:
		case kVtbl_BGSBodyPartData:
		case kVtbl_BGSEntryPointFunctionDataOneValue:
		case kVtbl_BGSEntryPointFunctionData:
		case kVtbl_BGSEntryPointFunctionDataTwoValue:
		case kVtbl_BGSEntryPointFunctionDataLeveledList:
		case kVtbl_BGSEntryPointFunctionDataActivateChoice:
		case kVtbl_BGSIdleMarker:
		case kVtbl_BGSPerkEntry:
		case kVtbl_BGSQuestPerkEntry:
		case kVtbl_BGSAbilityPerkEntry:
		case kVtbl_BGSEntryPointPerkEntry:
		case kVtbl_BGSQuestObjective:
		case kVtbl_BGSRagdoll:
		case kVtbl_BGSVoiceType:
		case kVtbl_Reset3DMobIterator:
		case kVtbl_TESAnimGroup:

		case kVtbl_TESActorBase:
		case kVtbl_ActorValueOwner:
		case kVtbl_TESIdleForm:
		case kVtbl_TESModelAnim:

		case kVtbl_BGSTextureModel:

		case kVtbl_TopicInfoArray:
		case kVtbl_TESTopicInfo:
		case kVtbl_BGSAcousticSpaceListener:
		case kVtbl_bhkEntityListener:
		case kVtbl_TargetEntry:
		case kVtbl_ZoneEntry:
		case kVtbl_TrapTargetEntry:
		case kVtbl_BGSZoneTargetListener:
		case kVtbl_hkpAllCdPointCollector:
		case kVtbl_hkpCdPointCollector:
		case kVtbl_CellMopp:
		case kVtbl_bhkTriSampledHeightFieldBvTreeShape:
		case kVtbl_bhkBvTreeShape:
		case kVtbl_FOCollisionListener:
		case kVtbl_bhkContactListener:
		case kVtbl_hkpContactListener:
		case kVtbl_FOIslandActivationListener:
		case kVtbl_hkpIslandActivationListener:
		case kVtbl_HavokStreambufFactory:
		case kVtbl_hkFileSystem:
		case kVtbl_HavokError:
		case kVtbl_hkError:
		case kVtbl_hkStreamReader:
		case kVtbl_hkStreamWriter:
		case kVtbl_HavokFileStreambufReader:
		case kVtbl_HavokFileStreambufWriter:
		case kVtbl_LoadedAreaBound:
		case kVtbl_SpecificItemCollector:
		case kVtbl_TESArrowTargetListener:
		case kVtbl_bhkArrowTargetListener:
		case kVtbl_hkpAllCdBodyPairCollector:
		case kVtbl_hkpCdBodyPairCollector:
		case kVtbl_TESTrapListener:
		case kVtbl_bhkTrapListener:
		case kVtbl_hkpPhantomListener:
		case kVtbl_TrapEntry:
		case kVtbl_WaterTargetEntry:
		case kVtbl_TESWaterListener:
		case kVtbl_WaterZone:
		case kVtbl_BlendSettingCollection:
		case kVtbl_Atmosphere:
		case kVtbl_Clouds:
		case kVtbl_Moon:
		case kVtbl_Precipitation:
		case kVtbl_Sky:
		case kVtbl_SkyObject:
		case kVtbl_SkyTask:
		case kVtbl_Stars:
		case kVtbl_Sun:
		case kVtbl_BSFaceGenAnimationData:
		case kVtbl_BSFaceGenImage:
		case kVtbl_BSFaceGenKeyframe:
		case kVtbl_BSFaceGenKeyframeMultiple:
		case kVtbl_BSFaceGenModelMap:
		case kVtbl_BSFaceGenModel:
		case kVtbl_BSFaceGenModelExtraData:
		case kVtbl_BSFaceGenMorph:
		case kVtbl_BSFaceGenMorphDifferential:
		case kVtbl_BSFaceGenMorphStatistical:
		case kVtbl_BSFaceGenMorphDataHead:
		case kVtbl_BSFaceGenMorphData:
		case kVtbl_BSFaceGenMorphDataHair:
		case kVtbl_BSFaceGenBaseMorphExtraData:
		case kVtbl_BSFaceGenNiNode:
		case kVtbl_BSTreeModel:
		case kVtbl_BSTreeNode:
		case kVtbl_bhkSPCollisionObject:
		case kVtbl_bhkPCollisionObject:
		case kVtbl_bhkMultiSphereShape:
		case kVtbl_bhkTransformShape:
		case kVtbl_bhkCapsuleShape:
		case kVtbl_ActorValueInfo:
		case kVtbl_TESPackage:
		case kVtbl_TESAmbushPackageData:
		case kVtbl_TESPackageData:
		case kVtbl_TESDialoguePackageData:
		case kVtbl_TESEatPackageData:
		case kVtbl_TESEscortPackageData:
		case kVtbl_TESFollowPackageData:
		case kVtbl_TESPatrolPackageData:
		case kVtbl_TESUseItemPackageData:
		case kVtbl_TESUseWeaponPackageData:
		case kVtbl_UseWeaponActorPackageData:
		case kVtbl_ActorPackageData:
		case kVtbl_NiTask:
		case kVtbl_BSTempEffect:
		case kVtbl_BSTempEffectDebris:
		case kVtbl_BSTempEffectGeometryDecal:
		case kVtbl_BSDismemberSkinInstance:
		case kVtbl_NiSkinInstance:
		case kVtbl_BSTempEffectParticle:
		case kVtbl_BSTempEffectSimpleDecal:
		case kVtbl_NavMesh:
		case kVtbl_NavMeshPOVSearch:
		case kVtbl_NavMeshSearch:
		case kVtbl_NavMeshSearchClosePoint:
		case kVtbl_NavMeshSearchFlee:
		case kVtbl_NavMeshSearchHide:
		case kVtbl_NavMeshSearchLOS:
		case kVtbl_NavMeshSearchMaxCost:
		case kVtbl_NavMeshSearchSLPoint:
		case kVtbl_PathSmootherPOVSearch:
		case kVtbl_NavMeshInfoMap:
		case kVtbl_NavMeshInfoSearch:
		case kVtbl_ObstacleUndoData:
		case kVtbl_bhkObstacleDeactivationListener:
		case kVtbl_hkpEntityActivationListener:
		case kVtbl_bhkObstacleRemovalListener:
		case kVtbl_ReferenceObstacleArray:
		case kVtbl_ObstacleData:
		case kVtbl_BSTasklet:
		case kVtbl_ObstacleTaskData:
		case kVtbl_BSWin32TaskletData:
		case kVtbl_BSTaskletData:
		case kVtbl_FindTriangleForLocationFilter:
		case kVtbl_CheckLineOfSightFilterWater:
		case kVtbl_PathingCoverLocation:
		case kVtbl_PathingLOSGridMap:
		case kVtbl_PathingLOSMap:
		case kVtbl_PathingRequest:
		case kVtbl_PathingAvoidNodeArray:
		case kVtbl_PathingRequestClosePoint:
		case kVtbl_PathingRequestCover:
		case kVtbl_PathingRequestCoveredMove:
		case kVtbl_PathingRequestFlee:
		case kVtbl_PathingRequestHide:
		case kVtbl_PathingRequestLOS:
		case kVtbl_PathingRequestOptimalLocation:
		case kVtbl_PathingRequestSafeStraightLine:
		case kVtbl_PathingSearchRayCast:
		case kVtbl_PathingSolution:
		case kVtbl_PathingTaskData:
		case kVtbl_TeleportDoorSearch:
		case kVtbl_BGSDistantObjectBlockLoadTask:
		case kVtbl_BGSDistantTreeBlockLoadTask:
		case kVtbl_BGSTerrainChunkLoadTask:
		case kVtbl_CCallbackBase:
		case kVtbl_FORenderedTerminal:


		case kVtbl_ListBoxBase:


		case kVtbl_LoadingMenuThread:


		case kVtbl_FORenderedMenuRaceSex:


		case kVtbl_FOPipboyManager:
		case kVtbl_FORenderedMenu:
		case kVtbl_AbsorbEffect:
		case kVtbl_ActiveEffect:
		case kVtbl_AssociatedItemEffect:
		case kVtbl_bhkCharacterListenerSpell:
		case kVtbl_bhkTelekinesisListener:
		case kVtbl_BoundItemEffect:
		case kVtbl_CalmEffect:
		case kVtbl_ChameleonEffect:
		case kVtbl_CommandCreatureEffect:
		case kVtbl_CommandEffect:
		case kVtbl_CommandHumanoidEffect:
		case kVtbl_ConcussionEffect:
		case kVtbl_CureEffect:
		case kVtbl_DarknessEffect:
		case kVtbl_DemoralizeEffect:
		case kVtbl_DetectLifeEffect:
		case kVtbl_DisintegrateArmorEffect:
		case kVtbl_DisintegrateWeaponEffect:
		case kVtbl_DispelEffect:
		case kVtbl_FrenzyEffect:
		case kVtbl_InvisibilityEffect:
		case kVtbl_LightEffect:
		case kVtbl_LimbConditionEffect:
		case kVtbl_LockEffect:
		case kVtbl_MagicBallProjectile:
		case kVtbl_MagicBoltProjectile:
		case kVtbl_MagicCaster:
		case kVtbl_MagicFogProjectile:
		case kVtbl_MagicHitEffect:
		case kVtbl_MagicModelHitEffect:
		case kVtbl_MagicProjectile:
		case kVtbl_MagicShaderHitEffect:
		case kVtbl_MagicSprayProjectile:
		case kVtbl_MagicTarget:
		case kVtbl_NightEyeEffect:
		case kVtbl_NonActorMagicCaster:
		case kVtbl_NonActorMagicTarget:
		case kVtbl_OpenEffect:
		case kVtbl_ParalysisEffect:
		case kVtbl_ReanimateEffect:
		case kVtbl_ScriptEffect:
		case kVtbl_ShieldEffect:
		case kVtbl_SummonCreatureEffect:
		case kVtbl_SunDamageEffect:
		case kVtbl_TelekinesisEffect:
		case kVtbl_TurnUndeadEffect:
		case kVtbl_ValueAndConditionsEffect:
		case kVtbl_ValueModifierEffect:
		case kVtbl_VampirismEffect:
		case kVtbl_CBSAStream:
		case kVtbl_CAsyncStream:
		case kVtbl_CBSAReader:
		case kVtbl_RadioConvTask:
		case kVtbl_BGSReconstructFormsMap:
		case kVtbl_BGSReconstructCellsMap:
		case kVtbl_BGSReconstructCellSubBlocksMap:
		case kVtbl_BGSReconstructCellBlocksMap:
		case kVtbl_BGSReconstructWorldSpacesMap:
		case kVtbl_BGSReconstructFormsInFileMap:
		case kVtbl_BGSOffsetDataArray:
		case kVtbl_BGSReconstructFormsInAllFilesMap:
		case kVtbl_BGSSaveLoadChangesMap:
		case kVtbl_BGSCellNumericIDArrayMap:
		case kVtbl_BGSSaveLoadStatsMap:
		case kVtbl_ChangesMap:
		case kVtbl_InteriorCellNewReferencesMap:
		case kVtbl_ExteriorCellNewReferencesMap:
		case kVtbl_NumericIDBufferMap:
		case kVtbl_SaveGameFile:
		case kVtbl_BGSLoadFormBuffer:
		case kVtbl_BGSLoadGameBuffer:
		case kVtbl_BGSSaveFormBuffer:
		case kVtbl_BGSSaveGameBuffer:
		case kVtbl_BGSMoviePlayer:
		case kVtbl_HighActorCuller:
		case kVtbl_BSFadeNodeCuller:
		case kVtbl_RegSettingCollection:
		case kVtbl_MemoryManagerFile:
		case kVtbl_IMemoryManagerFile:
		case kVtbl_SceneGraph:
		case kVtbl_SeenData:
		case kVtbl_IntSeenData:

		case kVtbl_CachedValuesOwner:
		case kVtbl_bhkFilteredCdBodyCollector:
		case kVtbl_AILinearTaskThread:
		case kVtbl_BSLinearTaskThread:
		case kVtbl_DetectionTaskData:
		case kVtbl_MobileObjectTaskletData:
		case kVtbl_AnimationTaskData:
		case kVtbl_ActorUpdateTaskData:
		case kVtbl_PackageUpdateTaskData:
		case kVtbl_ActorsScriptTaskData:
		case kVtbl_MovementTaskData:
		case kVtbl_AITaskThread:
		case kVtbl_ArrowProjectile:

		case kVtbl_bhkCharacterListenerArrow:
		case kVtbl_BSPlayerDistanceCheckController:


		case kVtbl_ActorPathingMessageQueue:
		case kVtbl_LipTask:


		case kVtbl_BeamProjectile:
		case kVtbl_CombatAction:
		case kVtbl_CombatActionAttackRanged:
		case kVtbl_CombatActionAttackRangedFromCover: 
		case kVtbl_CombatActionAttackRangedExplosive: 
		case kVtbl_CombatActionAttackRangedExplosiveFromCover:
		case kVtbl_CombatActionAttackMelee:
		case kVtbl_CombatActionAttackHandToHand:
		case kVtbl_CombatActionAttackGrenade:
		case kVtbl_CombatActionAttackGrenadeFlushTarget:
		case kVtbl_CombatActionAttackGrenadeFromCover: 
		case kVtbl_CombatActionMove:
		case kVtbl_CombatActionMoveAndSwitchToMelee:
		case kVtbl_CombatActionMoveAndAttackRanged:
		case kVtbl_CombatActionMoveAndAttackRangedExplosive:
		case kVtbl_CombatActionMoveAndAttackGrenade:
		case kVtbl_CombatActionDrawWeapon:
		case kVtbl_CombatActionSwitchWeapon:
		case kVtbl_CombatActionAvoidThreat:
		case kVtbl_CombatActionSearch:
		case kVtbl_CombatActionInvestigate:
		case kVtbl_CombatActionDodge:
		case kVtbl_CombatActionIgnoreBlockedTarget:
		case kVtbl_CombatActionFlee:
		case kVtbl_CombatActionActivateCombatItem:
		case kVtbl_CombatActionUseCombatItem:
		case kVtbl_CombatActionApproachTarget:
		case kVtbl_CombatActionAcquireLineOfSight:
		case kVtbl_CombatActionHide:
		case kVtbl_CombatController:
		case kVtbl_CombatPlanner:
		case kVtbl_CombatCoverMap:
		case kVtbl_CombatGoal:
		case kVtbl_CombatGoalKillTarget:
		case kVtbl_CombatGoalFindTarget:
		case kVtbl_CombatGoalAvoidThreats:
		case kVtbl_CombatGoalFlee:
		case kVtbl_CombatGoalActivateCombatItem:
		case kVtbl_CombatGoalUseCombatItem:
		case kVtbl_CombatGoalFlank:
		case kVtbl_CombatGoalFallBack:
		case kVtbl_CombatGoalAmbush:
		case kVtbl_CombatGoalReachDestination:
		case kVtbl_CombatGoalAdvance:
		case kVtbl_CombatGoalGuard:
		case kVtbl_CombatGoalCluster:
		case kVtbl_CombatGoalHideFromTarget:
		case kVtbl_CombatGoalFlushTarget:
		case kVtbl_CombatGoalStayInCombatArea:
		case kVtbl_CombatGroupStrategyFlank:
		case kVtbl_CombatGroupStrategy:
		case kVtbl_CombatGroupStrategyFallBack:
		case kVtbl_CombatGroupStrategyAmbush:
		case kVtbl_CombatGroupStrategyAdvance:
		case kVtbl_CombatTargetMap:
		case kVtbl_CombatThreatMap:
		case kVtbl_ContinuousBeamProjectile:
		case kVtbl_Explosion:
		case kVtbl_bhkLiquidAction:
		case kVtbl_hkpFixedBufferCdPointCollector:
		case kVtbl_FlameProjectile:
		case kVtbl_GrenadeProjectile:
		case kVtbl_MissileProjectile:
		case kVtbl_bhkHingeConstraint:
		case kVtbl_bhkConstraint:
		case kVtbl_hkHingeConstraintCinfo:
		case kVtbl_hkConstraintCinfo:
		case kVtbl_Projectile:
		case kVtbl_ProjectileListener:
		case kVtbl_CombatProcedureActivateObject:
		case kVtbl_CombatProcedure:
		case kVtbl_CombatProcedureAttackGrenade:
		case kVtbl_CombatProcedureAttackLow:
		case kVtbl_CombatProcedureAttackMelee:
		case kVtbl_CombatProcedureAttackRanged:
		case kVtbl_CombatProcedureBeInCover:
		case kVtbl_CombatProcedureEngageTarget:
		case kVtbl_CombatProcedureEvade:
		case kVtbl_CombatProcedureHideFromTarget:
		case kVtbl_CombatProcedureMove:
		case kVtbl_CombatProcedureSearch:
		case kVtbl_CombatProcedureSwitchWeapon:
		case kVtbl_CombatProcedureUseCombatItem:
		case kVtbl_ActorMover:
		case kVtbl_ActorPathHandler:
		case kVtbl_DetailedActorPathHandler:
		case kVtbl_DoorInPathFilter:
		case kVtbl_PlayerMover:
		case kVtbl_VirtualActorPathHandler:
		case kVtbl_AlarmPackage:
		case kVtbl_BackUpPackage:
		case kVtbl_DialoguePackage:
		case kVtbl_EscortActorPackageData:
		case kVtbl_FleePackage:
		case kVtbl_GuardActorPackageData:
		case kVtbl_PatrolActorPackageData:
		case kVtbl_SandBoxActorPackageData:
		case kVtbl_SearchPackage:
		case kVtbl_SpectatorPackage:
		case kVtbl_TrespassPackage:
		case kVtbl_CEnumPins:
		case kVtbl_CEnumMediaTypes:
		case kVtbl_CMediaSample:
		case kVtbl_CMemAllocator:
		case kVtbl_CAsyncOutputPin:
		case kVtbl_CAsyncReader:
		case kVtbl_TileText:
		case kVtbl_Tile3D:
		case kVtbl_TileExtra:
		case kVtbl_DebugText:
		case kVtbl_IDebugText:
		case kVtbl_DebugTextExtraData:

		case kVtbl_RadialTile:
		case kVtbl_NiAVObjectPalette:
		case kVtbl_NiControllerManager:
		case kVtbl_NiDefaultAVObjectPalette:
		case kVtbl_NiInterpController:
		case kVtbl_NiMultiTargetTransformController:
		case kVtbl_NiControllerSequence:
		case kVtbl_BSMultiTargetTreadTransfController: 
		case kVtbl_NiBlendAccumTransformInterpolator: 
		case kVtbl_NiTransformController:
		case kVtbl_NiBlendInterpolator:
		case kVtbl_NiSingleInterpController:
		case kVtbl_NiGeomMorpherController:
		case kVtbl_NiColorInterpolator:
		case kVtbl_NiFloatInterpolator:
		case kVtbl_NiKeyBasedInterpolator:
		case kVtbl_NiPosData:
		case kVtbl_NiPathInterpolator:
		case kVtbl_NiTransformInterpolator:
		case kVtbl_NiBlendFloatInterpolator:
		case kVtbl_NiBlendTransformInterpolator:
		case kVtbl_NiFloatExtraDataController:
		case kVtbl_NiMorphData:
		case kVtbl_BSFrustumFOVController:
		case kVtbl_BSBlendTreadTransfInterpolator:
		case kVtbl_BSTreadTransfInterpolator:
		case kVtbl_BSRotAccumTransfInterpolator:
		case kVtbl_BSAnimNote:
		case kVtbl_BSLookIKNote:
		case kVtbl_BSGrabIKNote:
		case kVtbl_BSAnimNotes:
		case kVtbl_NiVisController:
		case kVtbl_NiUVData:
		case kVtbl_NiUVController:
		case kVtbl_NiTextureTransformController:
		case kVtbl_NiTextKeyExtraData:
		case kVtbl_NiStringPalette:
		case kVtbl_NiSequenceStreamHelper:
		case kVtbl_NiSequence:
		case kVtbl_NiRotData:
		case kVtbl_NiFloatController:
		case kVtbl_NiRollController:
		case kVtbl_NiQuaternionInterpolator:
		case kVtbl_NiPoint3Interpolator:
		case kVtbl_NiPathController:
		case kVtbl_NiMaterialColorController:
		case kVtbl_NiLookAtInterpolator:
		case kVtbl_NiLookAtController:
		case kVtbl_NiLightDimmerController:
		case kVtbl_NiLightColorController:
		case kVtbl_NiKeyframeManager:
		case kVtbl_NiTransformData:
		case kVtbl_NiFloatsExtraDataPoint3Controller:
		case kVtbl_NiFloatsExtraDataController:
		case kVtbl_NiFlipController:
		case kVtbl_NiColorExtraDataController:
		case kVtbl_NiBSplineTransformInterpolator:
		case kVtbl_NiBSplinePoint3Interpolator:
		case kVtbl_NiBSplineFloatInterpolator:
		case kVtbl_NiBSplineCompTransformInterpolator:
		case kVtbl_NiBSplineCompPoint3Interpolator:
		case kVtbl_NiBSplineCompFloatInterpolator:
		case kVtbl_NiBSplineCompColorInterpolator:
		case kVtbl_NiBSplineColorInterpolator:
		case kVtbl_NiBSplineData:
		case kVtbl_NiBSplineBasisData:
		case kVtbl_NiBoolTimelineInterpolator:
		case kVtbl_NiBoolInterpolator:
		case kVtbl_NiBoolData:
		case kVtbl_NiBoneLODController:
		case kVtbl_NiBlendQuaternionInterpolator:
		case kVtbl_NiBlendPoint3Interpolator:
		case kVtbl_NiBlendColorInterpolator:
		case kVtbl_NiBlendBoolInterpolator:
		case kVtbl_NiAlphaController:
		case kVtbl_NiGeomMorpherUpdateTask:
		case kVtbl_NiInterpolator:
		case kVtbl_NiExtraDataController:
		case kVtbl_NiFloatInterpController:
		case kVtbl_NiBoolInterpController:
		case kVtbl_NiPoint3InterpController:
		case kVtbl_NiBSplineInterpolator:
		case kVtbl_NiAVObject:
		case kVtbl_NiObjectNET:
		case kVtbl_NiObject:

		case kVtbl_NiVertexColorProperty:
		case kVtbl_NiZBufferProperty:
		case kVtbl_NiTexture:
		case kVtbl_NiSourceTexture:
		case kVtbl_NiSourceCubeMap:
		case kVtbl_NiRenderer:
		case kVtbl_NiStream:
		case kVtbl_NiGeometryData:
		case kVtbl_NiDynamicEffect:
		case kVtbl_NiCullingProcess:
		case kVtbl_NiTexturingProperty:
		case kVtbl_NiTimeController:
		case kVtbl_NiCamera:
		case kVtbl_NiTriStrips:
		case kVtbl_NiLight:
		case kVtbl_NiStringExtraData:
		case kVtbl_NiTriShape:
		case kVtbl_NiLines:
		case kVtbl_NiMaterialProperty:
		case kVtbl_NiDirectionalLight:
		case kVtbl_NiTriStripsData:
		case kVtbl_NiImageConverter:
		case kVtbl_NiDevImageConverter:
		case kVtbl_NiExtraData:
		case kVtbl_NiTriShapeData:
		case kVtbl_NiPixelData:
		case kVtbl_NiPointLight:
		case kVtbl_NiGeometryGroupManager:
		case kVtbl_NiRenderedTexture:
		case kVtbl_NiGeometry:
		case kVtbl_NiCollisionObject:
		case kVtbl_NiShadeProperty:
		case kVtbl_NiTriShapeDynamicData:
		case kVtbl_NiSkinPartition:
		case kVtbl_NiSkinData:
		case kVtbl_BSResizableTriShape:
		case kVtbl_BSSegmentedTriShape:
		case kVtbl_NiScreenElements:
		case kVtbl_NiScreenElementsData:
		case kVtbl_NiFloatsExtraData:
		case kVtbl_NiColorExtraData:
		case kVtbl_NiTriBasedGeom:
		case kVtbl_NiVertWeightsExtraData:
		case kVtbl_NiLinesData:
		case kVtbl_NiTriBasedGeomData:
		case kVtbl_NiImageReader:
		case kVtbl_NiNIFImageReader:
		case kVtbl_NiPalette:
		case kVtbl_NiDDSReader:
		case kVtbl_NiTGAReader:
		case kVtbl_NiBMPReader:
		case kVtbl_Ni2DBuffer:
		case kVtbl_NiShader:
		case kVtbl_NiCullingProperty:
		case kVtbl_NiMaterial:
		case kVtbl_NiSingleShaderMaterialLibrary:
		case kVtbl_NiMaterialLibrary:
		case kVtbl_NiScreenGeometry:
		case kVtbl_NiScreenPolygon:
		case kVtbl_NiScreenGeometryData:
		case kVtbl_NiScreenSpaceCamera:
		case kVtbl_NiVectorExtraData:
		case kVtbl_NiTriStripsDynamicData:
		case kVtbl_NiTextureEffect:
		case kVtbl_NiSwitchStringExtraData:
		case kVtbl_NiSwitchNode:
		case kVtbl_NiStringsExtraData:
		case kVtbl_NiSpotLight:
		case kVtbl_NiSpecularProperty:
		case kVtbl_NiSortAdjustNode:
		case kVtbl_NiScreenTexture:
		case kVtbl_NiScreenLODData:
		case kVtbl_NiParticlesData:
		case kVtbl_NiRangeLODData:
		case kVtbl_NiPersistentSrcTextureRendererData:
		case kVtbl_NiParticleMeshesData:
		case kVtbl_NiParticleMeshes:
		case kVtbl_NiLODNode:
		case kVtbl_NiIntegersExtraData:
		case kVtbl_NiIntegerExtraData:
		case kVtbl_NiFogProperty:
		case kVtbl_NiFloatExtraData:
		case kVtbl_NiDitherProperty:
		case kVtbl_NiBSPNode:
		case kVtbl_NiBooleanExtraData:
		case kVtbl_NiParticles:
		case kVtbl_NiAmbientLight:
		case kVtbl_NiAlphaAccumulator:
		case kVtbl_NiShaderConstantMap:
		case kVtbl_NiDepthStencilBuffer:
		case kVtbl_NiGlobalConstantEntry:
		case kVtbl_NiShaderFactory:
		case kVtbl_NiSingleShaderMaterial:
		case kVtbl_NiLODData:
		case kVtbl_NiAccumulator:
		case kVtbl_NiBackToFrontAccumulator:
		case kVtbl_NiFile:
		case kVtbl_NiBinaryStream:
		case kVtbl_NiAllocator:
		case kVtbl_NiStandardAllocator:
		case kVtbl_BSNiAllocator:
		case kVtbl_NiSearchPath:
		case kVtbl_NiMemStream:
		case kVtbl_BSCoreMessage:
		case kVtbl_BSThread:
		case kVtbl_ZeroOverheadHeap:
		case kVtbl_IMemoryHeap:
		case kVtbl_MemoryHeap:
		case kVtbl_AbstractHeap:
		case kVtbl_FutBinaryFileC:
		case kVtbl_BSFaceGenBinaryFile:
		case kVtbl_BSAudio:
		case kVtbl_BSAudioManager:
		case kVtbl_BSAudioManagerThread:
		case kVtbl_AudioLoadTask:
		case kVtbl_BSGameSound:
		case kVtbl_BSWin32Audio:
		case kVtbl_BSWin32AudioListener:
		case kVtbl_BSAudioListener:
		case kVtbl_BSWin32GameSound:
		case kVtbl_BSSaveDataSystemUtility:
		case kVtbl_BSSystemUtility:
		case kVtbl_BSMsgDialogSystemUtility:
		case kVtbl_BSSystemUtilitiesStrings:
		case kVtbl_BSCacheDriveSystemUtility:
		case kVtbl_BSGameDataSystemUtility:
		case kVtbl_BSDiscBootSystemUtility:
		case kVtbl_BSSysInfoSystemUtility:
		case kVtbl_Archive:
		case kVtbl_ArchiveFile:
		case kVtbl_CompressedArchiveFile:
		case kVtbl_BSFile:
		case kVtbl_BSTaskletManagerCallback:
		case kVtbl_BSFileCache:
		case kVtbl_BSSearchPath:
		case kVtbl_BSWin32FileCache:
		case kVtbl_BSWin32TaskletManager:
		case kVtbl_BSTaskletManager:
		case kVtbl_BSWin32TaskletGroupData:
		case kVtbl_BSTaskletGroupData:
		case kVtbl_IdvFileError:
		case kVtbl_CTreeEngine:
		case kVtbl_CIdvCamera:
		case kVtbl_Uniform:
		case kVtbl_Random:
		case kVtbl_CBillboardLeaf:
		case kVtbl_PosGen:
		case kVtbl_Normal:
		case kVtbl_SymGen:

		case kVtbl_DistantLODShaderProperty:
		case kVtbl_TallGrassShaderProperty:
		case kVtbl_BSShaderPPLightingProperty:
		case kVtbl_WaterShaderProperty:
		case kVtbl_BSShaderNoLightingProperty:
		case kVtbl_BSShaderLightingProperty:
		case kVtbl_BSShaderBloodSplatterProperty:
		case kVtbl_BSBatchRenderer:
		case kVtbl_LightingData:
		case kVtbl_ParticleShader:
		case kVtbl_ParticleShaderProperty:
		case kVtbl_SpeedTreeBillboardShaderProperty:
		case kVtbl_BSShaderProperty:
		case kVtbl_TallGrassShader:
		case kVtbl_SkyShaderProperty:
		case kVtbl_PrecipitationShader:
		case kVtbl_PrecipitationShaderProperty:
		case kVtbl_SpeedTreeLeafShader:
		case kVtbl_SpeedTreeBranchShaderProperty:
		case kVtbl_SpeedTreeShaderPPLightingProperty:
		case kVtbl_SpeedTreeShaderLightingProperty:
		case kVtbl_SpeedTreeLeafShaderProperty:
		case kVtbl_GeometryDecalShaderProperty:
		case kVtbl_Lighting30ShaderProperty:
		case kVtbl_BSDistantTreeShader:
		case kVtbl_BSDistantTreeShaderProperty:
		case kVtbl_TileShaderProperty:
		case kVtbl_BSFogProperty:
		case kVtbl_Lighting30Shader:
		case kVtbl_BoltShader:
		case kVtbl_BoltShaderProperty:
		case kVtbl_BSClearZNode:
		case kVtbl_BSOrderedNode:
		case kVtbl_BSMaterialEmittanceMultController:
		case kVtbl_BSRefractionFirePeriodController: 
		case kVtbl_BSRefractionStrengthController:
		case kVtbl_VolumetricFogShaderProperty:
		case kVtbl_HairShaderProperty:
		case kVtbl_BeamShader:
		case kVtbl_VolumetricFogShader:
		case kVtbl_BSShaderNoLighting:
		case kVtbl_TileShader:
		case kVtbl_GeometryDecalShader:
		case kVtbl_ParallaxShader:
		case kVtbl_SkinShader:
		case kVtbl_HairShader:
		case kVtbl_SpeedTreeBranchShader:
		case kVtbl_DistantLODShader:
		case kVtbl_BSCubeMapCamera:
		case kVtbl_NiParticleSystem:
		case kVtbl_NiPSysEmitterCtlr:
		case kVtbl_NiPSysMeshEmitter:
		case kVtbl_NiPSysCylinderEmitter:
		case kVtbl_NiPSysSphereEmitter:
		case kVtbl_NiPSysBoxEmitter:
		case kVtbl_NiPSysRotationModifier:
		case kVtbl_NiPSysMeshUpdateModifier:
		case kVtbl_NiMeshParticleSystem:
		case kVtbl_NiPSysEmitter:
		case kVtbl_NiPSysGravityModifier:
		case kVtbl_NiPSysSpawnModifier:
		case kVtbl_NiPSysData:
		case kVtbl_BSStripParticleSystem:
		case kVtbl_NiPSysUpdateTask:
		case kVtbl_BSStripPSysData:
		case kVtbl_NiPSysVortexFieldModifier:
		case kVtbl_NiPSysUpdateCtlr:
		case kVtbl_NiPSysTurbulenceFieldModifier:
		case kVtbl_NiPSysSphericalCollider:
		case kVtbl_NiPSysResetOnLoopCtlr:
		case kVtbl_NiPSysRadialFieldModifier:
		case kVtbl_NiPSysPositionModifier:
		case kVtbl_NiPSysPlanarCollider:
		case kVtbl_NiPSysModifierActiveCtlr:
		case kVtbl_NiPSysInitialRotSpeedVarCtlr:
		case kVtbl_NiPSysInitialRotSpeedCtlr:
		case kVtbl_NiPSysInitialRotAngleVarCtlr:
		case kVtbl_NiPSysInitialRotAngleCtlr:
		case kVtbl_NiPSysGrowFadeModifier:
		case kVtbl_NiPSysGravityStrengthCtlr:
		case kVtbl_NiPSysGravityFieldModifier:
		case kVtbl_NiPSysFieldMaxDistanceCtlr:
		case kVtbl_NiPSysFieldMagnitudeCtlr:
		case kVtbl_NiPSysFieldAttenuationCtlr:
		case kVtbl_NiPSysEmitterSpeedCtlr:
		case kVtbl_NiPSysEmitterPlanarAngleVarCtlr:
		case kVtbl_NiPSysEmitterPlanarAngleCtlr:
		case kVtbl_NiPSysEmitterLifeSpanCtlr:
		case kVtbl_NiPSysEmitterInitialRadiusCtlr:
		case kVtbl_NiPSysEmitterDeclinationVarCtlr:
		case kVtbl_NiPSysEmitterDeclinationCtlr:
		case kVtbl_NiPSysEmitterCtlrData:
		case kVtbl_NiPSysDragModifier:
		case kVtbl_NiPSysDragFieldModifier:
		case kVtbl_NiPSysColorModifier:
		case kVtbl_NiPSysColliderManager:
		case kVtbl_NiPSysBoundUpdateModifier:
		case kVtbl_NiPSysBombModifier:
		case kVtbl_NiPSysAgeDeathModifier:
		case kVtbl_NiPSysAirFieldSpreadCtlr:
		case kVtbl_NiPSysAirFieldModifier:
		case kVtbl_NiPSysAirFieldInheritVelocityCtlr:
		case kVtbl_NiPSysAirFieldAirFrictionCtlr:
		case kVtbl_NiMeshPSysData:
		case kVtbl_NiPSysModifierCtlr:
		case kVtbl_NiPSysModifier:
		case kVtbl_NiPSysVolumeEmitter:
		case kVtbl_NiPSysFieldModifier:
		case kVtbl_NiPSysCollider:
		case kVtbl_NiPSysModifierBoolCtlr:
		case kVtbl_NiPSysModifierFloatCtlr:
		case kVtbl_BSMultiBound:
		case kVtbl_BSMultiBoundRoom:
		case kVtbl_BSReference:
		case kVtbl_BSNodeReferences:
		case kVtbl_MessageHandler:
		case kVtbl_QueuedChildren:
		case kVtbl_QueuedParents:
		case kVtbl_QueuedFile:
		case kVtbl_QueuedFileEntry:
		case kVtbl_IOManager:
		case kVtbl_BSTaskThread:
		case kVtbl_BSXFlags:
		case kVtbl_BSValueNode:
		case kVtbl_BSParallelTaskThread:
		case kVtbl_BSRangeNode:
		case kVtbl_BSWindModifier:
		case kVtbl_BSMultiBoundNode:
		case kVtbl_BSBlastNode:
		case kVtbl_BSDebrisNode:
		case kVtbl_BSDamageStage:
		case kVtbl_BSPortalSharedNode:
		case kVtbl_BSMultiBoundShape:
		case kVtbl_BSParticleSystemManager:
		case kVtbl_UpdateMasterPsysTaskletData:
		case kVtbl_BSSceneGraph:
		case kVtbl_NiBSBoneLODController:
		case kVtbl_BSFurnitureMarker:
		case kVtbl_BSBound:
		case kVtbl_BSPortalGraph:
		case kVtbl_BSPSysMultiTargetEmitterCtlr:
		case kVtbl_BSScissorTriShape:
		case kVtbl_BSMasterParticleSystem:
		case kVtbl_BSTextureData:
		case kVtbl_BSNiNode:
		case kVtbl_BSPSysStripUpdateModifier:
		case kVtbl_BSPSysSimpleColorModifier:
		case kVtbl_BSMultiBoundCapsule:
		case kVtbl_BSWArray:
		case kVtbl_PArrayPoint:
		case kVtbl_BSPSysArrayEmitter:
		case kVtbl_BSParentVelocityModifier:
		case kVtbl_BSDecalPlacementVectorExtraData:
		case kVtbl_BSAttachPoints:
		case kVtbl_bhkMemoryAllocator:
		case kVtbl_hkMemoryAllocator:
		case kVtbl_bhkOnStartupCallback:
		case kVtbl_bhkOnExitCallback:
		case kVtbl_hkpCollidableCollidableFilter:
		case kVtbl_hkpRayCollidableFilter:
		case kVtbl_hkpShapeCollectionFilter:
		case kVtbl_hkpRayShapeCollectionFilter:
		case kVtbl_hkpBroadPhaseCastCollector:
		case kVtbl_ahkpWorld:
		case kVtbl_hkpWorldRayCaster:
		case kVtbl_bhkCollisionFilter:
		case kVtbl_bhkWorldCinfo:
		case kVtbl_bhkWorld:
		case kVtbl_bhkNiCollisionObject:
		case kVtbl_hkpCharacterProxyListener:
		case kVtbl_bhkCharacterListener:
		case kVtbl_bhkShapeCollection:
		case kVtbl_bhkCharacterProxy:
		case kVtbl_bhkCachingShapePhantom:
		case kVtbl_hkpClosestCdPointCollector:
		case kVtbl_bhkCollisionBox:
		case kVtbl_bhkListShape:
		case kVtbl_bhkCharacterStateClimbing:
		case kVtbl_bhkCharacterStateFlying:
		case kVtbl_bhkCharacterController:
		case kVtbl_bhkWindListener:
		case kVtbl_hkaRaycastInterface:
		case kVtbl_bhkRagdollShareData:
		case kVtbl_bhkRagdollController:
		case kVtbl_hkaSkeleton:
		case kVtbl_bhkStiffSpringConstraint:
		case kVtbl_hkStiffSpringConstraintCinfo:
		case kVtbl_bhkBlendCollisionObject:
		case kVtbl_hkpMotion:
		case kVtbl_bhkRefObject:
		case kVtbl_WeaponObject:
		case kVtbl_bhkLimitedHingeConstraint:
		case kVtbl_hkLimitedHingeConstraintCinfo:
		case kVtbl_bhkBinaryAction:
		case kVtbl_bhkBlendCollisionObjectAddRotation:
		case kVtbl_hkReferencedObjectLock:
		case kVtbl_hkpNullContactMgr:
		case kVtbl_hkpBroadPhaseListener:
		case kVtbl_hkpDefaultConvexListFilter:
		case kVtbl_hkpPhantomBroadPhaseListener:
		case kVtbl_hkpBroadPhaseBorderListener:
		case kVtbl_hkpNullContactMgrFactory:
		case kVtbl_hkpNullCollisionFilter:
		case kVtbl_hkpSimpleWorldRayCaster:
		case kVtbl_hkpWorldLinearCaster:
		case kVtbl_hkpWorld:
		case kVtbl_hkDebugDisplay:
		case kVtbl_bhkWorldM:
		case kVtbl_bhkBlendController:
		case kVtbl_hkpKeyframedRigidMotion:
		case kVtbl_hkpMaxSizeMotion:
		case kVtbl_hkpFixedRigidMotion:
		case kVtbl_hkpSphereMotion:
		case kVtbl_hkpBoxMotion:
		case kVtbl_hkpThinBoxMotion:
		case kVtbl_hkpCharacterMotion:
		case kVtbl_hkpWorldObject:
		case kVtbl_hkpEntity:
		case kVtbl_hkpShapeContainer:
		case kVtbl_hkpSingleShapeContainer:
		case kVtbl_hkpBvShape:
		case kVtbl_hkpBoxShape:
		case kVtbl_hkpConvexTransformShapeBase:
		case kVtbl_hkpPhantom:
		case kVtbl_hkpShapePhantom:
		case kVtbl_hkaRagdollRaycastInterface:
		case kVtbl_bhkRagdollPenetrationUtil:
		case kVtbl_hkPackedNiTriStripsData:
		case kVtbl_hkpTriangleShape:
		case kVtbl_hkNormalTriangleShape:
		case kVtbl_hkPackedNiTriStripsShape:
		case kVtbl_hkNiTriStripsShape:
		case kVtbl_bhkPackedNiTriStripsShape:
		case kVtbl_bhkNiTriStripsShape:
		case kVtbl_hkpRigidBody:
		case kVtbl_hkpCharacterContext:
		case kVtbl_bhkMalleableConstraint:
		case kVtbl_hkMalleableConstraintCinfo:
		case kVtbl_hkpMalleableConstraintData:
		case kVtbl_ahkpMalleableConstraintData:
		case kVtbl_ahkpCharacterProxy:
		case kVtbl_hkpCharacterProxy:
		case kVtbl_hkBSHeightFieldShape:
		case kVtbl_hkErrStream:
		case kVtbl_bhkRigidBodyT:
		case kVtbl_bhkWaterListener:
		case kVtbl_bhkOrientHingedBodyAction:
		case kVtbl_hkpLimitedHingeConstraintData:
		case kVtbl_bhkForceController:
		case kVtbl_bhkBallAndSocketConstraint:
		case kVtbl_bhkRagdollConstraint:
		case kVtbl_hkBallAndSocketConstraintCinfo:
		case kVtbl_hkRagdollConstraintCinfo:
		case kVtbl_bhkRagdollTemplateData:
		case kVtbl_bhkRagdollTemplate:
		case kVtbl_hkpMouseSpringAction:
		case kVtbl_hkNativeFileSystem:
		case kVtbl_hkDummySingleton:
		case kVtbl_hkMemorySystem:
		case kVtbl_hkpCharacterStateManager:
		case kVtbl_hkpHingeConstraintData:
		case kVtbl_hkpPhantomOverlapListener:
		case kVtbl_hkpCollisionFilter:
		case kVtbl_hkOstream:
		case kVtbl_hkMemoryTrackStreamWriter:
		case kVtbl_hkArrayStreamWriter:
		case kVtbl_bhkPoseArray:
		case kVtbl_bhkExtraData:
		case kVtbl_bhkSpringAction:
		case kVtbl_bhkMotorAction:
		case kVtbl_bhkDashpotAction:
		case kVtbl_bhkAngularDashpotAction:
		case kVtbl_bhkBreakableConstraint:
		case kVtbl_ahkpBreakableConstraintData:
		case kVtbl_hkBreakableConstraintCinfo:
		case kVtbl_bhkWheelConstraint:
		case kVtbl_hkWheelConstraintCinfo:
		case kVtbl_bhkRagdollLimitsConstraint:
		case kVtbl_hkRagdollLimitsConstraintCinfo:
		case kVtbl_bhkPrismaticConstraint:
		case kVtbl_hkPrismaticConstraintCinfo:
		case kVtbl_bhkFixedConstraint:
		case kVtbl_bhkGenericConstraint:
		case kVtbl_hkGenericConstraintCinfo:
		case kVtbl_hkFixedConstraintCinfo:
		case kVtbl_bhkHingeLimitsConstraint:
		case kVtbl_hkHingeLimitsConstraintCinfo:
		case kVtbl_bhkBallSocketConstraintChain:
		case kVtbl_bhkConstraintChain:
		case kVtbl_hkBallSocketChainCinfo:
		case kVtbl_bhkMoppBvTreeShape:
		case kVtbl_hkScaledMoppBvTreeShape:
		case kVtbl_bhkTriangleShape:
		case kVtbl_bhkPlaneShape:
		case kVtbl_bhkHeightFieldShape:
		case kVtbl_bhkExtendedMeshShapeData:
		case kVtbl_bhkExtendedMeshShape:
		case kVtbl_bhkCylinderShape:
		case kVtbl_bhkConvexVerticesShape:
		case kVtbl_bhkConvexTransformShape:
		case kVtbl_bhkConvexSweepShape:
		case kVtbl_bhkConvexListShape:
		case kVtbl_hkThreadMemory:
		case kVtbl_hkMemoryBlockServer:
		case kVtbl_hkLeakDetectAllocator:
		case kVtbl_hkFreeListMemorySystem:
		case kVtbl_hkVisualDebugger:
		case kVtbl_hkpWorldDeletionListener:
		case kVtbl_hkpActionListener:
		case kVtbl_hkpConstraintListener:
		case kVtbl_hkProcessContext:
		case kVtbl_hkpPhysicsContext:
		case kVtbl_bhkCharacterPointCollector:
		case kVtbl_bhkCharacterStateProjectile:
		case kVtbl_bhkCharacterStateInAir:
		case kVtbl_bhkCharacterStateJumping:
		case kVtbl_bhkCharacterStateSwimming:
		case kVtbl_bhkCharacterStateOnGround:
		case kVtbl_hkCharControllerShape:
		case kVtbl_bhkCharControllerShape:
		case kVtbl_hkpSphereShape:
		case kVtbl_hkaFootPlacementIkSolver:
		case kVtbl_hkpRagdollConstraintData:
		case kVtbl_hkpConstraintMotor:
		case kVtbl_hkpLimitedForceConstraintMotor:
		case kVtbl_hkpPositionConstraintMotor:
		case kVtbl_hkaRagdollInstance:
		case kVtbl_hkaSkeletonMapper:
		case kVtbl_hkpAabbPhantom:
		case kVtbl_hkpConstraintInstance:
		case kVtbl_bhkPositionConstraintMotor:
		case kVtbl_hkpVelocityConstraintMotor:
		case kVtbl_bhkVelocityConstraintMotor:
		case kVtbl_bhkSpringDamperConstraintMotor:
		case kVtbl_hkPointToPathConstraintCinfo:
		case kVtbl_hkpAction:
		case kVtbl_hkpBinaryAction:
		case kVtbl_hkpShapeCollection:
		case kVtbl_hkpListShape:
		case kVtbl_hkpTransformShape:
		case kVtbl_hkpNullBroadPhaseListener:
		case kVtbl_hkpSimulation:
		case kVtbl_hkpCollisionDispatcher:
		case kVtbl_hkpReportContactMgr:
		case kVtbl_hkpSimpleContactConstraintData:
		case kVtbl_hkpSimpleConstraintContactMgr:
		case kVtbl_hkpMultiThreadedSimulation:
		case kVtbl_hkpPhysicsSystem:
		case kVtbl_hkStatisticsCollector:
		case kVtbl_hkpConstraintOwner:
		case kVtbl_hkpSimulationIsland:
		case kVtbl_hkpWorldPostSimulationListener:
		case kVtbl_hkKdTreeCastCollector:
		case kVtbl_hkpKdTreeCollidableRaycaster:
		case kVtbl_hkpKdTreeWorldLinearCaster:
		case kVtbl_hkpAabbTreeWorldManager:
		case kVtbl_hkpKdTreeWorldManager:
		case kVtbl_hkpCollisionCallbackUtil:
		case kVtbl_hkpDefaultWorldMaintenanceMgr:
		case kVtbl_hkpBroadPhaseBorder:
		case kVtbl_hkpContinuousSimulation:
		case kVtbl_hkpNullAgent:
		case kVtbl_hkpEntityEntityBroadPhaseListener:
		case kVtbl_hkp3AxisSweep:
		case kVtbl_hkpSimpleShapePhantom:
		case kVtbl_hkaDetectRagdollPenetration:
		case kVtbl_hkpCapsuleShape:
		case kVtbl_hkDisplayGeometry:
		case kVtbl_hkDisplayCapsule:
		case kVtbl_hkpSampledHeightFieldShape:
		case kVtbl_hkpTriSampledHeightFieldCollection:
		case kVtbl_hkpBvTreeShape:
		case kVtbl_hkpTriSampledHeightFieldBvTreeShape:
		case kVtbl_hkBufferedStreamWriter:
		case kVtbl_hkpMultiSphereShape:
		case kVtbl_hkOrientHingedBodyAction:
		case kVtbl_hkpUnaryAction:
		case kVtbl_hkDefaultError:
		case kVtbl_hkSocket:
		case kVtbl_hkBufferedStreamReader:
		case kVtbl_hkStdioStreamReader:
		case kVtbl_hkStdioStreamWriter:
		case kVtbl_hkLiquidAction:
		case kVtbl_hkCpuJobThreadPool:
		case kVtbl_hkpSymmetricAgentFlipCollector:
		case kVtbl_hkpSymmetricAgentFlipCastCollector:
		case kVtbl_hkpSymmetricAgentFlipBodyCollector:
		case kVtbl_hkpBvTreeStreamAgent:
		case kVtbl_hkpBvTreeAgent:
		case kVtbl_hkpMoppAgent:
		case kVtbl_hkHeightFieldRayForwardingCollector:
		case kVtbl_hkpHeightFieldAgent:
		case kVtbl_hkpMapPointsToSubShapeContactMgr:
		case kVtbl_hkpPredGskfAgent:
		case kVtbl_hkpFlagCdBodyPairCollector:
		case kVtbl_hkpSimpleClosestContactCollector:
		case kVtbl_hkpConvexListAgent:
		case kVtbl_hkpListAgent:
		case kVtbl_hkpMultiRayConvexAgent:
		case kVtbl_hkpMultiSphereTriangleAgent:
		case kVtbl_hkpCapsuleTriangleAgent:
		case kVtbl_hkpCapsuleCapsuleAgent:
		case kVtbl_hkpSphereBoxAgent:
		case kVtbl_hkpSphereTriangleAgent:
		case kVtbl_hkpSphereCapsuleAgent:
		case kVtbl_hkpSphereSphereAgent:
		case kVtbl_hkpBoxBoxAgent:
		case kVtbl_hkpPhantomAgent:
		case kVtbl_hkpTransformAgent:
		case kVtbl_hkpShapeCollectionAgent:
		case kVtbl_hkpMultiSphereAgent:
		case kVtbl_hkpBvAgent:
		case kVtbl_hkMemoryTrackStreamReader:
		case kVtbl_hkDefaultBuiltinTypeRegistry:
		case kVtbl_hkTypeInfoRegistry:
		case kVtbl_hkDynamicClassNameRegistry:
		case kVtbl_hkDefaultClassNameRegistry:
		case kVtbl_hkVtableClassRegistry:
		case kVtbl_hkpPhysicsData:
		case kVtbl_hkBinaryTagfileWriter:
		case kVtbl_hkpStorageSampledHeightFieldShape:
		case kVtbl_hkpStorageExtendedMeshShape:
		case kVtbl_hkpStorageMeshShape:
		case kVtbl_hkpPhysicsSystemWithContacts:
		case kVtbl_hkpCachingShapePhantom:
		case kVtbl_hkpSpringAction:
		case kVtbl_hkpMotorAction:
		case kVtbl_hkpDashpotAction:
		case kVtbl_hkpAngularDashpotAction:
		case kVtbl_hkpBreakableConstraintData:
		case kVtbl_hkpWheelConstraintData:
		case kVtbl_hkpStiffSpringConstraintData:
		case kVtbl_hkpRagdollLimitsData:
		case kVtbl_hkpPrismaticConstraintData:
		case kVtbl_hkpGenericConstraintData:
		case kVtbl_hkpHingeLimitsData:
		case kVtbl_hkpBallSocketChainData:
		case kVtbl_hkpBallAndSocketConstraintData:
		case kVtbl_hkMoppBvTreeShapeBase:
		case kVtbl_hkpMoppBvTreeShape:
		case kVtbl_hkpPlaneShape:
		case kVtbl_hkpConvexTranslateShape:
		case kVtbl_hkpExtendedMeshShape:
		case kVtbl_hkpCylinderShape:
		case kVtbl_hkDisplayCylinder:
		case kVtbl_hkpConvexVerticesShape:
		case kVtbl_hkpConvexTransformShape:
		case kVtbl_hkpConvexListShape:
		case kVtbl_hkSolverAllocator:
		case kVtbl_hkFreeListAllocator:
		case kVtbl_hkSystemMemoryBlockServer:
		case kVtbl_hkOArchive:
		case kVtbl_hkProcess:
		case kVtbl_hkProcessHandler:
		case kVtbl_hkCommandRouter:
		case kVtbl_hkServerProcessHandler:
		case kVtbl_hkpWeldingViewer:
		case kVtbl_hkpWorldSnapshotViewer:
		case kVtbl_hkpWorldMemoryViewer:
		case kVtbl_hkpVehicleViewer:
		case kVtbl_hkpSweptTransformDisplayViewer:
		case kVtbl_hkpSimulationIslandViewer:
		case kVtbl_hkpShapeDisplayViewer:
		case kVtbl_hkpPhantomDisplayViewer:
		case kVtbl_hkpUserShapePhantomTypeIdentifier:
		case kVtbl_hkpMousePickingViewer:
		case kVtbl_hkpRigidBodyInertiaViewer:
		case kVtbl_hkpToiCountViewer:
		case kVtbl_hkpToiContactPointViewer:
		case kVtbl_hkpInactiveContactPointViewer:
		case kVtbl_hkpActiveContactPointViewer:
		case kVtbl_hkpConvexRadiusViewer:
		case kVtbl_hkpConstraintViewer:
		case kVtbl_hkpRigidBodyCentreOfMassViewer:
		case kVtbl_hkpMidphaseViewer:
		case kVtbl_hkpBroadphaseViewer:
		case kVtbl_hkgpConvexHull:
		case kVtbl_hkpPointToPlaneConstraintData:
		case kVtbl_hkpSpringDamperConstraintMotor:
		case kVtbl_hkTraceStream:
		case kVtbl_hkpConstraintCollisionFilter:
		case kVtbl_hkpDeferredConstraintOwner:
		case kVtbl_hkpBroadPhase:
		case kVtbl_hkKdTreeBundleCastCollector:
		case kVtbl_hkKdTreeCollidableRayBundleCaster:
		case kVtbl_hkpAabbTreeCollidableRaycaster:
		case kVtbl_hkpCollidableMediator:
		case kVtbl_hkpEndOfStepCallbackUtil:
		case kVtbl_hkpDefaultToiResourceMgr:
		case kVtbl_hkLocalFrameGroup:
		case kVtbl_hkSimpleLocalFrame:
		case kVtbl_hkStackTracer:
		case kVtbl_hkBsdSocket:
		case kVtbl_hkpGskBaseAgent:
		case kVtbl_hkpGskfAgent:
		case kVtbl_hkCrc32StreamWriter:
		case kVtbl_hkStaticClassNameRegistry:
		case kVtbl_hkVersionPatchManager:
		case kVtbl_hkSerializeDeprecated:
		case kVtbl_hkDataRefCounted:
		case kVtbl_hkDataClassNative:
		case kVtbl_hkDataObjectNative:
		case kVtbl_hkDataArrayNative:
		case kVtbl_hkDataWorldNative:
		case kVtbl_hkBinaryPackfileWriter:
		case kVtbl_hkSubStreamWriter:
		case kVtbl_hkPackfileWriter:
		case kVtbl_hkBinaryTagfileReader:
		case kVtbl_ArrayOfTuplesImplementation:
		case kVtbl_hkDataObjectDict:
		case kVtbl_VecArrayImplementation:
		case kVtbl_PointerArrayImplementation:
		case kVtbl_ByteArrayImplementation:
		case kVtbl_IntArrayImplementation:
		case kVtbl_RealArrayImplementation:
		case kVtbl_CstringArrayImplementation:
		case kVtbl_hkDataClassDict:
		case kVtbl_StructArrayImplementation:
		case kVtbl_hkDataWorldDict:
		case kVtbl_hkIstream:
		case kVtbl_hkpMeshShape:
		case kVtbl_hkpSerializedAgentNnEntry:
		case kVtbl_hkpConstraintChainInstanceAction:
		case kVtbl_hkpConstraintChainInstance:
		case kVtbl_hkpMoppCostFunction:
		case kVtbl_hkpMoppShapeMediator:
		case kVtbl_hkpMoppCachedShapeMediator:
		case kVtbl_hkFreeListAllocatorServer:
		case kVtbl_hkLargeBlockAllocator:
		case kVtbl_hkWindowsSystemClock:
		case kVtbl_hkIArchive:
		case kVtbl_hkProcessFactory:
		case kVtbl_hkDebugDisplayHandler:
		case kVtbl_hkServerDebugDisplayHandler:
		case kVtbl_hkDisplaySerializeOStream:
		case kVtbl_hkDisplaySerializeIStream:
		case kVtbl_hkInspectProcess:
		case kVtbl_hkStatisticsProcess:
		case kVtbl_hkDebugDisplayProcess:
		case kVtbl_hkpPhysicsContextWorldListener:
		case kVtbl_hkpWorldViewerBase:
		case kVtbl_hkStreamStatisticsCollector:
		case kVtbl_hkpShapeDisplayBuilder:
		case kVtbl_hkpUserShapeDisplayBuilder:
		case kVtbl_hkDisplayAABB:
		case kVtbl_hkDisplayConvex:
		case kVtbl_hkDisplayBox:
		case kVtbl_hkpContactPointViewer:
		case kVtbl_hkpConvexRadiusBuilder:
		case kVtbl_hkpUserConvexRadiusBuilder:
		case kVtbl_hkDisplaySemiCircle:
		case kVtbl_hkDisplayPlane:
		case kVtbl_hkDisplayCone:
		case kVtbl_hkpPointToPathConstraintData:
		case kVtbl_hkpPulleyConstraintData:
		case kVtbl_hkpPairCollisionFilter:
		case kVtbl_hkCpuWorldGetClosestPointsCollector:
		case kVtbl_hkCpuWorldLinearCastCollector:
		case kVtbl_hkCpuWorldRayCastCollector:
		case kVtbl_hkpFixedBufferRayHitCollector:
		case kVtbl_hkaAnimation:
		case kVtbl_hkaWaveletCompressedAnimation:
		case kVtbl_hkaSplineCompressedAnimation:
		case kVtbl_hkaDeltaCompressedAnimation:
		case kVtbl_hkpRejectChassisListener:
		case kVtbl_hkpVehicleWheelCollide:
		case kVtbl_hkpVehicleLinearCastWheelCollide:
		case kVtbl_hkpVehicleInstance:
		case kVtbl_hkpVehicleDefaultSuspension:
		case kVtbl_hkpVehicleDefaultBrake:
		case kVtbl_hkpVehicleData:
		case kVtbl_hkpTyremarksWheel:
		case kVtbl_hkpTyremarksInfo:
		case kVtbl_hkpSerializedDisplayRbTransforms:
		case kVtbl_hkpPoweredChainMapper:
		case kVtbl_hkpRotationalConstraintData:
		case kVtbl_hkpSimpleMeshShape:
		case kVtbl_hkpMultiRayShape:
		case kVtbl_hkxAttributeHolder:
		case kVtbl_hkxNode:
		case kVtbl_hkxMaterial:
		case kVtbl_hkxSparselyAnimatedInt:
		case kVtbl_hkxSparselyAnimatedEnum:
		case kVtbl_hkxEnum:
		case kVtbl_hkMonitorStreamColorTable:
		case kVtbl_hkaInterleavedUncompressedAnimation:
		case kVtbl_hkaFootstepAnalysisInfoContainer:
		case kVtbl_hkaDefaultAnimatedReferenceFrame:
		case kVtbl_hkaBoneAttachment:
		case kVtbl_hkaAnimationPreviewColorContainer:
		case kVtbl_hkaAnimationContainer:
		case kVtbl_hkaAnimationBinding:
		case kVtbl_hkpVehicleRayCastWheelCollide:
		case kVtbl_hkpVehicleManager:
		case kVtbl_hkpVehicleRayCastBatchingManager:
		case kVtbl_hkpVehicleLinearCastBatchingManager:
		case kVtbl_hkpVehicleDriverInputAnalogStatus:
		case kVtbl_hkpVehicleDefaultAnalogDriverInput:
		case kVtbl_hkpVehicleDefaultVelocityDamper:
		case kVtbl_hkpVehicleDefaultTransmission:
		case kVtbl_hkpVehicleDefaultSteering:
		case kVtbl_hkpVehicleDefaultEngine:
		case kVtbl_hkpVehicleDefaultAerodynamics:
		case kVtbl_hkpSerializedDisplayMarkerList:
		case kVtbl_hkpSerializedDisplayMarker:
		case kVtbl_hkpReorientAction:
		case kVtbl_hkpGroupCollisionFilter:
		case kVtbl_hkpDisplayBindingData:
		case kVtbl_hkpDisableEntityCollisionFilter:
		case kVtbl_hkpConstrainedSystemFilter:
		case kVtbl_hkpConvexPieceStreamData:
		case kVtbl_hkpLinearParametricCurve:
		case kVtbl_hkpDefaultWorldMemoryWatchDog:
		case kVtbl_hkpCallbackConstraintMotor:
		case kVtbl_hkpShapeInfo:
		case kVtbl_hkpMoppModifier:
		case kVtbl_hkpRemoveTerminalsMoppModifier:
		case kVtbl_hkpGroupFilter:
		case kVtbl_hkpFastMeshShape:
		case kVtbl_hkpConvexVerticesConnectivity:
		case kVtbl_hkpConvexPieceMeshShape:
		case kVtbl_hkpCompressedSampledHeightFieldShape:
		case kVtbl_hkpCollisionFilterList:
		case kVtbl_hkMemoryMeshShape:
		case kVtbl_hkxVertexVectorDataChannel:
		case kVtbl_hkxVertexSelectionChannel:
		case kVtbl_hkxVertexIntDataChannel:
		case kVtbl_hkxVertexFloatDataChannel:
		case kVtbl_hkxTriangleSelectionChannel:
		case kVtbl_hkxTextureInplace:
		case kVtbl_hkxTextureFile:
		case kVtbl_hkxSparselyAnimatedString:
		case kVtbl_hkxSparselyAnimatedBool:
		case kVtbl_hkxSkinBinding:
		case kVtbl_hkxScene:
		case kVtbl_hkxNodeSelectionSet:
		case kVtbl_hkxMeshSection:
		case kVtbl_hkxMaterialShader:
		case kVtbl_hkxMaterialEffect:
		case kVtbl_hkxLight:
		case kVtbl_hkxIndexBuffer:
		case kVtbl_hkxEdgeSelectionChannel:
		case kVtbl_hkxCamera:
		case kVtbl_hkxAnimatedVector:
		case kVtbl_hkxAnimatedQuaternion:
		case kVtbl_hkxAnimatedMatrix:
		case kVtbl_hkxAnimatedFloat:
		case kVtbl_hkPlatformObjectWriter:
		case kVtbl_hkOffsetOnlyStreamWriter:
		case kVtbl_hkObjectCopier:
		case kVtbl_hkObjectResource:
		case kVtbl_hkPackfileData:
		case kVtbl_hkMemoryStreamReader:
		case kVtbl_hkpMoppCodeGenerator:
		case kVtbl_hkpMoppDefaultAssembler:
		case kVtbl_hkpMoppDefaultSplitter:
		case kVtbl_hkDisplaySphere:
		case kVtbl_IConvexOverlapImpl:
		case kVtbl_hkGskShape:
		case kVtbl_hkgpMesh:
		case kVtbl_hkResourceMap:
		case kVtbl_hkMemoryResourceHandle:
		case kVtbl_hkMemoryResourceContainer:
		case kVtbl_hkContainerResourceMap:
		case kVtbl_hkaMeshBinding:
		case kVtbl_hkpStiffSpringChainData:
		case kVtbl_hkpPoweredChainData:
		case kVtbl_hkpCompressedMeshShape:
		case kVtbl_hkxVertexBuffer:
		case kVtbl_hkxEnvironment:
		case kVtbl_hkaFootstepAnalysisInfo:
		case kVtbl_hkpGunProjectile:
		case kVtbl_hkpProjectileGun:
		case kVtbl_hkpMountedBallGun:
		case kVtbl_hkpGravityGun:
		case kVtbl_hkpFirstPersonGun:
		case kVtbl_hkpBallGun:
		case kVtbl_hkpRemoveTerminalsMoppModifier2:
		case kVtbl_hkxMesh:
		case kVtbl_hkxMaterialShaderSet:
		case kVtbl_hkAlignSceneToNodeOptions:
		case kVtbl_hkKdTreeCpuBroadphaseBundleCastCollector:
		case kVtbl_hkpConvexPieceShape:
		case kVtbl_NiDX9SourceTextureData:
		case kVtbl_NiDX9Renderer:
		case kVtbl_NiDX9ShaderDeclaration:
		case kVtbl_NiD3DShaderDeclaration:
		case kVtbl_NiD3DDefaultShader:
		case kVtbl_NiD3DShader:
		case kVtbl_NiDX92DBufferData:
		case kVtbl_NiDX9TextureBufferData:
		case kVtbl_NiDX9ImplicitBufferData:
		case kVtbl_NiDX9ImplicitDepthStencilBufferData:
		case kVtbl_NiDX9AdditionalDepthStencilBufferData:
		case kVtbl_NiD3DPass:
		case kVtbl_NiD3DShaderInterface:
		case kVtbl_NiShaderConstantMapEntry:
		case kVtbl_NiD3DShaderConstantMap:
		case kVtbl_NiD3DRenderState:
		case kVtbl_NiD3DGeometryGroupManager:
		case kVtbl_NiD3DShaderProgram:
		case kVtbl_NiD3DPixelShader:
		case kVtbl_NiD3DVertexShader:
		case kVtbl_NiDX9TextureData:
		case kVtbl_NiDX9SourceCubeMapData:
		case kVtbl_NiDX9VertexBufferManager:
		case kVtbl_NiFragmentShaderInstanceDescriptor:
		case kVtbl_NiDX9FragmentShader:
		case kVtbl_NiD3DGPUProgramCache:
		case kVtbl_NiGeometryBufferData:
		case kVtbl_NiDX9IndexBufferManager:
		case kVtbl_NiDX9RenderedTextureData:
		case kVtbl_NiDX9RenderedCubeMapData:
		case kVtbl_NiDX9DynamicTextureData:
		case kVtbl_NiDX9ErrorShader:
		case kVtbl_NiDX9TextureManager:
		case kVtbl_NiDX9RenderState:
		case kVtbl_NiDX9PersistentSrcTextureRendererData:
		case kVtbl_NiD3DShaderProgramCreator:
		case kVtbl_NiD3DShaderProgramCreatorHLSL:
		case kVtbl_NiD3DShaderProgramCreatorAsm:
		case kVtbl_NiD3DShaderProgramCreatorObj:
		case kVtbl_NiD3DShaderFactory:
		case kVtbl_NiD3DShaderProgramFactory:
		case kVtbl_NiStaticGeometryGroup:
		case kVtbl_NiUnsharedGeometryGroup:
		case kVtbl_NiDynamicGeometryGroup:
		case kVtbl_NiD3DHLSLPixelShader:
		case kVtbl_NiD3DHLSLVertexShader:
		case kVtbl_NiDX9ShaderConstantManager:
		case kVtbl_NiGeometryGroup:
		case kVtbl_NiD3DShaderConstantManager:
		case kVtbl_NiBoundingVolume:
		case kVtbl_NiHalfSpaceBV:
		case kVtbl_NiUnionBV:
		case kVtbl_NiCapsuleBV:
		case kVtbl_NiBoxBV:
		case kVtbl_NiSphereBV:
		case kVtbl_NiCollisionData:
		case kVtbl_NiIntersector:
		case kVtbl_NiCapsuleSphereIntersector:
		case kVtbl_NiCapsuleCapsuleIntersector:
		case kVtbl_NiCapsuleTriIntersector:
		case kVtbl_NiBoxCapsuleIntersector:
		case kVtbl_NiBoxSphereIntersector:
		case kVtbl_NiSphereTriIntersector:
		case kVtbl_NiOBBRoot:
		case kVtbl_NiOBBNode:
		case kVtbl_NiOBBLeaf:
		case kVtbl_MoviePlayer:
		case kVtbl_NiRenderedCubeMap:
		case kVtbl_NiRenderTargetGroup:
		case kVtbl_NiShaderDeclaration:
		case kVtbl_NiSCMExtraData:
		case kVtbl_NiGPUProgramCache:
		case kVtbl_NiMaterialResource:
		case kVtbl_StartMenuOption:
		case kVtbl_StartMenuUserOption: return printTypeName(ptr) + ":";


		default: break;
		}
		if (vtbl >= 0x1000000 && vtbl <= 0x1200000)
			return FormatString("¯\\_(ツ)_/¯: 0x%08X %s", vtbl, printTypeName(ptr).c_str());

		return "";
	}

	std::string CheckTopLevel(UInt32 ptr, UInt32 depth = 5)
	{
		if (!ptr) return "";

		UInt32 vtbl;

		std::string full;

		while (vtbl = CanDereference(ptr))
		{
			
			std::string onetime = GetStringForVTBL((void*)ptr, vtbl);
			if (!onetime.empty()) return onetime;


			if (depth == 0) break;

			depth--;

			full += FormatString("NOT_VTBL: 0x%08X; ", vtbl);

			ptr = vtbl;
		}

		return full;
	}
}

namespace CobbPatches {
	namespace Subroutines {
		const Label* GetLabel(UInt32 addr) {
			for (UInt32 i = 0; i < g_labelCount; i++) {
				auto& entry = g_labels[i];
				if (addr >= entry.start && addr <= (entry.start + entry.size))
					return &entry;
			}
			return nullptr;
		}
	}

	namespace CrashLog {
		static LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

		struct UserContext {
			UInt32 eip;
			UInt32 moduleBase;
			CHAR* name;
		};

		BOOL CALLBACK EumerateModulesCallback(PCSTR name, ULONG moduleBase, ULONG moduleSize, PVOID context) {
			UserContext* info = (UserContext*)context;
			if (info->eip >= (UInt32)moduleBase && info->eip <= (UInt32)moduleBase + (UInt32)moduleSize) {
				//Log() << FormatString("%0X %0X  %0X", (UInt32)moduleBase, info->eip, (UInt32)moduleBase + (UInt32) moduleSize);
				info->moduleBase = moduleBase;
				strcpy_s(info->name, 100, name);
			}
			Log() << FormatString(" - 0x%08X - 0x%08X: %s", (UInt32)moduleBase, (UInt32)moduleBase + (UInt32)moduleSize, name);
			return TRUE;
		}

		BOOL CALLBACK EumerateModulesCallbackSym(PCSTR name, ULONG moduleBase, PVOID context) {
			UserContext* info = (UserContext*)context;
			IMAGEHLP_MODULE modu;
			SymGetModuleInfo(GetCurrentProcess(), moduleBase, &modu);
			if (info->eip >= (UInt32)moduleBase && info->eip <= (UInt32)moduleBase + modu.ImageSize) {
				//			   Log() << FormatString("%0X %0X  %0X", (UInt32)moduleBase, info->eip, (UInt32)moduleBase + moduleSize);
				info->moduleBase = moduleBase;
				strcpy_s(info->name, 100, name);
			}
			Log() << FormatString(" - 0x%08X - 0x%08X: %s", (UInt32)moduleBase, (UInt32)moduleBase + modu.ImageSize, name);
			return TRUE;
		}

		void Log2(EXCEPTION_POINTERS* info) {
			HANDLE  processHandle = GetCurrentProcess();
			HANDLE  threadHandle = GetCurrentThread();
			Log() << FormatString("Exception %08X caught!", info->ExceptionRecord->ExceptionCode);
			Log() << FormatString("\nCalltrace: ");
			StackWalk(info, processHandle, threadHandle);

			const UInt32 eip = info->ContextRecord->Eip;
			Log() << FormatString("\nRegistry: ");
			Log() << FormatString("Instruction pointer (EIP): %08X", eip);
			Log() << FormatString("REG |    VALUE   | DEREFERENCE INFO");
			Log() << FormatString("%s | 0x%08X | %s ", "eax", info->ContextRecord->Eax, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Eax).c_str());
			Log() << FormatString("%s | 0x%08X | %s ", "ebx", info->ContextRecord->Ebx, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Ebx).c_str());
			Log() << FormatString("%s | 0x%08X | %s ", "ecx", info->ContextRecord->Ecx, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Ecx).c_str());
			Log() << FormatString("%s | 0x%08X | %s ", "edx", info->ContextRecord->Edx, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Edx).c_str());
			Log() << FormatString("%s | 0x%08X | %s ", "edi", info->ContextRecord->Edi, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Edi).c_str());
			Log() << FormatString("%s | 0x%08X | %s ", "esi", info->ContextRecord->Esi, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Esi).c_str());
			Log() << FormatString("%s | 0x%08X | %s ", "ebp", info->ContextRecord->Ebp, YvilesMagicBox::CheckTopLevel(info->ContextRecord->Ebp).c_str());
			UInt32* esp = (UInt32*)info->ContextRecord->Esp;

			Log() << FormatString("\nStack: ");
			Log() << FormatString(" # |    VALUE   | DEREFERENCE INFO");
			for (unsigned int i = 0; i < ce_printStackCount; i += 1) {

				if (i <= 8)
				{
					Log() << FormatString("%2X | 0x%08X | %s", i, esp[i], YvilesMagicBox::CheckTopLevel(esp[i]).c_str());


				}
				else
				{
					std::string result = YvilesMagicBox::CheckTopLevel(esp[i], 0);
					if (!result.empty()) Log() << FormatString("%2X | 0x%08X | %s", i, esp[i], result.c_str());

				}
			}
			Log() << FormatString("\nLISTING MODULE BASES ...");
			UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };
			EnumerateLoadedModules(processHandle, EumerateModulesCallback, &infoUser);
			//        SymEnumerateModules(processHandle, EumerateModulesCallbackSym, &infoUser);
			if (infoUser.moduleBase) {
				Log() << FormatString("\nGAME CRASHED AT INSTRUCTION Base+0x%08X IN MODULE: %s", (infoUser.eip - infoUser.moduleBase), infoUser.name);
				Log() << FormatString("Please note that this does not automatically mean that that module is responsible. \n"
					"It may have been supplied bad data or program state as the result of an issue in \n"
					"the base game or a different DLL.");
			}
			else {
				Log() << FormatString("\nUNABLE TO IDENTIFY MODULE CONTAINING THE CRASH ADDRESS.");
				Log() << FormatString("This can occur if the crashing instruction is located in the vanilla address space, \n"
					"but it can also occur if there are too many DLLs for us to list, and if the crash \n"
					"occurred in one of their address spaces. Please note that even if the crash occurred \n"
					"in vanilla code, that does not necessarily mean that it is a vanilla problem. The \n"
					"vanilla code may have been supplied bad data or program state as the result of an \n"
					"issue in a loaded DLL.");
			}

			SymCleanup(processHandle);
		};
		LONG WINAPI Filter(EXCEPTION_POINTERS* info) {
			static bool caught = false;
			bool ignored = false;
			//
			if (!caught) {
				caught = true;
				try {
					Log2(info);
				}
				catch (...) {};
			}
			else
				ignored = true;
			if (s_originalFilter)
				s_originalFilter(info); // don't return
			return !ignored ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
		};
		LPTOP_LEVEL_EXCEPTION_FILTER WINAPI FakeSetUnhandledExceptionFilter(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) {
			s_originalFilter = lpTopLevelExceptionFilter;
			return nullptr;
		}
		//
		void Apply(bool IsNewVegas) {
			s_originalFilter = SetUnhandledExceptionFilter(&Filter);
			//
			// Prevent disabling the filter:
			//
			if (IsNewVegas) SafeWrite32(0x00FDF180, (UInt32)&FakeSetUnhandledExceptionFilter);
			else SafeWrite32(0x00A281B4, (UInt32)&FakeSetUnhandledExceptionFilter);

		};
	};
}
