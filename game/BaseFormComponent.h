#pragma once
#include <NiTypes.h>
#include <NiObjects.h>
#include <Containers.h>

#define IS_ID(form, type) ((form)->typeID == kFormType_##type)
#define NOT_ID(form, type) ((form)->typeID != kFormType_##type)

#define IS_TYPE(form, type) (*(UInt32*)(form) == kVtbl_##type)
#define NOT_TYPE(form, type) (*(UInt32*)(form) != kVtbl_##type)

enum FormType
{
	kFormType_None = 0,
	kFormType_TES4,
	kFormType_Group,
	kFormType_GMST,
	kFormType_BGSTextureSet,
	kFormType_BGSMenuIcon,
	kFormType_TESGlobal,
	kFormType_TESClass,
	kFormType_TESFaction,
	kFormType_BGSHeadPart,
	kFormType_TESHair,
	kFormType_TESEyes,
	kFormType_TESRace,
	kFormType_TESSound,
	kFormType_BGSAcousticSpace,
	kFormType_TESSkill,
	kFormType_EffectSetting,
	kFormType_Script,
	kFormType_TESLandTexture,
	kFormType_EnchantmentItem,
	kFormType_SpellItem,
	kFormType_TESObjectACTI,
	kFormType_BGSTalkingActivator,
	kFormType_BGSTerminal,
	kFormType_TESObjectARMO,					// Inventory object
	kFormType_TESObjectBOOK,					// Inventory object
	kFormType_TESObjectCLOT,					// Inventory object
	kFormType_TESObjectCONT,
	kFormType_TESObjectDOOR,
	kFormType_IngredientItem,					// Inventory object
	kFormType_TESObjectLIGH,					// Inventory object
	kFormType_TESObjectMISC,					// Inventory object
	kFormType_TESObjectSTAT,
	kFormType_BGSStaticCollection,
	kFormType_BGSMovableStatic,
	kFormType_BGSPlaceableWater,
	kFormType_TESGrass,
	kFormType_TESObjectTREE,
	kFormType_TESFlora,
	kFormType_TESFurniture,
	kFormType_TESObjectWEAP,					// Inventory object
	kFormType_TESAmmo,							// Inventory object
	kFormType_TESNPC,
	kFormType_TESCreature,
	kFormType_TESLevCreature,
	kFormType_TESLevCharacter,
	kFormType_TESKey,							// Inventory object
	kFormType_AlchemyItem,						// Inventory object
	kFormType_BGSIdleMarker,
	kFormType_BGSNote,							// Inventory object
	kFormType_BGSConstructibleObject,			// Inventory object
	kFormType_BGSProjectile,
	kFormType_TESLevItem,						// Inventory object
	kFormType_TESWeather,
	kFormType_TESClimate,
	kFormType_TESRegion,
	kFormType_NavMeshInfoMap,
	kFormType_TESObjectCELL,
	kFormType_TESObjectREFR,
	kFormType_Character,
	kFormType_Creature,
	kFormType_MissileProjectile,
	kFormType_GrenadeProjectile,
	kFormType_BeamProjectile,
	kFormType_FlameProjectile,
	kFormType_TESWorldSpace,
	kFormType_TESObjectLAND,
	kFormType_NavMesh,
	kFormType_TLOD,
	kFormType_TESTopic,
	kFormType_TESTopicInfo,
	kFormType_TESQuest,
	kFormType_TESIdleForm,
	kFormType_TESPackage,
	kFormType_TESCombatStyle,
	kFormType_TESLoadScreen,
	kFormType_TESLevSpell,
	kFormType_TESObjectANIO,
	kFormType_TESWaterForm,
	kFormType_TESEffectShader,
	kFormType_TOFT,
	kFormType_BGSExplosion,
	kFormType_BGSDebris,
	kFormType_TESImageSpace,
	kFormType_TESImageSpaceModifier,
	kFormType_BGSListForm,
	kFormType_BGSPerk,
	kFormType_BGSBodyPartData,
	kFormType_BGSAddonNode,
	kFormType_ActorValueInfo,
	kFormType_BGSRadiationStage,
	kFormType_BGSCameraShot,
	kFormType_BGSCameraPath,
	kFormType_BGSVoiceType,
	kFormType_BGSImpactData,
	kFormType_BGSImpactDataSet,
	kFormType_TESObjectARMA,
	kFormType_BGSEncounterZone,
	kFormType_BGSMessage,
	kFormType_BGSRagdoll,
	kFormType_DOBJ,
	kFormType_BGSLightingTemplate,
	kFormType_BGSMusicType,
	kFormType_TESObjectIMOD,					// Inventory object
	kFormType_TESReputation,
	kFormType_ContinuousBeamProjectile,
	kFormType_TESRecipe,
	kFormType_TESRecipeCategory,
	kFormType_TESCasinoChips,					// Inventory object
	kFormType_TESCasino,
	kFormType_TESLoadScreenType,
	kFormType_MediaSet,
	kFormType_MediaLocationController,
	kFormType_TESChallenge,
	kFormType_TESAmmoEffect,
	kFormType_TESCaravanCard,					// Inventory object
	kFormType_TESCaravanMoney,					// Inventory object
	kFormType_TESCaravanDeck,
	kFormType_BGSDehydrationStage,
	kFormType_BGSHungerStage,
	kFormType_BGSSleepDeprevationStage,
	kFormType_Max,
};

enum ObjectVtbl
{

	kVtbl_BGSDehydrationStage = 0x101144C,
	kVtbl_BGSHungerStage = 0x10115B4,
	kVtbl_BGSSleepDeprevationStage = 0x10116FC,
	kVtbl_AlchemyItem = 0x1011964,
	kVtbl_MagicItemObject = 0x1011B4C,
	kVtbl_TESIcon = 0x1011CD4,
	kVtbl_MagicItem = 0x1011D0C,
	kVtbl_TESFullName = 0x1011D54,
	kVtbl_BaseFormComponent = 0x1011D68,
	kVtbl_Setting = 0x1012120,
	kVtbl_GameSettingCollection = 0x101212C,
	kVtbl_EffectItemList = 0x1012508,
	kVtbl_EffectSetting = 0x1012834,
	kVtbl_EnchantmentItem = 0x1012EA4,
	kVtbl_MagicItemForm = 0x1013044,
	kVtbl_IngredientItem = 0x1013284,
	kVtbl_SpellItem = 0x1013F8C,
	kVtbl_ExtraCell3D = 0x1014204,
	kVtbl_BSExtraData = 0x1014210,
	kVtbl_ExtraHavok = 0x101421C,
	kVtbl_ExtraRegionList = 0x1014228,
	kVtbl_ExtraCellMusicType = 0x1014234,
	kVtbl_ExtraCellAcousticSpace = 0x1014240,
	kVtbl_ExtraCellClimate = 0x101424C,
	kVtbl_ExtraCellImageSpace = 0x1014258,
	kVtbl_ExtraCellImpactSwap = 0x1014264,
	kVtbl_ExtraCellWaterType = 0x1014270,
	kVtbl_ExtraProcessMiddleLow = 0x101427C,
	kVtbl_ExtraCellCanopyShadowMask = 0x1014288,
	kVtbl_ExtraSeenData = 0x1014294,
	kVtbl_ExtraNorthRotation = 0x10142A0,
	kVtbl_ExtraDetachTime = 0x10142AC,
	kVtbl_BaseExtraList = 0x1014300,
	kVtbl_ExtraDataList = 0x10143E8,
	kVtbl_ExtraReflectedRefs = 0x1014428,
	kVtbl_ExtraReflectorRefs = 0x1014434,
	kVtbl_ExtraWaterLightRefs = 0x1014440,
	kVtbl_ExtraLitWaterRefs = 0x101444C,
	kVtbl_ExtraSpecialRenderFlags = 0x1014458,
	kVtbl_ExtraNavMeshPortal = 0x1014618,
	kVtbl_ExtraRadioData = 0x1015138,
	kVtbl_ExtraHealthPerc = 0x1015178,
	kVtbl_ExtraObjectHealth = 0x1015184,
	kVtbl_ExtraTerminalState = 0x1015190,
	kVtbl_ExtraEditorID = 0x101519C,
	kVtbl_ExtraOpenCloseActivateRef = 0x10151A8,
	kVtbl_ExtraPrimitive = 0x10151B4,
	kVtbl_ExtraPatrolRefData = 0x10151C0,
	kVtbl_ExtraOcclusionPlaneRefData = 0x10151CC,
	kVtbl_ExtraPortalRefData = 0x10151D8,
	kVtbl_ExtraRoomRefData = 0x10151E4,
	kVtbl_ExtraCollisionData = 0x10151F0,
	kVtbl_ExtraPackageData = 0x10151FC,
	kVtbl_ExtraRadius = 0x1015208,
	kVtbl_ExtraRadiation = 0x1015214,
	kVtbl_ExtraLock = 0x101589C,
	kVtbl_ExtraTeleport = 0x10158A8,
	kVtbl_ExtraOwnership = 0x10158B4,
	kVtbl_ExtraGlobal = 0x10158C0,
	kVtbl_ExtraRank = 0x10158CC,
	kVtbl_ExtraCount = 0x10158D8,
	kVtbl_ExtraHealth = 0x10158E4,
	kVtbl_ExtraUses = 0x10158F0,
	kVtbl_ExtraTimeLeft = 0x10158FC,
	kVtbl_ExtraCharge = 0x1015908,
	kVtbl_ExtraScript = 0x1015914,
	kVtbl_ExtraScale = 0x1015920,
	kVtbl_ExtraHotkey = 0x101592C,
	kVtbl_ExtraReferencePointer = 0x1015938,
	kVtbl_ExtraTresPassPackage = 0x1015944,
	kVtbl_ExtraLeveledItem = 0x1015950,
	kVtbl_ExtraPoison = 0x101595C,
	kVtbl_ExtraHeadingTarget = 0x1015968,
	kVtbl_ExtraHasNoRumors = 0x1015974,
	kVtbl_ExtraModelSwap = 0x1015980,
	kVtbl_ExtraActorCause = 0x101598C,
	kVtbl_ExtraAmmo = 0x1015998,
	kVtbl_ExtraWeaponModFlags = 0x10159A4,
	kVtbl_ExtraSecuritronFace = 0x10159B0,
	kVtbl_ExtraModdingItem = 0x10159BC,
	kVtbl_ExtraCombatStyle = 0x10159C8,
	kVtbl_ExtraAnim = 0x1015B28,
	kVtbl_ExtraDismemberedLimbs = 0x1015B34,
	kVtbl_ExtraStartingPosition = 0x1015B40,
	kVtbl_ExtraStartingWorldOrCell = 0x1015B4C,
	kVtbl_ExtraLight = 0x1015B58,
	kVtbl_ExtraFollower = 0x1015B64,
	kVtbl_ExtraGuardedRefData = 0x1015B70,
	kVtbl_ExtraLeveledCreature = 0x1015B7C,
	kVtbl_ExtraMapMarker = 0x1015B88,
	kVtbl_ExtraAudioMarker = 0x1015B94,
	kVtbl_ExtraAudioBuoyMarker = 0x1015BA0,
	kVtbl_ExtraAction = 0x1015BAC,
	kVtbl_ExtraContainerChanges = 0x1015BB8,
	kVtbl_ExtraOriginalReference = 0x1015BC4,
	kVtbl_ExtraGhost = 0x1015BD0,
	kVtbl_ExtraWorn = 0x1015BDC,
	kVtbl_ExtraWornLeft = 0x1015BE8,
	kVtbl_ExtraCannotWear = 0x1015BF4,
	kVtbl_ExtraInfoGeneralTopic = 0x1015C00,
	kVtbl_ExtraSeed = 0x1015C0C,
	kVtbl_ExtraPackageStartLocation = 0x1015C18,
	kVtbl_ExtraPackage = 0x1015C24,
	kVtbl_ExtraPlayerCrimeList = 0x1015C30,
	kVtbl_ExtraPersistentCell = 0x1015C3C,
	kVtbl_ExtraRagDollData = 0x1015C48,
	kVtbl_ExtraEncounterZone = 0x1015C54,
	kVtbl_ExtraUsedMarkers = 0x1015C60,
	kVtbl_ExtraReservedMarkers = 0x1015C6C,
	kVtbl_ExtraRunOncePacks = 0x1015C78,
	kVtbl_ExtraDistantData = 0x1015C84,
	kVtbl_ExtraEnableStateParent = 0x1015C90,
	kVtbl_ExtraEnableStateChildren = 0x1015C9C,
	kVtbl_ExtraRandomTeleportMarker = 0x1015CA8,
	kVtbl_ExtraLinkedRefChildren = 0x1015CB4,
	kVtbl_ExtraLinkedRef = 0x1015CC0,
	kVtbl_ExtraAshPileRef = 0x1015CCC,
	kVtbl_ExtraActivateRefChildren = 0x1015CD8,
	kVtbl_ExtraActivateRef = 0x1015CE4,
	kVtbl_ExtraDecalRefs = 0x1015CF0,
	kVtbl_ExtraMerchantContainer = 0x1015E04,
	kVtbl_ExtraLevCreaModifier = 0x1015E10,
	kVtbl_ExtraLastFinishedSequence = 0x1015E1C,
	kVtbl_ExtraXTarget = 0x1015E28,
	kVtbl_ExtraEmittanceSource = 0x1015E34,
	kVtbl_ExtraMultiBoundRef = 0x1015E40,
	kVtbl_ExtraMultiBoundData = 0x1015E4C,
	kVtbl_ExtraMultiBound = 0x1015E58,
	kVtbl_ExtraOcclusionPlane = 0x1015E64,
	kVtbl_ExtraPortal = 0x1015E70,
	kVtbl_ExtraRoom = 0x1015E7C,
	kVtbl_ExtraItemDropper = 0x1015E88,
	kVtbl_ExtraDroppedItemList = 0x1015E94,
	kVtbl_ExtraSavedAnimation = 0x1015EA0,
	kVtbl_ExtraSavedHavokData = 0x1015EAC,
	kVtbl_ExtraFriendHits = 0x1015EB8,
	kVtbl_ExtraRefractionProperty = 0x1015EC4,
	kVtbl_ExtraEditorRefMoveData = 0x1015ED0,
	kVtbl_ExtraSound = 0x1015EDC,
	kVtbl_ExtraCreatureAwakeSound = 0x1015EE8,
	kVtbl_ExtraCreatureMovementSound = 0x1015EF4,
	kVtbl_ExtraWeaponIdleSound = 0x1015F00,
	kVtbl_ExtraWeaponAttackSound = 0x1015F0C,
	kVtbl_ExtraActivateLoopSound = 0x1015F18,
	kVtbl_ExtraTalkingActor = 0x1015F24,
	kVtbl_ExtraFactionChanges = 0x1015F30,
	kVtbl_ExtraSayTopicInfoOnceADay = 0x1015F3C,
	kVtbl_ExtraSayToTopicInfo = 0x1015F48,
	kVtbl_ExtraWaterZoneMap = 0x1015F54,
	kVtbl_ExtraIgnoredBySandbox = 0x1015F64,
	kVtbl_ExtraPatrolRefInUseData = 0x1015F70,
	kVtbl_ExtraFollowerSwimBreadcrumbs = 0x1015F7C,
	kVtbl_NiAlphaProperty = 0x10162DC,
	kVtbl_NiProperty = 0x1016374,
	kVtbl_QueuedTexture = 0x1016788,
	kVtbl_QueuedModel = 0x1016890,
	kVtbl_BSStream = 0x1016904,
	kVtbl_QueuedTreeBillboard = 0x101698C,
	kVtbl_QueuedTreeModel = 0x10169D4,
	kVtbl_QueuedMagicItem = 0x1016A18,
	kVtbl_QueuedKF = 0x1016A48,
	kVtbl_QueuedAnimIdle = 0x1016A80,
	kVtbl_QueuedReplacementKF = 0x1016AC0,
	kVtbl_QueuedHead = 0x1016B04,
	kVtbl_QueuedHelmet = 0x1016B4C,
	kVtbl_QueuedReference = 0x1016BA4,
	kVtbl_AttachDistant3DTask = 0x1016BEC,
	kVtbl_IOTask = 0x1016C10,
	kVtbl_QueuedTree = 0x1016C5C,
	kVtbl_QueuedActor = 0x1016CA4,
	kVtbl_QueuedCharacter = 0x1016CEC,
	kVtbl_QueuedCreature = 0x1016D34,
	kVtbl_QueuedPlayer = 0x1016D7C,
	kVtbl_QueuedFileLoad = 0x1016DC4,
	kVtbl_QueuedFaceGenFile = 0x1016E1C,
	kVtbl_BackgroundCloneThread = 0x1016E50,
	kVtbl_QueuedReplacementKFList = 0x1016FC0,
	kVtbl_InterfacedClass = 0x1017458,
	kVtbl_INISettingCollection = 0x101772C,
	kVtbl_TES = 0x10178B4,
	kVtbl_BSTempNodeManager = 0x10178BC,
	kVtbl_BSTempNode = 0x1017D24,
	kVtbl_RendererSettingCollection = 0x1018508,
	kVtbl_BGSAmmoForm = 0x101A3B8,
	kVtbl_BGSBipedModelList = 0x101A4C4,
	kVtbl_BGSClipRoundsForm = 0x101A644,
	kVtbl_BGSDestructibleObjectForm = 0x101A6A0,
	kVtbl_BGSEquipType = 0x101AA14,
	kVtbl_BGSIdleCollection = 0x101AA98,
	kVtbl_BGSMessageIcon = 0x101AB58,
	kVtbl_BGSPickupPutdownSounds = 0x101ABD8,
	kVtbl_BGSRepairItemList = 0x101ADE8,
	kVtbl_BGSTouchSpellForm = 0x101AEF0,
	kVtbl_TESActorBaseData = 0x101B0DC,
	kVtbl_TESAIForm = 0x101BE98,
	kVtbl_TESAnimation = 0x101BF50,
	kVtbl_TESAttackDamageForm = 0x101BF68,
	kVtbl_TESAttributes = 0x101BF84,
	kVtbl_TESBipedModelForm = 0x101BFD8,
	kVtbl_TESModelRDT = 0x101BFEC,
	kVtbl_TESContainer = 0x101C130,
	kVtbl_TESDescription = 0x101C18C,
	kVtbl_TESEnchantableForm = 0x101C1B0,
	kVtbl_TESForm = 0x101C794,
	kVtbl_TESHealthForm = 0x101CB94,
	kVtbl_TESImageSpaceModifiableForm = 0x101CBD8,
	kVtbl_TESLeveledList = 0x101CD04,
	kVtbl_TESModel = 0x101CF44,
	kVtbl_TESModelList = 0x101D0D4,
	kVtbl_TESModelTextureSwap = 0x101D124,
	kVtbl_TESProduceForm = 0x101D2A4,
	kVtbl_TESRaceForm = 0x101D398,
	kVtbl_TESReactionForm = 0x101D46C,
	kVtbl_TESScriptableForm = 0x101D518,
	kVtbl_TESSpellList = 0x101D628,
	kVtbl_TESTexture = 0x101D7C4,
	kVtbl_TESValueForm = 0x101D81C,
	kVtbl_TESWeightForm = 0x101D874,
	kVtbl_AnimSequenceSingle = 0x101D8C8,
	kVtbl_AnimSequenceBase = 0x101D8E8,
	kVtbl_AnimSequenceMultiple = 0x101D908,
	kVtbl_AnimIdle = 0x101DCD8,
	kVtbl_NiRefObject = 0x101DCE4,
	kVtbl_BSAnimGroupSequence = 0x101DE3C,
	kVtbl_NiStencilProperty = 0x101E07C,
	kVtbl_BSCullingProcess = 0x101E2EC,
	kVtbl_hkpAllRayHitCollector = 0x101E588,
	kVtbl_hkpRayHitCollector = 0x101E594,
	kVtbl_BGSPrimitive = 0x101E72C,
	kVtbl_BGSPrimitivePlane = 0x101E75C,
	kVtbl_BSOcclusionPlane = 0x101E794,
	kVtbl_BSPortal = 0x101E82C,
	kVtbl_BGSPrimitiveBox = 0x101E8C4,
	kVtbl_BSMultiBoundAABB = 0x101E8E4,
	kVtbl_BSMultiBoundOBB = 0x101E9A4,
	kVtbl_BGSPrimitiveSphere = 0x101EA64,
	kVtbl_BSMultiBoundSphere = 0x101EA8C,
	kVtbl_BGSSceneInfo = 0x101EC68,
	kVtbl_BSAnimNoteReceiver = 0x101FC74,
	kVtbl_IBSAnimNoteReceiver = 0x101FC80,
	kVtbl_bhkMouseSpringAction = 0x101FFCC,
	kVtbl_bhkUnaryAction = 0x102009C,
	kVtbl_bhkAction = 0x102016C,
	kVtbl_bhkSerializable = 0x102023C,
	kVtbl_GridArray = 0x1020634,
	kVtbl_GridCellArray = 0x1020698,
	kVtbl_INIPrefSettingCollection = 0x1022744,
	kVtbl_BSCustomSplatterExtraData = 0x10228C4,
	kVtbl_BSSplatterExtraData = 0x1022A24,
	kVtbl_ImageSpaceEffectParam = 0x10231CC,
	kVtbl_BGSDecalNode = 0x1023734,
	kVtbl_TESRegion = 0x102397C,
	kVtbl_TESRegionData = 0x1023AC4,
	kVtbl_TESRegionDataGrass = 0x1023B24,
	kVtbl_TESRegionDataImposter = 0x1023BC8,
	kVtbl_TESRegionDataLandscape = 0x1023C8C,
	kVtbl_TESRegionDataManager = 0x1023CE4,
	kVtbl_TESRegionDataMap = 0x1023D28,
	kVtbl_TESRegionDataSound = 0x1023DA4,
	kVtbl_TESRegionDataWeather = 0x1023E18,
	kVtbl_TESRegionGrassObject = 0x1023EC4,
	kVtbl_TESRegionObjectBase = 0x1023EE4,
	kVtbl_TESRegionGrassObjectList = 0x1024094,
	kVtbl_TESRegionList = 0x10240A0,
	kVtbl_BGSAddonNode = 0x1024214,
	kVtbl_BGSAddonNodeSoundHandleExtra = 0x10243D4,
	kVtbl_BGSConstructibleObject = 0x10245A4,
	kVtbl_BGSDebris = 0x1024834,
	kVtbl_BGSPreloadable = 0x1024970,
	kVtbl_BGSExplosion = 0x1024A94,
	kVtbl_BGSMovableStatic = 0x1024CEC,	//	Has an irregular structure; the enumed vtbl is the "effective" one used at runtime; actual vtbl is 0x1024E84
	kVtbl_BGSPlaceableWater = 0x1024F4C,
	kVtbl_BGSProjectile = 0x10251AC,
	kVtbl_BGSStaticCollection = 0x102535C,
	kVtbl_BGSTalkingActivator = 0x1025594,
	kVtbl_PathingLocation = 0x102583C,
	kVtbl_BGSTerminal = 0x1025914,
	kVtbl_TESAmmo = 0x1026064,
	kVtbl_TESCasinoChips = 0x10263DC,
	kVtbl_TESCasino = 0x1026574,
	kVtbl_TESCombatStyle = 0x10266E4,
	kVtbl_TESEffectShader = 0x102685C,
	kVtbl_TESFlora = 0x1026BD0,
	kVtbl_TESFurniture = 0x1026D0C,
	kVtbl_TESGrass = 0x102814C,
	kVtbl_TESKey = 0x1028444,
	kVtbl_TESLevCharacter = 0x102864C,
	kVtbl_TESLevCreature = 0x102886C,
	kVtbl_TESLevItem = 0x1028A64,
	kVtbl_TESLevSpell = 0x1028C5C,
	kVtbl_TESObjectLIGH = 0x1028EE4,
	kVtbl_TESBoundObject = 0x102960C,
	kVtbl_TESBoundAnimObject = 0x102980C,
	kVtbl_TESBoundTreeObject = 0x1029994,
	kVtbl_TESObject = 0x1029B1C,
	kVtbl_TESObjectACTI = 0x1029D5C,
	kVtbl_BGSOpenCloseForm = 0x1029EE0,
	kVtbl_TESObjectANIO = 0x102A0A4,
	kVtbl_TESObjectARMA = 0x102A31C,
	kVtbl_TESObjectARMO = 0x102A62C,
	kVtbl_TESObjectBOOK = 0x102A9C4,
	kVtbl_TESObjectCLOT = 0x102AC0C,
	kVtbl_TESObjectCONT = 0x102AEB4,
	kVtbl_TESObjectDOOR = 0x102B1FC,
	kVtbl_TESObjectIMOD = 0x102B5AC,
	kVtbl_TESObjectMISC = 0x102B844,
	kVtbl_TESObjectSTAT = 0x102BA2C,
	kVtbl_TESObjectTREE = 0x102BC94,
	kVtbl_TESModelTree = 0x102BE70,
	kVtbl_TESIconTree = 0x102BE94,
	kVtbl_TESTexture1024 = 0x102BEB4,
	kVtbl_NiBillboardNode = 0x102BF44,
	kVtbl_BSShaderTextureSet = 0x102C04C,
	kVtbl_BSTextureSet = 0x102C0EC,
	kVtbl_TESObjectWEAP = 0x102C51C,
	kVtbl_BGSEncounterZone = 0x102CBBC,
	kVtbl_BGSLightingTemplate = 0x102CD94,
	kVtbl_LoadedReferenceMap = 0x102CF0C,
	kVtbl_ExteriorCellLoaderTask = 0x102D028,
	kVtbl_ImageSpaceModifierInstanceForm = 0x102D12C,
	kVtbl_ImageSpaceModifierInstance = 0x102D1CC,
	kVtbl_ImageSpaceModifierInstanceDOF = 0x102D2A4,
	kVtbl_ImageSpaceModifierInstanceRB = 0x102D36C,
	kVtbl_ImageSpaceModifierInstanceDRB = 0x102D454,
	kVtbl_ImageSpaceModifierInstanceTemp = 0x102D50C,
	kVtbl_TESClimate = 0x102D5C4,
	kVtbl_TESImageSpace = 0x102D7F4,
	kVtbl_TESImageSpaceModifier = 0x102D97C,
	kVtbl_NiFloatData = 0x102DABC,
	kVtbl_NiColorData = 0x102DB4C,
	kVtbl_TESObjectLAND = 0x102DCD4,
	kVtbl_TESChildCell = 0x102DE10,
	kVtbl_BSPackedAdditionalGeometryData = 0x102E1DC,
	kVtbl_NiAdditionalGeometryData = 0x102E274,
	kVtbl_hkpMoppCode = 0x102E368,
	kVtbl_hkReferencedObject = 0x102E378,
	kVtbl_hkBaseObject = 0x102E388,
	kVtbl_NiBinaryExtraData = 0x102E394,
	kVtbl_TESLandTexture = 0x102E6C4,
	kVtbl_TESObjectCELL = 0x102E9B4,
	kVtbl_CheckWithinMultiBoundTask = 0x102EE48,
	kVtbl_hkpWorldCinfo = 0x102F0EC,
	kVtbl_TESWindListener = 0x102F10C,
	kVtbl_hkpEntityListener = 0x102F13C,
	kVtbl_TESObjectREFR = 0x102F55C,
	kVtbl_bhkRigidBody = 0x10301B4,
	kVtbl_bhkEntity = 0x10302BC,
	kVtbl_bhkWorldObject = 0x1030394,
	kVtbl_bhkCollisionObject = 0x103046C,
	kVtbl_bhkWaterPhantomCallbackShape = 0x1030534,
	kVtbl_hkpPhantomCallbackShape = 0x1030570,
	kVtbl_hkpShape = 0x10305A8,
	kVtbl_bhkSimpleShapePhantom = 0x10305DC,
	kVtbl_bhkShapePhantom = 0x10306B4,
	kVtbl_bhkPhantom = 0x103078C,
	kVtbl_bhkAabbPhantom = 0x1030864,
	kVtbl_bhkBoxShape = 0x103093C,
	kVtbl_bhkConvexShape = 0x1030A24,
	kVtbl_bhkSphereRepShape = 0x1030B0C,
	kVtbl_bhkShape = 0x1030BF4,
	kVtbl_bhkSphereShape = 0x1030CDC,
	kVtbl_bhkAcousticSpacePhantomCallbackShape = 0x1030DC0,
	kVtbl_ExtraAnimNoteReceiver = 0x1030F14,
	kVtbl_TESWaterForm = 0x103140C,
	kVtbl_TESWeather = 0x103168C,
	kVtbl_TESWorldSpace = 0x103195C,
	kVtbl_BGSAcousticSpace = 0x10320FC,
	kVtbl_BGSCameraPath = 0x103245C,
	kVtbl_NiFormArray = 0x10326FC,
	kVtbl_BGSCameraShot = 0x10327F4,
	kVtbl_BGSDefaultObjectManager = 0x1032DC4,
	kVtbl_BGSImpactData = 0x1032F6C,
	kVtbl_BGSImpactDataSet = 0x103323C,
	kVtbl_BGSListForm = 0x10334B4,
	kVtbl_BGSMenuIcon = 0x1033654,
	kVtbl_BGSMessage = 0x10337C4,
	kVtbl_BGSMusicType = 0x103397C,
	kVtbl_TESSoundFile = 0x1033AB8,
	kVtbl_BGSRadiationStage = 0x1033B34,
	kVtbl_BGSTextureSet = 0x1033D1C,
	kVtbl_MediaLocationController = 0x10340C4,
	kVtbl_MediaSet = 0x10342EC,
	kVtbl_TESAmmoEffect = 0x103449C,
	kVtbl_TESCaravanCard = 0x103478C,
	kVtbl_TESCaravanMoney = 0x10349B4,
	kVtbl_TESCaravanDeck = 0x1034B4C,
	kVtbl_hkpClosestRayHitCollector = 0x1034EC4,
	kVtbl_TESGlobal = 0x1036524,
	kVtbl_TESLoadScreen = 0x10366CC,
	kVtbl_TESLoadScreenType = 0x1036854,
	kVtbl_TESRecipeCategory = 0x10369DC,
	kVtbl_TESRecipe = 0x1036B2C,
	kVtbl_Script = 0x1037094,
	kVtbl_MobIterOperator = 0x103CF5C,
	kVtbl_TESSound = 0x1044FFC,
	kVtbl_BGSBodyPart = 0x1045404,
	kVtbl_TESModelPSA = 0x104540C,
	kVtbl_BGSBodyPartData = 0x1045504,
	kVtbl_BGSEntryPointFunctionDataOneValue = 0x10462C0,
	kVtbl_BGSEntryPointFunctionData = 0x10462E0,
	kVtbl_BGSEntryPointFunctionDataTwoValue = 0x1046300,
	kVtbl_BGSEntryPointFunctionDataLeveledList = 0x1046320,
	kVtbl_BGSEntryPointFunctionDataActivateChoice = 0x1046340,
	kVtbl_BGSHeadPart = 0x10464B4,
	kVtbl_BGSIdleMarker = 0x104664C,
	kVtbl_BGSNote = 0x1046874,
	kVtbl_BGSPerkEntry = 0x1046B48,
	kVtbl_BGSQuestPerkEntry = 0x1046B84,
	kVtbl_BGSAbilityPerkEntry = 0x1046C44,
	kVtbl_BGSEntryPointPerkEntry = 0x1046D0C,
	kVtbl_BGSPerk = 0x1046EC4,
	kVtbl_BGSQuestObjective = 0x1047088,
	kVtbl_BGSRagdoll = 0x10470EC,
	kVtbl_BGSVoiceType = 0x104733C,
	kVtbl_Reset3DMobIterator = 0x1047670,
	kVtbl_TESAnimGroup = 0x10484E8,
	kVtbl_TESChallenge = 0x104891C,
	kVtbl_TESClass = 0x1048BB4,
	kVtbl_TESCreature = 0x1048F5C,
	kVtbl_TESActorBase = 0x10492AC,
	kVtbl_ActorValueOwner = 0x1049450,
	kVtbl_TESEyes = 0x104973C,
	kVtbl_TESFaction = 0x10498DC,
	kVtbl_TESHair = 0x1049B9C,
	kVtbl_TESIdleForm = 0x1049D0C,
	kVtbl_TESModelAnim = 0x1049E48,
	kVtbl_TESNPC = 0x104A2F4,
	kVtbl_TESQuest = 0x104AC44,
	kVtbl_TESRace = 0x104B4BC,
	kVtbl_BGSTextureModel = 0x104B5F8,
	kVtbl_TESReputation = 0x104BA24,
	kVtbl_TESSkill = 0x104CC0C,
	kVtbl_TESTopic = 0x104D19C,
	kVtbl_TopicInfoArray = 0x104D338,
	kVtbl_TESTopicInfo = 0x104D5B4,
	kVtbl_BGSAcousticSpaceListener = 0x104D8E8,
	kVtbl_bhkEntityListener = 0x104D91C,
	kVtbl_TargetEntry = 0x104D9C4,
	kVtbl_ZoneEntry = 0x104D9CC,
	kVtbl_TrapTargetEntry = 0x104D9D8,
	kVtbl_BGSZoneTargetListener = 0x104D9E0,
	kVtbl_hkpAllCdPointCollector = 0x104DA28,
	kVtbl_hkpCdPointCollector = 0x104DA3C,
	kVtbl_CellMopp = 0x104DAB4,
	kVtbl_bhkTriSampledHeightFieldBvTreeShape = 0x104DAC4,
	kVtbl_bhkBvTreeShape = 0x104DBAC,
	kVtbl_FOCollisionListener = 0x104DD38,
	kVtbl_bhkContactListener = 0x104DD5C,
	kVtbl_hkpContactListener = 0x104DD7C,
	kVtbl_FOIslandActivationListener = 0x104DE00,
	kVtbl_hkpIslandActivationListener = 0x104DE10,
	kVtbl_HavokStreambufFactory = 0x104DEA4,
	kVtbl_hkFileSystem = 0x104DEC0,
	kVtbl_HavokError = 0x104DEF4,
	kVtbl_hkError = 0x104DF8C,
	kVtbl_hkStreamReader = 0x104DFB4,
	kVtbl_hkStreamWriter = 0x104DFE8,
	kVtbl_HavokFileStreambufReader = 0x104E014,
	kVtbl_HavokFileStreambufWriter = 0x104E054,
	kVtbl_LoadedAreaBound = 0x104E130,
	kVtbl_SpecificItemCollector = 0x104E1DC,
	kVtbl_TESArrowTargetListener = 0x104E228,
	kVtbl_bhkArrowTargetListener = 0x104E254,
	kVtbl_hkpAllCdBodyPairCollector = 0x104E318,
	kVtbl_hkpCdBodyPairCollector = 0x104E328,
	kVtbl_TESTrapListener = 0x104E37C,
	kVtbl_bhkTrapListener = 0x104E3A4,
	kVtbl_hkpPhantomListener = 0x104E3D0,
	kVtbl_TrapEntry = 0x104E3E8,
	kVtbl_WaterTargetEntry = 0x104E438,
	kVtbl_TESWaterListener = 0x104E450,
	kVtbl_WaterZone = 0x104E4B0,
	kVtbl_BlendSettingCollection = 0x104E6AC,
	kVtbl_Atmosphere = 0x104EB68,
	kVtbl_Clouds = 0x104EC08,
	kVtbl_Moon = 0x104ED28,
	kVtbl_Precipitation = 0x104ED98,
	kVtbl_Sky = 0x104EDEC,
	kVtbl_SkyObject = 0x104F0F8,
	kVtbl_SkyTask = 0x104F118,
	kVtbl_Stars = 0x104F1D8,
	kVtbl_Sun = 0x104F298,
	kVtbl_BSFaceGenAnimationData = 0x1064E1C,
	kVtbl_BSFaceGenImage = 0x10653C4,
	kVtbl_BSFaceGenKeyframe = 0x10653FC,
	kVtbl_BSFaceGenKeyframeMultiple = 0x106544C,
	kVtbl_BSFaceGenModelMap = 0x1065678,
	kVtbl_BSFaceGenModel = 0x1065AF4,
	kVtbl_BSFaceGenModelExtraData = 0x1065BF4,
	kVtbl_BSFaceGenMorph = 0x1065CB8,
	kVtbl_BSFaceGenMorphDifferential = 0x1065CC4,
	kVtbl_BSFaceGenMorphStatistical = 0x1065CD0,
	kVtbl_BSFaceGenMorphDataHead = 0x1065EB4,
	kVtbl_BSFaceGenMorphData = 0x1065F04,
	kVtbl_BSFaceGenMorphDataHair = 0x1065F5C,
	kVtbl_BSFaceGenBaseMorphExtraData = 0x1065FAC,
	kVtbl_BSFaceGenNiNode = 0x10660DC,
	kVtbl_BSTreeModel = 0x1066768,
	kVtbl_BSTreeNode = 0x10668E4,
	kVtbl_bhkSPCollisionObject = 0x1066ACC,
	kVtbl_bhkPCollisionObject = 0x1066B94,
	kVtbl_bhkMultiSphereShape = 0x1066C5C,
	kVtbl_bhkTransformShape = 0x1066D44,
	kVtbl_bhkCapsuleShape = 0x1066E2C,
	kVtbl_ActorValueInfo = 0x1067A2C,
	kVtbl_TESPackage = 0x106847C,
	kVtbl_TESAmbushPackageData = 0x1068860,
	kVtbl_TESPackageData = 0x1068884,
	kVtbl_TESDialoguePackageData = 0x10688AC,
	kVtbl_TESEatPackageData = 0x1068938,
	kVtbl_TESEscortPackageData = 0x1068960,
	kVtbl_TESFollowPackageData = 0x1068988,
	kVtbl_TESPatrolPackageData = 0x10689E4,
	kVtbl_TESUseItemPackageData = 0x1068A0C,
	kVtbl_TESUseWeaponPackageData = 0x1068A64,
	kVtbl_UseWeaponActorPackageData = 0x1068A88,
	kVtbl_ActorPackageData = 0x1068AA4,
	kVtbl_NiTask = 0x1069674,
	kVtbl_BSTempEffect = 0x1069754,
	kVtbl_BSTempEffectDebris = 0x106986C,
	kVtbl_BSTempEffectGeometryDecal = 0x10699BC,
	kVtbl_BSDismemberSkinInstance = 0x1069A84,
	kVtbl_NiSkinInstance = 0x1069B14,
	kVtbl_BSTempEffectParticle = 0x1069C5C,
	kVtbl_BSTempEffectSimpleDecal = 0x1069DDC,
	kVtbl_NavMesh = 0x106A0B4,
	kVtbl_NavMeshPOVSearch = 0x106B39C,
	kVtbl_NavMeshSearch = 0x106B63C,
	kVtbl_NavMeshSearchClosePoint = 0x106B6FC,
	kVtbl_NavMeshSearchFlee = 0x106B78C,
	kVtbl_NavMeshSearchHide = 0x106B7DC,
	kVtbl_NavMeshSearchLOS = 0x106B830,
	kVtbl_NavMeshSearchMaxCost = 0x106B87C,
	kVtbl_NavMeshSearchSLPoint = 0x106B96C,
	kVtbl_PathSmootherPOVSearch = 0x106B9CC,
	kVtbl_NavMeshInfoMap = 0x106BB8C,
	kVtbl_NavMeshInfoSearch = 0x106C22C,
	kVtbl_ObstacleUndoData = 0x106C2D4,
	kVtbl_bhkObstacleDeactivationListener = 0x106C3B4,
	kVtbl_hkpEntityActivationListener = 0x106C3C4,
	kVtbl_bhkObstacleRemovalListener = 0x106C3D4,
	kVtbl_ReferenceObstacleArray = 0x106C484,
	kVtbl_ObstacleData = 0x106C490,
	kVtbl_BSTasklet = 0x106C5D8,
	kVtbl_ObstacleTaskData = 0x106C7EC,
	kVtbl_BSWin32TaskletData = 0x106C800,
	kVtbl_BSTaskletData = 0x106C814,
	kVtbl_FindTriangleForLocationFilter = 0x106C934,
	kVtbl_CheckLineOfSightFilterWater = 0x106C93C,
	kVtbl_PathingCoverLocation = 0x106CBC0,
	kVtbl_PathingLOSGridMap = 0x106CEAC,
	kVtbl_PathingLOSMap = 0x106CF5C,
	kVtbl_PathingRequest = 0x106D030,
	kVtbl_PathingAvoidNodeArray = 0x106D1E4,
	kVtbl_PathingRequestClosePoint = 0x106D230,
	kVtbl_PathingRequestCover = 0x106D29C,
	kVtbl_PathingRequestCoveredMove = 0x106D2D4,
	kVtbl_PathingRequestFlee = 0x106D300,
	kVtbl_PathingRequestHide = 0x106D32C,
	kVtbl_PathingRequestLOS = 0x106D38C,
	kVtbl_PathingRequestOptimalLocation = 0x106D3B8,
	kVtbl_PathingRequestSafeStraightLine = 0x106D414,
	kVtbl_PathingSearchRayCast = 0x106D47C,
	kVtbl_PathingSolution = 0x106D4BC,
	kVtbl_PathingTaskData = 0x106D550,
	kVtbl_TeleportDoorSearch = 0x106D8FC,
	kVtbl_BGSDistantObjectBlockLoadTask = 0x106DC3C,
	kVtbl_BGSDistantTreeBlockLoadTask = 0x106DED0,
	kVtbl_BGSTerrainChunkLoadTask = 0x106E1DC,
	kVtbl_CCallbackBase = 0x106E7B4,
	kVtbl_FORenderedTerminal = 0x106EBE4,
	kVtbl_TileMenu = 0x106ED44,
	kVtbl_TileRect = 0x106ED70,
	kVtbl_Tile = 0x106ED9C,
	kVtbl_TileImage = 0x106F01C,
	kVtbl_TutorialMenu = 0x106FF84,
	kVtbl_StatsMenu = 0x106FFD4,
	kVtbl_TextEditMenu = 0x1070034,
	kVtbl_SurgeryMenu = 0x1070084,
	kVtbl_VATSMenu = 0x10700D4,
	kVtbl_SpecialBookMenu = 0x1070124,
	kVtbl_LoveTesterMenu = 0x1070174,
	kVtbl_QuantityMenu = 0x10701C4,
	kVtbl_ListBoxBase = 0x1070418,
	kVtbl_RecipeMenu = 0x107048C,
	kVtbl_BarterMenu = 0x10706EC,
	kVtbl_BlackjackMenu = 0x10708FC,
	kVtbl_BookMenu = 0x1070ECC,
	kVtbl_CaravanMenu = 0x107108C,
	kVtbl_CharGenMenu = 0x1071BB4,
	kVtbl_CompanionWheelMenu = 0x1071D0C,
	kVtbl_ComputersMenu = 0x1072004,
	kVtbl_ContainerMenu = 0x10721AC,
	kVtbl_CreditsMenu = 0x107235C,
	kVtbl_DialogMenu = 0x107257C,
	kVtbl_HackingMenu = 0x10728F4,
	kVtbl_HUDMainMenu = 0x1072DF4,
	kVtbl_InventoryMenu = 0x10739B4,
	kVtbl_ItemModMenu = 0x1073B7C,
	kVtbl_LevelUpMenu = 0x1073CDC,
	kVtbl_LoadingMenu = 0x1073EBC,
	kVtbl_LoadingMenuThread = 0x10740AC,
	kVtbl_LockpickMenu = 0x107439C,
	kVtbl_MapMenu = 0x1074D44,
	kVtbl_MessageMenu = 0x107566C,
	kVtbl_PlayerNameEntryMenu = 0x10757FC,
	kVtbl_RaceSexMenu = 0x1075974,
	kVtbl_FORenderedMenuRaceSex = 0x10759EC,
	kVtbl_RepairMenu = 0x1075C5C,
	kVtbl_RepairServicesMenu = 0x1075DB4,
	kVtbl_RouletteMenu = 0x1075F7C,
	kVtbl_SleepWaitMenu = 0x10763AC,
	kVtbl_SlotMachineMenu = 0x10764DC,
	kVtbl_StartMenu = 0x1076D1C,
	kVtbl_TraitMenu = 0x10779BC,
	kVtbl_TraitSelectMenu = 0x1077ABC,
	kVtbl_FOPipboyManager = 0x10780AC,
	kVtbl_FORenderedMenu = 0x107841C,
	kVtbl_AbsorbEffect = 0x10792AC,
	kVtbl_ActiveEffect = 0x107935C,
	kVtbl_AssociatedItemEffect = 0x107951C,
	kVtbl_bhkCharacterListenerSpell = 0x10795E4,
	kVtbl_bhkTelekinesisListener = 0x1079744,
	kVtbl_BoundItemEffect = 0x107979C,
	kVtbl_CalmEffect = 0x107989C,
	kVtbl_ChameleonEffect = 0x107994C,
	kVtbl_CommandCreatureEffect = 0x10799FC,
	kVtbl_CommandEffect = 0x1079A9C,
	kVtbl_CommandHumanoidEffect = 0x1079B3C,
	kVtbl_ConcussionEffect = 0x1079BDC,
	kVtbl_CureEffect = 0x1079C8C,
	kVtbl_DarknessEffect = 0x1079D2C,
	kVtbl_DemoralizeEffect = 0x1079DDC,
	kVtbl_DetectLifeEffect = 0x1079E7C,
	kVtbl_DisintegrateArmorEffect = 0x1079F2C,
	kVtbl_DisintegrateWeaponEffect = 0x1079FFC,
	kVtbl_DispelEffect = 0x107A0CC,
	kVtbl_FrenzyEffect = 0x107A16C,
	kVtbl_InvisibilityEffect = 0x107A21C,
	kVtbl_LightEffect = 0x107A2CC,
	kVtbl_LimbConditionEffect = 0x107A3FC,
	kVtbl_LockEffect = 0x107A4AC,
	kVtbl_MagicBallProjectile = 0x107A554,
	kVtbl_MagicBoltProjectile = 0x107A8F4,
	kVtbl_MagicCaster = 0x107AC5C,
	kVtbl_MagicFogProjectile = 0x107AD84,
	kVtbl_MagicHitEffect = 0x107B0EC,
	kVtbl_MagicModelHitEffect = 0x107B24C,
	kVtbl_MagicProjectile = 0x107B394,
	kVtbl_MagicShaderHitEffect = 0x107B70C,
	kVtbl_MagicSprayProjectile = 0x107B8C4,
	kVtbl_MagicTarget = 0x107BC28,
	kVtbl_NightEyeEffect = 0x107BD7C,
	kVtbl_NonActorMagicCaster = 0x107BE80,
	kVtbl_NonActorMagicTarget = 0x107BECC,
	kVtbl_OpenEffect = 0x107BF1C,
	kVtbl_ParalysisEffect = 0x107BFBC,
	kVtbl_ReanimateEffect = 0x107C06C,
	kVtbl_ScriptEffect = 0x107C164,
	kVtbl_ShieldEffect = 0x107C1FC,
	kVtbl_SummonCreatureEffect = 0x107C2AC,
	kVtbl_SunDamageEffect = 0x107C3EC,
	kVtbl_TelekinesisEffect = 0x107C48C,
	kVtbl_TurnUndeadEffect = 0x107C53C,
	kVtbl_ValueAndConditionsEffect = 0x107C5DC,
	kVtbl_ValueModifierEffect = 0x107C68C,
	kVtbl_VampirismEffect = 0x107C7BC,
	kVtbl_CBSAStream = 0x107CB4C,
	kVtbl_CAsyncStream = 0x107CB6C,
	kVtbl_CBSAReader = 0x107CDAC,
	kVtbl_RadioConvTask = 0x107CF18,
	kVtbl_BGSReconstructFormsMap = 0x107E930,
	kVtbl_BGSReconstructCellsMap = 0x107E950,
	kVtbl_BGSReconstructCellSubBlocksMap = 0x107E970,
	kVtbl_BGSReconstructCellBlocksMap = 0x107E990,
	kVtbl_BGSReconstructWorldSpacesMap = 0x107E9B0,
	kVtbl_BGSReconstructFormsInFileMap = 0x107E9D0,
	kVtbl_BGSOffsetDataArray = 0x107EA60,
	kVtbl_BGSReconstructFormsInAllFilesMap = 0x107EA74,
	kVtbl_BGSSaveLoadChangesMap = 0x107ED68,
	kVtbl_BGSCellNumericIDArrayMap = 0x107FEF4,
	kVtbl_BGSSaveLoadStatsMap = 0x1080174,
	kVtbl_ChangesMap = 0x10804A8,
	kVtbl_InteriorCellNewReferencesMap = 0x1080514,
	kVtbl_ExteriorCellNewReferencesMap = 0x1080534,
	kVtbl_NumericIDBufferMap = 0x1080554,
	kVtbl_SaveGameFile = 0x1081A5C,
	kVtbl_BGSLoadFormBuffer = 0x1082028,
	kVtbl_BGSLoadGameBuffer = 0x1082048,
	kVtbl_BGSSaveFormBuffer = 0x1082308,
	kVtbl_BGSSaveGameBuffer = 0x1082324,
	kVtbl_BGSMoviePlayer = 0x1082564,
	kVtbl_HighActorCuller = 0x1082CC0,
	kVtbl_BSFadeNodeCuller = 0x1082CCC,
	kVtbl_RegSettingCollection = 0x1082ED8,
	kVtbl_MemoryManagerFile = 0x1083AE8,
	kVtbl_IMemoryManagerFile = 0x1083B00,
	kVtbl_SceneGraph = 0x1083B5C,
	kVtbl_SeenData = 0x1083FC4,
	kVtbl_IntSeenData = 0x1083FE4,
	kVtbl_Actor = 0x1084254,
	kVtbl_CachedValuesOwner = 0x1084734,
	kVtbl_bhkFilteredCdBodyCollector = 0x1084E54,
	kVtbl_AILinearTaskThread = 0x1085688,
	kVtbl_BSLinearTaskThread = 0x10856A0,
	kVtbl_DetectionTaskData = 0x1085728,
	kVtbl_MobileObjectTaskletData = 0x1085748,
	kVtbl_AnimationTaskData = 0x1085768,
	kVtbl_ActorUpdateTaskData = 0x1085788,
	kVtbl_PackageUpdateTaskData = 0x10857A8,
	kVtbl_ActorsScriptTaskData = 0x10857C8,
	kVtbl_MovementTaskData = 0x10857E8,
	kVtbl_AITaskThread = 0x1085818,
	kVtbl_ArrowProjectile = 0x1085954,
	kVtbl_BaseProcess = 0x1085E2C,
	kVtbl_bhkCharacterListenerArrow = 0x1086684,
	kVtbl_BSPlayerDistanceCheckController = 0x10867FC,
	kVtbl_Character = 0x1086A6C,
	kVtbl_Creature = 0x10870AC,
	kVtbl_HighProcess = 0x1087864,
	kVtbl_ActorPathingMessageQueue = 0x1088158,
	kVtbl_LipTask = 0x10885B8,
	kVtbl_LowProcess = 0x10886E4,
	kVtbl_MiddleHighProcess = 0x108904C,
	kVtbl_MiddleLowProcess = 0x1089BCC,
	kVtbl_MobileObject = 0x108A49C,
	kVtbl_PlayerCharacter = 0x108AA3C,
	kVtbl_BeamProjectile = 0x108C3C4,
	kVtbl_CombatAction = 0x108CAB8,
	kVtbl_CombatActionAttackRanged = 0x108CB50,
	kVtbl_CombatActionAttackRangedFromCover = 0x108CB7C,
	kVtbl_CombatActionAttackRangedExplosive = 0x108CB94,
	kVtbl_CombatActionAttackRangedExplosiveFromCover = 0x108CBAC,
	kVtbl_CombatActionAttackMelee = 0x108CBC4,
	kVtbl_CombatActionAttackHandToHand = 0x108CBDC,
	kVtbl_CombatActionAttackGrenade = 0x108CBF4,
	kVtbl_CombatActionAttackGrenadeFlushTarget = 0x108CC0C,
	kVtbl_CombatActionAttackGrenadeFromCover = 0x108CC24,
	kVtbl_CombatActionMove = 0x108CC3C,
	kVtbl_CombatActionMoveAndSwitchToMelee = 0x108CC54,
	kVtbl_CombatActionMoveAndAttackRanged = 0x108CC6C,
	kVtbl_CombatActionMoveAndAttackRangedExplosive = 0x108CC84,
	kVtbl_CombatActionMoveAndAttackGrenade = 0x108CC9C,
	kVtbl_CombatActionDrawWeapon = 0x108CCB4,
	kVtbl_CombatActionSwitchWeapon = 0x108CCCC,
	kVtbl_CombatActionAvoidThreat = 0x108CCE4,
	kVtbl_CombatActionSearch = 0x108CCFC,
	kVtbl_CombatActionInvestigate = 0x108CD14,
	kVtbl_CombatActionDodge = 0x108CD2C,
	kVtbl_CombatActionIgnoreBlockedTarget = 0x108CD44,
	kVtbl_CombatActionFlee = 0x108CD5C,
	kVtbl_CombatActionActivateCombatItem = 0x108CD74,
	kVtbl_CombatActionUseCombatItem = 0x108CD8C,
	kVtbl_CombatActionApproachTarget = 0x108CDA4,
	kVtbl_CombatActionAcquireLineOfSight = 0x108CDBC,
	kVtbl_CombatActionHide = 0x108CDD4,
	kVtbl_CombatController = 0x108CE74,
	kVtbl_CombatPlanner = 0x108CFD8,
	kVtbl_CombatCoverMap = 0x108D0E4,
	kVtbl_CombatGoal = 0x108D440,
	kVtbl_CombatGoalKillTarget = 0x108D47C,
	kVtbl_CombatGoalFindTarget = 0x108D488,
	kVtbl_CombatGoalAvoidThreats = 0x108D494,
	kVtbl_CombatGoalFlee = 0x108D4A0,
	kVtbl_CombatGoalActivateCombatItem = 0x108D4AC,
	kVtbl_CombatGoalUseCombatItem = 0x108D4B8,
	kVtbl_CombatGoalFlank = 0x108D4C4,
	kVtbl_CombatGoalFallBack = 0x108D4D0,
	kVtbl_CombatGoalAmbush = 0x108D4DC,
	kVtbl_CombatGoalReachDestination = 0x108D4E8,
	kVtbl_CombatGoalAdvance = 0x108D4F4,
	kVtbl_CombatGoalGuard = 0x108D500,
	kVtbl_CombatGoalCluster = 0x108D50C,
	kVtbl_CombatGoalHideFromTarget = 0x108D518,
	kVtbl_CombatGoalFlushTarget = 0x108D524,
	kVtbl_CombatGoalStayInCombatArea = 0x108D530,
	kVtbl_CombatGroupStrategyFlank = 0x108D93C,
	kVtbl_CombatGroupStrategy = 0x108D94C,
	kVtbl_CombatGroupStrategyFallBack = 0x108D95C,
	kVtbl_CombatGroupStrategyAmbush = 0x108D96C,
	kVtbl_CombatGroupStrategyAdvance = 0x108D97C,
	kVtbl_CombatTargetMap = 0x108E1E8,
	kVtbl_CombatThreatMap = 0x108E2E8,
	kVtbl_ContinuousBeamProjectile = 0x108EA64,
	kVtbl_Explosion = 0x108EE04,
	kVtbl_bhkLiquidAction = 0x108F1BC,
	kVtbl_hkpFixedBufferCdPointCollector = 0x108F288,
	kVtbl_FlameProjectile = 0x108F2F4,
	kVtbl_GrenadeProjectile = 0x108F674,
	kVtbl_MissileProjectile = 0x108FA44,
	kVtbl_bhkHingeConstraint = 0x108FDDC,
	kVtbl_bhkConstraint = 0x108FEBC,
	kVtbl_hkHingeConstraintCinfo = 0x108FF98,
	kVtbl_hkConstraintCinfo = 0x108FFB4,
	kVtbl_Projectile = 0x10900DC,
	kVtbl_ProjectileListener = 0x1090594,
	kVtbl_CombatProcedureActivateObject = 0x109082C,
	kVtbl_CombatProcedure = 0x1090874,
	kVtbl_CombatProcedureAttackGrenade = 0x10909BC,
	kVtbl_CombatProcedureAttackLow = 0x1090AEC,
	kVtbl_CombatProcedureAttackMelee = 0x1090C04,
	kVtbl_CombatProcedureAttackRanged = 0x1090ED4,
	kVtbl_CombatProcedureBeInCover = 0x10910F4,
	kVtbl_CombatProcedureEngageTarget = 0x1091514,
	kVtbl_CombatProcedureEvade = 0x1091984,
	kVtbl_CombatProcedureHideFromTarget = 0x1091A64,
	kVtbl_CombatProcedureMove = 0x1091BB4,
	kVtbl_CombatProcedureSearch = 0x1091DB4,
	kVtbl_CombatProcedureSwitchWeapon = 0x10920CC,
	kVtbl_CombatProcedureUseCombatItem = 0x109214C,
	kVtbl_ActorMover = 0x109223C,
	kVtbl_ActorPathHandler = 0x109231C,
	kVtbl_DetailedActorPathHandler = 0x10923EC,
	kVtbl_DoorInPathFilter = 0x1092484,
	kVtbl_PlayerMover = 0x109297C,
	kVtbl_VirtualActorPathHandler = 0x10929FC,
	kVtbl_AlarmPackage = 0x1092BDC,
	kVtbl_BackUpPackage = 0x1092D84,
	kVtbl_DialoguePackage = 0x1092F6C,
	kVtbl_EscortActorPackageData = 0x1093188,
	kVtbl_FleePackage = 0x10931E4,
	kVtbl_GuardActorPackageData = 0x10933C8,
	kVtbl_PatrolActorPackageData = 0x1093418,
	kVtbl_SandBoxActorPackageData = 0x10935F8,
	kVtbl_SearchPackage = 0x1093674,
	kVtbl_SpectatorPackage = 0x109381C,
	kVtbl_TrespassPackage = 0x10939EC,
	kVtbl_CEnumPins = 0x1093C50,
	kVtbl_CEnumMediaTypes = 0x1093C74,
	kVtbl_CMediaSample = 0x1093C9C,
	kVtbl_CMemAllocator = 0x1093D28,
	kVtbl_CAsyncOutputPin = 0x1093E24,
	kVtbl_CAsyncReader = 0x1093EAC,
	kVtbl_TileText = 0x1094878,
	kVtbl_Tile3D = 0x10948AC,
	kVtbl_TileExtra = 0x1094CFC,
	kVtbl_DebugText = 0x1094EFC,
	kVtbl_IDebugText = 0x1094F30,
	kVtbl_DebugTextExtraData = 0x1094FB4,
	kVtbl_Menu = 0x1095484,
	kVtbl_RadialTile = 0x1095750,
	kVtbl_NiAVObjectPalette = 0x109607C,
	kVtbl_NiControllerManager = 0x109619C,
	kVtbl_NiDefaultAVObjectPalette = 0x1096284,
	kVtbl_NiInterpController = 0x109632C,
	kVtbl_NiMultiTargetTransformController = 0x1096444,
	kVtbl_NiControllerSequence = 0x1096584,
	kVtbl_BSMultiTargetTreadTransfController = 0x1096694,
	kVtbl_NiBlendAccumTransformInterpolator = 0x1096784,
	kVtbl_NiTransformController = 0x109687C,
	kVtbl_NiBlendInterpolator = 0x109697C,
	kVtbl_NiSingleInterpController = 0x1096BD4,
	kVtbl_NiGeomMorpherController = 0x1096CEC,
	kVtbl_NiColorInterpolator = 0x1096E14,
	kVtbl_NiFloatInterpolator = 0x1096F34,
	kVtbl_NiKeyBasedInterpolator = 0x1097054,
	kVtbl_NiPosData = 0x109717C,
	kVtbl_NiPathInterpolator = 0x109720C,
	kVtbl_NiTransformInterpolator = 0x1097314,
	kVtbl_NiBlendFloatInterpolator = 0x1097414,
	kVtbl_NiBlendTransformInterpolator = 0x109750C,
	kVtbl_NiFloatExtraDataController = 0x10975FC,
	kVtbl_NiMorphData = 0x10976FC,
	kVtbl_BSFrustumFOVController = 0x109780C,
	kVtbl_BSBlendTreadTransfInterpolator = 0x1097904,
	kVtbl_BSTreadTransfInterpolator = 0x10979F4,
	kVtbl_BSRotAccumTransfInterpolator = 0x1097AF4,
	kVtbl_BSAnimNote = 0x1097BF4,
	kVtbl_BSLookIKNote = 0x1097C8C,
	kVtbl_BSGrabIKNote = 0x1097D34,
	kVtbl_BSAnimNotes = 0x1097DD4,
	kVtbl_NiVisController = 0x1097E8C,
	kVtbl_NiUVData = 0x1097F84,
	kVtbl_NiUVController = 0x1098064,
	kVtbl_NiTextureTransformController = 0x1098184,
	kVtbl_NiTextKeyExtraData = 0x109827C,
	kVtbl_NiStringPalette = 0x109832C,
	kVtbl_NiSequenceStreamHelper = 0x10983BC,
	kVtbl_NiSequence = 0x1098454,
	kVtbl_NiRotData = 0x10984EC,
	kVtbl_NiFloatController = 0x109857C,
	kVtbl_NiRollController = 0x1098634,
	kVtbl_NiQuaternionInterpolator = 0x10986F4,
	kVtbl_NiPoint3Interpolator = 0x109881C,
	kVtbl_NiPathController = 0x109893C,
	kVtbl_NiMaterialColorController = 0x1098A14,
	kVtbl_NiLookAtInterpolator = 0x1098B0C,
	kVtbl_NiLookAtController = 0x1098C14,
	kVtbl_NiLightDimmerController = 0x1098CCC,
	kVtbl_NiLightColorController = 0x1098DD4,
	kVtbl_NiKeyframeManager = 0x1098F4C,
	kVtbl_NiTransformData = 0x1099004,
	kVtbl_NiFloatsExtraDataPoint3Controller = 0x109917C,
	kVtbl_NiFloatsExtraDataController = 0x109928C,
	kVtbl_NiFlipController = 0x1099384,
	kVtbl_NiColorExtraDataController = 0x109947C,
	kVtbl_NiBSplineTransformInterpolator = 0x109957C,
	kVtbl_NiBSplinePoint3Interpolator = 0x10996DC,
	kVtbl_NiBSplineFloatInterpolator = 0x10997F4,
	kVtbl_NiBSplineCompTransformInterpolator = 0x109991C,
	kVtbl_NiBSplineCompPoint3Interpolator = 0x1099A24,
	kVtbl_NiBSplineCompFloatInterpolator = 0x1099B2C,
	kVtbl_NiBSplineCompColorInterpolator = 0x1099C34,
	kVtbl_NiBSplineColorInterpolator = 0x1099D3C,
	kVtbl_NiBSplineData = 0x1099E54,
	kVtbl_NiBSplineBasisData = 0x1099F24,
	kVtbl_NiBoolTimelineInterpolator = 0x1099FB4,
	kVtbl_NiBoolInterpolator = 0x109A0B4,
	kVtbl_NiBoolData = 0x109A1D4,
	kVtbl_NiBoneLODController = 0x109A2D4,
	kVtbl_NiBlendQuaternionInterpolator = 0x109A3AC,
	kVtbl_NiBlendPoint3Interpolator = 0x109A49C,
	kVtbl_NiBlendColorInterpolator = 0x109A58C,
	kVtbl_NiBlendBoolInterpolator = 0x109A67C,
	kVtbl_NiAlphaController = 0x109A76C,
	kVtbl_NiGeomMorpherUpdateTask = 0x109A864,
	kVtbl_NiInterpolator = 0x109A924,
	kVtbl_NiExtraDataController = 0x109AA14,
	kVtbl_NiFloatInterpController = 0x109AB44,
	kVtbl_NiBoolInterpController = 0x109AC54,
	kVtbl_NiPoint3InterpController = 0x109AD74,
	kVtbl_NiBSplineInterpolator = 0x109AE8C,
	kVtbl_NiAVObject = 0x109B00C,
	kVtbl_NiObjectNET = 0x109B244,
	kVtbl_NiObject = 0x109B4F4,
	kVtbl_NiNode = 0x109B5AC,
	kVtbl_NiVertexColorProperty = 0x109B72C,
	kVtbl_NiZBufferProperty = 0x109B7FC,
	kVtbl_NiTexture = 0x109B944,
	kVtbl_NiSourceTexture = 0x109B9EC,
	kVtbl_NiSourceCubeMap = 0x109BABC,
	kVtbl_NiRenderer = 0x109BB94,
	kVtbl_NiStream = 0x109BDCC,
	kVtbl_NiGeometryData = 0x109BF9C,
	kVtbl_NiDynamicEffect = 0x109C174,
	kVtbl_NiCullingProcess = 0x109C28C,
	kVtbl_NiTexturingProperty = 0x109C514,
	kVtbl_NiTimeController = 0x109C874,
	kVtbl_NiCamera = 0x109CB9C,
	kVtbl_NiTriStrips = 0x109CD44,
	kVtbl_NiLight = 0x109CE4C,
	kVtbl_NiStringExtraData = 0x109D39C,
	kVtbl_NiTriShape = 0x109D454,
	kVtbl_NiLines = 0x109D55C,
	kVtbl_NiMaterialProperty = 0x109D6C4,
	kVtbl_NiDirectionalLight = 0x109D7B4,
	kVtbl_NiTriStripsData = 0x109D8AC,
	kVtbl_NiImageConverter = 0x109D9A4,
	kVtbl_NiDevImageConverter = 0x109D9C4,
	kVtbl_NiExtraData = 0x109DA1C,
	kVtbl_NiTriShapeData = 0x109DAC4,
	kVtbl_NiPixelData = 0x109DBB4,
	kVtbl_NiPointLight = 0x109DD0C,
	kVtbl_NiGeometryGroupManager = 0x109DF2C,
	kVtbl_NiRenderedTexture = 0x109DF8C,
	kVtbl_NiGeometry = 0x109E04C,
	kVtbl_NiCollisionObject = 0x109E174,
	kVtbl_NiShadeProperty = 0x109E274,
	kVtbl_NiTriShapeDynamicData = 0x109E32C,
	kVtbl_NiSkinPartition = 0x109E47C,
	kVtbl_NiSkinData = 0x109E574,
	kVtbl_BSResizableTriShape = 0x109E704,
	kVtbl_BSSegmentedTriShape = 0x109E834,
	kVtbl_NiScreenElements = 0x109E984,
	kVtbl_NiScreenElementsData = 0x109EA94,
	kVtbl_NiFloatsExtraData = 0x109EC0C,
	kVtbl_NiColorExtraData = 0x109ECCC,
	kVtbl_NiTriBasedGeom = 0x109ED94,
	kVtbl_NiVertWeightsExtraData = 0x109EE9C,
	kVtbl_NiLinesData = 0x109F23C,
	kVtbl_NiTriBasedGeomData = 0x109F2F4,
	kVtbl_NiImageReader = 0x109F3D0,
	kVtbl_NiNIFImageReader = 0x109F3E4,
	kVtbl_NiPalette = 0x109F3FC,
	kVtbl_NiDDSReader = 0x109F48C,
	kVtbl_NiTGAReader = 0x109F4B0,
	kVtbl_NiBMPReader = 0x109F4C4,
	kVtbl_Ni2DBuffer = 0x109F4DC,
	kVtbl_NiShader = 0x109F584,
	kVtbl_NiCullingProperty = 0x109F68C,
	kVtbl_NiMaterial = 0x109F73C,
	kVtbl_NiSingleShaderMaterialLibrary = 0x109F7B4,
	kVtbl_NiMaterialLibrary = 0x109F7F4,
	kVtbl_NiScreenGeometry = 0x109F81C,
	kVtbl_NiScreenPolygon = 0x109F914,
	kVtbl_NiScreenGeometryData = 0x109F9C4,
	kVtbl_NiScreenSpaceCamera = 0x109FAB4,
	kVtbl_NiVectorExtraData = 0x109FB94,
	kVtbl_NiTriStripsDynamicData = 0x109FC54,
	kVtbl_NiTextureEffect = 0x109FD0C,
	kVtbl_NiSwitchStringExtraData = 0x109FF64,
	kVtbl_NiSwitchNode = 0x10A001C,
	kVtbl_NiStringsExtraData = 0x10A0134,
	kVtbl_NiSpotLight = 0x10A01CC,
	kVtbl_NiSpecularProperty = 0x10A02E4,
	kVtbl_NiSortAdjustNode = 0x10A0384,
	kVtbl_NiScreenTexture = 0x10A048C,
	kVtbl_NiScreenLODData = 0x10A051C,
	kVtbl_NiParticlesData = 0x10A05E4,
	kVtbl_NiRangeLODData = 0x10A073C,
	kVtbl_NiPersistentSrcTextureRendererData = 0x10A08AC,
	kVtbl_NiParticleMeshesData = 0x10A09BC,
	kVtbl_NiParticleMeshes = 0x10A0A6C,
	kVtbl_NiLODNode = 0x10A0B64,
	kVtbl_NiIntegersExtraData = 0x10A0C7C,
	kVtbl_NiIntegerExtraData = 0x10A0D24,
	kVtbl_NiFogProperty = 0x10A0E0C,
	kVtbl_NiFloatExtraData = 0x10A0EC4,
	kVtbl_NiDitherProperty = 0x10A0F6C,
	kVtbl_NiBSPNode = 0x10A1024,
	kVtbl_NiBooleanExtraData = 0x10A112C,
	kVtbl_NiParticles = 0x10A11D4,
	kVtbl_NiAmbientLight = 0x10A12CC,
	kVtbl_NiAlphaAccumulator = 0x10A13AC,
	kVtbl_NiShaderConstantMap = 0x10A1478,
	kVtbl_NiDepthStencilBuffer = 0x10A2084,
	kVtbl_NiGlobalConstantEntry = 0x10A2148,
	kVtbl_NiShaderFactory = 0x10A2164,
	kVtbl_NiSingleShaderMaterial = 0x10A21DC,
	kVtbl_NiLODData = 0x10A2254,
	kVtbl_NiAccumulator = 0x10A2304,
	kVtbl_NiBackToFrontAccumulator = 0x10A23AC,
	kVtbl_NiFile = 0x10A2490,
	kVtbl_NiBinaryStream = 0x10A24B8,
	kVtbl_NiAllocator = 0x10A24D0,
	kVtbl_NiStandardAllocator = 0x10A24FC,
	kVtbl_BSNiAllocator = 0x10A2528,
	kVtbl_NiSearchPath = 0x10A2554,
	kVtbl_NiMemStream = 0x10A256C,
	kVtbl_BSCoreMessage = 0x10A2708,
	kVtbl_BSThread = 0x10A27D4,
	kVtbl_ZeroOverheadHeap = 0x10A2884,
	kVtbl_IMemoryHeap = 0x10A28C8,
	kVtbl_MemoryHeap = 0x10A28FC,
	kVtbl_AbstractHeap = 0x10A2BBC,
	kVtbl_FutBinaryFileC = 0x10A2C20,
	kVtbl_BSFaceGenBinaryFile = 0x10A2C38,
	kVtbl_BSAudio = 0x10A323C,
	kVtbl_BSAudioManager = 0x10A3368,
	kVtbl_BSAudioManagerThread = 0x10A33D0,
	kVtbl_AudioLoadTask = 0x10A3718,
	kVtbl_BSGameSound = 0x10A3964,
	kVtbl_BSWin32Audio = 0x10A39D8,
	kVtbl_BSWin32AudioListener = 0x10A3B7C,
	kVtbl_BSAudioListener = 0x10A3BB4,
	kVtbl_BSWin32GameSound = 0x10A3BF4,
	kVtbl_BSSaveDataSystemUtility = 0x10A4288,
	kVtbl_BSSystemUtility = 0x10A42B4,
	kVtbl_BSMsgDialogSystemUtility = 0x10A42CC,
	kVtbl_BSSystemUtilitiesStrings = 0x10A42DC,
	kVtbl_BSCacheDriveSystemUtility = 0x10A42EC,
	kVtbl_BSGameDataSystemUtility = 0x10A42FC,
	kVtbl_BSDiscBootSystemUtility = 0x10A4318,
	kVtbl_BSSysInfoSystemUtility = 0x10A4328,
	kVtbl_Archive = 0x10A45AC,
	kVtbl_ArchiveFile = 0x10A45FC,
	kVtbl_CompressedArchiveFile = 0x10A464C,
	kVtbl_BSFile = 0x10A4764,
	kVtbl_BSTaskletManagerCallback = 0x10A47B8,
	kVtbl_BSFileCache = 0x10A47CC,
	kVtbl_BSSearchPath = 0x10A4810,
	kVtbl_BSWin32FileCache = 0x10A483C,
	kVtbl_BSWin32TaskletManager = 0x10A4880,
	kVtbl_BSTaskletManager = 0x10A48AC,
	kVtbl_BSWin32TaskletGroupData = 0x10A48D8,
	kVtbl_BSTaskletGroupData = 0x10A48E8,
	kVtbl_IdvFileError = 0x10A51E0,
	kVtbl_CTreeEngine = 0x10A5670,
	kVtbl_CIdvCamera = 0x10A5890,
	kVtbl_Uniform = 0x10A58A0,
	kVtbl_Random = 0x10A58C8,
	kVtbl_CBillboardLeaf = 0x10A58FC,
	kVtbl_PosGen = 0x10A59D4,
	kVtbl_Normal = 0x10A5A2C,
	kVtbl_SymGen = 0x10A5A54,
	kVtbl_BSFadeNode = 0x10A8F90,
	kVtbl_DistantLODShaderProperty = 0x10ADAD0,
	kVtbl_TallGrassShaderProperty = 0x10ADEA8,
	kVtbl_BSShaderPPLightingProperty = 0x10AE0D0,
	kVtbl_WaterShaderProperty = 0x10AE438,
	kVtbl_BSShaderNoLightingProperty = 0x10AE670,
	kVtbl_BSShaderLightingProperty = 0x10AE7B0,
	kVtbl_BSShaderBloodSplatterProperty = 0x10AF8E8,
	kVtbl_BSBatchRenderer = 0x10B7DC8,
	kVtbl_LightingData = 0x10B7EB8,
	kVtbl_ParticleShader = 0x10B80A8,
	kVtbl_ParticleShaderProperty = 0x10B8250,
	kVtbl_SpeedTreeBillboardShaderProperty = 0x10B8330,
	kVtbl_BSShaderProperty = 0x10B8488,
	kVtbl_TallGrassShader = 0x10B8980,
	kVtbl_SkyShaderProperty = 0x10B8CE0,
	kVtbl_PrecipitationShader = 0x10B8E80,
	kVtbl_PrecipitationShaderProperty = 0x10B9008,
	kVtbl_SpeedTreeLeafShader = 0x10B9190,
	kVtbl_SpeedTreeBranchShaderProperty = 0x10B9338,
	kVtbl_SpeedTreeShaderPPLightingProperty = 0x10B9490,
	kVtbl_SpeedTreeShaderLightingProperty = 0x10B95C0,
	kVtbl_SpeedTreeLeafShaderProperty = 0x10B9700,
	kVtbl_GeometryDecalShaderProperty = 0x10B9828,
	kVtbl_Lighting30ShaderProperty = 0x10B9910,
	kVtbl_BSDistantTreeShader = 0x10B9AA8,
	kVtbl_BSDistantTreeShaderProperty = 0x10B9C40,
	kVtbl_TileShaderProperty = 0x10B9D28,
	kVtbl_BSFogProperty = 0x10B9E38,
	kVtbl_Lighting30Shader = 0x10BA0F8,
	kVtbl_BoltShader = 0x10BA3B0,
	kVtbl_BoltShaderProperty = 0x10BA528,
	kVtbl_BSClearZNode = 0x10BA608,
	kVtbl_BSOrderedNode = 0x10BA730,
	kVtbl_BSMaterialEmittanceMultController = 0x10BA858,
	kVtbl_BSRefractionFirePeriodController = 0x10BA948,
	kVtbl_BSRefractionStrengthController = 0x10BAA38,
	kVtbl_VolumetricFogShaderProperty = 0x10BAB28,
	kVtbl_HairShaderProperty = 0x10BABF8,
	kVtbl_BeamShader = 0x10BAD98,
	kVtbl_VolumetricFogShader = 0x10BAF60,
	kVtbl_BSShaderNoLighting = 0x10BB230,
	kVtbl_TileShader = 0x10BB410,
	kVtbl_GeometryDecalShader = 0x10BB588,
	kVtbl_ParallaxShader = 0x10BB7A8,
	kVtbl_SkinShader = 0x10BB980,
	kVtbl_HairShader = 0x10BBB50,
	kVtbl_SpeedTreeBranchShader = 0x10BC070,
	kVtbl_DistantLODShader = 0x10BC260,
	kVtbl_BSCubeMapCamera = 0x10BC470,
	kVtbl_NiParticleSystem = 0x10BD44C,
	kVtbl_NiPSysEmitterCtlr = 0x10BD594,
	kVtbl_NiPSysMeshEmitter = 0x10BD7FC,
	kVtbl_NiPSysCylinderEmitter = 0x10BD8B4,
	kVtbl_NiPSysSphereEmitter = 0x10BD974,
	kVtbl_NiPSysBoxEmitter = 0x10BDA2C,
	kVtbl_NiPSysRotationModifier = 0x10BDAEC,
	kVtbl_NiPSysMeshUpdateModifier = 0x10BDC4C,
	kVtbl_NiMeshParticleSystem = 0x10BDD84,
	kVtbl_NiPSysEmitter = 0x10BDE84,
	kVtbl_NiPSysGravityModifier = 0x10BDFF4,
	kVtbl_NiPSysSpawnModifier = 0x10BE124,
	kVtbl_NiPSysData = 0x10BE254,
	kVtbl_BSStripParticleSystem = 0x10BE35C,
	kVtbl_NiPSysUpdateTask = 0x10BE45C,
	kVtbl_BSStripPSysData = 0x10BE514,
	kVtbl_NiPSysVortexFieldModifier = 0x10BE604,
	kVtbl_NiPSysUpdateCtlr = 0x10BE6B4,
	kVtbl_NiPSysTurbulenceFieldModifier = 0x10BE774,
	kVtbl_NiPSysSphericalCollider = 0x10BE834,
	kVtbl_NiPSysResetOnLoopCtlr = 0x10BE8E4,
	kVtbl_NiPSysRadialFieldModifier = 0x10BE99C,
	kVtbl_NiPSysPositionModifier = 0x10BEA54,
	kVtbl_NiPSysPlanarCollider = 0x10BEB0C,
	kVtbl_NiPSysModifierActiveCtlr = 0x10BEBBC,
	kVtbl_NiPSysInitialRotSpeedVarCtlr = 0x10BECBC,
	kVtbl_NiPSysInitialRotSpeedCtlr = 0x10BEDBC,
	kVtbl_NiPSysInitialRotAngleVarCtlr = 0x10BEEBC,
	kVtbl_NiPSysInitialRotAngleCtlr = 0x10BEFBC,
	kVtbl_NiPSysGrowFadeModifier = 0x10BF0BC,
	kVtbl_NiPSysGravityStrengthCtlr = 0x10BF1AC,
	kVtbl_NiPSysGravityFieldModifier = 0x10BF2AC,
	kVtbl_NiPSysFieldMaxDistanceCtlr = 0x10BF354,
	kVtbl_NiPSysFieldMagnitudeCtlr = 0x10BF454,
	kVtbl_NiPSysFieldAttenuationCtlr = 0x10BF554,
	kVtbl_NiPSysEmitterSpeedCtlr = 0x10BF654,
	kVtbl_NiPSysEmitterPlanarAngleVarCtlr = 0x10BF754,
	kVtbl_NiPSysEmitterPlanarAngleCtlr = 0x10BF854,
	kVtbl_NiPSysEmitterLifeSpanCtlr = 0x10BF954,
	kVtbl_NiPSysEmitterInitialRadiusCtlr = 0x10BFA54,
	kVtbl_NiPSysEmitterDeclinationVarCtlr = 0x10BFB54,
	kVtbl_NiPSysEmitterDeclinationCtlr = 0x10BFC54,
	kVtbl_NiPSysEmitterCtlrData = 0x10BFD54,
	kVtbl_NiPSysDragModifier = 0x10BFE44,
	kVtbl_NiPSysDragFieldModifier = 0x10BFF2C,
	kVtbl_NiPSysColorModifier = 0x10BFFE4,
	kVtbl_NiPSysColliderManager = 0x10C008C,
	kVtbl_NiPSysBoundUpdateModifier = 0x10C0134,
	kVtbl_NiPSysBombModifier = 0x10C01EC,
	kVtbl_NiPSysAgeDeathModifier = 0x10C0304,
	kVtbl_NiPSysAirFieldSpreadCtlr = 0x10C03CC,
	kVtbl_NiPSysAirFieldModifier = 0x10C04CC,
	kVtbl_NiPSysAirFieldInheritVelocityCtlr = 0x10C05E4,
	kVtbl_NiPSysAirFieldAirFrictionCtlr = 0x10C06E4,
	kVtbl_NiMeshPSysData = 0x10C07EC,
	kVtbl_NiPSysModifierCtlr = 0x10C08AC,
	kVtbl_NiPSysModifier = 0x10C09CC,
	kVtbl_NiPSysVolumeEmitter = 0x10C0A8C,
	kVtbl_NiPSysFieldModifier = 0x10C0B5C,
	kVtbl_NiPSysCollider = 0x10C0C4C,
	kVtbl_NiPSysModifierBoolCtlr = 0x10C0D2C,
	kVtbl_NiPSysModifierFloatCtlr = 0x10C0E44,
	kVtbl_BSMultiBound = 0x10C0F7C,
	kVtbl_BSMultiBoundRoom = 0x10C1064,
	kVtbl_BSReference = 0x10C1364,
	kVtbl_BSNodeReferences = 0x10C13F4,
	kVtbl_MessageHandler = 0x10C14A8,
	kVtbl_QueuedChildren = 0x10C14E8,
	kVtbl_QueuedParents = 0x10C1510,
	kVtbl_QueuedFile = 0x10C1524,
	kVtbl_QueuedFileEntry = 0x10C1554,
	kVtbl_IOManager = 0x10C1604,
	kVtbl_BSTaskThread = 0x10C1844,
	kVtbl_BSXFlags = 0x10C1854,
	kVtbl_BSValueNode = 0x10C195C,
	kVtbl_BSParallelTaskThread = 0x10C1AF0,
	kVtbl_BSRangeNode = 0x10C1B2C,
	kVtbl_BSWindModifier = 0x10C1C6C,
	kVtbl_BSMultiBoundNode = 0x10C1D14,
	kVtbl_BSBlastNode = 0x10C1EB4,
	kVtbl_BSDebrisNode = 0x10C1FC4,
	kVtbl_BSDamageStage = 0x10C20D4,
	kVtbl_BSPortalSharedNode = 0x10C21FC,
	kVtbl_BSMultiBoundShape = 0x10C2324,
	kVtbl_BSParticleSystemManager = 0x10C23FC,
	kVtbl_UpdateMasterPsysTaskletData = 0x10C256C,
	kVtbl_BSSceneGraph = 0x10C2714,
	kVtbl_NiBSBoneLODController = 0x10C29CC,
	kVtbl_BSFurnitureMarker = 0x10C2A94,
	kVtbl_BSBound = 0x10C2B64,
	kVtbl_BSPortalGraph = 0x10C2D44,
	kVtbl_BSPSysMultiTargetEmitterCtlr = 0x10C2D54,
	kVtbl_BSScissorTriShape = 0x10C2E7C,
	kVtbl_BSMasterParticleSystem = 0x10C2FB4,
	kVtbl_BSTextureData = 0x10C30F4,
	kVtbl_BSNiNode = 0x10C31A4,
	kVtbl_BSPSysStripUpdateModifier = 0x10C32C4,
	kVtbl_BSPSysSimpleColorModifier = 0x10C3384,
	kVtbl_BSMultiBoundCapsule = 0x10C34AC,
	kVtbl_BSWArray = 0x10C357C,
	kVtbl_PArrayPoint = 0x10C361C,
	kVtbl_BSPSysArrayEmitter = 0x10C36C4,
	kVtbl_BSParentVelocityModifier = 0x10C37FC,
	kVtbl_BSDecalPlacementVectorExtraData = 0x10C38AC,
	kVtbl_BSAttachPoints = 0x10C396C,
	kVtbl_bhkMemoryAllocator = 0x10C3AF8,
	kVtbl_hkMemoryAllocator = 0x10C3B30,
	kVtbl_bhkOnStartupCallback = 0x10C3B68,
	kVtbl_bhkOnExitCallback = 0x10C3B74,
	kVtbl_hkpCollidableCollidableFilter = 0x10C3B80,
	kVtbl_hkpRayCollidableFilter = 0x10C3B8C,
	kVtbl_hkpShapeCollectionFilter = 0x10C3B98,
	kVtbl_hkpRayShapeCollectionFilter = 0x10C3BAC,
	kVtbl_hkpBroadPhaseCastCollector = 0x10C3BB8,
	kVtbl_ahkpWorld = 0x10C3BC4,
	kVtbl_hkpWorldRayCaster = 0x10C3BD4,
	kVtbl_bhkCollisionFilter = 0x10C3C18,
	kVtbl_bhkWorldCinfo = 0x10C3C44,
	kVtbl_bhkWorld = 0x10C40B4,
	kVtbl_bhkNiCollisionObject = 0x10C4314,
	kVtbl_hkpCharacterProxyListener = 0x10C446C,
	kVtbl_bhkCharacterListener = 0x10C4488,
	kVtbl_bhkShapeCollection = 0x10C44E4,
	kVtbl_bhkCharacterProxy = 0x10C45CC,
	kVtbl_bhkCachingShapePhantom = 0x10C4694,
	kVtbl_hkpClosestCdPointCollector = 0x10C476C,
	kVtbl_bhkCollisionBox = 0x10C477C,
	kVtbl_bhkListShape = 0x10C485C,
	kVtbl_bhkCharacterStateClimbing = 0x10C4944,
	kVtbl_bhkCharacterStateFlying = 0x10C496C,
	kVtbl_bhkCharacterController = 0x10C49C4,
	kVtbl_bhkWindListener = 0x10C4B2C,
	kVtbl_hkaRaycastInterface = 0x10C4B60,
	kVtbl_bhkRagdollShareData = 0x10C4B74,
	kVtbl_bhkRagdollController = 0x10C4DDC,
	kVtbl_hkaSkeleton = 0x10C4DEC,
	kVtbl_bhkStiffSpringConstraint = 0x10C52BC,
	kVtbl_hkStiffSpringConstraintCinfo = 0x10C5398,
	kVtbl_bhkBlendCollisionObject = 0x10C53DC,
	kVtbl_hkpMotion = 0x10C59F4,
	kVtbl_bhkRefObject = 0x10C5B54,
	kVtbl_WeaponObject = 0x10C5C1C,
	kVtbl_bhkLimitedHingeConstraint = 0x10C5CE4,
	kVtbl_hkLimitedHingeConstraintCinfo = 0x10C5DC0,
	kVtbl_bhkBinaryAction = 0x10C5EF4,
	kVtbl_bhkBlendCollisionObjectAddRotation = 0x10C6234,
	kVtbl_hkReferencedObjectLock = 0x10C6334,
	kVtbl_hkpNullContactMgr = 0x10C6378,
	kVtbl_hkpBroadPhaseListener = 0x10C63BC,
	kVtbl_hkpDefaultConvexListFilter = 0x10C63CC,
	kVtbl_hkpPhantomBroadPhaseListener = 0x10C63F0,
	kVtbl_hkpBroadPhaseBorderListener = 0x10C6410,
	kVtbl_hkpNullContactMgrFactory = 0x10C6420,
	kVtbl_hkpNullCollisionFilter = 0x10C646C,
	kVtbl_hkpSimpleWorldRayCaster = 0x10C647C,
	kVtbl_hkpWorldLinearCaster = 0x10C6488,
	kVtbl_hkpWorld = 0x10C6714,
	kVtbl_hkDebugDisplay = 0x10C69CC,
	kVtbl_bhkWorldM = 0x10C69F4,
	kVtbl_bhkBlendController = 0x10C6B14,
	kVtbl_hkpKeyframedRigidMotion = 0x10C6BEC,
	kVtbl_hkpMaxSizeMotion = 0x10C6C64,
	kVtbl_hkpFixedRigidMotion = 0x10C6CDC,
	kVtbl_hkpSphereMotion = 0x10C6D54,
	kVtbl_hkpBoxMotion = 0x10C6DC4,
	kVtbl_hkpThinBoxMotion = 0x10C6E34,
	kVtbl_hkpCharacterMotion = 0x10C6EA4,
	kVtbl_hkpWorldObject = 0x10C6F64,
	kVtbl_hkpEntity = 0x10C6F7C,
	kVtbl_hkpShapeContainer = 0x10C6FB0,
	kVtbl_hkpSingleShapeContainer = 0x10C6FD0,
	kVtbl_hkpBvShape = 0x10C6FF8,
	kVtbl_hkpBoxShape = 0x10C70B4,
	kVtbl_hkpConvexTransformShapeBase = 0x10C710C,
	kVtbl_hkpPhantom = 0x10C7214,
	kVtbl_hkpShapePhantom = 0x10C7254,
	kVtbl_hkaRagdollRaycastInterface = 0x10C7304,
	kVtbl_bhkRagdollPenetrationUtil = 0x10C7324,
	kVtbl_hkPackedNiTriStripsData = 0x10C740C,
	kVtbl_hkpTriangleShape = 0x10C749C,
	kVtbl_hkNormalTriangleShape = 0x10C74EC,
	kVtbl_hkPackedNiTriStripsShape = 0x10C755C,
	kVtbl_hkNiTriStripsShape = 0x10C75C4,
	kVtbl_bhkPackedNiTriStripsShape = 0x10C761C,
	kVtbl_bhkNiTriStripsShape = 0x10C771C,
	kVtbl_hkpRigidBody = 0x10C7888,
	kVtbl_hkpCharacterContext = 0x10C8198,
	kVtbl_bhkMalleableConstraint = 0x10C81AC,
	kVtbl_hkMalleableConstraintCinfo = 0x10C8288,
	kVtbl_hkpMalleableConstraintData = 0x10C82A4,
	kVtbl_ahkpMalleableConstraintData = 0x10C82EC,
	kVtbl_ahkpCharacterProxy = 0x10C83E8,
	kVtbl_hkpCharacterProxy = 0x10C84A4,
	kVtbl_hkBSHeightFieldShape = 0x10C853C,
	kVtbl_hkErrStream = 0x10C85A0,
	kVtbl_bhkRigidBodyT = 0x10C879C,
	kVtbl_bhkWaterListener = 0x10C88B8,
	kVtbl_bhkOrientHingedBodyAction = 0x10C8914,
	kVtbl_hkpLimitedHingeConstraintData = 0x10C89E4,
	kVtbl_bhkForceController = 0x10C8A2C,
	kVtbl_bhkBallAndSocketConstraint = 0x10C8B0C,
	kVtbl_bhkRagdollConstraint = 0x10C8BEC,
	kVtbl_hkBallAndSocketConstraintCinfo = 0x10C8CC8,
	kVtbl_hkRagdollConstraintCinfo = 0x10C8CE4,
	kVtbl_bhkRagdollTemplateData = 0x10C8D3C,
	kVtbl_bhkRagdollTemplate = 0x10C8DCC,
	kVtbl_hkpMouseSpringAction = 0x10C8E94,
	kVtbl_hkNativeFileSystem = 0x10C8EB8,
	kVtbl_hkDummySingleton = 0x10C8ED4,
	kVtbl_hkMemorySystem = 0x10C8EFC,
	kVtbl_hkpCharacterStateManager = 0x10C9194,
	kVtbl_hkpHingeConstraintData = 0x10C91EC,
	kVtbl_hkpPhantomOverlapListener = 0x10C925C,
	kVtbl_hkpCollisionFilter = 0x10C92D4,
	kVtbl_hkOstream = 0x10C9314,
	kVtbl_hkMemoryTrackStreamWriter = 0x10C933C,
	kVtbl_hkArrayStreamWriter = 0x10C9368,
	kVtbl_bhkPoseArray = 0x10C93FC,
	kVtbl_bhkExtraData = 0x10C94A4,
	kVtbl_bhkSpringAction = 0x10C955C,
	kVtbl_bhkMotorAction = 0x10C9634,
	kVtbl_bhkDashpotAction = 0x10C970C,
	kVtbl_bhkAngularDashpotAction = 0x10C97E4,
	kVtbl_bhkBreakableConstraint = 0x10C98BC,
	kVtbl_ahkpBreakableConstraintData = 0x10C999C,
	kVtbl_hkBreakableConstraintCinfo = 0x10C99E0,
	kVtbl_bhkWheelConstraint = 0x10C9A14,
	kVtbl_hkWheelConstraintCinfo = 0x10C9AF0,
	kVtbl_bhkRagdollLimitsConstraint = 0x10C9B0C,
	kVtbl_hkRagdollLimitsConstraintCinfo = 0x10C9BE8,
	kVtbl_bhkPrismaticConstraint = 0x10C9C84,
	kVtbl_hkPrismaticConstraintCinfo = 0x10C9D60,
	kVtbl_bhkFixedConstraint = 0x10C9D7C,
	kVtbl_bhkGenericConstraint = 0x10C9E5C,
	kVtbl_hkGenericConstraintCinfo = 0x10C9F38,
	kVtbl_hkFixedConstraintCinfo = 0x10C9F54,
	kVtbl_bhkHingeLimitsConstraint = 0x10C9F74,
	kVtbl_hkHingeLimitsConstraintCinfo = 0x10CA050,
	kVtbl_bhkBallSocketConstraintChain = 0x10CA06C,
	kVtbl_bhkConstraintChain = 0x10CA14C,
	kVtbl_hkBallSocketChainCinfo = 0x10CA22C,
	kVtbl_bhkMoppBvTreeShape = 0x10CA24C,
	kVtbl_hkScaledMoppBvTreeShape = 0x10CA330,
	kVtbl_bhkTriangleShape = 0x10CA3F4,
	kVtbl_bhkPlaneShape = 0x10CA4DC,
	kVtbl_bhkHeightFieldShape = 0x10CA5C4,
	kVtbl_bhkExtendedMeshShapeData = 0x10CA6B4,
	kVtbl_bhkExtendedMeshShape = 0x10CA744,
	kVtbl_bhkCylinderShape = 0x10CA834,
	kVtbl_bhkConvexVerticesShape = 0x10CA924,
	kVtbl_bhkConvexTransformShape = 0x10CAA0C,
	kVtbl_bhkConvexSweepShape = 0x10CAAF4,
	kVtbl_bhkConvexListShape = 0x10CABDC,
	kVtbl_hkThreadMemory = 0x10CACD0,
	kVtbl_hkMemoryBlockServer = 0x10CAD0C,
	kVtbl_hkLeakDetectAllocator = 0x10CAD3C,
	kVtbl_hkFreeListMemorySystem = 0x10CAD74,
	kVtbl_hkVisualDebugger = 0x10CB100,
	kVtbl_hkpWorldDeletionListener = 0x10CB1C4,
	kVtbl_hkpActionListener = 0x10CB1D4,
	kVtbl_hkpConstraintListener = 0x10CB1E4,
	kVtbl_hkProcessContext = 0x10CB220,
	kVtbl_hkpPhysicsContext = 0x10CB2B4,
	kVtbl_bhkCharacterPointCollector = 0x10CB300,
	kVtbl_bhkCharacterStateProjectile = 0x10CB344,
	kVtbl_bhkCharacterStateInAir = 0x10CB36C,
	kVtbl_bhkCharacterStateJumping = 0x10CB398,
	kVtbl_bhkCharacterStateSwimming = 0x10CB3C4,
	kVtbl_bhkCharacterStateOnGround = 0x10CB3F0,
	kVtbl_hkCharControllerShape = 0x10CB45C,
	kVtbl_bhkCharControllerShape = 0x10CB4AC,
	kVtbl_hkpSphereShape = 0x10CB5AC,
	kVtbl_hkaFootPlacementIkSolver = 0x10CB644,
	kVtbl_hkpRagdollConstraintData = 0x10CB71C,
	kVtbl_hkpConstraintMotor = 0x10CB76C,
	kVtbl_hkpLimitedForceConstraintMotor = 0x10CB780,
	kVtbl_hkpPositionConstraintMotor = 0x10CB798,
	kVtbl_hkaRagdollInstance = 0x10CB830,
	kVtbl_hkaSkeletonMapper = 0x10CB864,
	kVtbl_hkpAabbPhantom = 0x10CC004,
	kVtbl_hkpConstraintInstance = 0x10CC040,
	kVtbl_bhkPositionConstraintMotor = 0x10CC064,
	kVtbl_hkpVelocityConstraintMotor = 0x10CC0D8,
	kVtbl_bhkVelocityConstraintMotor = 0x10CC0EC,
	kVtbl_bhkSpringDamperConstraintMotor = 0x10CC164,
	kVtbl_hkPointToPathConstraintCinfo = 0x10CC2E0,
	kVtbl_hkpAction = 0x10CC2FC,
	kVtbl_hkpBinaryAction = 0x10CC320,
	kVtbl_hkpShapeCollection = 0x10CC364,
	kVtbl_hkpListShape = 0x10CC3D8,
	kVtbl_hkpTransformShape = 0x10CC41C,
	kVtbl_hkpNullBroadPhaseListener = 0x10CC4E0,
	kVtbl_hkpSimulation = 0x10CC4F4,
	kVtbl_hkpCollisionDispatcher = 0x10CC7C0,
	kVtbl_hkpReportContactMgr = 0x10CC86C,
	kVtbl_hkpSimpleContactConstraintData = 0x10CC934,
	kVtbl_hkpSimpleConstraintContactMgr = 0x10CC984,
	kVtbl_hkpMultiThreadedSimulation = 0x10CCBC4,
	kVtbl_hkpPhysicsSystem = 0x10CCE28,
	kVtbl_hkStatisticsCollector = 0x10CCE98,
	kVtbl_hkpConstraintOwner = 0x10CCEBC,
	kVtbl_hkpSimulationIsland = 0x10CCF28,
	kVtbl_hkpWorldPostSimulationListener = 0x10CCF7C,
	kVtbl_hkKdTreeCastCollector = 0x10CCF8C,
	kVtbl_hkpKdTreeCollidableRaycaster = 0x10CCFB8,
	kVtbl_hkpKdTreeWorldLinearCaster = 0x10CCFCC,
	kVtbl_hkpAabbTreeWorldManager = 0x10CD078,
	kVtbl_hkpKdTreeWorldManager = 0x10CD278,
	kVtbl_hkpCollisionCallbackUtil = 0x10CD33C,
	kVtbl_hkpDefaultWorldMaintenanceMgr = 0x10CD34C,
	kVtbl_hkpBroadPhaseBorder = 0x10CD3C4,
	kVtbl_hkpContinuousSimulation = 0x10CD45C,
	kVtbl_hkpNullAgent = 0x10CD54C,
	kVtbl_hkpEntityEntityBroadPhaseListener = 0x10CD598,
	kVtbl_hkp3AxisSweep = 0x10CD5CC,
	kVtbl_hkpSimpleShapePhantom = 0x10CE15C,
	kVtbl_hkaDetectRagdollPenetration = 0x10CE1A8,
	kVtbl_hkpCapsuleShape = 0x10CE25C,
	kVtbl_hkDisplayGeometry = 0x10CE2B4,
	kVtbl_hkDisplayCapsule = 0x10CE2CC,
	kVtbl_hkpSampledHeightFieldShape = 0x10CE31C,
	kVtbl_hkpTriSampledHeightFieldCollection = 0x10CE390,
	kVtbl_hkpBvTreeShape = 0x10CE3C8,
	kVtbl_hkpTriSampledHeightFieldBvTreeShape = 0x10CE404,
	kVtbl_hkBufferedStreamWriter = 0x10CE448,
	kVtbl_hkpMultiSphereShape = 0x10CE4BC,
	kVtbl_hkOrientHingedBodyAction = 0x10CE504,
	kVtbl_hkpUnaryAction = 0x10CE528,
	kVtbl_hkDefaultError = 0x10CE5D4,
	kVtbl_hkSocket = 0x10CE658,
	kVtbl_hkBufferedStreamReader = 0x10CE688,
	kVtbl_hkStdioStreamReader = 0x10CE6C0,
	kVtbl_hkStdioStreamWriter = 0x10CE6F4,
	kVtbl_hkLiquidAction = 0x10CE740,
	kVtbl_hkCpuJobThreadPool = 0x10CE774,
	kVtbl_hkpSymmetricAgentFlipCollector = 0x10CE7FC,
	kVtbl_hkpSymmetricAgentFlipCastCollector = 0x10CE80C,
	kVtbl_hkpSymmetricAgentFlipBodyCollector = 0x10CE81C,
	kVtbl_hkpBvTreeStreamAgent = 0x10CE838,
	kVtbl_hkpBvTreeAgent = 0x10CE8F4,
	kVtbl_hkpMoppAgent = 0x10CE930,
	kVtbl_hkHeightFieldRayForwardingCollector = 0x10CEA14,
	kVtbl_hkpHeightFieldAgent = 0x10CEAA8,
	kVtbl_hkpMapPointsToSubShapeContactMgr = 0x10CEB44,
	kVtbl_hkpPredGskfAgent = 0x10CEB94,
	kVtbl_hkpFlagCdBodyPairCollector = 0x10CEBD0,
	kVtbl_hkpSimpleClosestContactCollector = 0x10CEBE0,
	kVtbl_hkpConvexListAgent = 0x10CEBF0,
	kVtbl_hkpListAgent = 0x10CED34,
	kVtbl_hkpMultiRayConvexAgent = 0x10CEDD4,
	kVtbl_hkpMultiSphereTriangleAgent = 0x10CEE4C,
	kVtbl_hkpCapsuleTriangleAgent = 0x10CEEFC,
	kVtbl_hkpCapsuleCapsuleAgent = 0x10CEF94,
	kVtbl_hkpSphereBoxAgent = 0x10CEFDC,
	kVtbl_hkpSphereTriangleAgent = 0x10CF060,
	kVtbl_hkpSphereCapsuleAgent = 0x10CF0E4,
	kVtbl_hkpSphereSphereAgent = 0x10CF16C,
	kVtbl_hkpBoxBoxAgent = 0x10CF1C4,
	kVtbl_hkpPhantomAgent = 0x10CF238,
	kVtbl_hkpTransformAgent = 0x10CF280,
	kVtbl_hkpShapeCollectionAgent = 0x10CF318,
	kVtbl_hkpMultiSphereAgent = 0x10CF354,
	kVtbl_hkpBvAgent = 0x10CF3CC,
	kVtbl_hkMemoryTrackStreamReader = 0x10CF734,
	kVtbl_hkDefaultBuiltinTypeRegistry = 0x10CF78C,
	kVtbl_hkTypeInfoRegistry = 0x10CF7B0,
	kVtbl_hkDynamicClassNameRegistry = 0x10CF7D4,
	kVtbl_hkDefaultClassNameRegistry = 0x10CF804,
	kVtbl_hkVtableClassRegistry = 0x10CF844,
	kVtbl_hkpPhysicsData = 0x10CF9A4,
	kVtbl_hkBinaryTagfileWriter = 0x10CF9C8,
	kVtbl_hkpStorageSampledHeightFieldShape = 0x10CFB34,
	kVtbl_hkpStorageExtendedMeshShape = 0x10CFD44,
	kVtbl_hkpStorageMeshShape = 0x10D055C,
	kVtbl_hkpPhysicsSystemWithContacts = 0x10D07F4,
	kVtbl_hkpCachingShapePhantom = 0x10D087C,
	kVtbl_hkpSpringAction = 0x10D08F4,
	kVtbl_hkpMotorAction = 0x10D0944,
	kVtbl_hkpDashpotAction = 0x10D0994,
	kVtbl_hkpAngularDashpotAction = 0x10D09F4,
	kVtbl_hkpBreakableConstraintData = 0x10D0A1C,
	kVtbl_hkpWheelConstraintData = 0x10D0A64,
	kVtbl_hkpStiffSpringConstraintData = 0x10D0AAC,
	kVtbl_hkpRagdollLimitsData = 0x10D0AF4,
	kVtbl_hkpPrismaticConstraintData = 0x10D0B3C,
	kVtbl_hkpGenericConstraintData = 0x10D0B9C,
	kVtbl_hkpHingeLimitsData = 0x10D0BE4,
	kVtbl_hkpBallSocketChainData = 0x10D0C8C,
	kVtbl_hkpBallAndSocketConstraintData = 0x10D0CD4,
	kVtbl_hkMoppBvTreeShapeBase = 0x10D0D70,
	kVtbl_hkpMoppBvTreeShape = 0x10D0DAC,
	kVtbl_hkpPlaneShape = 0x10D0E18,
	kVtbl_hkpConvexTranslateShape = 0x10D0E5C,
	kVtbl_hkpExtendedMeshShape = 0x10D0ECC,
	kVtbl_hkpCylinderShape = 0x10D0F1C,
	kVtbl_hkDisplayCylinder = 0x10D0F84,
	kVtbl_hkpConvexVerticesShape = 0x10D0FC4,
	kVtbl_hkpConvexTransformShape = 0x10D102C,
	kVtbl_hkpConvexListShape = 0x10D10C4,
	kVtbl_hkSolverAllocator = 0x10D1110,
	kVtbl_hkFreeListAllocator = 0x10D1464,
	kVtbl_hkSystemMemoryBlockServer = 0x10D14C8,
	kVtbl_hkOArchive = 0x10D14F8,
	kVtbl_hkProcess = 0x10D1508,
	kVtbl_hkProcessHandler = 0x10D1528,
	kVtbl_hkCommandRouter = 0x10D1548,
	kVtbl_hkServerProcessHandler = 0x10D1598,
	kVtbl_hkpWeldingViewer = 0x10D1634,
	kVtbl_hkpWorldSnapshotViewer = 0x10D1720,
	kVtbl_hkpWorldMemoryViewer = 0x10D1794,
	kVtbl_hkpVehicleViewer = 0x10D1854,
	kVtbl_hkpSweptTransformDisplayViewer = 0x10D1918,
	kVtbl_hkpSimulationIslandViewer = 0x10D1A28,
	kVtbl_hkpShapeDisplayViewer = 0x10D1B04,
	kVtbl_hkpPhantomDisplayViewer = 0x10D1BDC,
	kVtbl_hkpUserShapePhantomTypeIdentifier = 0x10D1C2C,
	kVtbl_hkpMousePickingViewer = 0x10D1CE4,
	kVtbl_hkpRigidBodyInertiaViewer = 0x10D1D80,
	kVtbl_hkpToiCountViewer = 0x10D1E6C,
	kVtbl_hkpToiContactPointViewer = 0x10D1F0C,
	kVtbl_hkpInactiveContactPointViewer = 0x10D1FB0,
	kVtbl_hkpActiveContactPointViewer = 0x10D203C,
	kVtbl_hkpConvexRadiusViewer = 0x10D20F8,
	kVtbl_hkpConstraintViewer = 0x10D21C4,
	kVtbl_hkpRigidBodyCentreOfMassViewer = 0x10D22A8,
	kVtbl_hkpMidphaseViewer = 0x10D2330,
	kVtbl_hkpBroadphaseViewer = 0x10D23B4,
	kVtbl_hkgpConvexHull = 0x10D36E0,
	kVtbl_hkpPointToPlaneConstraintData = 0x10D37E4,
	kVtbl_hkpSpringDamperConstraintMotor = 0x10D3838,
	kVtbl_hkTraceStream = 0x10D3CC0,
	kVtbl_hkpConstraintCollisionFilter = 0x10D3D18,
	kVtbl_hkpDeferredConstraintOwner = 0x10D3DF8,
	kVtbl_hkpBroadPhase = 0x10D3F7C,
	kVtbl_hkKdTreeBundleCastCollector = 0x10D4050,
	kVtbl_hkKdTreeCollidableRayBundleCaster = 0x10D4060,
	kVtbl_hkpAabbTreeCollidableRaycaster = 0x10D4094,
	kVtbl_hkpCollidableMediator = 0x10D40B4,
	kVtbl_hkpEndOfStepCallbackUtil = 0x10D40E0,
	kVtbl_hkpDefaultToiResourceMgr = 0x10D4310,
	kVtbl_hkLocalFrameGroup = 0x10D460C,
	kVtbl_hkSimpleLocalFrame = 0x10D461C,
	kVtbl_hkStackTracer = 0x10D4DE4,
	kVtbl_hkBsdSocket = 0x10D5030,
	kVtbl_hkpGskBaseAgent = 0x10D51CC,
	kVtbl_hkpGskfAgent = 0x10D5224,
	kVtbl_hkCrc32StreamWriter = 0x10D538C,
	kVtbl_hkStaticClassNameRegistry = 0x10D6014,
	kVtbl_hkVersionPatchManager = 0x10D6138,
	kVtbl_hkSerializeDeprecated = 0x10D6260,
	kVtbl_hkDataRefCounted = 0x10D62F4,
	kVtbl_hkDataClassNative = 0x10D62FC,
	kVtbl_hkDataObjectNative = 0x10D6334,
	kVtbl_hkDataArrayNative = 0x10D642C,
	kVtbl_hkDataWorldNative = 0x10D6518,
	kVtbl_hkBinaryPackfileWriter = 0x10D65BC,
	kVtbl_hkSubStreamWriter = 0x10D65EC,
	kVtbl_hkPackfileWriter = 0x10D6708,
	kVtbl_hkBinaryTagfileReader = 0x10D6CE8,
	kVtbl_ArrayOfTuplesImplementation = 0x10D6E4C,
	kVtbl_hkDataObjectDict = 0x10D6EE4,
	kVtbl_VecArrayImplementation = 0x10D6F4C,
	kVtbl_PointerArrayImplementation = 0x10D6FE4,
	kVtbl_ByteArrayImplementation = 0x10D707C,
	kVtbl_IntArrayImplementation = 0x10D7114,
	kVtbl_RealArrayImplementation = 0x10D71AC,
	kVtbl_CstringArrayImplementation = 0x10D7244,
	kVtbl_hkDataClassDict = 0x10D73C8,
	kVtbl_StructArrayImplementation = 0x10D73FC,
	kVtbl_hkDataWorldDict = 0x10D7510,
	kVtbl_hkIstream = 0x10D7540,
	kVtbl_hkpMeshShape = 0x10D7754,
	kVtbl_hkpSerializedAgentNnEntry = 0x10D77D8,
	kVtbl_hkpConstraintChainInstanceAction = 0x10D79C4,
	kVtbl_hkpConstraintChainInstance = 0x10D79E8,
	kVtbl_hkpMoppCostFunction = 0x10D7BA8,
	kVtbl_hkpMoppShapeMediator = 0x10D7BB8,
	kVtbl_hkpMoppCachedShapeMediator = 0x10D7BE4,
	kVtbl_hkFreeListAllocatorServer = 0x10D7C10,
	kVtbl_hkLargeBlockAllocator = 0x10D7C34,
	kVtbl_hkWindowsSystemClock = 0x10D7C58,
	kVtbl_hkIArchive = 0x10D7C80,
	kVtbl_hkProcessFactory = 0x10D7C90,
	kVtbl_hkDebugDisplayHandler = 0x10D7CFC,
	kVtbl_hkServerDebugDisplayHandler = 0x10D7DA8,
	kVtbl_hkDisplaySerializeOStream = 0x10D7DB8,
	kVtbl_hkDisplaySerializeIStream = 0x10D7DC8,
	kVtbl_hkInspectProcess = 0x10D7E0C,
	kVtbl_hkStatisticsProcess = 0x10D7E48,
	kVtbl_hkDebugDisplayProcess = 0x10D7ED0,
	kVtbl_hkpPhysicsContextWorldListener = 0x10D7F04,
	kVtbl_hkpWorldViewerBase = 0x10D7F44,
	kVtbl_hkStreamStatisticsCollector = 0x10D8040,
	kVtbl_hkpShapeDisplayBuilder = 0x10D8094,
	kVtbl_hkpUserShapeDisplayBuilder = 0x10D80A4,
	kVtbl_hkDisplayAABB = 0x10D80D0,
	kVtbl_hkDisplayConvex = 0x10D80E8,
	kVtbl_hkDisplayBox = 0x10D8100,
	kVtbl_hkpContactPointViewer = 0x10D8184,
	kVtbl_hkpConvexRadiusBuilder = 0x10D8214,
	kVtbl_hkpUserConvexRadiusBuilder = 0x10D8224,
	kVtbl_hkDisplaySemiCircle = 0x10D8274,
	kVtbl_hkDisplayPlane = 0x10D828C,
	kVtbl_hkDisplayCone = 0x10D82A4,
	kVtbl_hkpPointToPathConstraintData = 0x10DA004,
	kVtbl_hkpPulleyConstraintData = 0x10DA04C,
	kVtbl_hkpPairCollisionFilter = 0x10DA0C8,
	kVtbl_hkCpuWorldGetClosestPointsCollector = 0x10DB94C,
	kVtbl_hkCpuWorldLinearCastCollector = 0x10DB9E4,
	kVtbl_hkCpuWorldRayCastCollector = 0x10DBAF8,
	kVtbl_hkpFixedBufferRayHitCollector = 0x10DBB04,
	kVtbl_hkaAnimation = 0x10DDB3C,
	kVtbl_hkaWaveletCompressedAnimation = 0x10DDC5C,
	kVtbl_hkaSplineCompressedAnimation = 0x10DE0EC,
	kVtbl_hkaDeltaCompressedAnimation = 0x10DE90C,
	kVtbl_hkpRejectChassisListener = 0x10DEBF4,
	kVtbl_hkpVehicleWheelCollide = 0x10DEC04,
	kVtbl_hkpVehicleLinearCastWheelCollide = 0x10DEC3C,
	kVtbl_hkpVehicleInstance = 0x10DF34C,
	kVtbl_hkpVehicleDefaultSuspension = 0x10DF89C,
	kVtbl_hkpVehicleDefaultBrake = 0x10DF9F4,
	kVtbl_hkpVehicleData = 0x10DFF44,
	kVtbl_hkpTyremarksWheel = 0x10E00CC,
	kVtbl_hkpTyremarksInfo = 0x10E00DC,
	kVtbl_hkpSerializedDisplayRbTransforms = 0x10E04AC,
	kVtbl_hkpPoweredChainMapper = 0x10E0654,
	kVtbl_hkpRotationalConstraintData = 0x10E11EC,
	kVtbl_hkpSimpleMeshShape = 0x10E2144,
	kVtbl_hkpMultiRayShape = 0x10E222C,
	kVtbl_hkxAttributeHolder = 0x10E310C,
	kVtbl_hkxNode = 0x10E311C,
	kVtbl_hkxMaterial = 0x10E33DC,
	kVtbl_hkxSparselyAnimatedInt = 0x10E350C,
	kVtbl_hkxSparselyAnimatedEnum = 0x10E351C,
	kVtbl_hkxEnum = 0x10E352C,
	kVtbl_hkMonitorStreamColorTable = 0x10E3E5C,
	kVtbl_hkaInterleavedUncompressedAnimation = 0x10E5474,
	kVtbl_hkaFootstepAnalysisInfoContainer = 0x10E56D4,
	kVtbl_hkaDefaultAnimatedReferenceFrame = 0x10E57A4,
	kVtbl_hkaBoneAttachment = 0x10E587C,
	kVtbl_hkaAnimationPreviewColorContainer = 0x10E58C8,
	kVtbl_hkaAnimationContainer = 0x10E59AC,
	kVtbl_hkaAnimationBinding = 0x10E5A58,
	kVtbl_hkpVehicleRayCastWheelCollide = 0x10E5AE4,
	kVtbl_hkpVehicleManager = 0x10E5B4C,
	kVtbl_hkpVehicleRayCastBatchingManager = 0x10E5B68,
	kVtbl_hkpVehicleLinearCastBatchingManager = 0x10E5BFC,
	kVtbl_hkpVehicleDriverInputAnalogStatus = 0x10E5D94,
	kVtbl_hkpVehicleDefaultAnalogDriverInput = 0x10E5DA8,
	kVtbl_hkpVehicleDefaultVelocityDamper = 0x10E5E9C,
	kVtbl_hkpVehicleDefaultTransmission = 0x10E5FF4,
	kVtbl_hkpVehicleDefaultSteering = 0x10E60C4,
	kVtbl_hkpVehicleDefaultEngine = 0x10E62A4,
	kVtbl_hkpVehicleDefaultAerodynamics = 0x10E6394,
	kVtbl_hkpSerializedDisplayMarkerList = 0x10E66FC,
	kVtbl_hkpSerializedDisplayMarker = 0x10E676C,
	kVtbl_hkpReorientAction = 0x10E6834,
	kVtbl_hkpGroupCollisionFilter = 0x10E6DEC,
	kVtbl_hkpDisplayBindingData = 0x10E75EC,
	kVtbl_hkpDisableEntityCollisionFilter = 0x10E7700,
	kVtbl_hkpConstrainedSystemFilter = 0x10E7874,
	kVtbl_hkpConvexPieceStreamData = 0x10E7C24,
	kVtbl_hkpLinearParametricCurve = 0x10E80B4,
	kVtbl_hkpDefaultWorldMemoryWatchDog = 0x10E81A4,
	kVtbl_hkpCallbackConstraintMotor = 0x10E83B4,
	kVtbl_hkpShapeInfo = 0x10E8824,
	kVtbl_hkpMoppModifier = 0x10E8894,
	kVtbl_hkpRemoveTerminalsMoppModifier = 0x10E88B4,
	kVtbl_hkpGroupFilter = 0x10E8A6C,
	kVtbl_hkpFastMeshShape = 0x10E8AB0,
	kVtbl_hkpConvexVerticesConnectivity = 0x10E8D8C,
	kVtbl_hkpConvexPieceMeshShape = 0x10E8E94,
	kVtbl_hkpCompressedSampledHeightFieldShape = 0x10E901C,
	kVtbl_hkpCollisionFilterList = 0x10E90E8,
	kVtbl_hkMemoryMeshShape = 0x10E929C,
	kVtbl_hkxVertexVectorDataChannel = 0x10E9300,
	kVtbl_hkxVertexSelectionChannel = 0x10E9358,
	kVtbl_hkxVertexIntDataChannel = 0x10E93AC,
	kVtbl_hkxVertexFloatDataChannel = 0x10E943C,
	kVtbl_hkxTriangleSelectionChannel = 0x10E9494,
	kVtbl_hkxTextureInplace = 0x10E9544,
	kVtbl_hkxTextureFile = 0x10E95B4,
	kVtbl_hkxSparselyAnimatedString = 0x10E9614,
	kVtbl_hkxSparselyAnimatedBool = 0x10E96C8,
	kVtbl_hkxSkinBinding = 0x10E977C,
	kVtbl_hkxScene = 0x10E99D8,
	kVtbl_hkxNodeSelectionSet = 0x10E9A34,
	kVtbl_hkxMeshSection = 0x10E9B7C,
	kVtbl_hkxMaterialShader = 0x10E9CCC,
	kVtbl_hkxMaterialEffect = 0x10E9DEC,
	kVtbl_hkxLight = 0x10E9E74,
	kVtbl_hkxIndexBuffer = 0x10E9F48,
	kVtbl_hkxEdgeSelectionChannel = 0x10E9FA4,
	kVtbl_hkxCamera = 0x10EA0A4,
	kVtbl_hkxAnimatedVector = 0x10EA10C,
	kVtbl_hkxAnimatedQuaternion = 0x10EA154,
	kVtbl_hkxAnimatedMatrix = 0x10EA188,
	kVtbl_hkxAnimatedFloat = 0x10EA1AC,
	kVtbl_hkPlatformObjectWriter = 0x10EA370,
	kVtbl_hkOffsetOnlyStreamWriter = 0x10EA388,
	kVtbl_hkObjectCopier = 0x10EA3B0,
	kVtbl_hkObjectResource = 0x10EA4DC,
	kVtbl_hkPackfileData = 0x10EA51C,
	kVtbl_hkMemoryStreamReader = 0x10EA540,
	kVtbl_hkpMoppCodeGenerator = 0x10EA5B4,
	kVtbl_hkpMoppDefaultAssembler = 0x10EA5DC,
	kVtbl_hkpMoppDefaultSplitter = 0x10EA610,
	kVtbl_hkDisplaySphere = 0x10EA630,
	kVtbl_IConvexOverlapImpl = 0x10EADEC,
	kVtbl_hkGskShape = 0x10EADFC,
	kVtbl_hkgpMesh = 0x10EB000,
	kVtbl_hkResourceMap = 0x10EB19C,
	kVtbl_hkMemoryResourceHandle = 0x10EB288,
	kVtbl_hkMemoryResourceContainer = 0x10EB2C4,
	kVtbl_hkContainerResourceMap = 0x10EB308,
	kVtbl_hkaMeshBinding = 0x10EB770,
	kVtbl_hkpStiffSpringChainData = 0x10EB98C,
	kVtbl_hkpPoweredChainData = 0x10EB9D4,
	kVtbl_hkpCompressedMeshShape = 0x10EBA5C,
	kVtbl_hkxVertexBuffer = 0x10EBA94,
	kVtbl_hkxEnvironment = 0x10EBAB8,
	kVtbl_hkaFootstepAnalysisInfo = 0x10EBD64,
	kVtbl_hkpGunProjectile = 0x10EBF94,
	kVtbl_hkpProjectileGun = 0x10EBFB0,
	kVtbl_hkpMountedBallGun = 0x10EC004,
	kVtbl_hkpGravityGun = 0x10EC090,
	kVtbl_hkpFirstPersonGun = 0x10EC0E4,
	kVtbl_hkpBallGun = 0x10EC174,
	kVtbl_hkpRemoveTerminalsMoppModifier2 = 0x10EC1A4,
	kVtbl_hkxMesh = 0x10EC1DC,
	kVtbl_hkxMaterialShaderSet = 0x10EC1EC,
	kVtbl_hkAlignSceneToNodeOptions = 0x10EC1FC,
	kVtbl_hkKdTreeCpuBroadphaseBundleCastCollector = 0x10EC614,
	kVtbl_hkpConvexPieceShape = 0x10ECE2C,
	kVtbl_NiDX9SourceTextureData = 0x10ED37C,
	kVtbl_NiDX9Renderer = 0x10EE4BC,
	kVtbl_NiDX9ShaderDeclaration = 0x10EE70C,
	kVtbl_NiD3DShaderDeclaration = 0x10EE7CC,
	kVtbl_NiD3DDefaultShader = 0x10EECAC,
	kVtbl_NiD3DShader = 0x10EEDA4,
	kVtbl_NiDX92DBufferData = 0x10EEED4,
	kVtbl_NiDX9TextureBufferData = 0x10EEFBC,
	kVtbl_NiDX9ImplicitBufferData = 0x10EF06C,
	kVtbl_NiDX9ImplicitDepthStencilBufferData = 0x10EF114,
	kVtbl_NiDX9AdditionalDepthStencilBufferData = 0x10EF184,
	kVtbl_NiD3DPass = 0x10EF364,
	kVtbl_NiD3DShaderInterface = 0x10EF39C,
	kVtbl_NiShaderConstantMapEntry = 0x10EF4D8,
	kVtbl_NiD3DShaderConstantMap = 0x10EF544,
	kVtbl_NiD3DRenderState = 0x10EF60C,
	kVtbl_NiD3DGeometryGroupManager = 0x10EF718,
	kVtbl_NiD3DShaderProgram = 0x10EF73C,
	kVtbl_NiD3DPixelShader = 0x10EF7D4,
	kVtbl_NiD3DVertexShader = 0x10EF87C,
	kVtbl_NiDX9TextureData = 0x10EFB7C,
	kVtbl_NiDX9SourceCubeMapData = 0x10EFC34,
	kVtbl_NiDX9VertexBufferManager = 0x10EFE94,
	kVtbl_NiFragmentShaderInstanceDescriptor = 0x10EFEFC,
	kVtbl_NiDX9FragmentShader = 0x10EFF44,
	kVtbl_NiD3DGPUProgramCache = 0x10F0094,
	kVtbl_NiGeometryBufferData = 0x10F017C,
	kVtbl_NiDX9IndexBufferManager = 0x10F02F0,
	kVtbl_NiDX9RenderedTextureData = 0x10F02FC,
	kVtbl_NiDX9RenderedCubeMapData = 0x10F049C,
	kVtbl_NiDX9DynamicTextureData = 0x10F05EC,
	kVtbl_NiDX9ErrorShader = 0x10F0764,
	kVtbl_NiDX9TextureManager = 0x10F086C,
	kVtbl_NiDX9RenderState = 0x10F088C,
	kVtbl_NiDX9PersistentSrcTextureRendererData = 0x10F099C,
	kVtbl_NiD3DShaderProgramCreator = 0x10F0A68,
	kVtbl_NiD3DShaderProgramCreatorHLSL = 0x10F0A90,
	kVtbl_NiD3DShaderProgramCreatorAsm = 0x10F0AC0,
	kVtbl_NiD3DShaderProgramCreatorObj = 0x10F0AF4,
	kVtbl_NiD3DShaderFactory = 0x10F0CC4,
	kVtbl_NiD3DShaderProgramFactory = 0x10F0F70,
	kVtbl_NiStaticGeometryGroup = 0x10F0FD4,
	kVtbl_NiUnsharedGeometryGroup = 0x10F1008,
	kVtbl_NiDynamicGeometryGroup = 0x10F10C4,
	kVtbl_NiD3DHLSLPixelShader = 0x10F10F4,
	kVtbl_NiD3DHLSLVertexShader = 0x10F119C,
	kVtbl_NiDX9ShaderConstantManager = 0x10F1550,
	kVtbl_NiGeometryGroup = 0x10F1644,
	kVtbl_NiD3DShaderConstantManager = 0x10F16CC,
	kVtbl_NiBoundingVolume = 0x10F16F4,
	kVtbl_NiHalfSpaceBV = 0x10F1728,
	kVtbl_NiUnionBV = 0x10F1778,
	kVtbl_NiCapsuleBV = 0x10F17D0,
	kVtbl_NiBoxBV = 0x10F1814,
	kVtbl_NiSphereBV = 0x10F1854,
	kVtbl_NiCollisionData = 0x10F18BC,
	kVtbl_NiIntersector = 0x10F1A84,
	kVtbl_NiCapsuleSphereIntersector = 0x10F1AA0,
	kVtbl_NiCapsuleCapsuleIntersector = 0x10F1ABC,
	kVtbl_NiCapsuleTriIntersector = 0x10F1AD8,
	kVtbl_NiBoxCapsuleIntersector = 0x10F1AF4,
	kVtbl_NiBoxSphereIntersector = 0x10F1B10,
	kVtbl_NiSphereTriIntersector = 0x10F1B2C,
	kVtbl_NiOBBRoot = 0x10F1B44,
	kVtbl_NiOBBNode = 0x10F1B58,
	kVtbl_NiOBBLeaf = 0x10F1B70,
	kVtbl_MoviePlayer = 0x10F1C44,
	kVtbl_NiRenderedCubeMap = 0x110102C,
	kVtbl_NiRenderTargetGroup = 0x11010EC,
	kVtbl_NiShaderDeclaration = 0x11011EC,
	kVtbl_NiSCMExtraData = 0x110129C,
	kVtbl_NiGPUProgramCache = 0x11013B4,
	kVtbl_NiMaterialResource = 0x1101464,
	kVtbl_StartMenuOption = 0x1076EE0,
	kVtbl_StartMenuUserOption = 0x107704C,

};

class BaseFormComponent
{
public:
	BaseFormComponent();
	~BaseFormComponent();

	virtual void	Init();
	virtual void	Free();
	virtual void	CopyFromBase(BaseFormComponent* component);
	virtual bool	CompareWithBase(BaseFormComponent* src);

	//	void		** _vtbl;	// 000
};

// C
class TESFullName : public BaseFormComponent
{
public:
	TESFullName();
	~TESFullName();

	String	name;		// 004
};

// 00C
class TESTexture : public BaseFormComponent
{
public:
	TESTexture();
	~TESTexture();

	virtual UInt32	Unk_04();
	virtual void	GetNormalMap(String* str);
	virtual char* GetPathRoot();

	String ddsPath;
};

// 00C
class TESIcon : public TESTexture
{
public:
	TESIcon();
	~TESIcon();

	void SetPath(const char* newPath) { ddsPath.Set(newPath); }
};


// C
class TESScriptableForm : public BaseFormComponent
{
public:
	TESScriptableForm();
	~TESScriptableForm();

	Script* script;		// 004
	bool	resolved;	// 008	called during LoadForm, so scripts do not wait for TESForm_InitItem to be resolved
	UInt8	pad[3];		// 009
};

// 010
class BGSMessageIcon : public BaseFormComponent
{
public:
	BGSMessageIcon();
	~BGSMessageIcon();

	TESIcon	icon;		// 004
};


// 008
class TESValueForm : public BaseFormComponent
{
public:
	enum
	{
		kModified_GoldValue = 0x00000008,
		// UInt32	value
	};

	TESValueForm();
	~TESValueForm();

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	//	DEFINE_MEMBER_FN_LONG(TESValueForm, SetValue, void, _TESValueForm_SetValue, UInt32 newVal);

	UInt32	value;
	// 008
};

// 10
class TESEnchantableForm : public BaseFormComponent
{
public:
	TESEnchantableForm();
	~TESEnchantableForm();

	virtual UInt32	Unk_04();	// returns unk2

	EnchantmentItem* enchantItem;	// 04
	UInt16	enchantment;			// 08
	UInt16	unk1;					// 0A
	UInt32	unk2;					// 0C
	// 010
};

// 08
class TESImageSpaceModifiableForm : public BaseFormComponent
{
public:
	TESImageSpaceModifiableForm();
	~TESImageSpaceModifiableForm();

	UInt32	unk04;	// 04
};

// 008
class TESWeightForm : public BaseFormComponent
{
public:
	TESWeightForm();
	~TESWeightForm();

	float	weight;		// 004
	// 008
};

// 008
class TESHealthForm : public BaseFormComponent
{
public:
	TESHealthForm();
	~TESHealthForm();

	virtual UInt32	GetHealth();	// 0004

	UInt32	health;		// 004
};

// 008
class TESAttackDamageForm : public BaseFormComponent
{
public:
	TESAttackDamageForm();
	~TESAttackDamageForm();

	virtual UInt16	GetDamage();

	UInt16	damage;	// 04
	UInt16	unk0;	// 06 - bitmask? perhaps 2 UInt8s?
	// 008
};

// C
class EffectItemList : public BSSimpleList<EffectItem>
{
public:
	EffectItemList();
	~EffectItemList();

	UInt32	unk00C;	// 00C

	EffectItem* ItemAt(UInt32 whichItem);
	UInt32 CountItems() const;
	const char* GetNthEIName(UInt32 whichEffect) const;

	//	bool HasNonHostileItem() const;
	//	UInt32 CountHostileItems() const;
	//	UInt32 AddItem(EffectItem* effectItem);
	//	UInt32 AddItemCopy(EffectItem* effectItem);
	//	UInt32 CopyItemFrom(EffectItemList& fromList, UInt32 whichItem);
	//	bool RemoveItem(UInt32 whichItem);
};
static_assert(sizeof(EffectItemList) == 0x10);

// 1C
class MagicItem : public TESFullName
{
public:
	MagicItem();
	~MagicItem();

	// pure virtual args/return values unknown
	virtual void	Unk_04(); // pure virtual
	virtual void	Unk_05(); // pure virtual
	virtual void	Unk_06(); // pure virtual
	virtual bool	Unk_07();
	virtual bool	Unk_08();
	virtual void	Unk_09(); // pure virtual
	virtual void	Unk_0A(); // pure virtual
	virtual void	Unk_0B(); // pure virtual
	virtual void	Unk_0C(); // pure virtual
	virtual void	Unk_0D(); // pure virtual
	virtual void	Unk_0E();
	virtual void	Unk_0F(); // pure virtual

	EffectItemList	list;	// 00C

	// perhaps types are no longer correct!
	enum EType {
		kType_None = 0,
		kType_Spell = 1,
		kType_Enchantment = 2,
		kType_Alchemy = 3,
		kType_Ingredient = 4,
	};
	EType Type() const;
};
static_assert(sizeof(MagicItem) == 0x1C);


// 18
class TESModel : public BaseFormComponent
{
public:
	TESModel();
	~TESModel();

	enum
	{
		kFacegenFlag_Head = 0x01,
		kFacegenFlag_Torso = 0x02,
		kFacegenFlag_RightHand = 0x04,
		kFacegenFlag_LeftHand = 0x08,
	};

	virtual void* Destroy(bool noDealloc);	// 04
	virtual char* GetModelPath();
	virtual void	SetModelPath(char* path);	// 06

	String	nifPath;		// 04
	UInt32	unk0C;			// 0C	referenced when saving Texture Hashes, init'd as a byte or is it a pointer to a structure starting with a byte followed by a pointer to some allocated data ?
	void* unk10;		// 10
	UInt8	facegenFlags;	// 14
	UInt8	pad15[3];		// 15

	void SetPath(const char* newPath) { nifPath.Set(newPath); }
};
static_assert(sizeof(TESModel) == 0x18);

// 18
class BGSTextureModel : public TESModel
{
public:
	BGSTextureModel();
	~BGSTextureModel();
};

// 020
class TESModelTextureSwap : public TESModel
{
public:
	TESModelTextureSwap();
	~TESModelTextureSwap();

	struct Texture
	{
		UInt32	textureID;			// 00
		UInt32	index3D;			// 04
		char	textureName[0x80];	// 08
	};	// there seem to be an array (length 6) post 0x88

	virtual void* Destroy(bool noDealloc);
	virtual char* GetPath();
	virtual void	SetPath(char* path);
	virtual void* Unk_07();

	TList<Texture> textureList;	// 018
};

// 008
class BGSClipRoundsForm : public BaseFormComponent
{
public:
	BGSClipRoundsForm();
	~BGSClipRoundsForm();

	UInt8	clipRounds;
	UInt8	padding[3];
	// 008
};

// 18
struct DestructionStage
{
	enum
	{
		kFlags_CapDamage = 1,
		kFlags_DisableObject = 2,
		kFlags_DestroyObject = 4,
	};

	UInt8				dmgStage;		// 00
	UInt8				healthPrc;		// 01
	UInt16				flags;			// 02
	UInt32				selfDmgSec;		// 04
	BGSExplosion* explosion;		// 08
	BGSDebris* debris;		// 0C
	UInt32				debrisCount;	// 10
	TESModelTextureSwap* replacement;	// 14
};

// 0C
struct DestructibleData
{
	UInt32				health;		// 00
	UInt8				stageCount;	// 04
	bool				targetable;	// 05
	UInt8				unk06[2];	// 06
	DestructionStage** stages;	// 08
};

// 08
class BGSDestructibleObjectForm : public BaseFormComponent
{
public:
	BGSDestructibleObjectForm();
	~BGSDestructibleObjectForm();

	DestructibleData* data;			// 04
};

static_assert(sizeof(BGSDestructibleObjectForm) == 0x8);

// 00C
class BGSPickupPutdownSounds : public BaseFormComponent
{
public:
	BGSPickupPutdownSounds();
	~BGSPickupPutdownSounds();

	TESSound* pickupSound;		// 004
	TESSound* putdownSound;		// 008
};

// 008
class BGSAmmoForm : public BaseFormComponent
{
public:
	BGSAmmoForm();
	~BGSAmmoForm();

	TESForm* ammo; // 04	either TESAmmo or BGSListForm
};

// 008
class BGSRepairItemList : public BaseFormComponent
{
public:
	BGSRepairItemList();
	~BGSRepairItemList();

	BGSListForm* listForm;	// 04
};

// 008
class BGSEquipType : public BaseFormComponent
{
public:
	BGSEquipType();
	~BGSEquipType();

	UInt32	equipType;	// 08
};

// 004
class BGSPreloadable : public BaseFormComponent
{
public:
	BGSPreloadable();
	~BGSPreloadable();

	virtual void	Fn_04(); // pure virtual
};

// 008
class BGSBipedModelList : public BaseFormComponent
{
public:
	BGSBipedModelList();
	~BGSBipedModelList();

	BGSListForm* models;		// 004
	// 008
};

// 018
class TESModelRDT : public TESModel
{
public:
	TESModelRDT();
	~TESModelRDT();

	virtual UInt32	Fn_07();
};

// 0DC
class TESBipedModelForm : public BaseFormComponent
{
public:
	TESBipedModelForm();
	~TESBipedModelForm();

	// bit indices starting from lsb
	enum ePartBit {
		ePart_Head = 0,
		ePart_Hair,
		ePart_UpperBody,
		ePart_LeftHand,
		ePart_RightHand,
		ePart_Weapon,
		ePart_PipBoy,
		ePart_Backpack,
		ePart_Necklace,
		ePart_Headband,
		ePart_Hat,
		ePart_Eyeglasses,
		ePart_Nosering,
		ePart_Earrings,
		ePart_Mask,
		ePart_Choker,
		ePart_MouthObject,
		ePart_BodyAddon1,
		ePart_BodyAddon2,
		ePart_BodyAddon3
	};

	enum EPartBitMask {
		ePartBitMask_Full = 0x07FFFF,
	};

	enum ESlot {
		eSlot_Head = 0x1 << ePart_Head,
		eSlot_Hair = 0x1 << ePart_Hair,
		eSlot_UpperBody = 0x1 << ePart_UpperBody,
		eSlot_LeftHand = 0x1 << ePart_LeftHand,
		eSlot_RightHand = 0x1 << ePart_RightHand,
		eSlot_Weapon = 0x1 << ePart_Weapon,
		eSlot_PipBoy = 0x1 << ePart_PipBoy,
		eSlot_Backpack = 0x1 << ePart_Backpack,
		eSlot_Necklace = 0x1 << ePart_Necklace,
		eSlot_Headband = 0x1 << ePart_Headband,
		eSlot_Hat = 0x1 << ePart_Hat,
		eSlot_Eyeglasses = 0x1 << ePart_Eyeglasses,
		eSlot_Nosering = 0x1 << ePart_Nosering,
		eSlot_Earrings = 0x1 << ePart_Earrings,
		eSlot_Mask = 0x1 << ePart_Mask,
		eSlot_Choker = 0x1 << ePart_Choker,
		eSlot_MouthObject = 0x1 << ePart_MouthObject,
		eSlot_BodyAddon1 = 0x1 << ePart_BodyAddon1,
		eSlot_BodyAddon2 = 0x1 << ePart_BodyAddon2,
		eSlot_BodyAddon3 = 0x1 << ePart_BodyAddon3
	};

	enum EBipedFlags {
		eBipedFlag_HasBackPack = 0x4,
		eBipedFlag_MediumArmor = 0x8,
		eBipedFlag_PowerArmor = 0x20,
		eBipedFlag_NonPlayable = 0x40,
		eBipedFlag_HeavyArmor = 0x80,
	};

	enum EBipedPath {
		ePath_Biped,
		ePath_Ground,
		ePath_Icon,
		ePath_Max
	};

	// missing part mask and flags
	UInt32					partMask;			// 004
	UInt8					bipedFlags;			// 008
	UInt8					fil009[3];			// 009 seen badly initialized (ArmorCombat)
	TESModelTextureSwap		bipedModel[2];		// 00C
	TESModelTextureSwap		groundModel[2];		// 04C
	TESIcon					icon[2];			// 08C
	BGSMessageIcon			messageIcon[2];		// 0A4
	TESModelRDT				modelRDT;			// 0C4
	// 0DC

	static UInt32 MaskForSlot(UInt32 mask);

	bool IsPowerArmor() const { return (bipedFlags & eBipedFlag_PowerArmor) == eBipedFlag_PowerArmor; }
	bool IsNonPlayable() const { return (bipedFlags & eBipedFlag_NonPlayable) == eBipedFlag_NonPlayable; }
	bool IsPlayable() const { return !IsNonPlayable(); }
	void SetPowerArmor(bool bPA) {
		if (bPA) {
			bipedFlags |= eBipedFlag_PowerArmor;
		}
		else {
			bipedFlags &= ~eBipedFlag_PowerArmor;
		}
	}
	void SetNonPlayable(bool bNP) {
		if (bNP) {
			bipedFlags |= eBipedFlag_NonPlayable;
		}
		else {
			bipedFlags &= ~eBipedFlag_NonPlayable;
		}
	}
	void  SetPath(const char* newPath, UInt32 whichPath, bool bfemalePath);
	const char* GetPath(UInt32 whichPath, bool bFemalePath);

	UInt32 GetSlotsMask() const;
	void SetSlotsMask(UInt32 mask);	// Limited by ePartBitMask_Full

	UInt32 GetBipedMask() const;
	void SetBipedMask(UInt32 mask);
};

static_assert(sizeof(TESBipedModelForm) == 0x0DC);

struct ContainerExtraData
{
	UInt32	owner;
	UInt32	globalOrRequiredRank;
	double	itemCondition;
};

// 00C
class TESContainer : public BaseFormComponent
{
public:
	TESContainer();
	~TESContainer();

	struct FormCount
	{
		SInt32				count;			//	00
		TESForm* form;			//	04
		ContainerExtraData* contExtraData;	//	08
	};

	typedef TList<FormCount> FormCountList;
	FormCountList formCountList;	// 004

	SInt32 GetCountForForm(TESForm* form);
};

// 00C
class BGSTouchSpellForm : public BaseFormComponent
{
public:
	BGSTouchSpellForm();
	~BGSTouchSpellForm();

	MagicItemForm* unarmedEffect;	// 04
	UInt16	unarmedAnim;	// 08
	UInt16	pad0A;		// 0A
};

// 034
class TESActorBaseData : public BaseFormComponent
{
public:
	TESActorBaseData();
	~TESActorBaseData();

	virtual void			Fn_04(TESForm* selectedForm);	// Called during form initialization after LoadForm and InitForm
	// flags access
	virtual bool			Fn_05();	// 00100000
	virtual bool			Fn_06();	// 00200000
	virtual bool			Fn_07();	// 10000000
	virtual bool			Fn_08();	// 20000000
	virtual bool			Fn_09();	// 80000000
	virtual bool			Fn_0A();	// 00400000
	virtual bool			Fn_0B();	// 00400000
	virtual bool			Fn_0C();	// 00800000
	virtual bool			Fn_0D();
	virtual bool			Fn_0E();
	virtual bool			Fn_0F();
	virtual bool			Fn_10();
	virtual bool			Fn_11();
	virtual bool			Fn_12();
	virtual void			Fn_13(void* arg);
	virtual bool			Fn_14();
	virtual void			Fn_15(void* arg);
	virtual UInt32			Fn_16();
	virtual void			Fn_17(void* arg);
	virtual UInt32			Fn_18();	// return unk08
	virtual float			Fn_19();	// return unk14
	virtual BGSVoiceType* GetVoiceType();

	enum
	{
		kFlags_Female = 1 << 0,
		kFlags_CreatureBiped = kFlags_Female,
		kFlags_Essential = 1 << 1,
		kFlags_HasCharGenFace = 1 << 2,
		kFlags_CreatureWeaponAndShield = kFlags_HasCharGenFace,
		kFlags_Respawn = 1 << 3,
		kFlags_AutoCalcStats = 1 << 4,
		kFlags_CreatureSwims = kFlags_AutoCalcStats,
		kFlags_CreatureFlies = 1 << 5,
		kFlags_CreatureWalks = 1 << 6,
		kFlags_PCLevelMult = 1 << 7,
		kFlags_UseTemplate = 1 << 8,
		kFlags_NoLowLevelProcessing = 1 << 9,
		//									1 << 10,
		kFlags_NoBloodSpray = 1 << 11,
		kFlags_NoBloodDecal = 1 << 12,
		//									1 << 13,
		//									1 << 14,
		kFlags_CreatureNoHead = 1 << 15,
		kFlags_CreatureNoRightArm = 1 << 16,
		kFlags_CreatureNoLeftArm = 1 << 17,
		kFlags_CreatureNoCombatInWater = 1 << 18,
		kFlags_CreatureNoShadow = 1 << 19,
		kFlags_NoVATSMelee = 1 << 20,
		kFlags_CreatureAllowPCDialogue = 1 << 21,
		kFlags_CanBeAllRaces = 1 << 22,
		kFlags_CreatureCantOpenDoor = kFlags_CanBeAllRaces,
		kFlags_CreatureImmobile = 1 << 23,
		kFlags_CreatureTiltFrontBack = 1 << 24,
		kFlags_CreatureTiltLeftRight = 1 << 25,
		kFlags_NoKnockdowns = 1 << 26,
		kFlags_NotPushable = 1 << 27,
		kFlags_CreatureAllowPickpocket = 1 << 28,
		kFlags_CreatureIsGhost = 1 << 29,
		kFlags_NoRotateToHeadTrack = 1 << 30,
		kFlags_CreatureInvulnerable = 1 << 31,
	};

	enum
	{
		kTemplateFlag_UseTraits = 1 << 0,
		kTemplateFlag_UseStats = 1 << 1,
		kTemplateFlag_UseFactions = 1 << 2,
		kTemplateFlag_UseActorEffectList = 1 << 3,
		kTemplateFlag_UseAIData = 1 << 4,
		kTemplateFlag_UseAIPackages = 1 << 5,
		kTemplateFlag_UseModelAnimation = 1 << 6,
		kTemplateFlag_UseBaseData = 1 << 7,
		kTemplateFlag_UseInventory = 1 << 8,
		kTemplateFlag_UseScript = 1 << 9,
	};

	struct FactionListData
	{
		TESFaction* faction;
		UInt8		rank;
		UInt8		pad[3];
	};

	UInt32			flags;				// 04	Comparing with LoadForm and FNVEdit
	UInt16			fatigue;			// 08	Fatique
	UInt16			barterGold;			// 0A	Barter Gold
	SInt16			level;				// 0C	Level/ Level Mult
	UInt16			calcMin;			// 0E	Calc min
	UInt16			calcMax;			// 10	Calc max
	UInt16			speedMultiplier;	// 12	Speed Multiplier (confirmed)
	float			Karma;				// 14	Karma
	UInt16			dispositionBase;	// 18	Disposition Base
	UInt16			templateFlags;		// 1A	Template Flags
	TESForm* deathItem;			// 1C	Death Item: object or FormList
	BGSVoiceType* voiceType;			// 20
	TESForm* templateActor;		// 24	Points toward Template
	UInt32			changedFlags;		// 28/000	Absent in Editor
	TList<FactionListData>	factionList;	// 2C/28

	SInt8 GetFactionRank(TESFaction* faction);

	bool IsFemale() { return flags & kFlags_Female ? true : false; }
};

// 14
class TESSpellList : public BaseFormComponent
{
public:
	enum
	{
		kModified_BaseSpellList = 0x00000020,
		// CHANGE_ACTOR_BASE_SPELLLIST
		//	UInt16	numSpells;
		//	UInt32	spells[numSpells];
	};

	TESSpellList();
	~TESSpellList();

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	TList<SpellItem>	spellList;			// 004
	TList<SpellItem>	leveledSpellList;	// 00C

	UInt32	GetSpellCount() const {
		return spellList.count();
	}

	// return the nth spell
	SpellItem* GetNthSpell(SInt32 whichSpell) const
	{
		return spellList.GetNth(whichSpell);
	}

	// removes all spells and returns how many spells were removed
	//UInt32 RemoveAllSpells();
};

// 020
class TESAIForm : public BaseFormComponent
{
public:
	TESAIForm();
	~TESAIForm();

	typedef TList<TESPackage> PackageList;

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	UInt8	agression;				// 04
	UInt8	confidence;				// 05 
	UInt8	energyLevel;			// 06
	UInt8	responsibility;			// 07
	UInt8	mood;					// 08
	UInt8	pad09[3];				// 09

	UInt32	buySellsAndServices;	// 0C
	UInt8	teaches;				// 10
	UInt8	maximumTrainingLevel;	// 11
	UInt8	assistance;				// 12
	UInt8	aggroRadiusBehavior;	// 13
	SInt32	aggroRadius;			// 14

	PackageList	packageList;	// 18

	UInt32	GetPackageCount() const {
		return packageList.count();
	}

	// return the nth package
	TESPackage* GetNthPackage(SInt32 anIndex) const
	{
		return packageList.GetNth(anIndex);
	}

	// replace the nth package
	TESPackage* SetNthPackage(TESPackage* pPackage, SInt32 anIndex)
	{
		return packageList.ReplaceNth(anIndex == -1 ? eListEnd : anIndex, pPackage);
	}

	// return the nth package
	SInt32 AddPackageAt(TESPackage* pPackage, SInt32 anIndex)
	{
		return packageList.AddAt(pPackage, anIndex == -1 ? eListEnd : anIndex);
	}

	TESPackage* RemovePackageAt(SInt32 anIndex)
	{
		return packageList.RemoveNth(anIndex == -1 ? eListEnd : anIndex);
	}

	// removes all packages and returns how many were removed
	UInt32 RemoveAllPackages() const
	{
		UInt32 cCount = GetPackageCount();
		packageList.RemoveAll();
		return cCount - GetPackageCount();
	}
};

// 00C
class TESAttributes : public BaseFormComponent
{
public:
	TESAttributes();
	~TESAttributes();

	enum
	{
		kStrength = 0,
		kPerception,
		kEndurance,
		kCharisma,
		kIntelligence,
		kAgility,
		kLuck,
	};

	UInt8	attributes[7];	// 4
	UInt8	padB;			// B
};

// 00C
class TESAnimation : public BaseFormComponent
{
public:
	TESAnimation();
	~TESAnimation();

	typedef TList<char> AnimNames;

	// constructor and Fn_01 sugest this is a TList of char string.
	// In GECK it is an array
	AnimNames	animNames;
	// 00C
};

// 14
class TESModelList : public BaseFormComponent
{
public:
	TESModelList();
	~TESModelList();

	TList<char>	modelList;	// 04
	UInt32		count;		// 0C
	UInt32		unk10;		// 10

	SInt32 GetIndex(char* path);
	bool RemoveEntry(char* nifToRemove);
	bool AddEntry(char* nifToAdd);
	bool ModelListAction(char* path, SInt8 action);
};

// 008
class TESDescription : public BaseFormComponent
{
public:
	TESDescription();
	~TESDescription();

	virtual char* Get(TESForm* overrideForm, UInt32 chunkID);

	UInt32	formDiskOffset;	// 4 - how does this work for descriptions in mods?
	// maybe extracts the mod ID then uses that to find the src file?
};

// 10
class TESReactionForm : public BaseFormComponent
{
public:
	TESReactionForm();
	~TESReactionForm();

	struct Reaction
	{
		enum
		{
			kNeutral = 0,
			kEnemy,
			kAlly,
			kFriend
		};

		TESFaction* faction;
		SInt32		modifier;
		UInt32		reaction;
	};

	TList <Reaction>	reactions;	// 4
	UInt8	unkC;		// C
	UInt8	padD[3];	// D
};

// 08
class TESRaceForm : public BaseFormComponent
{
public:
	TESRaceForm();
	~TESRaceForm();

	TESRace* race;	// 04
};

// C?
class TESSoundFile : public BaseFormComponent
{
public:
	TESSoundFile();
	~TESSoundFile();

	virtual void	Set(const char* str);

	String	path;	// 4
};

class TESModelAnim : public TESModel
{
public:
	TESModelAnim();		// Identical to TESModel with a different vTable
	~TESModelAnim();

};	// 018

class BaseExtraList
{
public:
	virtual	BaseExtraList* Destroy(char chr);

	bool						HasType(UInt32 type) const;
	__forceinline BSExtraData* GetByType(UInt8 type) const { return ThisStdCall<BSExtraData*>(0x410220, this, type); };
	void						MarkType(UInt32 type, bool bCleared);
	__forceinline void			Remove(BSExtraData* toRemove, bool doFree) { ThisStdCall(0x410020, this, toRemove, doFree); }
	__forceinline void			RemoveByType(UInt32 type) { ThisStdCall(0x410140, this, type); }
	__forceinline BSExtraData* Add(BSExtraData* toAdd) { return ThisStdCall<BSExtraData*>(0x40FF60, this, toAdd); }
	__forceinline void			RemoveAll(bool doFree) { ThisStdCall(0x40FAE0, this, doFree); }
	bool						MarkScriptEvent(UInt32 eventMask, TESForm* eventTarget);
	__forceinline void			Copy(BaseExtraList* from) { ThisStdCall(0x411EC0, this, from); }

	enum JIPExtraListFlags
	{
		kJIPExtraListFlag_Update3D = 1 << 0,
		kJIPExtraListFlag_DisableCollision = 1 << 1,
		kJIPExtraListFlag_AltRefName = 1 << 2,
		kJIPExtraListFlag_RefrModelPath = 1 << 3,
		kJIPExtraListFlag_RefNameOrModel = kJIPExtraListFlag_AltRefName | kJIPExtraListFlag_RefrModelPath,
		kJIPExtraListFlag_NoZPosReset = 1 << 4,
	};

	BSExtraData* m_data;						// 004
	UInt8			m_presenceBitfield[0x13];	// 008 - if a bit is set, then the extralist should contain that extradata
	UInt8			JIPExtraListFlags;			// 1B	0x5F in TESObjectREFR
	UInt16			JIPPerksInfo;
};
static_assert(sizeof(BaseExtraList) == 0x020);

class ExtraDataList : public BaseExtraList
{
public:
	static ExtraDataList* Create(BSExtraData* xBSData = NULL);
};
static_assert(sizeof(ExtraDataList) == 0x020);

class ExtendDataList : public TList<ExtraDataList>
{
public:
	static ExtendDataList* Create(ExtraDataList* pExtraDataList = nullptr);
	static void Free(ExtendDataList* xData, bool bFreeList = false);
	static bool Add(ExtendDataList* xData, ExtraDataList* xList);
	static bool Remove(ExtendDataList* xData, ExtraDataList* xList, bool bFreeList = false);
};

typedef std::vector<ExtendDataList*> ExtendDataArray;


class TESLeveledList : public BaseFormComponent
{
public:
	struct LoadBaseData	// as used by LoadForm
	{
		SInt16			level;		// 000
		UInt16			fill002;	// 002
		TESObjectREFR* refr;		// 004
		SInt16			count;		// 008
		UInt16			fill00A;	// 00A
	};	// 00C

	struct BaseData
	{
		TESObjectREFR* refr;		// 000
		SInt16				count;		// 004
		SInt16				level;		// 006
		ContainerExtraData* coed;		// 008
	};	// 00C

	TList<BaseData>	datas;			// 004
	UInt8			chanceNone;		// 00C
	UInt8			flags;			// 00D
	UInt8			fill00E[2];		// 00E
	UInt32			unk010;			// 010	LVLG
	ExtraDataList	extraDatas;		// 014
};	// 01C

// 08
class TESRegionData : public BaseFormComponent
{
public:
	TESRegionData();
	~TESRegionData();

	bool		bOverride;	// 04
	UInt8		unk5;		// 05
	UInt8		priority;	// 06
	UInt8		unk7;		// 07
};
typedef TList<TESRegionData> RegionDataEntryList;

class TESRegionDataGrass : public TESRegionData
{
public:
	TESRegionDataGrass();
	~TESRegionDataGrass();
};

// 10
class TESRegionDataImposter : public TESRegionData
{
public:
	TESRegionDataImposter();
	~TESRegionDataImposter();

	TList<TESObjectREFR>	imposters;	// 08
};

class TESRegionDataLandscape : public TESRegionData
{
public:
	TESRegionDataLandscape();
	~TESRegionDataLandscape();
};

// 10
class TESRegionDataMap : public TESRegionData
{
public:
	TESRegionDataMap();
	~TESRegionDataMap();

	String	mapName;	// 08
};

struct SoundType
{
	TESSound* sound;
	UInt32			flags;
	UInt32			chance;
};
typedef TList<SoundType> SoundTypeList;

// 20
class TESRegionDataSound : public TESRegionData
{
public:
	TESRegionDataSound();
	~TESRegionDataSound();

	UInt32			unk08;					// 08
	SoundTypeList	soundTypes;				// 0C
	UInt32			incidentalMediaSet;		// 14
	TList<UInt32>	mediaSetEntries;		// 18
};

struct WeatherEntry
{
	TESWeather* weather;
	UInt32			chance;
	TESGlobal* global;

	void Set(TESWeather* pWtr, UInt32 pChn, TESGlobal* pGlb)
	{
		weather = pWtr;
		chance = pChn;
		global = pGlb;
	}
};
typedef TList<WeatherEntry> WeatherTypes;

// 10
class TESRegionDataWeather : public TESRegionData
{
public:
	TESRegionDataWeather();
	~TESRegionDataWeather();

	WeatherTypes	weatherTypes;	// 08
};


class BGSBodyPart : public BaseFormComponent
{
public:
	BGSBodyPart();
	~BGSBodyPart();

	enum
	{
		kFlags_Severable = 1 << 0,
		kFlags_IKData = 1 << 1,
		kFlags_BipedData = 1 << 2,
		kFlags_Explodable = 1 << 3,
		kFlags_IsHead = 1 << 4,
		kFlags_Headtracking = 1 << 5,
		kFlags_Absolute = 1 << 6,
	};

	String				partNode;				// 04
	String				VATSTarget;				// 0C
	String				startNode;				// 14
	String				partName;				// 1C
	String				targetBone;				// 24
	TESModel			limbReplacement;		// 2C
	UInt32				unk44[6];				// 44	Another model?
	float				damageMult;				// 5C
	UInt8				flags;					// 60
	UInt8				pad61;					// 61
	UInt8				healthPercent;			// 62
	UInt8				actorValue;				// 63
	UInt8				toHitChance;			// 64
	UInt8				explChance;				// 65
	UInt8				explDebrisCount;		// 66
	UInt8				pad67;					// 67
	BGSDebris* explDebris;				// 68
	BGSExplosion* explExplosion;			// 6C
	float				trackingMaxAngle;		// 70
	float				explDebrisScale;		// 74
	UInt8				sevrDebrisCount;		// 78
	UInt8				pad79[3];				// 79
	BGSDebris* sevrDebris;				// 7C
	BGSExplosion* sevrExplosion;			// 80
	float				sevrDebrisScale;		// 84
	float				goreEffTranslate[3];	// 88
	float				goreEffRotation[3];		// 94
	BGSImpactDataSet* sevrImpactDS;			// A0
	BGSImpactDataSet* explImpactDS;			// A4
	UInt8				sevrDecalCount;			// A8
	UInt8				explDecalCount;			// A9
	UInt8				padAA[2];				// AA
	float				limbRepScale;			// AC

	void SetFlag(UInt32 pFlag, bool bMod) { flags = bMod ? (flags | pFlag) : (flags & ~pFlag); }
};
static_assert(sizeof(BGSBodyPart) == 0xB0);
