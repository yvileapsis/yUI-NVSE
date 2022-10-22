#pragma once
// 4018 - signed/unsigned mismatch
// 4244 - loss of data by assignment
// 4267 - possible loss of data (truncation)
// 4305 - truncation by assignment
// 4288 - disable warning for crap microsoft extension screwing up the scope of variables defined in for loops
// 4311 - pointer truncation
// 4312 - pointer extension
// credits to Ian Patterson
#pragma warning(disable: 4018 4244 4267 4305 4288 4312 4311 26812)

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <winsock2.h>
#include <Windows.h>

#ifndef __NVSE_VERSION_H__
#define __NVSE_VERSION_H__

// these have to be macros so they can be used in the .rc
#define NVSE_VERSION_INTEGER		6
#define NVSE_VERSION_INTEGER_MINOR	2
#define NVSE_VERSION_INTEGER_BETA	8
#define NVSE_VERSION_VERSTRING		"0, 6, 2, 8"
#define NVSE_VERSION_PADDEDSTRING	"0006"

// build numbers do not appear to follow the same format as with oblivion
#define MAKE_NEW_VEGAS_VERSION_EX(major, minor, build, sub)	(((major & 0xFF) << 24) | ((minor & 0xFF) << 16) | ((build & 0xFFF) << 4) | (sub & 0xF))
#define MAKE_NEW_VEGAS_VERSION(major, minor, build)			MAKE_NEW_VEGAS_VERSION_EX(major, minor, build, 0)

// assume the major version number is 1.x
#define RUNTIME_VERSION_1_0_0_240	MAKE_NEW_VEGAS_VERSION(0, 0, 240)		// 0x00000F00
#define RUNTIME_VERSION_1_4_0_525	MAKE_NEW_VEGAS_VERSION(4, 0, 525)		// 0x040020D0

#define CS_VERSION_1_1_0_262		MAKE_NEW_VEGAS_VERSION(1, 0, 262)		// 0x01001060
#define CS_VERSION_1_4_0_518		MAKE_NEW_VEGAS_VERSION(4, 0, 518)		// 0x04002060

#define PACKED_NVSE_VERSION			MAKE_NEW_VEGAS_VERSION(NVSE_VERSION_INTEGER, NVSE_VERSION_INTEGER_MINOR, NVSE_VERSION_INTEGER_BETA)

#endif /* __NVSE_VERSION_H__ */

typedef unsigned char		UInt8;		//!< An unsigned 8-bit integer value
typedef unsigned short		UInt16;		//!< An unsigned 16-bit integer value
typedef unsigned long		UInt32;		//!< An unsigned 32-bit integer value
typedef unsigned long long	UInt64;		//!< An unsigned 64-bit integer value
typedef signed char			SInt8;		//!< A signed 8-bit integer value
typedef signed short		SInt16;		//!< A signed 16-bit integer value
typedef signed long			SInt32;		//!< A signed 32-bit integer value
typedef signed long long	SInt64;		//!< A signed 64-bit integer value
typedef float				Float32;	//!< A 32-bit floating point value
typedef double				Float64;	//!< A 64-bit floating point value

void _AssertionFailed(const char* file, unsigned long line, const char* desc);
void _AssertionFailed_ErrCode(const char* file, unsigned long line, const char* desc, unsigned long long code);
void _AssertionFailed_ErrCode(const char* file, unsigned long line, const char* desc, const char* code);

//! Exit the program if the condition is not true
#define ASSERT(a)					do { if(!(a)) _AssertionFailed(__FILE__, __LINE__, #a); } while(0)
//! Exit the program if the condition is not true, with an error message
#define ASSERT_STR(a, b)			do { if(!(a)) _AssertionFailed(__FILE__, __LINE__, b); } while(0)
//! Exit the program if the condition is not true, reporting an error code
#define ASSERT_CODE(a, b)			do { if(!(a)) _AssertionFailed_ErrCode(__FILE__, __LINE__, #a, b); } while(0)
//! Exit the program if the condition is not true, reporting an error code and message
#define ASSERT_STR_CODE(a, b, c)	do { if(!(a)) _AssertionFailed_ErrCode(__FILE__, __LINE__, b, c); } while(0)
//! Exit the program with an error message
#define HALT(a)						do { _AssertionFailed(__FILE__, __LINE__, a); } while(0)
//! Exit the program with and error code and message
#define HALT_CODE(a, b)				do { _AssertionFailed_ErrCode(__FILE__, __LINE__, a, b); } while(0)

// based on the boost implementation of static asserts
template <bool x> struct StaticAssertFailure;
template <> struct StaticAssertFailure <true> { enum { a = 1 }; };
template <int x> struct static_assert_test { };

#define VERSION_CODE(primary, secondary, sub)	(((primary & 0xFFF) << 20) | ((secondary & 0xFFF) << 8) | ((sub & 0xFF) << 0))
#define VERSION_CODE_PRIMARY(in)				((in >> 20) & 0xFFF)
#define VERSION_CODE_SECONDARY(in)				((in >> 8) & 0xFFF)
#define VERSION_CODE_SUB(in)					((in >> 0) & 0xFF)

template <typename T>
class Bitfield
{
public:
	Bitfield() { }
	~Bitfield() { }

	void	Clear(void) { field = 0; }						//!< Clears all bits
	void	RawSet(UInt32 data) { field = data; }			//!< Modifies all bits

	void	Set(UInt32 data) { field |= data; }				//!< Sets individual bits
	void	Clear(UInt32 data) { field &= ~data; }			//!< Clears individual bits
	void	UnSet(UInt32 data) { Clear(data); }				//!< Clears individual bits
	void	Mask(UInt32 data) { field &= data; }			//!< Masks individual bits
	void	Toggle(UInt32 data) { field ^= data; }			//!< Toggles individual bits
	void	Write(UInt32 data, bool state)
	{
		if (state) Set(data); else Clear(data);
	}

	T		Get(void) const { return field; }								//!< Gets all bits
	T		Get(UInt32 data) const { return field & data; }					//!< Gets individual bits
	T		Extract(UInt32 bit) const { return (field >> bit) & 1; }		//!< Extracts a bit
	T		ExtractField(UInt32 shift, UInt32 length)						//!< Extracts a series of bits
	{
		return (field >> shift) & (0xFFFFFFFF >> (32 - length));
	}

	bool	IsSet(UInt32 data) const { return field & data ? true : false; }	//!< Are all these bits set?
	bool	IsUnSet(UInt32 data) const { return field & data ? false : true; }			//!< Are all these bits clear?
	bool	IsClear(UInt32 data) const { return IsUnSet(data); }							//!< Are all these bits clear?

private:
	T		field;	//!< bitfield data
};

typedef Bitfield <UInt8>	Bitfield8;		//!< An 8-bit bitfield
typedef Bitfield <UInt16>	Bitfield16;		//!< A 16-bit bitfield
typedef Bitfield <UInt32>	Bitfield32;		//!< A 32-bit bitfield
static_assert(sizeof(Bitfield8) == 1);
static_assert(sizeof(Bitfield16) == 2);
static_assert(sizeof(Bitfield32) == 4);

const float kFloatEpsilon = 0.0001f;
inline bool FloatEqual(float a, float b) { float magnitude = a - b; if (magnitude < 0) magnitude = -magnitude; return magnitude < kFloatEpsilon; }

class ActiveEffect;
class Actor;
class TESDataHandler;
class ActorMover;
class ActorValueInfo;
class ActorValueOwner;
class AlchemyItem;
class Animation;
class Atmosphere;
class AuxVariableValue;
class BackUpPackage;
class BarterMenu;
class BaseFormComponent;
class BeamProjectile;
class BGSAbilityPerkEntry;
class BGSAcousticSpace;
class BGSAddonNode;
class BGSAmmoForm;
class BGSBipedModelList;
class BGSBodyPart;
class BGSBodyPartData;
class BGSCameraPath;
class BGSCameraShot;
class BGSClipRoundsForm;
class BGSConstructibleObject;
class BGSDebris;
class BGSDefaultObjectManager;
class BGSDehydrationStage;
class BGSDestructibleObjectForm;
class BGSEncounterZone;
class BGSEntryPointFunctionData;
class BGSEntryPointFunctionDataActivateChoice;
class BGSEntryPointFunctionDataLeveledList;
class BGSEntryPointFunctionDataOneValue;
class BGSEntryPointFunctionDataTwoValue;
class BGSEntryPointPerkEntry;
class BGSEquipType;
class BGSExplosion;
class BGSHeadPart;
class BGSHungerStage;
class BGSIdleMarker;
class BGSImpactData;
class BGSImpactDataSet;
class BGSItemList;
class BGSLightingTemplate;
class BGSListForm;
class BGSMenuIcon;
class BGSMessage;
class BGSMessageIcon;
class BGSMovableStatic;
class BGSMusicType;
class BGSNote;
class BGSOpenCloseForm;
class BGSPerk;
class BGSPerkEntry;
class BGSPickupPutdownSounds;
class BGSPlaceableWater;
class BGSPreloadable;
class BGSPrimitive;
class BGSPrimitiveBox;
class BGSPrimitivePlane;
class BGSPrimitiveSphere;
class BGSProjectile;
class BGSQuestObjective;
class BGSQuestPerkEntry;
class BGSRadiationStage;
class BGSRagdoll;
class BGSRepairItemList;
class BGSSaveLoadGame;
class BGSSleepDeprevationStage;
class BGSSleepDeprivationStage;
class BGSStaticCollection;
class BGSTalkingActivator;
class BGSTerminal;
class BGSTerrainChunkLoadTask;
class BGSTextureModel;
class BGSTextureSet;
class BGSTouchSpellForm;
class BGSVoiceType;
class bhkBlendCollisionObject;
class bhkCharacterController;
class bhkConstraint;
class bhkLimitedHingeConstraint;
class bhkNiCollisionObject;
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
class bhkRefObject;
class bhkRigidBody;
class bhkRigidBodyT;
class bhkWorld;
class bhkWorldM;
class BoundObjectListHead;
class BSAnimGroupSequence;
class BSAudioManagerThread;
class BSBound;
class BSFaceGenAnimationData;
class BSFaceGenNiNode;
class BSFadeNode;
class BSFile;
class BSMultiBoundNode;
class BSPortalGraph;
class BSResizableTriShape;
class BSSceneGraph;
class BSScissorTriShape;
class BSSegmentedTriShape;
class BSShaderAccumulator;
class BSTempEffect;
class BSTempEffectParticle;
class BSTempNodeManager;
class BSTextureSet;
class BSWin32Audio;
class CachedValuesOwner;
class CaravanMenu;
class Character;
class Clouds;
class CombatController;
class CompanionWheelMenu;
class ComputersMenu;
class ContainerMenu;
class ContinuousBeamProjectile;
class Creature;
class DecalVertex;
class DialogMenu;
class DialoguePackage;
class DIHookControl;
class EffectItem;
class EffectItemList;
class EffectSetting;
class EnchantmentItem;
class Explosion;
class ExtraCount;
class FaceGenUndo;
class FlameProjectile;
class FleePackage;
class FontManager;
class FOPipboyManager;
class FORenderedMenu;
class GrenadeProjectile;
class GridArray;
class GridCellArray;
class HackingMenu;
class HighProcess;
class hkpRigidBody;
class hkpWorld;
class hkpWorldObject;
class hkReferencedObject;
class HUDMainMenu;
class ImageSpaceModifierInstance;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;
class ImageSpaceModifierInstanceForm;
class ImageSpaceModifierInstanceRB;
class IngredientItem;
class InventoryMenu;
class ItemModMenu;
class KFModel;
class LevelUpMenu;
class LoadedAreaBound;
class LoadingMenu;
class LockPickMenu;
class LODNode;
class LowProcess;
class MagicCaster;
class MagicHitEffect;
class MagicItem;
class MagicItemForm;
class MagicShaderHitEffect;
class MagicTarget;
class MapMenu;
class MediaLocationController;
class MediaSet;
class Menu;
class MessageMenu;
class MiddleHighProcess;
class MiddleLowProcess;
class MissileProjectile;
class MobileObject;
class ModelLoader;
class Moon;
class NavMesh;
class NavMeshInfoMap;
class NiAVObject;
class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;
class NiCamera;
class NiCollisionObject;
class NiControllerManager;
class NiControllerSequence;
class NiExtraData;
class NiGeometry;
class NiInterpController;
class NiInterpolator;
class NiMemObject;
class NiNode;
class NiObject;
class NiObjectNET;
class NiProperty;
class NiRefObject;
class NiRenderedTexture;
class NiSourceTexture;
class NiTexture;
class NiTimeController;
class NiTransformController;
class NiTransformInterpolator;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class PathingLocation;
class PlayerCharacter;
class PlayerMover;
class Precipitation;
class Projectile;
class QuantityMenu;
class QueuedFile;
class RaceSexMenu;
class RecipeMenu;
class RepairMenu;
class RepairServicesMenu;
class SceneGraph;
class Script;
class Setting;
class ShadowSceneNode;
class Sky;
class SkyObject;
class SleepWaitMenu;
class SpectatorPackage;
class SpellItem;
class Stars;
class StartMenu;
class StatsMenu;
class Sun;
class TES;
class TESActorBase;
class TESActorBaseData;
class TESAIForm;
class TESAmmo;
class TESAmmoEffect;
class TESAnimation;
class TESAnimGroup;
class TESAttackDamageForm;
class TESAttributes;
class TESBipedModelForm;
class TESBoundAnimObject;
class TESBoundObject;
class TESCaravanCard;
class TESCaravanDeck;
class TESCaravanMoney;
class TESCasino;
class TESCasinoChips;
class TESChallenge;
class TESChildCell;
class TESClass;
class TESClimate;
class TESCombatStyle;
class TESContainer;
class TESCreature;
class TESDescription;
class TESEffectShader;
class TESEnchantableForm;
class TESEyes;
class TESFaction;
class TESFlora;
class TESForm;
class TESFullName;
class TESFurniture;
class TESGlobal;
class TESGrass;
class TESHair;
class TESHealthForm;
class TESIcon;
class TESIdleForm;
class TESImageSpace;
class TESImageSpaceModifiableForm;
class TESImageSpaceModifier;
class TESKey;
class TESLandTexture;
class TESLevCharacter;
class TESLevCreature;
class TESLeveledList;
class TESLevItem;
class TESLevSpell;
class TESLoadScreen;
class TESLoadScreenType;
class TESModel;
class TESModelAnim;
class TESModelList;
class TESModelRDT;
class TESModelTextureSwap;
class TESNPC;
class TESObject;
class TESObjectACTI;
class TESObjectANIO;
class TESObjectARMA;
class TESObjectARMO;
class TESObjectBOOK;
class TESObjectCELL;
class TESObjectCLOT;
class TESObjectCONT;
class TESObjectDOOR;
class TESObjectIMOD;
class TESObjectLAND;
class TESObjectLIGH;
class TESObjectMISC;
class TESObjectREFR;
class TESObjectSTAT;
class TESObjectTREE;
class TESObjectWEAP;
class TESPackage;
class TESPackageData;
class TESQuest;
class TESRace;
class TESRaceForm;
class TESReactionForm;
class TESRecipe;
class TESRecipeCategory;
class TESRegion;
class TESRegionData;
class TESRegionDataGrass;
class TESRegionDataImposter;
class TESRegionDataLandscape;
class TESRegionDataMap;
class TESRegionDataSound;
class TESRegionDataWeather;
class TESRegionList;
class TESRegionManager;
class TESReputation;
class TESScriptableForm;
class TESSkill;
class TESSound;
class TESSoundFile;
class TESSpellList;
class TESTexture1024;
class TESTexture;
class TESTopic;
class TESTopicInfo;
class TESValueForm;
class TESWaterForm;
class TESWeather;
class TESWeightForm;
class TESWorldSpace;
class TextEditMenu;
class Tile3D;
class Tile;
class TileImage;
class TileMenu;
class TileRect;
class TileText;
class TraitMenu;
class TraitSelectMenu;
class TressPassPackage;
class VATSMenu;
struct ActorHitData;
struct AnimData;
struct BaseExtraList;
struct CombatActors;
struct CommandInfo;
struct Condition;
struct DetectionData;
struct DetectionEvent;
struct ExtraDataList;
struct FontInfo;
struct hkCdBody;
struct InventoryRef;
struct ModInfo;
struct ParamInfo;
struct PluginInfo;
struct PosAndCell;
struct ScriptEventList;
struct ScriptVar;
struct Semaphore;
struct Sound;
struct StartMenuOption;
struct ValidBip01Names;
struct VATSCameraData;
struct VATSTargetBodyPartData;
struct VATSTargetInfo;
struct ScriptEventList;
struct ScriptLineBuffer;
struct ScriptBuffer;
class BGSCellNumericIDArrayMap;
class BGSLoadGameSubBuffer;
class BGSReconstructFormsInFileMap;
class BGSReconstructFormsInAllFilesMap;
class ChangesMap;
class InteriorCellNewReferencesMap;
class ExteriorCellNewReferencesMap;
class NumericIDBufferMap;
class ExtraContainerChanges;
struct ExtraLockData;
struct ExtraMapMarkerData;