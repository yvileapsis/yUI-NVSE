#pragma once
#include "NiPoint3.hpp"
#include "Character.hpp"
#include "BSSimpleList.hpp"
#include "TESRegion.hpp"
#include "TESSound.hpp"
#include "QuestTarget.hpp"
#include "BSSoundHandle.hpp"

class bhkRigidBody;
class bhkMouseSpringAction;

class AlchemyItem;

class BGSNote;
class BGSQuestObjective;
class BGSMusicType;
class BGSEntryPointPerkEntry;

class CameraCaster;
class CombatGroup;
class DialoguePackage;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;
class MagicItem;
class MagicShaderHitEffect;
class MagicTarget;
class MusicMarker;
class QuestObjectiveTargets;

class TESCaravanCard;
class TESClass;
class TESEnchantableForm;
class TESObjectCELL;
class TESObjectREFR;
class TESObjectWEAP;
class TESQuest;
class TESReputation;
class TESTopic;
class TESWorldSpace;

struct AnimData;
struct BipedAnim;
class HighProcess;

class PlayerCharacter : public Character {
public:
	struct WobbleNodes
	{
		NiNode*	pWobbleAnimNodes[12];
		NiNode*	pWobbleAnimNodes2[12];
	};

	struct LevelupInfo 
	{
		bool	bShouldLevelUp;
		UInt32	uiXPForNextLevel;
		bool	byte08;
	};

	struct CompassTarget {
		Actor*	pTarget;
		bool	bIsHostile;
		bool	bIsUndetected;
	};



	struct PositionRequest
	{
		TESWorldSpace*	pkWorldSpace;
		TESObjectCELL*	pkCell;
		NiPoint3		kPosition;
		NiPoint3		kRotation;
		UInt8			bResetWeather;
		UInt8			gap21[3];
		void			(__cdecl* pfnCallbackFunc)(void*);
		void*			pCallbackFuncArg;
		TESObjectREFR*	pkDestination;
		TESObjectREFR*	pkFastTravel;
	};

	struct FlyCamData
	{
		Float32		fRotX;
		Float32		fRotZ;
		NiPoint3	kPosition;
	};
	static_assert(sizeof(FlyCamData) == 0x14);

	enum GrabMode
	{
		kGrabMode_ZKey = 0x1,
		kGrabMode_Telekenesis = 0x3,
	};

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};

	enum KillcamMode
	{
		kKillcamMode_None = 0x0,
		kKillcamMode_PlayerView = 0x1,
		kKillcamMode_Cinematic = 0x2,
	};

	PlayerCharacter();
	~PlayerCharacter() override;
	virtual void		ReturnFalse();
	virtual void		GetPerkRanks();

	UInt32								unk1C8;
	UInt32								unk1CC;
	UInt32								unk1D0;
	UInt32								unk1D4;
	UInt32								unk1D8;
	UInt32								unk1DC;
	UInt32								unk1E0;
	UInt32								unk1E4;
	UInt32								unk1E8;
	PositionRequest*					pkPositionRequest;
	TESObjectWEAP*						pkWeap1F0;
	Float32								fTime1F4;
	UInt8								byte1F8;
	UInt8								gap1F9[3];
	UInt32								sandmanDetectionValue;
	UInt32								unk200;
	UInt8								byte204;
	UInt8								byte205;
	UInt8								bQuestTargetsNeedRecalculated;
	UInt8								byte207;
	DialoguePackage*					pkClosestConversation;
	bool								bIsPlayerMovingIntoNewSpace;
	BSSimpleList<ActiveEffect*>*		pkActiveEffectLists;
	MagicItem*							pkMagicItem214;
	MagicTarget*						pkMagicTarget218;
	CameraCaster*						pkCameraCaster;
	UInt32								unk220;
	UInt32								unk224;
	UInt32 								unk228;
	Float32								time22C;
	UInt32								unk230;
	Float32								time234;
	BSSimpleList<UInt32>*				list238;
	BSSimpleList<TESEnchantableForm*>*	enchantmentList23C;
	bool								showQuestItems;
	UInt8								byte241;
	UInt8								byte242;
	UInt8								byte243;
	Float32								fActorValue244[77];
	Float32								fPermAVMods[77];
	Float32								fHealthAV_4AC;
	Float32								fActorValues4B0[77];
	BSSimpleList<BGSNote*>				kNotes;
	ImageSpaceModifierInstanceDOF*		pIronSightsDOFInstance;
	ImageSpaceModifierInstanceDOF*		pVATSDOFInstance;
	ImageSpaceModifierInstanceDRB*		pVATSDRBInstance;
	bool								bIsDetected;
	bool								bPreventRegionSoundUpdates;
	UInt8								byte5FA;
	UInt8								byte5FB;
	BSSimpleList<Actor*>				pkTeammateList;
	TESObjectREFR*						pkLastExteriorDoor;
	UInt8								bIsPlayingCombatAttackSound;
	BSSimpleList<UInt32>*				pkActionList;
	BSSimpleList<UInt32>*				pkCasinoDataList;
	BSSimpleList<TESCaravanCard*>*		pkCaravanCards1;
	BSSimpleList<TESCaravanCard*>*		pkCaravanCards2;
	UInt32								uiCaravanCapsEarned;
	UInt32								uiCaravanCapsLost;
	UInt32								uiNumCaravanWins;
	UInt32								uiNumCaravanLosses;
	UInt32								uiLargestCaravanWin;
	UInt32								unk630;
	bhkMouseSpringAction*				pkGrabSpringAction;
	TESObjectREFR*						pkGrabbedRef;
	GrabMode							eGrabType;
	Float32								fCurrentGrabbedItemWeight;
	Float32								fGrabDistance;
	UInt8								byte648;
	UInt8								byte649;
	bool								bIs3rdPersonVisible;
	bool								bIs3rdPerson;
	bool								bIs3rdPersonConsistent;
	bool								bShouldRestoreFirstPersonAfterVanityMode;
	bool								bWasFirstPersonBeforeVanityCam;
	bool								bIsUsingScope; // bIsForceFirstPerson
	UInt8								byte650;
	bool								bAlwaysRun;
	bool								bAutoMove;
	UInt8								byte653;
	UInt32								uiSleepHours;
	bool								bIsResting;
	UInt8								byte659;
	UInt8								byte65A;
	UInt8								byte65B;
	Float32								unk65C;
	Float32								fTime660;
	Float32								fTime664;
	Float32								fUnused_time668;
	bool								bIsActorWithinSneakSkillUseDistance;
	UInt8								flag66D;
	bool								bCanSleepWait;
	UInt8								byte66F;
	Float32								fWorldFOV;
	Float32								f1stPersonFOV;
	Float32								fOverShoulderFOV;
	UInt32								unk67C;
	UInt8								eControlFlags;
	bool								bIsWaitingForOpenContainerAnim;
	UInt8								byte682;
	UInt8								byte683;
	Float32								fWaitingForContainerOpenAnimPreventActivateTimer;
	TESObjectREFR*						pkActivatedDoor;
	BipedAnim*							pkBipedAnims1st;
	AnimData*							pkAnimData1st;
	NiPointer<NiNode>					spkPlayerNode;
	Float32								fEyeHeight;
	NiNode*								pkInventoryMenu;
	AnimData*							pkAnimData6A0;
	MagicShaderHitEffect*				pkMagicShaderHitEffect;
	BSSimpleList<UInt32>				kTopicList;
	BSSimpleList<UInt32>				kList6B0;
	TESQuest*							pkActiveQuest;
	BSSimpleList<BGSQuestObjective*>	kQuestObjectiveList;
	BSSimpleList<UInt32>				kQuestTargetList;
	bool								bPlayerGreetFlag;
	Float32								fPlayerGreetTimer;
	UInt32								unk6D4;
	bool								bIsAMurderer;
	UInt32								uiAmountSoldStolen;
	Float32								fSortActorDistanceListTimer;
	Float32								fSeatedRotation;
	UInt8								byte6E8;
	UInt8								byte6E9;
	UInt8								byte6EA;
	UInt8								byte6EB;
	MagicItem*							pkMagicItem6EC;
	UInt32								playerSpell;
	TESObjectREFR*						pkPlacedMarker;
	QuestTarget							kPlacedMarkerTarget;
	Float32								fTimeGrenadeHeld;
	UInt32								unk734;
	UInt32								unk738;
	TESClass*							pClass73C_unset;
	UInt32								unk740;
	UInt32								uiCrimeCounts[5];
	AlchemyItem*						pkPoisonToApply;
	bool								bInCharGen;
	UInt8								byte75D;
	bool								bCanUseTelekinesis;
	UInt8								byte75F;
	TESRegion*							pkCurrentRegion;
	BSSimpleArray<TESRegion*>			pkRegionList;
	BSSimpleList<UInt32>				list774;
	BSSoundHandle						kHeartBeatSound;
	UInt32								unk788;
	UInt32								iLastPlayingTimeUpdate;
	UInt32								iTotalPlayingTime;
	UInt32								unk794;
	bool								bIsInSleepPackage;
	bool								bIsTransitioningSeatedState;
	bool								bIsDrinkingPlacedWater;
	bool								bIsPackagePlaying;
	UInt8								byte79C;
	UInt8								byte79D;
	NiPoint3							kLastBorderRegionPosition;
	union
	{
		TESWorldSpace*					pkWorldSpace;
		TESObjectCELL*					pkParentCell;
	};
	NiTPrimitiveArray<TESRegion>*		pRegionArray;
	BGSMusicType*						pMusicType;
	UInt32								uiGameDifficulty;
	bool								bIsHardcore;
	KillcamMode							eKillCamMode;
	bool								bInCombatWithGuard;
	bool								bIsYoung;
	bool								bIsToddler;
	bool								bCanUsePA;
	BSSimpleList<UInt32>				kMapMarkers;
	void*								pWorldspaceForMapMarkerUpdates;
	BSSimpleList<MusicMarker*>			kMusicMarkers;
	MusicMarker*						pCurrMusicMarker;
	FlyCamData							kFlycamPos;
	UInt32								unk7F4;
	BSSoundHandle						sound7F8_unused;
	BSSoundHandle						unk808[6];
	BSSimpleList<TESObjectREFR*>		kDroppedRefList;
	NiTMap<UInt32, UInt8>				unk858;
	UInt32								unk868;
	UInt32								unk86C;
	bool								bHasShownNoChargeWarning;
	Float32								fDropAngleMod;
	Float32								fLastDropAngleMod;
	LevelupInfo*						pkLevelUpInfo;
	BSSimpleList<UInt32>				kPerkRanksPC;
	BSSimpleList<BGSEntryPointPerkEntry*> kPerkEntriesPC[74];
	BSSimpleList<UInt32>				kPerkRanksTM;
	BSSimpleList<BGSEntryPointPerkEntry*> kPerkEntriesTM[74];
	Actor*								pkAutoAimActor;
	NiPoint3							kBulletAutoAim;
	NiPointer<NiNode>					spAutoAimNode;
	bool								bIsActorWithinInterfaceManagerPickDistance;
	Actor*								pkReticleActor;
	BSSimpleList<CompassTarget*>*		pkCompassTargets;
	Float32								fPipboyLightHeldTime;
	Float32								fAmmoSwapTimer;
	bool								bShouldOpenPipboy;
	char								byteD55;
	char								byteD56;
	char								byteD57;
	NiPoint3							kPtD58;
	CombatGroup*						pkCombatGroup;
	UInt32								uiTeammateCount;
	Float32								fCombatYieldTimer;
	Float32								fCombatYieldRetryTimer;
	WobbleNodes							kWobbleAnims;
	NiPoint3							kVectorDD4;
	NiPoint3							kCameraPos;
	bhkRigidBody*						pkRigidBody;
	bool								bInCombat;
	bool								bIsUnseen;
	bool								bIsLODApocalypse;
	bool								byteDF3;
	BSSimpleArray<UInt32>				kRockitLauncherContainer;
	Float32								fRockItLauncherWeight;
	bool								bHasNightVisionApplied;
	char								byteE09;
	char								byteE0A;
	char								byteE0B;
	TESReputation*						pkModifiedReputation;
	int									unkE10;
	int									unkE14;
	Float32								fKillCamTimer;
	Float32								fKillCamCooldown;
	char								byteE20;
	bool								bIsUsingTurbo;
	char								byteE22;
	char								byteE23;
	Float32								fLastHelloTime;
	Float32								fCounterAttackTimer;
	char								byteE2C;
	bool								bIsCateyeEnabled;
	bool								bIsSpottingImprovedActive;
	char								byteE2F;
	Float32								fItemDetectionTimer;
	NiNode*								pkIronSightNode;
	bool								bNoHardcoreTracking;
	bool								bSkipHCNeedsUpdate;
	char								byteE3A;
	char								byteE3B;
	BSSimpleArray<TESAmmo*>				kHotkeyedWeaponAmmos;
	TESQuest*							pPatch04DebugQuest;

	bool IsSleepingorResting() { return uiSleepHours > 0; }

	bool HasPipBoyOpen() { return ThisStdCall(0x967AE0, this); }

	bool IsVanityMode();

	NiNode* GetNode(const bool bCurrentActive);

	void UpdatePlayerControlsMask(bool abEnable, UInt32 aMask);

	bool					IsThirdPerson() { return bIs3rdPersonConsistent; }
	UInt32					GetMovementFlags() { return pkActorMover->GetMovementFlags(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool					IsPlayerSwimming() { return GetMovementFlags()  >> 11 & 1; }

	__forceinline static PlayerCharacter*	GetSingleton() { return *reinterpret_cast<PlayerCharacter**>(0x011DEA3C); };
	bool					SetSkeletonPath(const char* newPath);
	bool					SetSkeletonPath_v1c(const char* newPath);	// Less worse version as used by some modders
	static void				UpdateHead();
	QuestObjectiveTargets*	GetCurrentQuestObjectiveTargets();
	TESObjectREFR*			GetPlacedMarkerOrTeleportLink();
	HighProcess*			GetHighProcess() { return reinterpret_cast<HighProcess*>(pkBaseProcess); };
	__forceinline bool		UsingIronSights() { return pkIronSightNode && pkBaseProcess->IsWeaponOut() || pkBaseProcess->IsAiming(); }
};
static_assert(sizeof(PlayerCharacter) == 0xE50);