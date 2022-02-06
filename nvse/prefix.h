#pragma once
// 4018 - signed/unsigned mismatch
// 4244 - loss of data by assignment
// 4267 - possible loss of data (truncation)
// 4305 - truncation by assignment
// 4288 - disable warning for crap microsoft extension screwing up the scope of variables defined in for loops
// 4311 - pointer truncation
// 4312 - pointer extension
// credits to Ian Patterson
#pragma warning(disable: 4018 4244 4267 4305 4288 4312 4311)

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <winsock2.h>
#include <Windows.h>
#include <nvse_version.h>

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

#define __MACRO_JOIN__(a, b)		__MACRO_JOIN_2__(a, b)
#define __MACRO_JOIN_2__(a, b)		__MACRO_JOIN_3__(a, b)
#define __MACRO_JOIN_3__(a, b)		a##b
#define __PREPRO_TOKEN_STR2__(a)	#a
#define __PREPRO_TOKEN_STR__(a)		__PREPRO_TOKEN_STR2__(a)
#define __LOC__						__FILE__ "("__PREPRO_TOKEN_STR__(__LINE__)") : "

#define STATIC_ASSERT(a)	typedef static_assert_test <sizeof(StaticAssertFailure<(bool)(a)>)> __MACRO_JOIN__(static_assert_typedef_, __COUNTER__)

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

	bool	IsSet(UInt32 data) const { return ((field & data) == data) ? true : false; }	//!< Are all these bits set?
	bool	IsUnSet(UInt32 data) const { return (field & data) ? false : true; }			//!< Are all these bits clear?
	bool	IsClear(UInt32 data) const { return IsUnSet(data); }							//!< Are all these bits clear?

private:
	T		field;	//!< bitfield data
};

typedef Bitfield <UInt8>	Bitfield8;		//!< An 8-bit bitfield
typedef Bitfield <UInt16>	Bitfield16;		//!< A 16-bit bitfield
typedef Bitfield <UInt32>	Bitfield32;		//!< A 32-bit bitfield
STATIC_ASSERT(sizeof(Bitfield8) == 1);
STATIC_ASSERT(sizeof(Bitfield16) == 2);
STATIC_ASSERT(sizeof(Bitfield32) == 4);

const float kFloatEpsilon = 0.0001f;
inline bool FloatEqual(float a, float b) { float magnitude = a - b; if (magnitude < 0) magnitude = -magnitude; return magnitude < kFloatEpsilon; }

class BaseFormComponent;
class TESFullName;
class TESTexture;
class TESTexture1024;
class TESIcon;
class TESScriptableForm;
class BGSMessageIcon;
class TESValueForm;
class TESEnchantableForm;
class TESImageSpaceModifiableForm;
class TESWeightForm;
class TESHealthForm;
class TESAttackDamageForm;
class MagicItem;
class TESModel;
class BGSTextureModel;
class TESModelTextureSwap;
class BGSClipRoundsForm;
class BGSDestructibleObjectForm;
class BGSPickupPutdownSounds;
class BGSAmmoForm;
class BGSRepairItemList;
class BGSEquipType;
class BGSPreloadable;
class BGSBipedModelList;
class TESModelRDT;
class TESBipedModelForm;
class TESContainer;
class BGSTouchSpellForm;
class TESActorBaseData;
class TESSpellList;
class TESAIForm;
class TESAttributes;
class TESAnimation;
class TESModelList;
class TESDescription;
class TESReactionForm;
class TESRaceForm;
class TESSoundFile;
class TESModelAnim;
class TESLeveledList;
class TESForm;
class TESObject;
class TESBoundObject;
class EffectItem;
class EffectItemList;
class MagicItemForm;
class TESBoundAnimObject;
class ActorValueOwner;
class CachedValuesOwner;
class TESActorBase;
class BSTextureSet;
class BGSQuestObjective;
class BGSOpenCloseForm;
class TESIdleForm;
class TESTopicInfo;
class TESTopic;
class BGSTextureSet;
class BGSMenuIcon;
class TESGlobal;
class TESClass;
class TESReputation;
class TESFaction;
class BGSHeadPart;
class TESHair;
class TESEyes;
class TESRace;
class TESSound;
class BGSAcousticSpace;
class TESSkill;
class EffectSetting;
class TESGrass;
class TESLandTexture;
class EnchantmentItem;
class SpellItem;
class TESObjectACTI;
class BGSTalkingActivator;
class BGSTerminal;
class TESFurniture;
class TESObjectARMO;
class TESObjectBOOK;
class TESObjectCLOT;
class TESObjectCONT;
class TESObjectDOOR;
class IngredientItem;
class TESObjectLIGH;
class TESObjectMISC;
class TESCasinoChips;
class TESCaravanMoney;
class TESObjectSTAT;
class BGSMovableStatic;
class BGSStaticCollection;
class BGSPlaceableWater;
class TESObjectTREE;
class TESFlora;
class TESObjectIMOD;
class TESObjectWEAP;
class TESAmmoEffect;
class TESAmmo;
class TESCaravanCard;
class TESNPC;
class TESCreature;
class TESLevCreature;
class TESLevCharacter;
class TESKey;
class AlchemyItem;
class BGSIdleMarker;
class BGSNote;
class BGSConstructibleObject;
class BGSProjectile;
class TESLevItem;
class TESWeather;
class TESClimate;
class TESRegionData;
class TESRegionDataGrass;
class TESRegionDataImposter;
class TESRegionDataLandscape;
class TESRegionDataMap;
class TESRegionDataSound;
class TESRegionDataWeather;
class TESRegion;
class TESRegionList;
class NavMeshInfoMap;
class NavMesh;
class TESObjectCELL;
class TESWorldSpace;
//class TESChildCell;
class TESObjectLAND;
class TESQuest;
class TESPackage;
class DialoguePackage;
class FleePackage;
class TressPassPackage;
class SpectatorPackage;
class BackUpPackage;
class CombatController;
class TESCombatStyle;
class TESRecipeCategory;
class TESRecipe;
class TESLoadScreenType;
class TESLoadScreen;
class TESLevSpell;
class TESObjectANIO;
class TESWaterForm;
class TESEffectShader;
class BGSExplosion;
class BGSDebris;
class TESImageSpace;
class TESImageSpaceModifier;
class BGSListForm;
class BGSPerkEntry;
class BGSQuestPerkEntry;
class BGSAbilityPerkEntry;
class BGSEntryPointFunctionData;
class BGSEntryPointFunctionDataOneValue;
class BGSEntryPointFunctionDataTwoValue;
class BGSEntryPointFunctionDataLeveledList;
class BGSEntryPointFunctionDataActivateChoice;
class BGSEntryPointPerkEntry;
class BGSPerk;
class TESCasino;
class TESChallenge;
class BGSBodyPart;
class BGSBodyPartData;
class MediaSet;
class MediaLocationController;
class BGSAddonNode;
class ActorValueInfo;
class BGSRadiationStage;
class BGSDehydrationStage;
class BGSHungerStage;
class BGSSleepDeprevationStage;
class BGSCameraShot;
class BGSCameraPath;
class BGSVoiceType;
class BGSImpactData;
class BGSImpactDataSet;
class TESObjectARMA;
class BGSEncounterZone;
class BGSMessage;
class BGSRagdoll;
class BGSLightingTemplate;
class BGSMusicType;
class TESCaravanDeck;
class BGSDefaultObjectManager;
class BGSItemList;
class BoundObjectListHead;
class FaceGenUndo;
class BGSSaveLoadGame;

struct ValidBip01Names;
class TESObjectREFR;
class MobileObject;
class MagicCaster;
class MagicTarget;
class PathingLocation;
class ActorMover;
class PlayerMover;
class Actor;
class Creature;
class Character;
class PlayerCharacter;
class Projectile;
class BeamProjectile;
class ContinuousBeamProjectile;
class FlameProjectile;
class GrenadeProjectile;
class MissileProjectile;
class Explosion;
class Setting;
struct BaseExtraList;
//struct ExtraDataList;
class ExtraCount;
class Script;
struct ScriptEventList;
struct ScriptVar;
class BGSPrimitive;
class BGSPrimitivePlane;
class BGSPrimitiveBox;
class BGSPrimitiveSphere;
class BSTempEffect;
class MagicHitEffect;
class MagicShaderHitEffect;
class ActiveEffect;
struct PackageInfo;
struct ActorHitData;
struct CombatActors;
struct DetectionData;
struct DetectionEvent;
struct AnimData;
class BaseProcess;
class LowProcess;
class MiddleLowProcess;
class MiddleHighProcess;
class HighProcess;
class Tile;
class TileRect;
class TileMenu;
class TileImage;
class TileText;
class Tile3D;
class Menu;
class MessageMenu;
class InventoryMenu;
class StatsMenu;
class HUDMainMenu;
class LoadingMenu;
class ContainerMenu;
class DialogMenu;
class SleepWaitMenu;
class StartMenu;
class LockPickMenu;
class QuantityMenu;
class MapMenu;
class LevelUpMenu;
class RepairMenu;
class TextEditMenu;
class BarterMenu;
class HackingMenu;
class VATSMenu;
class ComputersMenu;
class RepairServicesMenu;
class ItemModMenu;
class CompanionWheelMenu;
class TraitSelectMenu;
class RecipeMenu;
class CaravanMenu;
class TraitMenu;
class RaceSexMenu;
class FORenderedMenu;
class FOPipboyManager;
struct FontInfo;
class ImageSpaceModifierInstance;
class ImageSpaceModifierInstanceForm;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;
class ImageSpaceModifierInstanceRB;
class SkyObject;
class Atmosphere;
class Stars;
class Sun;
class Clouds;
class Moon;
class Precipitation;
class Sky;
class GridArray;
class GridCellArray;
class LoadedAreaBound;
class TES;
class FontManager;
struct ModInfo;
struct ParamInfo;
struct CommandInfo;
struct PluginInfo;
class BSFile;
class QueuedFile;
class KFModel;
class ModelLoader;
struct InventoryRef;
class AuxVariableValue;

class NiMemObject;
class NiRefObject;
class NiObject;
class NiInterpolator;
class NiTransformInterpolator;
class NiControllerSequence;
class NiTimeController;
class NiControllerManager;
class NiInterpController;
class NiTransformController;
class NiExtraData;
class NiObjectNET;
class NiProperty;
class NiAVObject;
class NiNode;
class BSFadeNode;
class BSMultiBoundNode;
class ShadowSceneNode;
class NiCamera;
class BSSceneGraph;
class SceneGraph;
class TESAnimGroup;
class BSAnimGroupSequence;
class NiTexture;
class NiSourceTexture;
class NiRenderedTexture;
class NiGeometry;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class BSScissorTriShape;
class BSSegmentedTriShape;
class BSResizableTriShape;
class BSFaceGenNiNode;
class BSShaderAccumulator;
class BSFaceGenAnimationData;
class BSTempNodeManager;

class hkReferencedObject;
class hkpWorldObject;
class bhkRefObject;
class hkpWorld;
class bhkWorld;
class bhkWorldM;
class NiCollisionObject;
class bhkNiCollisionObject;
class bhkBlendCollisionObject;
class bhkCharacterController;
class hkpRigidBody;
class bhkRigidBody;
class bhkRigidBodyT;
class bhkConstraint;
class bhkLimitedHingeConstraint;
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
struct hkCdBody;

class BSWin32Audio;
struct Sound;
class DIHookControl;

struct VATSTargetInfo;
struct VATSTargetBodyPartData;
struct StartMenuOption;
class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;
class DecalVertex;
class BSTempEffectParticle;
class LODNode;
class BGSTerrainChunkLoadTask;
struct Semaphore;
struct VATSCameraData;
struct PosAndCell;
class TESPackageData;