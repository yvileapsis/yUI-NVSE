#pragma once

#include "ExtraContainerChanges.hpp"
#include "TESChildCell.hpp"
#include "ExtraDroppedItemList.hpp"

class ExtraScript;
class NiNode;
class ActorCause;
struct ScriptEventList;
class TESContainer;
class TESSound;
struct AnimData;
struct BipedAnim;
class NiGeometry;
class ExtraDroppedItemList;
class BSFaceGenNiNode;
class BSFaceGenAnimationData;
class BSAnimNoteReceiver;

struct LoadedRefData {
	TESObjectREFR*	pCurrentWaterType;
	UInt32			iUnderwaterCount;
	float			fRelevantWaterHeight;
	float			fRevealDistance;
	UInt32			uiFlags;
	NiNodePtr		spNode;
	NiNode*			niNode18;
};

class TESObjectREFR : public TESForm, public TESChildCell {
public:
	MEMBER_FN_PREFIX(TESObjectREFR);

	TESObjectREFR();
	virtual ~TESObjectREFR();

	virtual bool					GetStartingPosition(NiPoint3& arPos, NiPoint3& arRot, TESForm* apParentLocation, TESObjectCELL* apCell);
	virtual void					VoiceSoundFunction();
	virtual void					Unk_50();
	virtual void					DamageObject(float afDamage, bool abForce);
	virtual bool					IsCastShadows();
	virtual void					SetCastShadows();
	virtual void					GetMotionBlur();
	virtual void					Unk_55();
	virtual void					Unk_56();
	virtual void					IsObstacle();
	virtual void					IsQuestObject();
	virtual void					SetActorCause(ActorCause* apActorCause);
	virtual ActorCause*				GetActorCause();
	virtual NiPoint3*				GetStartingAngle(NiPoint3& arRot);
	virtual NiPoint3*				GetStartingPos(NiPoint3& arPos);
	virtual void					SetStartingPos(NiPoint3 akPos);
	virtual void					UpdateLight();
	virtual TESObjectREFR*			RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 count, UInt32 unk3, UInt32 unk4, TESObjectREFR* destRef, UInt32 unk6, UInt32 unk7, UInt32 unk8, UInt8 unk9);	// 40 unk2 quantity? Returns the reference assigned to the removed item.
	virtual void					Unk_60();
	virtual void					LoadEquipedItem();	// Linked to AddItem, (item, count, ExtraDataList), func0042 in OBSE
	virtual void					Unk_62();	// Linked to Unequip (and or equip maybe)
	virtual void					Unk_63();
	virtual void					AddItem(TESForm* item, ExtraDataList* xDataList, UInt32 Quantity);	// Needs confirmation
	virtual NiPoint3*				GetBoundCenter(NiPoint3& arBoundCenter);
	virtual void					Unk_66();
	virtual void					GetMagicEffectList();				// 67
	virtual bool					GetIsChildSize(bool checkHeight);	// 68
	virtual UInt32					GetActorUnk0148();					// 69
	virtual void					SetActorUnk0148(UInt32 arg0);		// 6A
	virtual NiNode*					GetFaceNodeBiped(UInt32 unk = 0);			// 6B
	virtual BSFaceGenNiNode*		GetFaceNodeSkinned(UInt32 unk = 0);		// 6C
	virtual BSFaceGenNiNode*		GetFaceNode(UInt32 unk = 0);		// 6D
	virtual BSFaceGenAnimationData* CreateFaceAnimationData();
	virtual bool					ClampToGround();
	virtual void					Unload3D();
	virtual void					InitHavok();
	virtual void					Load3D(bool abBackgroundLoading);
	virtual void					Set3D(NiNode* niNode, bool unloadArt);
	virtual NiNode*					Get3D();
	virtual void					Unk_75();
	virtual NiPoint3*				GetMinBounds(NiPoint3* apMin);
	virtual NiPoint3*				GetMaxBounds(NiPoint3* apMax);
	virtual void					UpdateAnimation();
	virtual AnimData*				GetAnimData();
	virtual BipedAnim*				GetValidBip01Names();
	virtual BipedAnim*				CallGetBipedAnim();
	virtual void					SetValidBip01Names(BipedAnim validBip01Names);
	virtual NiPoint3*				GetPos() const;
	virtual void					Unk_7E(UInt32 arg0);
	virtual void					ResetHavokSimulation(bool abRecursive);
	virtual void					Unk_80(UInt32 arg0);
	virtual void					Unk_81(UInt32 arg0);
	virtual void					Unk_82();
	virtual NiNode*					GetProjectileNode();
	virtual void					Unk_84(UInt32 arg0);
	virtual UInt32					Unk_85();				// 0 or GetActor::Unk01AC
	virtual bool					IsCharacter();			// return false for Projectile, Actor and Creature, true for character and PlayerCharacter
	virtual bool					IsCreature();			// seems to always return 0
	virtual bool					IsExplosion();			// seems to always return 0
	virtual bool					IsProjectile();
	virtual void					SetParentCell();			// SetParentCell (Interior only ?)
	virtual bool					IsDying(bool);			// IsDead = IsDying (baseForm health <= 0 or Flags bit23 set)
	virtual bool					GetHasKnockedState();
	virtual bool					Unk_8D();
	virtual BSAnimNoteReceiver*		CreateExtraAnimNoteReceiver();
	virtual BSAnimNoteReceiver*		GetExtraDataAnimNoteReceiver();

#ifdef EDITOR
	EditorData	editorData;			// +04
#endif
	TESSound*		pLoopSound;
	TESForm*		pBaseForm;
	NiPoint3		kRotation;
	NiPoint3		kPosition;
	float			fRefScale;
	TESObjectCELL*	pParentCell;
	ExtraDataList	kExtraList;
	LoadedRefData*	pLoadedData;

	ExtraDataList*		GetExtraDataList();
	ExtraScript*		GetExtraScript() const;
	ScriptEventList*	GetEventList() const;

	__forceinline bool HasExtraData(UInt32 extraData) const {
		return kExtraList.HasType(extraData);
	}

	bool Update3D();
	bool Update3D_v1c();	// Less worse version as used by some modders
	TESContainer* GetContainer();
	bool IsMapMarker() const;
	bool GetInventoryItems(InventoryItemsMap& invItems);
	ExtraDroppedItemList* GetDroppedItems() const;
	bool GetVisibleWhenDistant() const;

	double GetHeadingAngle(const TESObjectREFR* to) const;
	double GetWaterLevel() const;

	static TESObjectREFR* Create(bool abTemp = false);

	const char* GetModelPath();

	__forceinline TESObjectCELL* GetParentCell() const { return pParentCell; }

	TESObjectCELL* GetNearbyWaterContainingCell(float afRadius) const;

	static TESObjectREFR* FindReferenceFor3D(NiAVObject* apObject) {
		return CdeclCall<TESObjectREFR*>(0x56F930, apObject);
	}

	static bool HasAddonNodes(NiNode* apNode);

	TESForm* GetBaseForm() const;
	void InitBaseForm(TESForm* apForm);

	bool HandleActivation();
};

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

// Taken from JIP LN NVSE.
float __vectorcall GetDistance3D(const TESObjectREFR* ref1, const TESObjectREFR* ref2);

ASSERT_OFFSET(TESObjectREFR, pBaseForm, 0x20);
ASSERT_OFFSET(TESObjectREFR, pParentCell, 0x40);
ASSERT_OFFSET(TESObjectREFR, kExtraList, 0x44);
ASSERT_SIZE(TESObjectREFR, 0x68);