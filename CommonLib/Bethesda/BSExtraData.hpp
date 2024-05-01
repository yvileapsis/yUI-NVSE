#pragma once

#define GetByTypeCast(kExtraList, Type) DYNAMIC_CAST((kExtraList).GetExtra(BSExtraData::k ## Type), BSExtraData, Type)

// 0xC
class BSExtraData
{
public:
	enum Type : UInt8
	{
		kExtraHavok						= 0x01,
		kExtraCell3D					= 0x02,
		kExtraCellWaterType				= 0x03,
		kExtraRegionList				= 0x04,
		kExtraSeenData					= 0x05,
		//								= 0x06,
		kExtraCellMusicType				= 0x07,
		kExtraCellClimate 				= 0x08,
		kExtraProcessMiddleLow 			= 0x09,
		kExtraCellCanopyShadowMask		= 0x0A,
		kExtraDetachTime 				= 0x0B,
		kExtraPersistentCell 			= 0x0C,
		kExtraScript 					= 0x0D,
		kExtraAction 					= 0x0E,
		kExtraStartingPosition			= 0x0F,
		kExtraAnim 						= 0x10,
		// 								= 0x11,
		kExtraUsedMarkers 				= 0x12,
		kExtraDistantData				= 0x13,
		kExtraRagdollData 				= 0x14,
		kExtraContainerChanges 			= 0x15,
		kExtraWorn 						= 0x16,
		kExtraWornLeft 					= 0x17,
		kExtraPackageStartLocation		= 0x18,
		kExtraPackage 					= 0x19,
		kExtraTrespassPackage			= 0x1A,
		kExtraRunOncePacks 				= 0x1B,
		kExtraReferencePointer 			= 0x1C,
		kExtraFollower 					= 0x1D,
		kExtraLevCreaModifier 			= 0x1E,
		kExtraGhost 					= 0x1F,
		kExtraOriginalReference			= 0x20,
		kExtraOwnership 				= 0x21,
		kExtraGlobal 					= 0x22,
		kExtraRank 						= 0x23,
		kExtraCount 					= 0x24,
		kExtraHealth 					= 0x25,
		kExtraUses 						= 0x26,
		kExtraTimeLeft 					= 0x27,
		kExtraCharge 					= 0x28,
		kExtraLight 					= 0x29,
		kExtraLock 						= 0x2A,
		kExtraTeleport 					= 0x2B,
		kExtraMapMarker 				= 0x2C,
		kExtraLeveledCreature 			= 0x2E,
		kExtraLeveledItem 				= 0x2F,
		kExtraScale 					= 0x30,
		kExtraSeed 						= 0x31,
		kExtraNonActorMagicCaster		= 0x32,
		kExtraNonActorMagicTarget		= 0x33,
		kExtraPlayerCrimeList			= 0x35,
		kExtraEnableStateParent			= 0x37,
		kExtraEnableStateChildren		= 0x38,
		kExtraItemDropper 				= 0x39,
		kExtraDroppedItemList 			= 0x3A,
		kExtraRandomTeleportMarker		= 0x3B,
		kExtraMerchantContainer			= 0x3C,
		kExtraSavedHavokData 			= 0x3D,
		kExtraCannotWear 				= 0x3E,
		kExtraPoison 					= 0x3F,
		kExtraUnk040 					= 0x40,	// referenced during LoadFormInModule (in oposition to kLight)
		kExtraLastFinishedSequence		= 0x41,
		kExtraSavedAnimation			= 0x42,
		kExtraNorthRotation 			= 0x43,
		kExtraXTarget 					= 0x44,
		kExtraFriendHits 				= 0x45,
		kExtraHeadingTarget 			= 0x46,
		kExtraRefractionProperty		= 0x48,
		kExtraStartingWorldOrCell		= 0x49,
		kExtraHotkey 					= 0x4A,
		kExtraEditorRefMovedData		= 0x4C,
		kExtraInfoGeneralTopic 			= 0x4D,
		kExtraHasNoRumors 				= 0x4E,
		kExtraSound 					= 0x4F,
		kExtraTerminalState 			= 0x50,
		kExtraLinkedRef 				= 0x51,
		kExtraLinkedRefChildren 		= 0x52,
		kExtraActivateRef 				= 0x53,
		kExtraActivateRefChildren		= 0x54,
		kExtraTalkingActor 				= 0x55,
		kExtraObjectHealth 				= 0x56,
		kExtraDecalRefs 				= 0x57,
		kExtraCellImageSpace 			= 0x59,
		kExtraNavMeshPortal 			= 0x5A,
		kExtraModelSwap 				= 0x5B,
		kExtraRadius 					= 0x5C,
		kExtraRadiation 				= 0x5D,
		kExtraFactionChanges 			= 0x5E,
		kExtraDismemberedLimbs 			= 0x5F,
		kExtraMultiBound 				= 0x61,
		kExtraMultiBoundData 			= 0x62,
		kExtraMultiBoundRef				= 0x63,
		kExtraReflectedRefs 			= 0x65,
		kExtraReflectorRefs 			= 0x66,
		kExtraEmittanceSource 			= 0x67,
		kExtraRadioData 				= 0x68,
		kExtraCombatStyle 				= 0x69,
		kExtraPrimitive 				= 0x6B,
		kExtraOpenCloseActivateRef		= 0x6C,
		kExtraAnimNoteReciever			= 0x6D,
		kExtraAmmo 						= 0x6E,
		kExtraPatrolRefData 			= 0x6F,
		kExtraPackageData 				= 0x70,
		kExtraOcclusionPlane 			= 0x71,
		kExtraCollisionData				= 0x72,
		kExtraSayTopicInfoOnceADay		= 0x73,
		kExtraEncounterZone				= 0x74,
		kExtraSayToTopicInfo			= 0x75,
		kExtraOcclusionPlaneRefData		= 0x76,
		kExtraPortalRefData				= 0x77,
		kExtraPortal					= 0x78,
		kExtraRoom						= 0x79,
		kExtraHealthPerc				= 0x7A,
		kExtraRoomRefData				= 0x7B,
		kExtraGuardedRefData			= 0x7C,
		kExtraCreatureAwakeSound		= 0x7D,
		kExtraWaterZoneMap				= 0x7E,
		kExtraIgnoredBySandbox			= 0x80,
		kExtraCellAcousticSpace			= 0x81,
		kExtraReservedMarkers			= 0x82,
		kExtraWeaponIdleSound			= 0x83,
		kExtraWaterLightRefs			= 0x84,
		kExtraLitWaterRefs				= 0x85,
		kExtraWeaponAttackSound			= 0x86,
		kExtraActivateLoopSound			= 0x87,
		kExtraPatrolRefInUseData		= 0x88,
		kExtraAshPileRef				= 0x89,
		kExtraCreatureMovementSound		= 0x8A,
		kExtraFollowerSwimBreadcrumbs	= 0x8B,
		//								= 0x8C,
		kExtraWeaponModFlags			= 0x8D,
		//								= 0x8E
		//								= 0x8F
		kExtra0x90						= 0x90,	// referenced in LoadGame but no data
		kExtra0x91						= 0x91,	// referenced in LoadGame but no data
		kExtraSpecialRenderFlags		= 0x92
	};


	BSExtraData();

	virtual			~BSExtraData();
	virtual bool	Differs(BSExtraData* extra);	// 001

	Type			eType;
	UInt8			pad[3];
	BSExtraData*	pkNext;

	static BSExtraData* Create(UInt8 xType, UInt32 size, UInt32 vtbl);
	static bool			IsCopyable(BSExtraData* apkExtra) { return StdCall<bool>(0x411F20, apkExtra); }
};
static_assert(sizeof(BSExtraData) == 0xC);