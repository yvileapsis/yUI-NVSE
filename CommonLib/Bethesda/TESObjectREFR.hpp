#pragma once
#include "TESForm.hpp"
#include "NiPoint3.hpp"

#include "ExtraContainerChanges.hpp"
#include "TESChildCell.hpp"
#include "ExtraDroppedItemList.hpp"

class hkpRigidBody;

class NiNode;
class NiAVObject;
class NiGeometry;

struct AnimData;
struct BipedAnim;
struct ScriptEventList;

class Actor;
class ActorCause;

class BSFaceGenNiNode;
class BSFaceGenAnimationData;
class BSAnimNoteReceiver;

class ExtraScript;
class ExtraDroppedItemList;
class ExtraLockData;

class TESContainer;
class TESSound;

struct LoadedRefData
{
	TESObjectREFR*		pCurrentWaterType;
	UInt32				iUnderwaterCount;
	Float32				fRelevantWaterHeight;
	Float32				fRevealDistance;
	UInt32				eFlags;
	NiPointer<NiNode>	spNode;
	NiNode*				pkNiNode18;
};

class TESObjectREFR : public TESForm
{
public:

	TESObjectREFR();
	~TESObjectREFR() override;
	virtual bool					SetStartingPosition(NiPoint3& arPos, NiPoint3& arRot, TESForm* apParentLocation, TESObjectCELL* apCell);
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

	virtual void					RemoveByType(int type, char keepOwner, int count);
	virtual bool					EquipObject(TESForm* item, UInt32 count, ExtraDataList* xData, bool lockEquip); // Linked to AddItem, (item, count, ExtraDataList), func0042 in OBSE
	virtual void					Unk_62();	// Linked to Unequip (and or equip maybe)
	virtual void					Unk_63();
	virtual void					AddItem(TESForm* apkItem, ExtraDataList* apkExtraDataList, UInt32 auiQuantity);
	virtual NiPoint3*				GetBoundCenter(NiPoint3& arBoundCenter);
	virtual void					GetMagicCaster();
	virtual void					GetMagicTarget();					// Actor: GetMagicEffectList
	virtual bool					GetIsChildSize(bool checkHeight);	// 68
	virtual UInt32					GetActorUnk0148();					// 69
	virtual void					SetActorUnk0148(UInt32 arg0);		// 6A
	virtual NiNode*					GetFaceNodeBiped(UInt32 unk = 0);	// 6B
	virtual BSFaceGenNiNode*		GetFaceNodeSkinned(UInt32 unk = 0);	// 6C
	virtual BSFaceGenNiNode*		GetFaceNode(UInt32 unk = 0);		// 6D
	virtual BSFaceGenAnimationData* CreateFaceAnimationData();
	virtual bool					ClampToGround();
	virtual void					Unload3D();
	virtual void					InitHavok();
	virtual void					Load3D(bool abBackgroundLoading);
	virtual void					Set3D(NiNode* niNode, bool unloadArt);
	virtual NiNode*					Get3D();							// same in FOSE !
	virtual void					IsBaseFormCastsShadows();
	virtual NiPoint3*				GetMinBounds(NiPoint3* apMin);
	virtual NiPoint3*				GetMaxBounds(NiPoint3* apMax);
	virtual void					UpdateAnimation();
	virtual AnimData*				GetAnimData();						// 0079
	virtual BipedAnim*				GetValidBip01Names();				// 007A	Character only
	virtual BipedAnim*				CallGetValidBip01Names();
	virtual void					SetValidBip01Names(BipedAnim validBip01Names);
	virtual NiPoint3*				GetPos() const;						// GetPos or GetDistance
	virtual void					Unk_7E(UInt32 arg0);
	virtual void					ResetHavokSimulation(bool abRecursive);
	virtual void					Unk_80(UInt32 arg0);
	virtual void					Unk_81(UInt32 arg0);
	virtual void					Unk_82();
	virtual NiNode*					GetProjectileNode();
	virtual void					Unk_84(UInt32 arg0);
	virtual UInt32					GetSitSleepState();				// 0 or GetActor::Unk01AC
	virtual bool					IsCharacter();
	virtual bool					IsCreature();
	virtual bool					IsExplosion();
	virtual bool					IsProjectile();
	virtual void					SetParentCell();				// SetParentCell (Interior only ?)
	virtual bool					IsDying(bool essentialUnconcious = false);
	virtual bool					GetHasKnockedState();
	virtual bool					Unk_8D();
	virtual BSAnimNoteReceiver*		CreateExtraAnimNoteReceiver();
	virtual BSAnimNoteReceiver*		GetExtraDataAnimNoteReceiver();


	TESChildCell*	pkChildCell;
	TESSound*		pkLoopSound;
	TESForm*		pkObjectReference;
	NiPoint3		kRotation;
	NiPoint3		kPosition;
	Float32			fRefScale;
	TESObjectCELL*	pkParentCell;
	ExtraDataList	kExtraList;
	LoadedRefData*	pkLoadedData;

	ExtraScript* GetExtraScript() const;
	ScriptEventList* GetEventList() const;

	bool Update3D();
	bool Update3D_v1c();	// Less worse version as used by some modders
	TESContainer* GetContainer();
	bool IsMapMarker();
//	bool GetInventoryItems(InventoryItemsMap& invItems);
	ExtraDroppedItemList* GetDroppedItems();
	bool GetVisibleWhenDistant();

	double GetHeadingAngle(const TESObjectREFR* to) const;

	static TESObjectREFR* Create(bool abTemp = false);

	const char* GetModelPath();

	__forceinline TESObjectCELL* GetParentCell() const { return pkParentCell; }

	static TESObjectREFR* FindReferenceFor3D(NiAVObject* apObject) {
		return CdeclCall<TESObjectREFR*>(0x56F930, apObject);
	}

	TESForm* GetBaseForm();
	void InitBaseForm(TESForm* apForm);

	bool HandleActivation();


	SInt32					GetItemCount(TESForm* form) { return ThisCall<SInt32>(0x575610, this, form); };
	void					AddItemAlt(TESForm* item, UInt32 count, float condition, bool doEquip);
	bool					GetInventoryItems(UInt8 typeID);
	TESWorldSpace*			GetParentWorld() { return ThisCall<TESWorldSpace*>(0x575D70, this); };
	bool __fastcall			GetInSameCellOrWorld(const TESObjectREFR* target) const;
	Float32 __vectorcall	GetDistance(TESObjectREFR* target);
	Float32 __vectorcall	GetDistance2D(TESObjectREFR* target);
	void					SetPos(NiPoint3& posVector);
	void					SetAngle(NiPoint3& rotVector);
	Float32					GetHeadingAngle(const TESObjectREFR* target);
	bool					MoveToCell(TESForm* worldOrCell, NiPoint3& posVector);
	void					DeleteReference();
	TESObjectREFR*			GetMerchantContainer();
	float					GetWaterImmersionPerc();
	bool					IsMobile();
	bool					IsGrabbable();
	bool					ValidForHooks();
	NiAVObject*				GetNiBlock(const char* blockName);
	NiNode* __fastcall		GetNode(const char* nodeName);
	hkpRigidBody*			GetRigidBody(const char* nodeName);
	bool					RunScriptSource(const char* sourceStr);
	ExtraLockData*			GetLockData() const { return ThisCall<ExtraLockData*>(0x569160, (void*) this); }
	void					SetScale(float scale);
	bool					IsOwnedByActor(Actor* actor, bool includeFactionOwnership) { return ThisCall<bool>(0x5785E0, this, actor, includeFactionOwnership); };
	TESObjectREFR*			ResolveOwnership() { return ThisCall<TESObjectREFR*>(0x567790, this); };
	bool					IsInInterior() { return ThisCall<bool>(0x575D10, this); };
	NiNode*					GetNiNode2();
	NiAVObject*				GetNifBlock(UInt32 pcNode, const char* blockName);
	bool					IsCrimeOrEnemy();
	NiPoint3				GetCenter() const;
	NiPoint3				GetDimensions() const;
	__forceinline Float32	GetScale() { return ThisCall<float>(0x567400, this); }
	TESObjectREFR*			ResolveAshpile();
	bool					IsLocked() const;

	__forceinline bool		Activate(Actor* activator, UInt32 unk1, UInt32 unk2, UInt32 unk3) { return ThisCall<bool>(0x573170, this, activator, unk1, unk2, unk3); }

	Float64					GetInventoryWeight();

	std::vector<InventoryChanges*> GetAllItems(UInt32 checkLinkedRec = 0);

	void					OpenCloseContainer(bool open = false, bool sounds = true);

	const char*				GetBaseFormFullName() { return ThisCall<const char*>(0x55D520, this); }
	const char*				GetJIPName();
	static TESObjectREFR*	FindReferenceFor3D(const NiNode* node) { return CdeclCall<TESObjectREFR*>(0x56F930, node); }

	TESObjectCELL* GetNearbyWaterContainingCell(float afRadius) const;

	static bool HasAddonNodes(NiNode* apNode);

	TESForm* GetBaseForm() const;
};
static_assert(sizeof(TESObjectREFR) == 0x68);

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

// Taken from JIP LN NVSE.
float __vectorcall GetDistance3D(const TESObjectREFR* ref1, const TESObjectREFR* ref2);