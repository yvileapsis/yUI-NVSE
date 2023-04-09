#include <main.h>

#include "CrashLog.h"

#include <DbgHelp.h>
#include <map>
#include <set>

#include "Form.h"
#include "GameTasks.h"
#include "Menus.h"
#include "Reference.h"
#include "SafeWrite.h"
#include "Tiles.h"

#define SYMOPT_EX_WINE_NATIVE_MODULES  1000

constexpr UInt32 ce_printStackCount = 256;

namespace CrashLogger::Handle
{
	typedef std::string (*_Handler)(void* ptr);

	std::string AsUInt32(void* ptr)
	{
		return FormatString("0x%08X", **static_cast<UInt32**>(ptr));
	}

	std::string AsMenu(void* ptr) { return static_cast<Menu*>(ptr)->tile->GetFullPath(); }
	std::string AsTile(void* ptr) { return static_cast<Tile*>(ptr)->GetFullPath(); }

	std::string AsTESForm(void* ptr) { const auto form = static_cast<TESForm*>(ptr); return FormatString("%08X (%s)", form->refID, form->GetName()); }
	std::string AsTESObjectREFR(void* ptr) { const auto refr = static_cast<TESObjectREFR*>(ptr);
		return FormatString("%08X (%s), BaseForm %08X (%s)", refr->refID, refr->GetName(),
		                    refr->TryGetREFRParent()->refID, refr->TryGetREFRParent()->GetName());
	}
	std::string AsBaseProcess(void* ptr)
	{
		const auto process = static_cast<BaseProcess*>(ptr);
		for (const auto iter : *TESForm::GetAll())
		{
			if (iter->typeID == kFormType_Character && static_cast<Character*>(iter)->baseProcess == process)
				return FormatString("%08X (%s)", iter->refID, iter->GetName());
			if (iter->typeID == kFormType_Creature && static_cast<Creature*>(iter)->baseProcess == process)
				return FormatString("%08X (%s)", iter->refID, iter->GetName());
		}
		return "";
	}

	std::string AsBSAnimGroupSequence(void* ptr)
	{
		const auto sequence = static_cast<BSAnimGroupSequence*>(ptr);
		return FormatString("%s, %s, AnimGroup %04X", sequence->sequenceName, sequence->accumRootName,
		                    sequence->animGroup->groupID);
	}
	std::string AsAnimSequenceSingle(void* ptr)
	{
		const auto sequence = static_cast<AnimSequenceSingle*>(ptr);
		return FormatString("%s, %s, AnimGroup %04X", sequence->anim->sequenceName, sequence->anim->accumRootName,
		                    sequence->anim->animGroup->groupID);
	}

	std::string AsNiObjectNET(void* ptr) { const auto object = static_cast<NiObjectNET*>(ptr); return FormatString("%s", object->m_pcName); }


	std::string AsBSFile(void* ptr) { const auto file = static_cast<BSFile*>(ptr); return FormatString("%s", file->m_path); }

	std::string AsTESModel(void* ptr) { const auto model = static_cast<TESModel*>(ptr); return FormatString("%s", model->nifPath.CStr()); }
	std::string AsQueuedModel(void* ptr) { const auto model = static_cast<QueuedModel*>(ptr); return FormatString("%s %s", model->model->path, model->tesModel->nifPath.CStr()); }

	std::string AsTESTexture(void* ptr) { const auto texture = static_cast<TESTexture*>(ptr); return FormatString("%s", texture->ddsPath.CStr()); }
	std::string AsQueuedTexture(void* ptr) { const auto texture = static_cast<QueuedTexture*>(ptr); return FormatString("%s", texture->name); }

}
namespace CrashLogger::NVVtables
{
	Handle::_Handler lastHandler = nullptr;

	class Label {
	public:
		enum Type : UInt8 {
			kType_None = 0,
			kType_RTTIClass = 1,
			kType_Global = 2,
		};

		Type type = kType_None;

		UInt32 address;
		UInt32 size;
		Handle::_Handler function;
		std::string name;

		Label(UInt32 address, Handle::_Handler function = lastHandler, Type type = kType_RTTIClass, std::string name = "", UInt32 size = 4)
			: type(type), address(address), size(size), function(function), name(std::move(name))
		{
			lastHandler = function;
		};

		bool Satisfies(void* ptr) const
		{
			return *static_cast<UInt32*>(ptr) >= address && *static_cast<UInt32*>(ptr) <= address + size;
		}

		static std::string GetTypeName(void* ptr)
		{
			return printTypeName(ptr);
		}

		[[nodiscard]] std::string GetLabelName() const
		{
			if (type == kType_RTTIClass) return "RTTI: ";
			if (type == kType_Global) return "Global: ";
			return "None: ";
		}

		std::string Get(void* ptr) const
		{
			std::string details;
			if (function) details = function(ptr);
			return GetLabelName() + (name.empty() && type == kType_RTTIClass ? GetTypeName(ptr) : name + (details.empty() ? "" : ": " + details));
		}
	};

	std::vector setOfLabels{

		Label(0x011F3374, Handle::AsUInt32, Label::kType_Global, "TileValueIndirectTemp"),

		Label(kVtbl_Menu, Handle::AsMenu),
		Label(kVtbl_TutorialMenu),
		Label(kVtbl_StatsMenu),
		Label(kVtbl_TextEditMenu),
		Label(kVtbl_SurgeryMenu),
		Label(kVtbl_VATSMenu),
		Label(kVtbl_SpecialBookMenu),
		Label(kVtbl_LoveTesterMenu),
		Label(kVtbl_QuantityMenu),
		Label(kVtbl_RecipeMenu),
		Label(kVtbl_BarterMenu),
		Label(kVtbl_BlackjackMenu),
		Label(kVtbl_BookMenu),
		Label(kVtbl_CaravanMenu),
		Label(kVtbl_CharGenMenu),
		Label(kVtbl_CompanionWheelMenu),
		Label(kVtbl_ComputersMenu),
		Label(kVtbl_ContainerMenu),
		Label(kVtbl_CreditsMenu),
		Label(kVtbl_DialogMenu),
		Label(kVtbl_HackingMenu),
		Label(kVtbl_HUDMainMenu),
		Label(kVtbl_InventoryMenu),
		Label(kVtbl_ItemModMenu),
		Label(kVtbl_LevelUpMenu),
		Label(kVtbl_LoadingMenu),
		Label(kVtbl_LockpickMenu),
		Label(kVtbl_MapMenu),
		Label(kVtbl_MessageMenu),
		Label(kVtbl_PlayerNameEntryMenu),
		Label(kVtbl_RaceSexMenu),
		Label(kVtbl_RepairMenu),
		Label(kVtbl_RepairServicesMenu),
		Label(kVtbl_RouletteMenu),
		Label(kVtbl_SleepWaitMenu),
		Label(kVtbl_SlotMachineMenu),
		Label(kVtbl_StartMenu),
		Label(kVtbl_TraitMenu),
		Label(kVtbl_TraitSelectMenu),
		Label(kVtbl_TraitSelectMenu),
		Label(kVtbl_TutorialMenu),

		Label(kVtbl_Tile, Handle::AsTile),
		Label(kVtbl_TileMenu),
		Label(kVtbl_TileRect),
		Label(kVtbl_TileImage),


		Label(kVtbl_TESForm, Handle::AsTESForm),
		Label(kVtbl_AlchemyItem),
		Label(kVtbl_BGSDehydrationStage),
		Label(kVtbl_BGSExplosion),
		Label(kVtbl_BGSHeadPart),
		Label(kVtbl_BGSHungerStage),
		Label(kVtbl_BGSListForm),
		Label(kVtbl_BGSMessage),
		Label(kVtbl_BGSNote),
		Label(kVtbl_BGSPerk),
		Label(kVtbl_BGSPlaceableWater),
		Label(kVtbl_BGSProjectile),
		Label(kVtbl_BGSSleepDeprevationStage),
		Label(kVtbl_BGSTalkingActivator),
		Label(kVtbl_BGSTerminal),
		Label(kVtbl_EffectSetting),
		Label(kVtbl_EnchantmentItem),
		Label(kVtbl_IngredientItem),
		Label(kVtbl_MagicItemForm),
		Label(kVtbl_MagicItemObject),
		Label(kVtbl_NavMesh),
		Label(kVtbl_Script),
		Label(kVtbl_SpellItem),
		Label(kVtbl_TESAmmo),
		Label(kVtbl_TESAmmoEffect),
		Label(kVtbl_TESBoundAnimObject),
		Label(kVtbl_TESBoundObject),
		Label(kVtbl_TESBoundTreeObject),
		Label(kVtbl_TESCaravanCard),
		Label(kVtbl_TESCaravanDeck),
		Label(kVtbl_TESCaravanMoney),
		Label(kVtbl_TESCasino),
		Label(kVtbl_TESCasinoChips),
		Label(kVtbl_TESChallenge),
		Label(kVtbl_TESClass),
		Label(kVtbl_TESClimate),
		Label(kVtbl_TESCombatStyle),
		Label(kVtbl_TESCreature),
		Label(kVtbl_TESEffectShader),
		Label(kVtbl_TESEyes),
		Label(kVtbl_TESFaction),
		Label(kVtbl_TESFlora),
		Label(kVtbl_TESFurniture),
		Label(kVtbl_TESGlobal),
		Label(kVtbl_TESGrass),
		Label(kVtbl_TESHair),
		Label(kVtbl_TESImageSpace),
		Label(kVtbl_TESImageSpaceModifier),
		Label(kVtbl_TESKey),
		Label(kVtbl_TESLandTexture),
		Label(kVtbl_TESLevCharacter),
		Label(kVtbl_TESLevCreature),
		Label(kVtbl_TESLevItem),
		Label(kVtbl_TESLevSpell),
		Label(kVtbl_TESNPC),
		Label(kVtbl_TESObject),
		Label(kVtbl_TESObjectACTI),
		Label(kVtbl_TESObjectANIO),
		Label(kVtbl_TESObjectARMA),
		Label(kVtbl_TESObjectARMO),
		Label(kVtbl_TESObjectBOOK),
		Label(kVtbl_TESObjectCELL),
		Label(kVtbl_TESObjectCLOT),
		Label(kVtbl_TESObjectCONT),
		Label(kVtbl_TESObjectDOOR),
		Label(kVtbl_TESObjectIMOD),
		Label(kVtbl_TESObjectMISC),
		Label(kVtbl_TESObjectSTAT),
		Label(kVtbl_TESObjectTREE),
		Label(kVtbl_TESObjectWEAP),
		Label(kVtbl_TESPackage),
		Label(kVtbl_TESQuest),
		Label(kVtbl_TESRace),
		Label(kVtbl_TESRecipe),
		Label(kVtbl_TESReputation),
		Label(kVtbl_TESSkill),
		Label(kVtbl_TESSound),
		Label(kVtbl_TESTopic),
		Label(kVtbl_TESTopicInfo),
		Label(kVtbl_TESWaterForm),
		Label(kVtbl_TESWeather),
		Label(kVtbl_TESWorldSpace),

		Label(kVtbl_TESObjectREFR, Handle::AsTESObjectREFR),
		Label(kVtbl_MobileObject),
		Label(kVtbl_Actor),
		Label(kVtbl_Creature),
		Label(kVtbl_Character),
		Label(kVtbl_PlayerCharacter),

		Label(kVtbl_BaseProcess, Handle::AsBaseProcess),
		Label(kVtbl_LowProcess),
		Label(kVtbl_MiddleLowProcess),
		Label(kVtbl_MiddleHighProcess),
		Label(kVtbl_HighProcess),



		Label(kVtbl_TESTexture, Handle::AsTESTexture),
		Label(kVtbl_TESIcon),
		Label(kVtbl_QueuedTexture, Handle::AsQueuedTexture),


		// Ni

		Label(kVtbl_NiNode, Handle::AsNiObjectNET),
		Label(kVtbl_BSFadeNode),


		// animations
		Label(kVtbl_BSAnimGroupSequence, Handle::AsBSAnimGroupSequence),
		Label(kVtbl_AnimSequenceSingle, Handle::AsAnimSequenceSingle),

		// model
		Label(kVtbl_BSFile, Handle::AsBSFile),
		Label(kVtbl_TESModel, Handle::AsTESModel),
		Label(kVtbl_QueuedModel, Handle::AsQueuedModel),


		Label(kVtbl_MagicItem, nullptr),
		Label(kVtbl_TESFullName),
		Label(kVtbl_BaseFormComponent),
		Label(kVtbl_Setting),
		Label(kVtbl_GameSettingCollection),
		Label(kVtbl_EffectItemList),
		Label(kVtbl_ExtraCell3D),
		Label(kVtbl_BSExtraData),
		Label(kVtbl_ExtraHavok),
		Label(kVtbl_ExtraRegionList),
		Label(kVtbl_ExtraCellMusicType),
		Label(kVtbl_ExtraCellAcousticSpace),
		Label(kVtbl_ExtraCellClimate),
		Label(kVtbl_ExtraCellImageSpace),
		Label(kVtbl_ExtraCellImpactSwap),
		Label(kVtbl_ExtraCellWaterType),
		Label(kVtbl_ExtraProcessMiddleLow),
		Label(kVtbl_ExtraCellCanopyShadowMask),
		Label(kVtbl_ExtraSeenData),
		Label(kVtbl_ExtraNorthRotation),
		Label(kVtbl_ExtraDetachTime),
		Label(kVtbl_BaseExtraList),
		Label(kVtbl_ExtraDataList),
		Label(kVtbl_ExtraReflectedRefs),
		Label(kVtbl_ExtraReflectorRefs),
		Label(kVtbl_ExtraWaterLightRefs),
		Label(kVtbl_ExtraLitWaterRefs),
		Label(kVtbl_ExtraSpecialRenderFlags),
		Label(kVtbl_ExtraNavMeshPortal),
		Label(kVtbl_ExtraRadioData),
		Label(kVtbl_ExtraHealthPerc),
		Label(kVtbl_ExtraObjectHealth),
		Label(kVtbl_ExtraTerminalState),
		Label(kVtbl_ExtraEditorID),
		Label(kVtbl_ExtraOpenCloseActivateRef),
		Label(kVtbl_ExtraPrimitive),
		Label(kVtbl_ExtraPatrolRefData),
		Label(kVtbl_ExtraOcclusionPlaneRefData),
		Label(kVtbl_ExtraPortalRefData),
		Label(kVtbl_ExtraRoomRefData),
		Label(kVtbl_ExtraCollisionData),
		Label(kVtbl_ExtraPackageData),
		Label(kVtbl_ExtraRadius),
		Label(kVtbl_ExtraRadiation),
		Label(kVtbl_ExtraLock),
		Label(kVtbl_ExtraTeleport),
		Label(kVtbl_ExtraOwnership),
		Label(kVtbl_ExtraGlobal),
		Label(kVtbl_ExtraRank),
		Label(kVtbl_ExtraCount),
		Label(kVtbl_ExtraHealth),
		Label(kVtbl_ExtraUses),
		Label(kVtbl_ExtraTimeLeft),
		Label(kVtbl_ExtraCharge),
		Label(kVtbl_ExtraScript),
		Label(kVtbl_ExtraScale),
		Label(kVtbl_ExtraHotkey),
		Label(kVtbl_ExtraReferencePointer),
		Label(kVtbl_ExtraTresPassPackage),
		Label(kVtbl_ExtraLeveledItem),
		Label(kVtbl_ExtraPoison),
		Label(kVtbl_ExtraHeadingTarget),
		Label(kVtbl_ExtraHasNoRumors),
		Label(kVtbl_ExtraModelSwap),
		Label(kVtbl_ExtraActorCause),
		Label(kVtbl_ExtraAmmo),
		Label(kVtbl_ExtraWeaponModFlags),
		Label(kVtbl_ExtraSecuritronFace),
		Label(kVtbl_ExtraModdingItem),
		Label(kVtbl_ExtraCombatStyle),
		Label(kVtbl_ExtraAnim),
		Label(kVtbl_ExtraDismemberedLimbs),
		Label(kVtbl_ExtraStartingPosition),
		Label(kVtbl_ExtraStartingWorldOrCell),
		Label(kVtbl_ExtraLight),
		Label(kVtbl_ExtraFollower),
		Label(kVtbl_ExtraGuardedRefData),
		Label(kVtbl_ExtraLeveledCreature),
		Label(kVtbl_ExtraMapMarker),
		Label(kVtbl_ExtraAudioMarker),
		Label(kVtbl_ExtraAudioBuoyMarker),
		Label(kVtbl_ExtraAction),
		Label(kVtbl_ExtraContainerChanges),
		Label(kVtbl_ExtraOriginalReference),
		Label(kVtbl_ExtraGhost),
		Label(kVtbl_ExtraWorn),
		Label(kVtbl_ExtraWornLeft),
		Label(kVtbl_ExtraCannotWear),
		Label(kVtbl_ExtraInfoGeneralTopic),
		Label(kVtbl_ExtraSeed),
		Label(kVtbl_ExtraPackageStartLocation),
		Label(kVtbl_ExtraPackage),
		Label(kVtbl_ExtraPlayerCrimeList),
		Label(kVtbl_ExtraPersistentCell),
		Label(kVtbl_ExtraRagDollData),
		Label(kVtbl_ExtraEncounterZone),
		Label(kVtbl_ExtraUsedMarkers),
		Label(kVtbl_ExtraReservedMarkers),
		Label(kVtbl_ExtraRunOncePacks),
		Label(kVtbl_ExtraDistantData),
		Label(kVtbl_ExtraEnableStateParent),
		Label(kVtbl_ExtraEnableStateChildren),
		Label(kVtbl_ExtraRandomTeleportMarker),
		Label(kVtbl_ExtraLinkedRefChildren),
		Label(kVtbl_ExtraLinkedRef),
		Label(kVtbl_ExtraAshPileRef),
		Label(kVtbl_ExtraActivateRefChildren),
		Label(kVtbl_ExtraActivateRef),
		Label(kVtbl_ExtraDecalRefs),
		Label(kVtbl_ExtraMerchantContainer),
		Label(kVtbl_ExtraLevCreaModifier),
		Label(kVtbl_ExtraLastFinishedSequence),
		Label(kVtbl_ExtraXTarget),
		Label(kVtbl_ExtraEmittanceSource),
		Label(kVtbl_ExtraMultiBoundRef),
		Label(kVtbl_ExtraMultiBoundData),
		Label(kVtbl_ExtraMultiBound),
		Label(kVtbl_ExtraOcclusionPlane),
		Label(kVtbl_ExtraPortal),
		Label(kVtbl_ExtraRoom),
		Label(kVtbl_ExtraItemDropper),
		Label(kVtbl_ExtraDroppedItemList),
		Label(kVtbl_ExtraSavedAnimation),
		Label(kVtbl_ExtraSavedHavokData),
		Label(kVtbl_ExtraFriendHits),
		Label(kVtbl_ExtraRefractionProperty),
		Label(kVtbl_ExtraEditorRefMoveData),
		Label(kVtbl_ExtraSound),
		Label(kVtbl_ExtraCreatureAwakeSound),
		Label(kVtbl_ExtraCreatureMovementSound),
		Label(kVtbl_ExtraWeaponIdleSound),
		Label(kVtbl_ExtraWeaponAttackSound),
		Label(kVtbl_ExtraActivateLoopSound),
		Label(kVtbl_ExtraTalkingActor),
		Label(kVtbl_ExtraFactionChanges),
		Label(kVtbl_ExtraSayTopicInfoOnceADay),
		Label(kVtbl_ExtraSayToTopicInfo),
		Label(kVtbl_ExtraWaterZoneMap),
		Label(kVtbl_ExtraIgnoredBySandbox),
		Label(kVtbl_ExtraPatrolRefInUseData),
		Label(kVtbl_ExtraFollowerSwimBreadcrumbs),
		Label(kVtbl_NiAlphaProperty),
		Label(kVtbl_NiProperty),

		Label(kVtbl_BSStream),
		Label(kVtbl_QueuedTreeBillboard),
		Label(kVtbl_QueuedTreeModel),
		Label(kVtbl_QueuedMagicItem),
		Label(kVtbl_QueuedKF),
		Label(kVtbl_QueuedAnimIdle),
		Label(kVtbl_QueuedReplacementKF),
		Label(kVtbl_QueuedHead),
		Label(kVtbl_QueuedHelmet),
		Label(kVtbl_QueuedReference),
		Label(kVtbl_AttachDistant3DTask),
		Label(kVtbl_IOTask),
		Label(kVtbl_QueuedTree),
		Label(kVtbl_QueuedActor),
		Label(kVtbl_QueuedCharacter),
		Label(kVtbl_QueuedCreature),
		Label(kVtbl_QueuedPlayer),
		Label(kVtbl_QueuedFileLoad),
		Label(kVtbl_QueuedFaceGenFile),
		Label(kVtbl_BackgroundCloneThread),
		Label(kVtbl_QueuedReplacementKFList),
		Label(kVtbl_InterfacedClass),
		Label(kVtbl_INISettingCollection),
		Label(kVtbl_TES),
		Label(kVtbl_BSTempNodeManager),
		Label(kVtbl_BSTempNode),
		Label(kVtbl_RendererSettingCollection),
		Label(kVtbl_BGSAmmoForm),
		Label(kVtbl_BGSBipedModelList),
		Label(kVtbl_BGSClipRoundsForm),
		Label(kVtbl_BGSDestructibleObjectForm),
		Label(kVtbl_BGSEquipType),
		Label(kVtbl_BGSIdleCollection),
		Label(kVtbl_BGSMessageIcon),
		Label(kVtbl_BGSPickupPutdownSounds),
		Label(kVtbl_BGSRepairItemList),
		Label(kVtbl_BGSTouchSpellForm),
		Label(kVtbl_TESActorBaseData),
		Label(kVtbl_TESAIForm),
		Label(kVtbl_TESAnimation),
		Label(kVtbl_TESAttackDamageForm),
		Label(kVtbl_TESAttributes),
		Label(kVtbl_TESBipedModelForm),
		Label(kVtbl_TESModelRDT),
		Label(kVtbl_TESContainer),
		Label(kVtbl_TESDescription),
		Label(kVtbl_TESEnchantableForm),

		Label(kVtbl_TESImageSpaceModifiableForm),
		Label(kVtbl_TESLeveledList),

		Label(kVtbl_TESModelList),
		Label(kVtbl_TESModelTextureSwap),

		Label(kVtbl_TESHealthForm),

		Label(kVtbl_TESProduceForm),
		Label(kVtbl_TESRaceForm),
		Label(kVtbl_TESReactionForm),
		Label(kVtbl_TESScriptableForm),

		Label(kVtbl_TESValueForm),
		Label(kVtbl_TESWeightForm),


		Label(kVtbl_TESSpellList),

		Label(kVtbl_AnimSequenceBase),
		Label(kVtbl_AnimSequenceMultiple),
		Label(kVtbl_AnimIdle),
		Label(kVtbl_NiRefObject),

		Label(kVtbl_NiStencilProperty),
		Label(kVtbl_BSCullingProcess),
		Label(kVtbl_hkpAllRayHitCollector),
		Label(kVtbl_hkpRayHitCollector),
		Label(kVtbl_BGSPrimitive),
		Label(kVtbl_BGSPrimitivePlane),
		Label(kVtbl_BSOcclusionPlane),
		Label(kVtbl_BSPortal),
		Label(kVtbl_BGSPrimitiveBox),
		Label(kVtbl_BSMultiBoundAABB),
		Label(kVtbl_BSMultiBoundOBB),
		Label(kVtbl_BGSPrimitiveSphere),
		Label(kVtbl_BSMultiBoundSphere),
		Label(kVtbl_BGSSceneInfo),
		Label(kVtbl_BSAnimNoteReceiver),
		Label(kVtbl_IBSAnimNoteReceiver),
		Label(kVtbl_bhkMouseSpringAction),
		Label(kVtbl_bhkUnaryAction),
		Label(kVtbl_bhkAction),
		Label(kVtbl_bhkSerializable),
		Label(kVtbl_GridArray),
		Label(kVtbl_GridCellArray),
		Label(kVtbl_INIPrefSettingCollection),
		Label(kVtbl_BSCustomSplatterExtraData),
		Label(kVtbl_BSSplatterExtraData),
		Label(kVtbl_ImageSpaceEffectParam),
		Label(kVtbl_BGSDecalNode),
		Label(kVtbl_TESRegion),
		Label(kVtbl_TESRegionData),
		Label(kVtbl_TESRegionDataGrass),
		Label(kVtbl_TESRegionDataImposter),
		Label(kVtbl_TESRegionDataLandscape),
		Label(kVtbl_TESRegionDataManager),
		Label(kVtbl_TESRegionDataMap),
		Label(kVtbl_TESRegionDataSound),
		Label(kVtbl_TESRegionDataWeather),
		Label(kVtbl_TESRegionGrassObject),
		Label(kVtbl_TESRegionObjectBase),
		Label(kVtbl_TESRegionGrassObjectList),
		Label(kVtbl_TESRegionList),
		Label(kVtbl_BGSAddonNode),
		Label(kVtbl_BGSAddonNodeSoundHandleExtra),
		Label(kVtbl_BGSConstructibleObject),
		Label(kVtbl_BGSDebris),
		Label(kVtbl_BGSPreloadable),

		Label(kVtbl_BGSMovableStatic),


		Label(kVtbl_BGSStaticCollection),
		Label(kVtbl_PathingLocation),

		Label(kVtbl_BGSOpenCloseForm),

		Label(kVtbl_TESModelTree),
		Label(kVtbl_TESIconTree),
		Label(kVtbl_TESTexture1024),
		Label(kVtbl_NiBillboardNode),
		Label(kVtbl_BSShaderTextureSet),
		Label(kVtbl_BSTextureSet),
		Label(kVtbl_BGSEncounterZone),
		Label(kVtbl_BGSLightingTemplate),
		Label(kVtbl_LoadedReferenceMap),
		Label(kVtbl_ExteriorCellLoaderTask),
		Label(kVtbl_ImageSpaceModifierInstanceForm),
		Label(kVtbl_ImageSpaceModifierInstance),
		Label(kVtbl_ImageSpaceModifierInstanceDOF),
		Label(kVtbl_ImageSpaceModifierInstanceRB),
		Label(kVtbl_ImageSpaceModifierInstanceDRB),
		Label(kVtbl_ImageSpaceModifierInstanceTemp),

		Label(kVtbl_NiFloatData),
		Label(kVtbl_NiColorData),
		Label(kVtbl_TESObjectLAND),
		Label(kVtbl_TESChildCell),
		Label(kVtbl_BSPackedAdditionalGeometryData),
		Label(kVtbl_NiAdditionalGeometryData),
		Label(kVtbl_hkpMoppCode),
		Label(kVtbl_hkReferencedObject),
		Label(kVtbl_hkBaseObject),
		Label(kVtbl_NiBinaryExtraData),

		Label(kVtbl_CheckWithinMultiBoundTask),
		Label(kVtbl_hkpWorldCinfo),
		Label(kVtbl_TESWindListener),
		Label(kVtbl_hkpEntityListener),

		Label(kVtbl_bhkRigidBody),
		Label(kVtbl_bhkEntity),
		Label(kVtbl_bhkWorldObject),
		Label(kVtbl_bhkCollisionObject),
		Label(kVtbl_bhkWaterPhantomCallbackShape),
		Label(kVtbl_hkpPhantomCallbackShape),
		Label(kVtbl_hkpShape),
		Label(kVtbl_bhkSimpleShapePhantom),
		Label(kVtbl_bhkShapePhantom),
		Label(kVtbl_bhkPhantom),
		Label(kVtbl_bhkAabbPhantom),
		Label(kVtbl_bhkBoxShape),
		Label(kVtbl_bhkConvexShape),
		Label(kVtbl_bhkSphereRepShape),
		Label(kVtbl_bhkShape),
		Label(kVtbl_bhkSphereShape),
		Label(kVtbl_bhkAcousticSpacePhantomCallbackShape),
		Label(kVtbl_ExtraAnimNoteReceiver),


		Label(kVtbl_BGSAcousticSpace),
		Label(kVtbl_BGSCameraPath),
		Label(kVtbl_NiFormArray),
		Label(kVtbl_BGSCameraShot),
		Label(kVtbl_BGSDefaultObjectManager),
		Label(kVtbl_BGSImpactData),
		Label(kVtbl_BGSImpactDataSet),
		Label(kVtbl_BGSMenuIcon),
		Label(kVtbl_BGSMusicType),
		Label(kVtbl_TESSoundFile),
		Label(kVtbl_BGSRadiationStage),
		Label(kVtbl_BGSTextureSet),
		Label(kVtbl_MediaLocationController),
		Label(kVtbl_MediaSet),
		// w
		Label(kVtbl_hkpClosestRayHitCollector),
		Label(kVtbl_TESLoadScreen),
		Label(kVtbl_TESLoadScreenType),
		Label(kVtbl_TESRecipeCategory),

		Label(kVtbl_MobIterOperator),
		Label(kVtbl_BGSBodyPart),
		Label(kVtbl_TESModelPSA),
		Label(kVtbl_BGSBodyPartData),
		Label(kVtbl_BGSEntryPointFunctionDataOneValue),
		Label(kVtbl_BGSEntryPointFunctionData),
		Label(kVtbl_BGSEntryPointFunctionDataTwoValue),
		Label(kVtbl_BGSEntryPointFunctionDataLeveledList),
		Label(kVtbl_BGSEntryPointFunctionDataActivateChoice),
		Label(kVtbl_BGSIdleMarker),
		Label(kVtbl_BGSPerkEntry),
		Label(kVtbl_BGSQuestPerkEntry),
		Label(kVtbl_BGSAbilityPerkEntry),
		Label(kVtbl_BGSEntryPointPerkEntry),
		Label(kVtbl_BGSQuestObjective),
		Label(kVtbl_BGSRagdoll),
		Label(kVtbl_BGSVoiceType),
		Label(kVtbl_Reset3DMobIterator),
		Label(kVtbl_TESAnimGroup),

		Label(kVtbl_TESActorBase),
		Label(kVtbl_ActorValueOwner),
		Label(kVtbl_TESIdleForm),
		Label(kVtbl_TESModelAnim),

		Label(kVtbl_BGSTextureModel),

		Label(kVtbl_TopicInfoArray),

		Label(kVtbl_BGSAcousticSpaceListener),
		Label(kVtbl_bhkEntityListener),
		Label(kVtbl_TargetEntry),
		Label(kVtbl_ZoneEntry),
		Label(kVtbl_TrapTargetEntry),
		Label(kVtbl_BGSZoneTargetListener),
		Label(kVtbl_hkpAllCdPointCollector),
		Label(kVtbl_hkpCdPointCollector),
		Label(kVtbl_CellMopp),
		Label(kVtbl_bhkTriSampledHeightFieldBvTreeShape),
		Label(kVtbl_bhkBvTreeShape),
		Label(kVtbl_FOCollisionListener),
		Label(kVtbl_bhkContactListener),
		Label(kVtbl_hkpContactListener),
		Label(kVtbl_FOIslandActivationListener),
		Label(kVtbl_hkpIslandActivationListener),
		Label(kVtbl_HavokStreambufFactory),
		Label(kVtbl_hkFileSystem),
		Label(kVtbl_HavokError),
		Label(kVtbl_hkError),
		Label(kVtbl_hkStreamReader),
		Label(kVtbl_hkStreamWriter),
		Label(kVtbl_HavokFileStreambufReader),
		Label(kVtbl_HavokFileStreambufWriter),
		Label(kVtbl_LoadedAreaBound),
		Label(kVtbl_SpecificItemCollector),
		Label(kVtbl_TESArrowTargetListener),
		Label(kVtbl_bhkArrowTargetListener),
		Label(kVtbl_hkpAllCdBodyPairCollector),
		Label(kVtbl_hkpCdBodyPairCollector),
		Label(kVtbl_TESTrapListener),
		Label(kVtbl_bhkTrapListener),
		Label(kVtbl_hkpPhantomListener),
		Label(kVtbl_TrapEntry),
		Label(kVtbl_WaterTargetEntry),
		Label(kVtbl_TESWaterListener),
		Label(kVtbl_WaterZone),
		Label(kVtbl_BlendSettingCollection),
		Label(kVtbl_Atmosphere),
		Label(kVtbl_Clouds),
		Label(kVtbl_Moon),
		Label(kVtbl_Precipitation),
		Label(kVtbl_Sky),
		Label(kVtbl_SkyObject),
		Label(kVtbl_SkyTask),
		Label(kVtbl_Stars),
		Label(kVtbl_Sun),
		Label(kVtbl_BSFaceGenAnimationData),
		Label(kVtbl_BSFaceGenImage),
		Label(kVtbl_BSFaceGenKeyframe),
		Label(kVtbl_BSFaceGenKeyframeMultiple),
		Label(kVtbl_BSFaceGenModelMap),
		Label(kVtbl_BSFaceGenModel),
		Label(kVtbl_BSFaceGenModelExtraData),
		Label(kVtbl_BSFaceGenMorph),
		Label(kVtbl_BSFaceGenMorphDifferential),
		Label(kVtbl_BSFaceGenMorphStatistical),
		Label(kVtbl_BSFaceGenMorphDataHead),
		Label(kVtbl_BSFaceGenMorphData),
		Label(kVtbl_BSFaceGenMorphDataHair),
		Label(kVtbl_BSFaceGenBaseMorphExtraData),
		Label(kVtbl_BSFaceGenNiNode),
		Label(kVtbl_BSTreeModel),
		Label(kVtbl_BSTreeNode),
		Label(kVtbl_bhkSPCollisionObject),
		Label(kVtbl_bhkPCollisionObject),
		Label(kVtbl_bhkMultiSphereShape),
		Label(kVtbl_bhkTransformShape),
		Label(kVtbl_bhkCapsuleShape),
		Label(kVtbl_ActorValueInfo),

		Label(kVtbl_TESAmbushPackageData),
		Label(kVtbl_TESPackageData),
		Label(kVtbl_TESDialoguePackageData),
		Label(kVtbl_TESEatPackageData),
		Label(kVtbl_TESEscortPackageData),
		Label(kVtbl_TESFollowPackageData),
		Label(kVtbl_TESPatrolPackageData),
		Label(kVtbl_TESUseItemPackageData),
		Label(kVtbl_TESUseWeaponPackageData),
		Label(kVtbl_UseWeaponActorPackageData),
		Label(kVtbl_ActorPackageData),
		Label(kVtbl_NiTask),
		Label(kVtbl_BSTempEffect),
		Label(kVtbl_BSTempEffectDebris),
		Label(kVtbl_BSTempEffectGeometryDecal),
		Label(kVtbl_BSDismemberSkinInstance),
		Label(kVtbl_NiSkinInstance),
		Label(kVtbl_BSTempEffectParticle),
		Label(kVtbl_BSTempEffectSimpleDecal),

		Label(kVtbl_NavMeshPOVSearch),
		Label(kVtbl_NavMeshSearch),
		Label(kVtbl_NavMeshSearchClosePoint),
		Label(kVtbl_NavMeshSearchFlee),
		Label(kVtbl_NavMeshSearchHide),
		Label(kVtbl_NavMeshSearchLOS),
		Label(kVtbl_NavMeshSearchMaxCost),
		Label(kVtbl_NavMeshSearchSLPoint),
		Label(kVtbl_PathSmootherPOVSearch),
		Label(kVtbl_NavMeshInfoMap),
		Label(kVtbl_NavMeshInfoSearch),
		Label(kVtbl_ObstacleUndoData),
		Label(kVtbl_bhkObstacleDeactivationListener),
		Label(kVtbl_hkpEntityActivationListener),
		Label(kVtbl_bhkObstacleRemovalListener),
		Label(kVtbl_ReferenceObstacleArray),
		Label(kVtbl_ObstacleData),
		Label(kVtbl_BSTasklet),
		Label(kVtbl_ObstacleTaskData),
		Label(kVtbl_BSWin32TaskletData),
		Label(kVtbl_BSTaskletData),
		Label(kVtbl_FindTriangleForLocationFilter),
		Label(kVtbl_CheckLineOfSightFilterWater),
		Label(kVtbl_PathingCoverLocation),
		Label(kVtbl_PathingLOSGridMap),
		Label(kVtbl_PathingLOSMap),
		Label(kVtbl_PathingRequest),
		Label(kVtbl_PathingAvoidNodeArray),
		Label(kVtbl_PathingRequestClosePoint),
		Label(kVtbl_PathingRequestCover),
		Label(kVtbl_PathingRequestCoveredMove),
		Label(kVtbl_PathingRequestFlee),
		Label(kVtbl_PathingRequestHide),
		Label(kVtbl_PathingRequestLOS),
		Label(kVtbl_PathingRequestOptimalLocation),
		Label(kVtbl_PathingRequestSafeStraightLine),
		Label(kVtbl_PathingSearchRayCast),
		Label(kVtbl_PathingSolution),
		Label(kVtbl_PathingTaskData),
		Label(kVtbl_TeleportDoorSearch),
		Label(kVtbl_BGSDistantObjectBlockLoadTask),
		Label(kVtbl_BGSDistantTreeBlockLoadTask),
		Label(kVtbl_BGSTerrainChunkLoadTask),
		Label(kVtbl_CCallbackBase),
		Label(kVtbl_FORenderedTerminal),


		Label(kVtbl_ListBoxBase),


		Label(kVtbl_LoadingMenuThread),


		Label(kVtbl_FORenderedMenuRaceSex),


		Label(kVtbl_FOPipboyManager),
		Label(kVtbl_FORenderedMenu),
		Label(kVtbl_AbsorbEffect),
		Label(kVtbl_ActiveEffect),
		Label(kVtbl_AssociatedItemEffect),
		Label(kVtbl_bhkCharacterListenerSpell),
		Label(kVtbl_bhkTelekinesisListener),
		Label(kVtbl_BoundItemEffect),
		Label(kVtbl_CalmEffect),
		Label(kVtbl_ChameleonEffect),
		Label(kVtbl_CommandCreatureEffect),
		Label(kVtbl_CommandEffect),
		Label(kVtbl_CommandHumanoidEffect),
		Label(kVtbl_ConcussionEffect),
		Label(kVtbl_CureEffect),
		Label(kVtbl_DarknessEffect),
		Label(kVtbl_DemoralizeEffect),
		Label(kVtbl_DetectLifeEffect),
		Label(kVtbl_DisintegrateArmorEffect),
		Label(kVtbl_DisintegrateWeaponEffect),
		Label(kVtbl_DispelEffect),
		Label(kVtbl_FrenzyEffect),
		Label(kVtbl_InvisibilityEffect),
		Label(kVtbl_LightEffect),
		Label(kVtbl_LimbConditionEffect),
		Label(kVtbl_LockEffect),
		Label(kVtbl_MagicBallProjectile),
		Label(kVtbl_MagicBoltProjectile),
		Label(kVtbl_MagicCaster),
		Label(kVtbl_MagicFogProjectile),
		Label(kVtbl_MagicHitEffect),
		Label(kVtbl_MagicModelHitEffect),
		Label(kVtbl_MagicProjectile),
		Label(kVtbl_MagicShaderHitEffect),
		Label(kVtbl_MagicSprayProjectile),
		Label(kVtbl_MagicTarget),
		Label(kVtbl_NightEyeEffect),
		Label(kVtbl_NonActorMagicCaster),
		Label(kVtbl_NonActorMagicTarget),
		Label(kVtbl_OpenEffect),
		Label(kVtbl_ParalysisEffect),
		Label(kVtbl_ReanimateEffect),
		Label(kVtbl_ScriptEffect),
		Label(kVtbl_ShieldEffect),
		Label(kVtbl_SummonCreatureEffect),
		Label(kVtbl_SunDamageEffect),
		Label(kVtbl_TelekinesisEffect),
		Label(kVtbl_TurnUndeadEffect),
		Label(kVtbl_ValueAndConditionsEffect),
		Label(kVtbl_ValueModifierEffect),
		Label(kVtbl_VampirismEffect),
		Label(kVtbl_CBSAStream),
		Label(kVtbl_CAsyncStream),
		Label(kVtbl_CBSAReader),
		Label(kVtbl_RadioConvTask),
		Label(kVtbl_BGSReconstructFormsMap),
		Label(kVtbl_BGSReconstructCellsMap),
		Label(kVtbl_BGSReconstructCellSubBlocksMap),
		Label(kVtbl_BGSReconstructCellBlocksMap),
		Label(kVtbl_BGSReconstructWorldSpacesMap),
		Label(kVtbl_BGSReconstructFormsInFileMap),
		Label(kVtbl_BGSOffsetDataArray),
		Label(kVtbl_BGSReconstructFormsInAllFilesMap),
		Label(kVtbl_BGSSaveLoadChangesMap),
		Label(kVtbl_BGSCellNumericIDArrayMap),
		Label(kVtbl_BGSSaveLoadStatsMap),
		Label(kVtbl_ChangesMap),
		Label(kVtbl_InteriorCellNewReferencesMap),
		Label(kVtbl_ExteriorCellNewReferencesMap),
		Label(kVtbl_NumericIDBufferMap),
		Label(kVtbl_SaveGameFile),
		Label(kVtbl_BGSLoadFormBuffer),
		Label(kVtbl_BGSLoadGameBuffer),
		Label(kVtbl_BGSSaveFormBuffer),
		Label(kVtbl_BGSSaveGameBuffer),
		Label(kVtbl_BGSMoviePlayer),
		Label(kVtbl_HighActorCuller),
		Label(kVtbl_BSFadeNodeCuller),
		Label(kVtbl_RegSettingCollection),
		Label(kVtbl_MemoryManagerFile),
		Label(kVtbl_IMemoryManagerFile),
		Label(kVtbl_SceneGraph),
		Label(kVtbl_SeenData),
		Label(kVtbl_IntSeenData),

		Label(kVtbl_CachedValuesOwner),
		Label(kVtbl_bhkFilteredCdBodyCollector),
		Label(kVtbl_AILinearTaskThread),
		Label(kVtbl_BSLinearTaskThread),
		Label(kVtbl_DetectionTaskData),
		Label(kVtbl_MobileObjectTaskletData),
		Label(kVtbl_AnimationTaskData),
		Label(kVtbl_ActorUpdateTaskData),
		Label(kVtbl_PackageUpdateTaskData),
		Label(kVtbl_ActorsScriptTaskData),
		Label(kVtbl_MovementTaskData),
		Label(kVtbl_AITaskThread),
		Label(kVtbl_ArrowProjectile),

		Label(kVtbl_bhkCharacterListenerArrow),
		Label(kVtbl_BSPlayerDistanceCheckController),


		Label(kVtbl_ActorPathingMessageQueue),
		Label(kVtbl_LipTask),


		Label(kVtbl_BeamProjectile),
		Label(kVtbl_CombatAction),
		Label(kVtbl_CombatActionAttackRanged),
		Label(kVtbl_CombatActionAttackRangedFromCover),
		Label(kVtbl_CombatActionAttackRangedExplosive),
		Label(kVtbl_CombatActionAttackRangedExplosiveFromCover),
		Label(kVtbl_CombatActionAttackMelee),
		Label(kVtbl_CombatActionAttackHandToHand),
		Label(kVtbl_CombatActionAttackGrenade),
		Label(kVtbl_CombatActionAttackGrenadeFlushTarget),
		Label(kVtbl_CombatActionAttackGrenadeFromCover),
		Label(kVtbl_CombatActionMove),
		Label(kVtbl_CombatActionMoveAndSwitchToMelee),
		Label(kVtbl_CombatActionMoveAndAttackRanged),
		Label(kVtbl_CombatActionMoveAndAttackRangedExplosive),
		Label(kVtbl_CombatActionMoveAndAttackGrenade),
		Label(kVtbl_CombatActionDrawWeapon),
		Label(kVtbl_CombatActionSwitchWeapon),
		Label(kVtbl_CombatActionAvoidThreat),
		Label(kVtbl_CombatActionSearch),
		Label(kVtbl_CombatActionInvestigate),
		Label(kVtbl_CombatActionDodge),
		Label(kVtbl_CombatActionIgnoreBlockedTarget),
		Label(kVtbl_CombatActionFlee),
		Label(kVtbl_CombatActionActivateCombatItem),
		Label(kVtbl_CombatActionUseCombatItem),
		Label(kVtbl_CombatActionApproachTarget),
		Label(kVtbl_CombatActionAcquireLineOfSight),
		Label(kVtbl_CombatActionHide),
		Label(kVtbl_CombatController),
		Label(kVtbl_CombatPlanner),
		Label(kVtbl_CombatCoverMap),
		Label(kVtbl_CombatGoal),
		Label(kVtbl_CombatGoalKillTarget),
		Label(kVtbl_CombatGoalFindTarget),
		Label(kVtbl_CombatGoalAvoidThreats),
		Label(kVtbl_CombatGoalFlee),
		Label(kVtbl_CombatGoalActivateCombatItem),
		Label(kVtbl_CombatGoalUseCombatItem),
		Label(kVtbl_CombatGoalFlank),
		Label(kVtbl_CombatGoalFallBack),
		Label(kVtbl_CombatGoalAmbush),
		Label(kVtbl_CombatGoalReachDestination),
		Label(kVtbl_CombatGoalAdvance),
		Label(kVtbl_CombatGoalGuard),
		Label(kVtbl_CombatGoalCluster),
		Label(kVtbl_CombatGoalHideFromTarget),
		Label(kVtbl_CombatGoalFlushTarget),
		Label(kVtbl_CombatGoalStayInCombatArea),
		Label(kVtbl_CombatGroupStrategyFlank),
		Label(kVtbl_CombatGroupStrategy),
		Label(kVtbl_CombatGroupStrategyFallBack),
		Label(kVtbl_CombatGroupStrategyAmbush),
		Label(kVtbl_CombatGroupStrategyAdvance),
		Label(kVtbl_CombatTargetMap),
		Label(kVtbl_CombatThreatMap),
		Label(kVtbl_ContinuousBeamProjectile),
		Label(kVtbl_Explosion),
		Label(kVtbl_bhkLiquidAction),
		Label(kVtbl_hkpFixedBufferCdPointCollector),
		Label(kVtbl_FlameProjectile),
		Label(kVtbl_GrenadeProjectile),
		Label(kVtbl_MissileProjectile),
		Label(kVtbl_bhkHingeConstraint),
		Label(kVtbl_bhkConstraint),
		Label(kVtbl_hkHingeConstraintCinfo),
		Label(kVtbl_hkConstraintCinfo),
		Label(kVtbl_Projectile),
		Label(kVtbl_ProjectileListener),
		Label(kVtbl_CombatProcedureActivateObject),
		Label(kVtbl_CombatProcedure),
		Label(kVtbl_CombatProcedureAttackGrenade),
		Label(kVtbl_CombatProcedureAttackLow),
		Label(kVtbl_CombatProcedureAttackMelee),
		Label(kVtbl_CombatProcedureAttackRanged),
		Label(kVtbl_CombatProcedureBeInCover),
		Label(kVtbl_CombatProcedureEngageTarget),
		Label(kVtbl_CombatProcedureEvade),
		Label(kVtbl_CombatProcedureHideFromTarget),
		Label(kVtbl_CombatProcedureMove),
		Label(kVtbl_CombatProcedureSearch),
		Label(kVtbl_CombatProcedureSwitchWeapon),
		Label(kVtbl_CombatProcedureUseCombatItem),
		Label(kVtbl_ActorMover),
		Label(kVtbl_ActorPathHandler),
		Label(kVtbl_DetailedActorPathHandler),
		Label(kVtbl_DoorInPathFilter),
		Label(kVtbl_PlayerMover),
		Label(kVtbl_VirtualActorPathHandler),
		Label(kVtbl_AlarmPackage),
		Label(kVtbl_BackUpPackage),
		Label(kVtbl_DialoguePackage),
		Label(kVtbl_EscortActorPackageData),
		Label(kVtbl_FleePackage),
		Label(kVtbl_GuardActorPackageData),
		Label(kVtbl_PatrolActorPackageData),
		Label(kVtbl_SandBoxActorPackageData),
		Label(kVtbl_SearchPackage),
		Label(kVtbl_SpectatorPackage),
		Label(kVtbl_TrespassPackage),
		Label(kVtbl_CEnumPins),
		Label(kVtbl_CEnumMediaTypes),
		Label(kVtbl_CMediaSample),
		Label(kVtbl_CMemAllocator),
		Label(kVtbl_CAsyncOutputPin),
		Label(kVtbl_CAsyncReader),
		Label(kVtbl_TileText),
		Label(kVtbl_Tile3D),
		Label(kVtbl_TileExtra),
		Label(kVtbl_DebugText),
		Label(kVtbl_IDebugText),
		Label(kVtbl_DebugTextExtraData),

		Label(kVtbl_RadialTile),
		Label(kVtbl_NiAVObjectPalette),
		Label(kVtbl_NiControllerManager),
		Label(kVtbl_NiDefaultAVObjectPalette),
		Label(kVtbl_NiInterpController),
		Label(kVtbl_NiMultiTargetTransformController),
		Label(kVtbl_NiControllerSequence),
		Label(kVtbl_BSMultiTargetTreadTransfController),
		Label(kVtbl_NiBlendAccumTransformInterpolator),
		Label(kVtbl_NiTransformController),
		Label(kVtbl_NiBlendInterpolator),
		Label(kVtbl_NiSingleInterpController),
		Label(kVtbl_NiGeomMorpherController),
		Label(kVtbl_NiColorInterpolator),
		Label(kVtbl_NiFloatInterpolator),
		Label(kVtbl_NiKeyBasedInterpolator),
		Label(kVtbl_NiPosData),
		Label(kVtbl_NiPathInterpolator),
		Label(kVtbl_NiTransformInterpolator),
		Label(kVtbl_NiBlendFloatInterpolator),
		Label(kVtbl_NiBlendTransformInterpolator),
		Label(kVtbl_NiFloatExtraDataController),
		Label(kVtbl_NiMorphData),
		Label(kVtbl_BSFrustumFOVController),
		Label(kVtbl_BSBlendTreadTransfInterpolator),
		Label(kVtbl_BSTreadTransfInterpolator),
		Label(kVtbl_BSRotAccumTransfInterpolator),
		Label(kVtbl_BSAnimNote),
		Label(kVtbl_BSLookIKNote),
		Label(kVtbl_BSGrabIKNote),
		Label(kVtbl_BSAnimNotes),
		Label(kVtbl_NiVisController),
		Label(kVtbl_NiUVData),
		Label(kVtbl_NiUVController),
		Label(kVtbl_NiTextureTransformController),
		Label(kVtbl_NiTextKeyExtraData),
		Label(kVtbl_NiStringPalette),
		Label(kVtbl_NiSequenceStreamHelper),
		Label(kVtbl_NiSequence),
		Label(kVtbl_NiRotData),
		Label(kVtbl_NiFloatController),
		Label(kVtbl_NiRollController),
		Label(kVtbl_NiQuaternionInterpolator),
		Label(kVtbl_NiPoint3Interpolator),
		Label(kVtbl_NiPathController),
		Label(kVtbl_NiMaterialColorController),
		Label(kVtbl_NiLookAtInterpolator),
		Label(kVtbl_NiLookAtController),
		Label(kVtbl_NiLightDimmerController),
		Label(kVtbl_NiLightColorController),
		Label(kVtbl_NiKeyframeManager),
		Label(kVtbl_NiTransformData),
		Label(kVtbl_NiFloatsExtraDataPoint3Controller),
		Label(kVtbl_NiFloatsExtraDataController),
		Label(kVtbl_NiFlipController),
		Label(kVtbl_NiColorExtraDataController),
		Label(kVtbl_NiBSplineTransformInterpolator),
		Label(kVtbl_NiBSplinePoint3Interpolator),
		Label(kVtbl_NiBSplineFloatInterpolator),
		Label(kVtbl_NiBSplineCompTransformInterpolator),
		Label(kVtbl_NiBSplineCompPoint3Interpolator),
		Label(kVtbl_NiBSplineCompFloatInterpolator),
		Label(kVtbl_NiBSplineCompColorInterpolator),
		Label(kVtbl_NiBSplineColorInterpolator),
		Label(kVtbl_NiBSplineData),
		Label(kVtbl_NiBSplineBasisData),
		Label(kVtbl_NiBoolTimelineInterpolator),
		Label(kVtbl_NiBoolInterpolator),
		Label(kVtbl_NiBoolData),
		Label(kVtbl_NiBoneLODController),
		Label(kVtbl_NiBlendQuaternionInterpolator),
		Label(kVtbl_NiBlendPoint3Interpolator),
		Label(kVtbl_NiBlendColorInterpolator),
		Label(kVtbl_NiBlendBoolInterpolator),
		Label(kVtbl_NiAlphaController),
		Label(kVtbl_NiGeomMorpherUpdateTask),
		Label(kVtbl_NiInterpolator),
		Label(kVtbl_NiExtraDataController),
		Label(kVtbl_NiFloatInterpController),
		Label(kVtbl_NiBoolInterpController),
		Label(kVtbl_NiPoint3InterpController),
		Label(kVtbl_NiBSplineInterpolator),
		Label(kVtbl_NiAVObject),
		Label(kVtbl_NiObjectNET),
		Label(kVtbl_NiObject),

		Label(kVtbl_NiVertexColorProperty),
		Label(kVtbl_NiZBufferProperty),
		Label(kVtbl_NiTexture),
		Label(kVtbl_NiSourceTexture),
		Label(kVtbl_NiSourceCubeMap),
		Label(kVtbl_NiRenderer),
		Label(kVtbl_NiStream),
		Label(kVtbl_NiGeometryData),
		Label(kVtbl_NiDynamicEffect),
		Label(kVtbl_NiCullingProcess),
		Label(kVtbl_NiTexturingProperty),
		Label(kVtbl_NiTimeController),
		Label(kVtbl_NiCamera),
		Label(kVtbl_NiTriStrips),
		Label(kVtbl_NiLight),
		Label(kVtbl_NiStringExtraData),
		Label(kVtbl_NiTriShape),
		Label(kVtbl_NiLines),
		Label(kVtbl_NiMaterialProperty),
		Label(kVtbl_NiDirectionalLight),
		Label(kVtbl_NiTriStripsData),
		Label(kVtbl_NiImageConverter),
		Label(kVtbl_NiDevImageConverter),
		Label(kVtbl_NiExtraData),
		Label(kVtbl_NiTriShapeData),
		Label(kVtbl_NiPixelData),
		Label(kVtbl_NiPointLight),
		Label(kVtbl_NiGeometryGroupManager),
		Label(kVtbl_NiRenderedTexture),
		Label(kVtbl_NiGeometry),
		Label(kVtbl_NiCollisionObject),
		Label(kVtbl_NiShadeProperty),
		Label(kVtbl_NiTriShapeDynamicData),
		Label(kVtbl_NiSkinPartition),
		Label(kVtbl_NiSkinData),
		Label(kVtbl_BSResizableTriShape),
		Label(kVtbl_BSSegmentedTriShape),
		Label(kVtbl_NiScreenElements),
		Label(kVtbl_NiScreenElementsData),
		Label(kVtbl_NiFloatsExtraData),
		Label(kVtbl_NiColorExtraData),
		Label(kVtbl_NiTriBasedGeom),
		Label(kVtbl_NiVertWeightsExtraData),
		Label(kVtbl_NiLinesData),
		Label(kVtbl_NiTriBasedGeomData),
		Label(kVtbl_NiImageReader),
		Label(kVtbl_NiNIFImageReader),
		Label(kVtbl_NiPalette),
		Label(kVtbl_NiDDSReader),
		Label(kVtbl_NiTGAReader),
		Label(kVtbl_NiBMPReader),
		Label(kVtbl_Ni2DBuffer),
		Label(kVtbl_NiShader),
		Label(kVtbl_NiCullingProperty),
		Label(kVtbl_NiMaterial),
		Label(kVtbl_NiSingleShaderMaterialLibrary),
		Label(kVtbl_NiMaterialLibrary),
		Label(kVtbl_NiScreenGeometry),
		Label(kVtbl_NiScreenPolygon),
		Label(kVtbl_NiScreenGeometryData),
		Label(kVtbl_NiScreenSpaceCamera),
		Label(kVtbl_NiVectorExtraData),
		Label(kVtbl_NiTriStripsDynamicData),
		Label(kVtbl_NiTextureEffect),
		Label(kVtbl_NiSwitchStringExtraData),
		Label(kVtbl_NiSwitchNode),
		Label(kVtbl_NiStringsExtraData),
		Label(kVtbl_NiSpotLight),
		Label(kVtbl_NiSpecularProperty),
		Label(kVtbl_NiSortAdjustNode),
		Label(kVtbl_NiScreenTexture),
		Label(kVtbl_NiScreenLODData),
		Label(kVtbl_NiParticlesData),
		Label(kVtbl_NiRangeLODData),
		Label(kVtbl_NiPersistentSrcTextureRendererData),
		Label(kVtbl_NiParticleMeshesData),
		Label(kVtbl_NiParticleMeshes),
		Label(kVtbl_NiLODNode),
		Label(kVtbl_NiIntegersExtraData),
		Label(kVtbl_NiIntegerExtraData),
		Label(kVtbl_NiFogProperty),
		Label(kVtbl_NiFloatExtraData),
		Label(kVtbl_NiDitherProperty),
		Label(kVtbl_NiBSPNode),
		Label(kVtbl_NiBooleanExtraData),
		Label(kVtbl_NiParticles),
		Label(kVtbl_NiAmbientLight),
		Label(kVtbl_NiAlphaAccumulator),
		Label(kVtbl_NiShaderConstantMap),
		Label(kVtbl_NiDepthStencilBuffer),
		Label(kVtbl_NiGlobalConstantEntry),
		Label(kVtbl_NiShaderFactory),
		Label(kVtbl_NiSingleShaderMaterial),
		Label(kVtbl_NiLODData),
		Label(kVtbl_NiAccumulator),
		Label(kVtbl_NiBackToFrontAccumulator),
		Label(kVtbl_NiFile),
		Label(kVtbl_NiBinaryStream),
		Label(kVtbl_NiAllocator),
		Label(kVtbl_NiStandardAllocator),
		Label(kVtbl_BSNiAllocator),
		Label(kVtbl_NiSearchPath),
		Label(kVtbl_NiMemStream),
		Label(kVtbl_BSCoreMessage),
		Label(kVtbl_BSThread),
		Label(kVtbl_ZeroOverheadHeap),
		Label(kVtbl_IMemoryHeap),
		Label(kVtbl_MemoryHeap),
		Label(kVtbl_AbstractHeap),
		Label(kVtbl_FutBinaryFileC),
		Label(kVtbl_BSFaceGenBinaryFile),
		Label(kVtbl_BSAudio),
		Label(kVtbl_BSAudioManager),
		Label(kVtbl_BSAudioManagerThread),
		Label(kVtbl_AudioLoadTask),
		Label(kVtbl_BSGameSound),
		Label(kVtbl_BSWin32Audio),
		Label(kVtbl_BSWin32AudioListener),
		Label(kVtbl_BSAudioListener),
		Label(kVtbl_BSWin32GameSound),
		Label(kVtbl_BSSaveDataSystemUtility),
		Label(kVtbl_BSSystemUtility),
		Label(kVtbl_BSMsgDialogSystemUtility),
		Label(kVtbl_BSSystemUtilitiesStrings),
		Label(kVtbl_BSCacheDriveSystemUtility),
		Label(kVtbl_BSGameDataSystemUtility),
		Label(kVtbl_BSDiscBootSystemUtility),
		Label(kVtbl_BSSysInfoSystemUtility),
		Label(kVtbl_Archive),
		Label(kVtbl_ArchiveFile),
		Label(kVtbl_CompressedArchiveFile),

		Label(kVtbl_BSTaskletManagerCallback),
		Label(kVtbl_BSFileCache),
		Label(kVtbl_BSSearchPath),
		Label(kVtbl_BSWin32FileCache),
		Label(kVtbl_BSWin32TaskletManager),
		Label(kVtbl_BSTaskletManager),
		Label(kVtbl_BSWin32TaskletGroupData),
		Label(kVtbl_BSTaskletGroupData),
		Label(kVtbl_IdvFileError),
		Label(kVtbl_CTreeEngine),
		Label(kVtbl_CIdvCamera),
		Label(kVtbl_Uniform),
		Label(kVtbl_Random),
		Label(kVtbl_CBillboardLeaf),
		Label(kVtbl_PosGen),
		Label(kVtbl_Normal),
		Label(kVtbl_SymGen),

		Label(kVtbl_DistantLODShaderProperty),
		Label(kVtbl_TallGrassShaderProperty),
		Label(kVtbl_BSShaderPPLightingProperty),
		Label(kVtbl_WaterShaderProperty),
		Label(kVtbl_BSShaderNoLightingProperty),
		Label(kVtbl_BSShaderLightingProperty),
		Label(kVtbl_BSShaderBloodSplatterProperty),
		Label(kVtbl_BSBatchRenderer),
		Label(kVtbl_LightingData),
		Label(kVtbl_ParticleShader),
		Label(kVtbl_ParticleShaderProperty),
		Label(kVtbl_SpeedTreeBillboardShaderProperty),
		Label(kVtbl_BSShaderProperty),
		Label(kVtbl_TallGrassShader),
		Label(kVtbl_SkyShaderProperty),
		Label(kVtbl_PrecipitationShader),
		Label(kVtbl_PrecipitationShaderProperty),
		Label(kVtbl_SpeedTreeLeafShader),
		Label(kVtbl_SpeedTreeBranchShaderProperty),
		Label(kVtbl_SpeedTreeShaderPPLightingProperty),
		Label(kVtbl_SpeedTreeShaderLightingProperty),
		Label(kVtbl_SpeedTreeLeafShaderProperty),
		Label(kVtbl_GeometryDecalShaderProperty),
		Label(kVtbl_Lighting30ShaderProperty),
		Label(kVtbl_BSDistantTreeShader),
		Label(kVtbl_BSDistantTreeShaderProperty),
		Label(kVtbl_TileShaderProperty),
		Label(kVtbl_BSFogProperty),
		Label(kVtbl_Lighting30Shader),
		Label(kVtbl_BoltShader),
		Label(kVtbl_BoltShaderProperty),
		Label(kVtbl_BSClearZNode),
		Label(kVtbl_BSOrderedNode),
		Label(kVtbl_BSMaterialEmittanceMultController),
		Label(kVtbl_BSRefractionFirePeriodController),
		Label(kVtbl_BSRefractionStrengthController),
		Label(kVtbl_VolumetricFogShaderProperty),
		Label(kVtbl_HairShaderProperty),
		Label(kVtbl_BeamShader),
		Label(kVtbl_VolumetricFogShader),
		Label(kVtbl_BSShaderNoLighting),
		Label(kVtbl_TileShader),
		Label(kVtbl_GeometryDecalShader),
		Label(kVtbl_ParallaxShader),
		Label(kVtbl_SkinShader),
		Label(kVtbl_HairShader),
		Label(kVtbl_SpeedTreeBranchShader),
		Label(kVtbl_DistantLODShader),
		Label(kVtbl_BSCubeMapCamera),
		Label(kVtbl_NiParticleSystem),
		Label(kVtbl_NiPSysEmitterCtlr),
		Label(kVtbl_NiPSysMeshEmitter),
		Label(kVtbl_NiPSysCylinderEmitter),
		Label(kVtbl_NiPSysSphereEmitter),
		Label(kVtbl_NiPSysBoxEmitter),
		Label(kVtbl_NiPSysRotationModifier),
		Label(kVtbl_NiPSysMeshUpdateModifier),
		Label(kVtbl_NiMeshParticleSystem),
		Label(kVtbl_NiPSysEmitter),
		Label(kVtbl_NiPSysGravityModifier),
		Label(kVtbl_NiPSysSpawnModifier),
		Label(kVtbl_NiPSysData),
		Label(kVtbl_BSStripParticleSystem),
		Label(kVtbl_NiPSysUpdateTask),
		Label(kVtbl_BSStripPSysData),
		Label(kVtbl_NiPSysVortexFieldModifier),
		Label(kVtbl_NiPSysUpdateCtlr),
		Label(kVtbl_NiPSysTurbulenceFieldModifier),
		Label(kVtbl_NiPSysSphericalCollider),
		Label(kVtbl_NiPSysResetOnLoopCtlr),
		Label(kVtbl_NiPSysRadialFieldModifier),
		Label(kVtbl_NiPSysPositionModifier),
		Label(kVtbl_NiPSysPlanarCollider),
		Label(kVtbl_NiPSysModifierActiveCtlr),
		Label(kVtbl_NiPSysInitialRotSpeedVarCtlr),
		Label(kVtbl_NiPSysInitialRotSpeedCtlr),
		Label(kVtbl_NiPSysInitialRotAngleVarCtlr),
		Label(kVtbl_NiPSysInitialRotAngleCtlr),
		Label(kVtbl_NiPSysGrowFadeModifier),
		Label(kVtbl_NiPSysGravityStrengthCtlr),
		Label(kVtbl_NiPSysGravityFieldModifier),
		Label(kVtbl_NiPSysFieldMaxDistanceCtlr),
		Label(kVtbl_NiPSysFieldMagnitudeCtlr),
		Label(kVtbl_NiPSysFieldAttenuationCtlr),
		Label(kVtbl_NiPSysEmitterSpeedCtlr),
		Label(kVtbl_NiPSysEmitterPlanarAngleVarCtlr),
		Label(kVtbl_NiPSysEmitterPlanarAngleCtlr),
		Label(kVtbl_NiPSysEmitterLifeSpanCtlr),
		Label(kVtbl_NiPSysEmitterInitialRadiusCtlr),
		Label(kVtbl_NiPSysEmitterDeclinationVarCtlr),
		Label(kVtbl_NiPSysEmitterDeclinationCtlr),
		Label(kVtbl_NiPSysEmitterCtlrData),
		Label(kVtbl_NiPSysDragModifier),
		Label(kVtbl_NiPSysDragFieldModifier),
		Label(kVtbl_NiPSysColorModifier),
		Label(kVtbl_NiPSysColliderManager),
		Label(kVtbl_NiPSysBoundUpdateModifier),
		Label(kVtbl_NiPSysBombModifier),
		Label(kVtbl_NiPSysAgeDeathModifier),
		Label(kVtbl_NiPSysAirFieldSpreadCtlr),
		Label(kVtbl_NiPSysAirFieldModifier),
		Label(kVtbl_NiPSysAirFieldInheritVelocityCtlr),
		Label(kVtbl_NiPSysAirFieldAirFrictionCtlr),
		Label(kVtbl_NiMeshPSysData),
		Label(kVtbl_NiPSysModifierCtlr),
		Label(kVtbl_NiPSysModifier),
		Label(kVtbl_NiPSysVolumeEmitter),
		Label(kVtbl_NiPSysFieldModifier),
		Label(kVtbl_NiPSysCollider),
		Label(kVtbl_NiPSysModifierBoolCtlr),
		Label(kVtbl_NiPSysModifierFloatCtlr),
		Label(kVtbl_BSMultiBound),
		Label(kVtbl_BSMultiBoundRoom),
		Label(kVtbl_BSReference),
		Label(kVtbl_BSNodeReferences),
		Label(kVtbl_MessageHandler),
		Label(kVtbl_QueuedChildren),
		Label(kVtbl_QueuedParents),
		Label(kVtbl_QueuedFile),
		Label(kVtbl_QueuedFileEntry),
		Label(kVtbl_IOManager),
		Label(kVtbl_BSTaskThread),
		Label(kVtbl_BSXFlags),
		Label(kVtbl_BSValueNode),
		Label(kVtbl_BSParallelTaskThread),
		Label(kVtbl_BSRangeNode),
		Label(kVtbl_BSWindModifier),
		Label(kVtbl_BSMultiBoundNode),
		Label(kVtbl_BSBlastNode),
		Label(kVtbl_BSDebrisNode),
		Label(kVtbl_BSDamageStage),
		Label(kVtbl_BSPortalSharedNode),
		Label(kVtbl_BSMultiBoundShape),
		Label(kVtbl_BSParticleSystemManager),
		Label(kVtbl_UpdateMasterPsysTaskletData),
		Label(kVtbl_BSSceneGraph),
		Label(kVtbl_NiBSBoneLODController),
		Label(kVtbl_BSFurnitureMarker),
		Label(kVtbl_BSBound),
		Label(kVtbl_BSPortalGraph),
		Label(kVtbl_BSPSysMultiTargetEmitterCtlr),
		Label(kVtbl_BSScissorTriShape),
		Label(kVtbl_BSMasterParticleSystem),
		Label(kVtbl_BSTextureData),
		Label(kVtbl_BSNiNode),
		Label(kVtbl_BSPSysStripUpdateModifier),
		Label(kVtbl_BSPSysSimpleColorModifier),
		Label(kVtbl_BSMultiBoundCapsule),
		Label(kVtbl_BSWArray),
		Label(kVtbl_PArrayPoint),
		Label(kVtbl_BSPSysArrayEmitter),
		Label(kVtbl_BSParentVelocityModifier),
		Label(kVtbl_BSDecalPlacementVectorExtraData),
		Label(kVtbl_BSAttachPoints),
		Label(kVtbl_bhkMemoryAllocator),
		Label(kVtbl_hkMemoryAllocator),
		Label(kVtbl_bhkOnStartupCallback),
		Label(kVtbl_bhkOnExitCallback),
		Label(kVtbl_hkpCollidableCollidableFilter),
		Label(kVtbl_hkpRayCollidableFilter),
		Label(kVtbl_hkpShapeCollectionFilter),
		Label(kVtbl_hkpRayShapeCollectionFilter),
		Label(kVtbl_hkpBroadPhaseCastCollector),
		Label(kVtbl_ahkpWorld),
		Label(kVtbl_hkpWorldRayCaster),
		Label(kVtbl_bhkCollisionFilter),
		Label(kVtbl_bhkWorldCinfo),
		Label(kVtbl_bhkWorld),
		Label(kVtbl_bhkNiCollisionObject),
		Label(kVtbl_hkpCharacterProxyListener),
		Label(kVtbl_bhkCharacterListener),
		Label(kVtbl_bhkShapeCollection),
		Label(kVtbl_bhkCharacterProxy),
		Label(kVtbl_bhkCachingShapePhantom),
		Label(kVtbl_hkpClosestCdPointCollector),
		Label(kVtbl_bhkCollisionBox),
		Label(kVtbl_bhkListShape),
		Label(kVtbl_bhkCharacterStateClimbing),
		Label(kVtbl_bhkCharacterStateFlying),
		Label(kVtbl_bhkCharacterController),
		Label(kVtbl_bhkWindListener),
		Label(kVtbl_hkaRaycastInterface),
		Label(kVtbl_bhkRagdollShareData),
		Label(kVtbl_bhkRagdollController),
		Label(kVtbl_hkaSkeleton),
		Label(kVtbl_bhkStiffSpringConstraint),
		Label(kVtbl_hkStiffSpringConstraintCinfo),
		Label(kVtbl_bhkBlendCollisionObject),
		Label(kVtbl_hkpMotion),
		Label(kVtbl_bhkRefObject),
		Label(kVtbl_WeaponObject),
		Label(kVtbl_bhkLimitedHingeConstraint),
		Label(kVtbl_hkLimitedHingeConstraintCinfo),
		Label(kVtbl_bhkBinaryAction),
		Label(kVtbl_bhkBlendCollisionObjectAddRotation),
		Label(kVtbl_hkReferencedObjectLock),
		Label(kVtbl_hkpNullContactMgr),
		Label(kVtbl_hkpBroadPhaseListener),
		Label(kVtbl_hkpDefaultConvexListFilter),
		Label(kVtbl_hkpPhantomBroadPhaseListener),
		Label(kVtbl_hkpBroadPhaseBorderListener),
		Label(kVtbl_hkpNullContactMgrFactory),
		Label(kVtbl_hkpNullCollisionFilter),
		Label(kVtbl_hkpSimpleWorldRayCaster),
		Label(kVtbl_hkpWorldLinearCaster),
		Label(kVtbl_hkpWorld),
		Label(kVtbl_hkDebugDisplay),
		Label(kVtbl_bhkWorldM),
		Label(kVtbl_bhkBlendController),
		Label(kVtbl_hkpKeyframedRigidMotion),
		Label(kVtbl_hkpMaxSizeMotion),
		Label(kVtbl_hkpFixedRigidMotion),
		Label(kVtbl_hkpSphereMotion),
		Label(kVtbl_hkpBoxMotion),
		Label(kVtbl_hkpThinBoxMotion),
		Label(kVtbl_hkpCharacterMotion),
		Label(kVtbl_hkpWorldObject),
		Label(kVtbl_hkpEntity),
		Label(kVtbl_hkpShapeContainer),
		Label(kVtbl_hkpSingleShapeContainer),
		Label(kVtbl_hkpBvShape),
		Label(kVtbl_hkpBoxShape),
		Label(kVtbl_hkpConvexTransformShapeBase),
		Label(kVtbl_hkpPhantom),
		Label(kVtbl_hkpShapePhantom),
		Label(kVtbl_hkaRagdollRaycastInterface),
		Label(kVtbl_bhkRagdollPenetrationUtil),
		Label(kVtbl_hkPackedNiTriStripsData),
		Label(kVtbl_hkpTriangleShape),
		Label(kVtbl_hkNormalTriangleShape),
		Label(kVtbl_hkPackedNiTriStripsShape),
		Label(kVtbl_hkNiTriStripsShape),
		Label(kVtbl_bhkPackedNiTriStripsShape),
		Label(kVtbl_bhkNiTriStripsShape),
		Label(kVtbl_hkpRigidBody),
		Label(kVtbl_hkpCharacterContext),
		Label(kVtbl_bhkMalleableConstraint),
		Label(kVtbl_hkMalleableConstraintCinfo),
		Label(kVtbl_hkpMalleableConstraintData),
		Label(kVtbl_ahkpMalleableConstraintData),
		Label(kVtbl_ahkpCharacterProxy),
		Label(kVtbl_hkpCharacterProxy),
		Label(kVtbl_hkBSHeightFieldShape),
		Label(kVtbl_hkErrStream),
		Label(kVtbl_bhkRigidBodyT),
		Label(kVtbl_bhkWaterListener),
		Label(kVtbl_bhkOrientHingedBodyAction),
		Label(kVtbl_hkpLimitedHingeConstraintData),
		Label(kVtbl_bhkForceController),
		Label(kVtbl_bhkBallAndSocketConstraint),
		Label(kVtbl_bhkRagdollConstraint),
		Label(kVtbl_hkBallAndSocketConstraintCinfo),
		Label(kVtbl_hkRagdollConstraintCinfo),
		Label(kVtbl_bhkRagdollTemplateData),
		Label(kVtbl_bhkRagdollTemplate),
		Label(kVtbl_hkpMouseSpringAction),
		Label(kVtbl_hkNativeFileSystem),
		Label(kVtbl_hkDummySingleton),
		Label(kVtbl_hkMemorySystem),
		Label(kVtbl_hkpCharacterStateManager),
		Label(kVtbl_hkpHingeConstraintData),
		Label(kVtbl_hkpPhantomOverlapListener),
		Label(kVtbl_hkpCollisionFilter),
		Label(kVtbl_hkOstream),
		Label(kVtbl_hkMemoryTrackStreamWriter),
		Label(kVtbl_hkArrayStreamWriter),
		Label(kVtbl_bhkPoseArray),
		Label(kVtbl_bhkExtraData),
		Label(kVtbl_bhkSpringAction),
		Label(kVtbl_bhkMotorAction),
		Label(kVtbl_bhkDashpotAction),
		Label(kVtbl_bhkAngularDashpotAction),
		Label(kVtbl_bhkBreakableConstraint),
		Label(kVtbl_ahkpBreakableConstraintData),
		Label(kVtbl_hkBreakableConstraintCinfo),
		Label(kVtbl_bhkWheelConstraint),
		Label(kVtbl_hkWheelConstraintCinfo),
		Label(kVtbl_bhkRagdollLimitsConstraint),
		Label(kVtbl_hkRagdollLimitsConstraintCinfo),
		Label(kVtbl_bhkPrismaticConstraint),
		Label(kVtbl_hkPrismaticConstraintCinfo),
		Label(kVtbl_bhkFixedConstraint),
		Label(kVtbl_bhkGenericConstraint),
		Label(kVtbl_hkGenericConstraintCinfo),
		Label(kVtbl_hkFixedConstraintCinfo),
		Label(kVtbl_bhkHingeLimitsConstraint),
		Label(kVtbl_hkHingeLimitsConstraintCinfo),
		Label(kVtbl_bhkBallSocketConstraintChain),
		Label(kVtbl_bhkConstraintChain),
		Label(kVtbl_hkBallSocketChainCinfo),
		Label(kVtbl_bhkMoppBvTreeShape),
		Label(kVtbl_hkScaledMoppBvTreeShape),
		Label(kVtbl_bhkTriangleShape),
		Label(kVtbl_bhkPlaneShape),
		Label(kVtbl_bhkHeightFieldShape),
		Label(kVtbl_bhkExtendedMeshShapeData),
		Label(kVtbl_bhkExtendedMeshShape),
		Label(kVtbl_bhkCylinderShape),
		Label(kVtbl_bhkConvexVerticesShape),
		Label(kVtbl_bhkConvexTransformShape),
		Label(kVtbl_bhkConvexSweepShape),
		Label(kVtbl_bhkConvexListShape),
		Label(kVtbl_hkThreadMemory),
		Label(kVtbl_hkMemoryBlockServer),
		Label(kVtbl_hkLeakDetectAllocator),
		Label(kVtbl_hkFreeListMemorySystem),
		Label(kVtbl_hkVisualDebugger),
		Label(kVtbl_hkpWorldDeletionListener),
		Label(kVtbl_hkpActionListener),
		Label(kVtbl_hkpConstraintListener),
		Label(kVtbl_hkProcessContext),
		Label(kVtbl_hkpPhysicsContext),
		Label(kVtbl_bhkCharacterPointCollector),
		Label(kVtbl_bhkCharacterStateProjectile),
		Label(kVtbl_bhkCharacterStateInAir),
		Label(kVtbl_bhkCharacterStateJumping),
		Label(kVtbl_bhkCharacterStateSwimming),
		Label(kVtbl_bhkCharacterStateOnGround),
		Label(kVtbl_hkCharControllerShape),
		Label(kVtbl_bhkCharControllerShape),
		Label(kVtbl_hkpSphereShape),
		Label(kVtbl_hkaFootPlacementIkSolver),
		Label(kVtbl_hkpRagdollConstraintData),
		Label(kVtbl_hkpConstraintMotor),
		Label(kVtbl_hkpLimitedForceConstraintMotor),
		Label(kVtbl_hkpPositionConstraintMotor),
		Label(kVtbl_hkaRagdollInstance),
		Label(kVtbl_hkaSkeletonMapper),
		Label(kVtbl_hkpAabbPhantom),
		Label(kVtbl_hkpConstraintInstance),
		Label(kVtbl_bhkPositionConstraintMotor),
		Label(kVtbl_hkpVelocityConstraintMotor),
		Label(kVtbl_bhkVelocityConstraintMotor),
		Label(kVtbl_bhkSpringDamperConstraintMotor),
		Label(kVtbl_hkPointToPathConstraintCinfo),
		Label(kVtbl_hkpAction),
		Label(kVtbl_hkpBinaryAction),
		Label(kVtbl_hkpShapeCollection),
		Label(kVtbl_hkpListShape),
		Label(kVtbl_hkpTransformShape),
		Label(kVtbl_hkpNullBroadPhaseListener),
		Label(kVtbl_hkpSimulation),
		Label(kVtbl_hkpCollisionDispatcher),
		Label(kVtbl_hkpReportContactMgr),
		Label(kVtbl_hkpSimpleContactConstraintData),
		Label(kVtbl_hkpSimpleConstraintContactMgr),
		Label(kVtbl_hkpMultiThreadedSimulation),
		Label(kVtbl_hkpPhysicsSystem),
		Label(kVtbl_hkStatisticsCollector),
		Label(kVtbl_hkpConstraintOwner),
		Label(kVtbl_hkpSimulationIsland),
		Label(kVtbl_hkpWorldPostSimulationListener),
		Label(kVtbl_hkKdTreeCastCollector),
		Label(kVtbl_hkpKdTreeCollidableRaycaster),
		Label(kVtbl_hkpKdTreeWorldLinearCaster),
		Label(kVtbl_hkpAabbTreeWorldManager),
		Label(kVtbl_hkpKdTreeWorldManager),
		Label(kVtbl_hkpCollisionCallbackUtil),
		Label(kVtbl_hkpDefaultWorldMaintenanceMgr),
		Label(kVtbl_hkpBroadPhaseBorder),
		Label(kVtbl_hkpContinuousSimulation),
		Label(kVtbl_hkpNullAgent),
		Label(kVtbl_hkpEntityEntityBroadPhaseListener),
		Label(kVtbl_hkp3AxisSweep),
		Label(kVtbl_hkpSimpleShapePhantom),
		Label(kVtbl_hkaDetectRagdollPenetration),
		Label(kVtbl_hkpCapsuleShape),
		Label(kVtbl_hkDisplayGeometry),
		Label(kVtbl_hkDisplayCapsule),
		Label(kVtbl_hkpSampledHeightFieldShape),
		Label(kVtbl_hkpTriSampledHeightFieldCollection),
		Label(kVtbl_hkpBvTreeShape),
		Label(kVtbl_hkpTriSampledHeightFieldBvTreeShape),
		Label(kVtbl_hkBufferedStreamWriter),
		Label(kVtbl_hkpMultiSphereShape),
		Label(kVtbl_hkOrientHingedBodyAction),
		Label(kVtbl_hkpUnaryAction),
		Label(kVtbl_hkDefaultError),
		Label(kVtbl_hkSocket),
		Label(kVtbl_hkBufferedStreamReader),
		Label(kVtbl_hkStdioStreamReader),
		Label(kVtbl_hkStdioStreamWriter),
		Label(kVtbl_hkLiquidAction),
		Label(kVtbl_hkCpuJobThreadPool),
		Label(kVtbl_hkpSymmetricAgentFlipCollector),
		Label(kVtbl_hkpSymmetricAgentFlipCastCollector),
		Label(kVtbl_hkpSymmetricAgentFlipBodyCollector),
		Label(kVtbl_hkpBvTreeStreamAgent),
		Label(kVtbl_hkpBvTreeAgent),
		Label(kVtbl_hkpMoppAgent),
		Label(kVtbl_hkHeightFieldRayForwardingCollector),
		Label(kVtbl_hkpHeightFieldAgent),
		Label(kVtbl_hkpMapPointsToSubShapeContactMgr),
		Label(kVtbl_hkpPredGskfAgent),
		Label(kVtbl_hkpFlagCdBodyPairCollector),
		Label(kVtbl_hkpSimpleClosestContactCollector),
		Label(kVtbl_hkpConvexListAgent),
		Label(kVtbl_hkpListAgent),
		Label(kVtbl_hkpMultiRayConvexAgent),
		Label(kVtbl_hkpMultiSphereTriangleAgent),
		Label(kVtbl_hkpCapsuleTriangleAgent),
		Label(kVtbl_hkpCapsuleCapsuleAgent),
		Label(kVtbl_hkpSphereBoxAgent),
		Label(kVtbl_hkpSphereTriangleAgent),
		Label(kVtbl_hkpSphereCapsuleAgent),
		Label(kVtbl_hkpSphereSphereAgent),
		Label(kVtbl_hkpBoxBoxAgent),
		Label(kVtbl_hkpPhantomAgent),
		Label(kVtbl_hkpTransformAgent),
		Label(kVtbl_hkpShapeCollectionAgent),
		Label(kVtbl_hkpMultiSphereAgent),
		Label(kVtbl_hkpBvAgent),
		Label(kVtbl_hkMemoryTrackStreamReader),
		Label(kVtbl_hkDefaultBuiltinTypeRegistry),
		Label(kVtbl_hkTypeInfoRegistry),
		Label(kVtbl_hkDynamicClassNameRegistry),
		Label(kVtbl_hkDefaultClassNameRegistry),
		Label(kVtbl_hkVtableClassRegistry),
		Label(kVtbl_hkpPhysicsData),
		Label(kVtbl_hkBinaryTagfileWriter),
		Label(kVtbl_hkpStorageSampledHeightFieldShape),
		Label(kVtbl_hkpStorageExtendedMeshShape),
		Label(kVtbl_hkpStorageMeshShape),
		Label(kVtbl_hkpPhysicsSystemWithContacts),
		Label(kVtbl_hkpCachingShapePhantom),
		Label(kVtbl_hkpSpringAction),
		Label(kVtbl_hkpMotorAction),
		Label(kVtbl_hkpDashpotAction),
		Label(kVtbl_hkpAngularDashpotAction),
		Label(kVtbl_hkpBreakableConstraintData),
		Label(kVtbl_hkpWheelConstraintData),
		Label(kVtbl_hkpStiffSpringConstraintData),
		Label(kVtbl_hkpRagdollLimitsData),
		Label(kVtbl_hkpPrismaticConstraintData),
		Label(kVtbl_hkpGenericConstraintData),
		Label(kVtbl_hkpHingeLimitsData),
		Label(kVtbl_hkpBallSocketChainData),
		Label(kVtbl_hkpBallAndSocketConstraintData),
		Label(kVtbl_hkMoppBvTreeShapeBase),
		Label(kVtbl_hkpMoppBvTreeShape),
		Label(kVtbl_hkpPlaneShape),
		Label(kVtbl_hkpConvexTranslateShape),
		Label(kVtbl_hkpExtendedMeshShape),
		Label(kVtbl_hkpCylinderShape),
		Label(kVtbl_hkDisplayCylinder),
		Label(kVtbl_hkpConvexVerticesShape),
		Label(kVtbl_hkpConvexTransformShape),
		Label(kVtbl_hkpConvexListShape),
		Label(kVtbl_hkSolverAllocator),
		Label(kVtbl_hkFreeListAllocator),
		Label(kVtbl_hkSystemMemoryBlockServer),
		Label(kVtbl_hkOArchive),
		Label(kVtbl_hkProcess),
		Label(kVtbl_hkProcessHandler),
		Label(kVtbl_hkCommandRouter),
		Label(kVtbl_hkServerProcessHandler),
		Label(kVtbl_hkpWeldingViewer),
		Label(kVtbl_hkpWorldSnapshotViewer),
		Label(kVtbl_hkpWorldMemoryViewer),
		Label(kVtbl_hkpVehicleViewer),
		Label(kVtbl_hkpSweptTransformDisplayViewer),
		Label(kVtbl_hkpSimulationIslandViewer),
		Label(kVtbl_hkpShapeDisplayViewer),
		Label(kVtbl_hkpPhantomDisplayViewer),
		Label(kVtbl_hkpUserShapePhantomTypeIdentifier),
		Label(kVtbl_hkpMousePickingViewer),
		Label(kVtbl_hkpRigidBodyInertiaViewer),
		Label(kVtbl_hkpToiCountViewer),
		Label(kVtbl_hkpToiContactPointViewer),
		Label(kVtbl_hkpInactiveContactPointViewer),
		Label(kVtbl_hkpActiveContactPointViewer),
		Label(kVtbl_hkpConvexRadiusViewer),
		Label(kVtbl_hkpConstraintViewer),
		Label(kVtbl_hkpRigidBodyCentreOfMassViewer),
		Label(kVtbl_hkpMidphaseViewer),
		Label(kVtbl_hkpBroadphaseViewer),
		Label(kVtbl_hkgpConvexHull),
		Label(kVtbl_hkpPointToPlaneConstraintData),
		Label(kVtbl_hkpSpringDamperConstraintMotor),
		Label(kVtbl_hkTraceStream),
		Label(kVtbl_hkpConstraintCollisionFilter),
		Label(kVtbl_hkpDeferredConstraintOwner),
		Label(kVtbl_hkpBroadPhase),
		Label(kVtbl_hkKdTreeBundleCastCollector),
		Label(kVtbl_hkKdTreeCollidableRayBundleCaster),
		Label(kVtbl_hkpAabbTreeCollidableRaycaster),
		Label(kVtbl_hkpCollidableMediator),
		Label(kVtbl_hkpEndOfStepCallbackUtil),
		Label(kVtbl_hkpDefaultToiResourceMgr),
		Label(kVtbl_hkLocalFrameGroup),
		Label(kVtbl_hkSimpleLocalFrame),
		Label(kVtbl_hkStackTracer),
		Label(kVtbl_hkBsdSocket),
		Label(kVtbl_hkpGskBaseAgent),
		Label(kVtbl_hkpGskfAgent),
		Label(kVtbl_hkCrc32StreamWriter),
		Label(kVtbl_hkStaticClassNameRegistry),
		Label(kVtbl_hkVersionPatchManager),
		Label(kVtbl_hkSerializeDeprecated),
		Label(kVtbl_hkDataRefCounted),
		Label(kVtbl_hkDataClassNative),
		Label(kVtbl_hkDataObjectNative),
		Label(kVtbl_hkDataArrayNative),
		Label(kVtbl_hkDataWorldNative),
		Label(kVtbl_hkBinaryPackfileWriter),
		Label(kVtbl_hkSubStreamWriter),
		Label(kVtbl_hkPackfileWriter),
		Label(kVtbl_hkBinaryTagfileReader),
		Label(kVtbl_ArrayOfTuplesImplementation),
		Label(kVtbl_hkDataObjectDict),
		Label(kVtbl_VecArrayImplementation),
		Label(kVtbl_PointerArrayImplementation),
		Label(kVtbl_ByteArrayImplementation),
		Label(kVtbl_IntArrayImplementation),
		Label(kVtbl_RealArrayImplementation),
		Label(kVtbl_CstringArrayImplementation),
		Label(kVtbl_hkDataClassDict),
		Label(kVtbl_StructArrayImplementation),
		Label(kVtbl_hkDataWorldDict),
		Label(kVtbl_hkIstream),
		Label(kVtbl_hkpMeshShape),
		Label(kVtbl_hkpSerializedAgentNnEntry),
		Label(kVtbl_hkpConstraintChainInstanceAction),
		Label(kVtbl_hkpConstraintChainInstance),
		Label(kVtbl_hkpMoppCostFunction),
		Label(kVtbl_hkpMoppShapeMediator),
		Label(kVtbl_hkpMoppCachedShapeMediator),
		Label(kVtbl_hkFreeListAllocatorServer),
		Label(kVtbl_hkLargeBlockAllocator),
		Label(kVtbl_hkWindowsSystemClock),
		Label(kVtbl_hkIArchive),
		Label(kVtbl_hkProcessFactory),
		Label(kVtbl_hkDebugDisplayHandler),
		Label(kVtbl_hkServerDebugDisplayHandler),
		Label(kVtbl_hkDisplaySerializeOStream),
		Label(kVtbl_hkDisplaySerializeIStream),
		Label(kVtbl_hkInspectProcess),
		Label(kVtbl_hkStatisticsProcess),
		Label(kVtbl_hkDebugDisplayProcess),
		Label(kVtbl_hkpPhysicsContextWorldListener),
		Label(kVtbl_hkpWorldViewerBase),
		Label(kVtbl_hkStreamStatisticsCollector),
		Label(kVtbl_hkpShapeDisplayBuilder),
		Label(kVtbl_hkpUserShapeDisplayBuilder),
		Label(kVtbl_hkDisplayAABB),
		Label(kVtbl_hkDisplayConvex),
		Label(kVtbl_hkDisplayBox),
		Label(kVtbl_hkpContactPointViewer),
		Label(kVtbl_hkpConvexRadiusBuilder),
		Label(kVtbl_hkpUserConvexRadiusBuilder),
		Label(kVtbl_hkDisplaySemiCircle),
		Label(kVtbl_hkDisplayPlane),
		Label(kVtbl_hkDisplayCone),
		Label(kVtbl_hkpPointToPathConstraintData),
		Label(kVtbl_hkpPulleyConstraintData),
		Label(kVtbl_hkpPairCollisionFilter),
		Label(kVtbl_hkCpuWorldGetClosestPointsCollector),
		Label(kVtbl_hkCpuWorldLinearCastCollector),
		Label(kVtbl_hkCpuWorldRayCastCollector),
		Label(kVtbl_hkpFixedBufferRayHitCollector),
		Label(kVtbl_hkaAnimation),
		Label(kVtbl_hkaWaveletCompressedAnimation),
		Label(kVtbl_hkaSplineCompressedAnimation),
		Label(kVtbl_hkaDeltaCompressedAnimation),
		Label(kVtbl_hkpRejectChassisListener),
		Label(kVtbl_hkpVehicleWheelCollide),
		Label(kVtbl_hkpVehicleLinearCastWheelCollide),
		Label(kVtbl_hkpVehicleInstance),
		Label(kVtbl_hkpVehicleDefaultSuspension),
		Label(kVtbl_hkpVehicleDefaultBrake),
		Label(kVtbl_hkpVehicleData),
		Label(kVtbl_hkpTyremarksWheel),
		Label(kVtbl_hkpTyremarksInfo),
		Label(kVtbl_hkpSerializedDisplayRbTransforms),
		Label(kVtbl_hkpPoweredChainMapper),
		Label(kVtbl_hkpRotationalConstraintData),
		Label(kVtbl_hkpSimpleMeshShape),
		Label(kVtbl_hkpMultiRayShape),
		Label(kVtbl_hkxAttributeHolder),
		Label(kVtbl_hkxNode),
		Label(kVtbl_hkxMaterial),
		Label(kVtbl_hkxSparselyAnimatedInt),
		Label(kVtbl_hkxSparselyAnimatedEnum),
		Label(kVtbl_hkxEnum),
		Label(kVtbl_hkMonitorStreamColorTable),
		Label(kVtbl_hkaInterleavedUncompressedAnimation),
		Label(kVtbl_hkaFootstepAnalysisInfoContainer),
		Label(kVtbl_hkaDefaultAnimatedReferenceFrame),
		Label(kVtbl_hkaBoneAttachment),
		Label(kVtbl_hkaAnimationPreviewColorContainer),
		Label(kVtbl_hkaAnimationContainer),
		Label(kVtbl_hkaAnimationBinding),
		Label(kVtbl_hkpVehicleRayCastWheelCollide),
		Label(kVtbl_hkpVehicleManager),
		Label(kVtbl_hkpVehicleRayCastBatchingManager),
		Label(kVtbl_hkpVehicleLinearCastBatchingManager),
		Label(kVtbl_hkpVehicleDriverInputAnalogStatus),
		Label(kVtbl_hkpVehicleDefaultAnalogDriverInput),
		Label(kVtbl_hkpVehicleDefaultVelocityDamper),
		Label(kVtbl_hkpVehicleDefaultTransmission),
		Label(kVtbl_hkpVehicleDefaultSteering),
		Label(kVtbl_hkpVehicleDefaultEngine),
		Label(kVtbl_hkpVehicleDefaultAerodynamics),
		Label(kVtbl_hkpSerializedDisplayMarkerList),
		Label(kVtbl_hkpSerializedDisplayMarker),
		Label(kVtbl_hkpReorientAction),
		Label(kVtbl_hkpGroupCollisionFilter),
		Label(kVtbl_hkpDisplayBindingData),
		Label(kVtbl_hkpDisableEntityCollisionFilter),
		Label(kVtbl_hkpConstrainedSystemFilter),
		Label(kVtbl_hkpConvexPieceStreamData),
		Label(kVtbl_hkpLinearParametricCurve),
		Label(kVtbl_hkpDefaultWorldMemoryWatchDog),
		Label(kVtbl_hkpCallbackConstraintMotor),
		Label(kVtbl_hkpShapeInfo),
		Label(kVtbl_hkpMoppModifier),
		Label(kVtbl_hkpRemoveTerminalsMoppModifier),
		Label(kVtbl_hkpGroupFilter),
		Label(kVtbl_hkpFastMeshShape),
		Label(kVtbl_hkpConvexVerticesConnectivity),
		Label(kVtbl_hkpConvexPieceMeshShape),
		Label(kVtbl_hkpCompressedSampledHeightFieldShape),
		Label(kVtbl_hkpCollisionFilterList),
		Label(kVtbl_hkMemoryMeshShape),
		Label(kVtbl_hkxVertexVectorDataChannel),
		Label(kVtbl_hkxVertexSelectionChannel),
		Label(kVtbl_hkxVertexIntDataChannel),
		Label(kVtbl_hkxVertexFloatDataChannel),
		Label(kVtbl_hkxTriangleSelectionChannel),
		Label(kVtbl_hkxTextureInplace),
		Label(kVtbl_hkxTextureFile),
		Label(kVtbl_hkxSparselyAnimatedString),
		Label(kVtbl_hkxSparselyAnimatedBool),
		Label(kVtbl_hkxSkinBinding),
		Label(kVtbl_hkxScene),
		Label(kVtbl_hkxNodeSelectionSet),
		Label(kVtbl_hkxMeshSection),
		Label(kVtbl_hkxMaterialShader),
		Label(kVtbl_hkxMaterialEffect),
		Label(kVtbl_hkxLight),
		Label(kVtbl_hkxIndexBuffer),
		Label(kVtbl_hkxEdgeSelectionChannel),
		Label(kVtbl_hkxCamera),
		Label(kVtbl_hkxAnimatedVector),
		Label(kVtbl_hkxAnimatedQuaternion),
		Label(kVtbl_hkxAnimatedMatrix),
		Label(kVtbl_hkxAnimatedFloat),
		Label(kVtbl_hkPlatformObjectWriter),
		Label(kVtbl_hkOffsetOnlyStreamWriter),
		Label(kVtbl_hkObjectCopier),
		Label(kVtbl_hkObjectResource),
		Label(kVtbl_hkPackfileData),
		Label(kVtbl_hkMemoryStreamReader),
		Label(kVtbl_hkpMoppCodeGenerator),
		Label(kVtbl_hkpMoppDefaultAssembler),
		Label(kVtbl_hkpMoppDefaultSplitter),
		Label(kVtbl_hkDisplaySphere),
		Label(kVtbl_IConvexOverlapImpl),
		Label(kVtbl_hkGskShape),
		Label(kVtbl_hkgpMesh),
		Label(kVtbl_hkResourceMap),
		Label(kVtbl_hkMemoryResourceHandle),
		Label(kVtbl_hkMemoryResourceContainer),
		Label(kVtbl_hkContainerResourceMap),
		Label(kVtbl_hkaMeshBinding),
		Label(kVtbl_hkpStiffSpringChainData),
		Label(kVtbl_hkpPoweredChainData),
		Label(kVtbl_hkpCompressedMeshShape),
		Label(kVtbl_hkxVertexBuffer),
		Label(kVtbl_hkxEnvironment),
		Label(kVtbl_hkaFootstepAnalysisInfo),
		Label(kVtbl_hkpGunProjectile),
		Label(kVtbl_hkpProjectileGun),
		Label(kVtbl_hkpMountedBallGun),
		Label(kVtbl_hkpGravityGun),
		Label(kVtbl_hkpFirstPersonGun),
		Label(kVtbl_hkpBallGun),
		Label(kVtbl_hkpRemoveTerminalsMoppModifier2),
		Label(kVtbl_hkxMesh),
		Label(kVtbl_hkxMaterialShaderSet),
		Label(kVtbl_hkAlignSceneToNodeOptions),
		Label(kVtbl_hkKdTreeCpuBroadphaseBundleCastCollector),
		Label(kVtbl_hkpConvexPieceShape),
		Label(kVtbl_NiDX9SourceTextureData),
		Label(kVtbl_NiDX9Renderer),
		Label(kVtbl_NiDX9ShaderDeclaration),
		Label(kVtbl_NiD3DShaderDeclaration),
		Label(kVtbl_NiD3DDefaultShader),
		Label(kVtbl_NiD3DShader),
		Label(kVtbl_NiDX92DBufferData),
		Label(kVtbl_NiDX9TextureBufferData),
		Label(kVtbl_NiDX9ImplicitBufferData),
		Label(kVtbl_NiDX9ImplicitDepthStencilBufferData),
		Label(kVtbl_NiDX9AdditionalDepthStencilBufferData),
		Label(kVtbl_NiD3DPass),
		Label(kVtbl_NiD3DShaderInterface),
		Label(kVtbl_NiShaderConstantMapEntry),
		Label(kVtbl_NiD3DShaderConstantMap),
		Label(kVtbl_NiD3DRenderState),
		Label(kVtbl_NiD3DGeometryGroupManager),
		Label(kVtbl_NiD3DShaderProgram),
		Label(kVtbl_NiD3DPixelShader),
		Label(kVtbl_NiD3DVertexShader),
		Label(kVtbl_NiDX9TextureData),
		Label(kVtbl_NiDX9SourceCubeMapData),
		Label(kVtbl_NiDX9VertexBufferManager),
		Label(kVtbl_NiFragmentShaderInstanceDescriptor),
		Label(kVtbl_NiDX9FragmentShader),
		Label(kVtbl_NiD3DGPUProgramCache),
		Label(kVtbl_NiGeometryBufferData),
		Label(kVtbl_NiDX9IndexBufferManager),
		Label(kVtbl_NiDX9RenderedTextureData),
		Label(kVtbl_NiDX9RenderedCubeMapData),
		Label(kVtbl_NiDX9DynamicTextureData),
		Label(kVtbl_NiDX9ErrorShader),
		Label(kVtbl_NiDX9TextureManager),
		Label(kVtbl_NiDX9RenderState),
		Label(kVtbl_NiDX9PersistentSrcTextureRendererData),
		Label(kVtbl_NiD3DShaderProgramCreator),
		Label(kVtbl_NiD3DShaderProgramCreatorHLSL),
		Label(kVtbl_NiD3DShaderProgramCreatorAsm),
		Label(kVtbl_NiD3DShaderProgramCreatorObj),
		Label(kVtbl_NiD3DShaderFactory),
		Label(kVtbl_NiD3DShaderProgramFactory),
		Label(kVtbl_NiStaticGeometryGroup),
		Label(kVtbl_NiUnsharedGeometryGroup),
		Label(kVtbl_NiDynamicGeometryGroup),
		Label(kVtbl_NiD3DHLSLPixelShader),
		Label(kVtbl_NiD3DHLSLVertexShader),
		Label(kVtbl_NiDX9ShaderConstantManager),
		Label(kVtbl_NiGeometryGroup),
		Label(kVtbl_NiD3DShaderConstantManager),
		Label(kVtbl_NiBoundingVolume),
		Label(kVtbl_NiHalfSpaceBV),
		Label(kVtbl_NiUnionBV),
		Label(kVtbl_NiCapsuleBV),
		Label(kVtbl_NiBoxBV),
		Label(kVtbl_NiSphereBV),
		Label(kVtbl_NiCollisionData),
		Label(kVtbl_NiIntersector),
		Label(kVtbl_NiCapsuleSphereIntersector),
		Label(kVtbl_NiCapsuleCapsuleIntersector),
		Label(kVtbl_NiCapsuleTriIntersector),
		Label(kVtbl_NiBoxCapsuleIntersector),
		Label(kVtbl_NiBoxSphereIntersector),
		Label(kVtbl_NiSphereTriIntersector),
		Label(kVtbl_NiOBBRoot),
		Label(kVtbl_NiOBBNode),
		Label(kVtbl_NiOBBLeaf),
		Label(kVtbl_MoviePlayer),
		Label(kVtbl_NiRenderedCubeMap),
		Label(kVtbl_NiRenderTargetGroup),
		Label(kVtbl_NiShaderDeclaration),
		Label(kVtbl_NiSCMExtraData),
		Label(kVtbl_NiGPUProgramCache),
		Label(kVtbl_NiMaterialResource),
		Label(kVtbl_StartMenuOption),
		Label(kVtbl_StartMenuUserOption),
		Label(kVtbl_ShadowSceneNode)

	};

	std::string GetStringForVTBL(void* ptr, UInt32 vtbl)
	{
		for (const auto& iter : setOfLabels)
		{
			if (iter.Satisfies(ptr)) return iter.Get(ptr);
		}

		switch (vtbl)
		{
		case 0x1094D9C:				return "Tile::MenuStringMap";
		case 0x10472D0:				return "BSSimpleArray<char const *>";
		case 0x10472CC:				return "BSSimpleArray<char const *>";
		case 0x106EDEC:				return "BSSimpleArray<Tile::Value*>";
		case 0x106CC64:				return "BSSimpleArray<float>";

		case 0x010C1740:			return "BSTaskManagerThread<__int64>";
		case 0x01094E7C:			return "NiTPointerMap<Tile::Value*, Tile::Reaction*>";

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

		default: break;
		}
		if (vtbl > 0x1000000 && vtbl < 0x1200000)
			return FormatString("¯\\_(Ö)_/¯: 0x%08X", vtbl);

		return "";
	}
}

// take a look inside
namespace CrashLogger::YvilesMagicBox
{

	UInt32 DereferenceExcept(UInt32 ptr)
	{
		__try { UInt32 vtbl = *(UInt32*)(ptr); return vtbl; }
		__except (EXCEPTION_EXECUTE_HANDLER) { return 0; }
	}

	UInt32 DereferenceCatch(UInt32 ptr)
	{
		try { return DereferenceExcept(ptr); }
		catch (...) { return 0; }
	}


	std::string DecryptPtr(UInt32 ptr, UInt32 depth = 5)
	{
		try
		{
			if (!ptr) return "";

			UInt32 vtbl;

			std::string full;

			while (vtbl = DereferenceCatch(ptr))
			{
				if (std::string onetime = NVVtables::GetStringForVTBL((void*)ptr, vtbl); !onetime.empty()) return full + onetime;

				if (depth == 0) break;

				depth--;

				full += FormatString("NOT_VTBL: 0x%08X; ", vtbl);

				ptr = vtbl;
			}

			return "";

		}
		catch (...)
		{
			return "";
		}
	}
}

namespace CrashLogger::Calltrace
{
	void Get(EXCEPTION_POINTERS* info, HANDLE process, HANDLE thread) {

		Log() << FormatString("Exception %08X caught!", info->ExceptionRecord->ExceptionCode);
		Log() << FormatString("\nCalltrace: ");

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

}

namespace CrashLogger::Registry
{
	void Get(EXCEPTION_POINTERS* info)
	{
		Log() << FormatString("\nRegistry: ");
		Log() << FormatString("REG |    VALUE   | DEREFERENCE INFO");
		Log() << FormatString("%s | 0x%08X | %s ", "eax", info->ContextRecord->Eax, YvilesMagicBox::DecryptPtr(info->ContextRecord->Eax).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "ebx", info->ContextRecord->Ebx, YvilesMagicBox::DecryptPtr(info->ContextRecord->Ebx).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "ecx", info->ContextRecord->Ecx, YvilesMagicBox::DecryptPtr(info->ContextRecord->Ecx).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "edx", info->ContextRecord->Edx, YvilesMagicBox::DecryptPtr(info->ContextRecord->Edx).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "edi", info->ContextRecord->Edi, YvilesMagicBox::DecryptPtr(info->ContextRecord->Edi).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "esi", info->ContextRecord->Esi, YvilesMagicBox::DecryptPtr(info->ContextRecord->Esi).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "ebp", info->ContextRecord->Ebp, YvilesMagicBox::DecryptPtr(info->ContextRecord->Ebp).c_str());
		Log() << FormatString("%s | 0x%08X |", "eip", info->ContextRecord->Eip);
		Log() << FormatString("%s | 0x%08X |", "esp", info->ContextRecord->Esp);
	}
}

namespace CrashLogger::Stack
{
	void Get(EXCEPTION_POINTERS* info)
	{
		Log() << FormatString("\nStack: ");
		Log() << FormatString(" # |    VALUE   | DEREFERENCE INFO");

		UInt32* esp = (UInt32*)info->ContextRecord->Esp;
		for (unsigned int i = 0; i < ce_printStackCount; i += 1) {

			if (i <= 8)
			{
				Log() << FormatString("%2X | 0x%08X | %s", i, esp[i], YvilesMagicBox::DecryptPtr(esp[i]).c_str());
			}
			else
			{
				std::string result = YvilesMagicBox::DecryptPtr(esp[i], 0);
				if (!result.empty()) Log() << FormatString("%2X | 0x%08X | %s", i, esp[i], result.c_str());

			}
		}
	}
}

namespace CrashLogger::ModuleBases
{
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

	void Get(EXCEPTION_POINTERS* info, HANDLE processHandle)
	{
		const UInt32 eip = info->ContextRecord->Eip;

		Log() << FormatString("\nModule bases:");
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
	}
}

namespace CrashLogger
{
	void Get(EXCEPTION_POINTERS* info) {
		HANDLE  processHandle = GetCurrentProcess();
		HANDLE  threadHandle = GetCurrentThread();

		Calltrace::Get(info, processHandle, threadHandle);

		Registry::Get(info);

		Stack::Get(info);

		ModuleBases::Get(info, processHandle);

		SymCleanup(processHandle);
	};

	static LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

	LONG WINAPI Filter(EXCEPTION_POINTERS* info) {
		static bool caught = false;
		bool ignored = false;
		//
		if (caught) ignored = true;
		else
		{
			caught = true;
			try { Get(info); }
			catch (...) {};
		}
		if (s_originalFilter) s_originalFilter(info); // don't return
		return !ignored ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	};

	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI FakeSetUnhandledExceptionFilter(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) {
		s_originalFilter = lpTopLevelExceptionFilter;
		return nullptr;
	}
	//
	void Apply() {
		s_originalFilter = SetUnhandledExceptionFilter(&Filter);
		//
		// Prevent disabling the filter:
		//
		SafeWrite32(0x00FDF180, (UInt32)&FakeSetUnhandledExceptionFilter);
		//else SafeWrite32(0x00A281B4, (UInt32)&FakeSetUnhandledExceptionFilter);

	};
};