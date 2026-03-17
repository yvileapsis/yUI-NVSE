#pragma once

#include "NiObjectNET.hpp"
#include "NiTPointerList.hpp"
#include "NiTransform.hpp"
#include "NiUpdateData.hpp"

class NiCullingProcess;
class NiNode;
class NiCamera;
class bhkNiCollisionObject;
class NiFixedString;
class NiBound;
class NiProperty;
class NiPropertyState;

NiSmartPointer(NiAVObject);
NiSmartPointer(NiProperty);

class NiAVObject : public NiObjectNET {
public:
	NiAVObject();
	virtual ~NiAVObject();

	virtual void			UpdateControllers(NiUpdateData& arData);
	virtual void			ApplyTransform(NiMatrix3& arMat, NiPoint3& arTrn, bool abOnLeft);
	virtual void			Unk_39();
	virtual NiAVObject*		GetObject_(const NiFixedString& arName);
	virtual NiAVObject*		GetObjectByName(const NiFixedString& arName);
	virtual void			SetSelectiveUpdateFlags(bool& arSelectiveUpdate, BOOL abSelectiveUpdateTransforms, bool& arRigid);
	virtual void			UpdateDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdateSelectedDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdateRigidDownwardPass(const NiUpdateData& arData, UInt32 auiFlags);
	virtual void			UpdatePropertiesDownward(NiPropertyState* apParentState);
	virtual void			Unk_47();
	virtual void			UpdateWorldData(const NiUpdateData& arData);
	virtual void			UpdateWorldBound();
	virtual void			UpdateTransformAndBounds(const NiUpdateData& arData);
	virtual void			PreAttachUpdate(NiNode* apEventualParent, const NiUpdateData& arData);
	virtual void			PreAttachUpdateProperties(NiNode* apEventualParent);
	virtual void			DetachParent();
	virtual void			UpdateUpwardPassParent(void* arg);
	virtual void			OnVisible(NiCullingProcess* apCuller);
	virtual void			PurgeRendererData(NiDX9Renderer* apRenderer);

	enum NiFlags : DWORD {
		APP_CULLED                              = 1u <<  0, // Forces culled state
		SELECTIVE_UPDATE						= 1u <<  1, // Allows selective updates - see flags below
		SELECTIVE_UPDATE_TRANSFORMS             = 1u <<  2, // Allows transform controller update
		SELECTIVE_UPDATE_CONTROLLER				= 1u <<  3, // Allows controller update
		SELECTIVE_UPDATE_RIGID                  = 1u <<  4, // Forces use of UpdateRigidDownwardPass
		DISPLAY_OBJECT                          = 1u <<  5, // Used only by sky objects
		DISABLE_SORTING                         = 1u <<  6,	// Unused, just like Gamebryo's sorter
		SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE	= 1u <<  7, // Forces UPDATE_TRANSFORMS even if node has no transform controllers
		OBJECT_PREPARED							= 1u <<  8, // Xbox 360, used to mark prepared objects by BSShaderManager::PrepareGeometry
		TRANS_IS_IDENTITY						= 1u <<  9, // Assumes local transform is an identity one, skipping transformations
		NO_DECALS								= 1u << 10,	// Disables decals for this object
		ALWAYS_DRAW                             = 1u << 11, // Forces light inclusion, and skips culling
		PREPROCESSED_NODE						= 1u << 12, // Used to mark actor nodes, for actor culling
		FIXED_BOUND                             = 1u << 13, // Prevents bound updates
		FADED_IN								= 1u << 14, // BSFadeNode only; Marks fade state
		IGNORE_FADE								= 1u << 15, // BSFadeNode only; Disables fading
		LOD_FADING_OUT							= 1u << 16, // BSFadeNode only; Looks unused
		HAS_MOVING_SOUND						= 1u << 17, // Used for sound updates
		HAS_PROPERTY_CONTROLLER					= 1u << 18, // Marks the presence of a property controller
		ORPHANED_BOUND							= 1u << 19, // Marks lack of a bound
		NOT_VISIBLE                             = 1u << 20, // Used for actor culling
		IGNORES_PICKING							= 1u << 21, // Disables picking for this object
		WORLD_BOUND_CHANGE						= 1u << 22,	// Forces multibound update/attachment
		NOT_SHADOW_RECEIVER						= 1u << 23, // Set if bActorSelfShadowing == false, checks for it look broken (maybe why the setting doesn't work?)
		HIGH_DETAIL	                            = 1u << 24, // BSFadeNode only; Unused, meant for actors 0x936F75
		ALREADY_TRAVERSED						= 1u << 25, // Xbox 360, used for texture streaming
		UNK_26									= 1u << 26, // ??
		PLAYER_BONE                             = 1u << 27,	// Marks player's bones, not read anywhere?
		RELEVANT_IMPOSTER						= 1u << 28, // BSFadeNode only; Marks the imposter state to override fading
		IS_POINTLIGHT                           = 1u << 29, // Added by JIP
		DONE_INIT_LIGHTS                        = 1u << 30, // Added by JIP
		IS_INSERTED                             = 1u << 31  // Added by JIP
	};

	NiNode*							m_pkParent;
	NiPointer<bhkNiCollisionObject> m_spCollisionObject;
	NiBound*						m_pWorldBound;
	NiTPointerList<NiPropertyPtr>	m_kPropertyList;
	Bitfield32						m_uiFlags;
	NiTransform						m_kLocal;
	NiTransform						m_kWorld;

	static BSSpinLock* pPropertyStateLock;

	NiTransform* GetLocalTransform();
	NiTransform* GetWorldTransform();

	const float GetLocalScale() const;
	void SetLocalScale(float fScale) { m_kLocal.m_fScale = fScale; };

	const float GetWorldScale() const;
	void SetWorldScale(float fScale) { m_kWorld.m_fScale = fScale; };


	void SetLocalTranslate(const NiPoint3& pos);
	void SetLocalTranslate(const NiPoint3* pos);
	const NiPoint3& GetLocalTranslate() const;

	void SetWorldTranslate(const NiPoint3& pos);
	void SetWorldTranslate(const NiPoint3* pos);
	const NiPoint3& GetWorldTranslate() const;

	void SetLocalRotate(float x, float y, float z);
	void SetLocalRotate(const NiMatrix3& kRot);
	void SetLocalRotate(const NiMatrix3* rot);
	void SetLocalRotateDeg(float x, float y, float z);
	const NiMatrix3& GetLocalRotate() const;

	void SetWorldRotate(const NiMatrix3& rot);
	void SetWorldRotate(const NiMatrix3* rot);
	const NiMatrix3& GetWorldRotate() const;


	// FLAGS

	void SetBit(bool bVal, UInt32 uMask);
	bool GetBit(UInt32 uMask) const;

	void SetAppCulled(bool bVal);
	bool GetAppCulled() const;

	void SetSelectiveUpdate(bool bVal);
	bool GetSelectiveUpdate() const;

	void SetSelUpdTransforms(bool bVal);
	bool GetSelUpdTransforms() const;

	void SetSelUpdController(bool bVal);
	bool GetSelUpdController() const;

	void SetSelUpdRigid(bool bVal);
	bool GetSelUpdRigid() const;

	void SetDisplayObject(bool bVal);
	bool GetDisplayObject() const;

	void SetDisableSorting(bool bVal);
	bool GetDisableSorting() const;

	void SetSelUpdTransformsOverride(bool bVal);
	bool GetSelUpdTransformsOverride() const;

	void SetSaveExternalGeomData(bool bVal);
	bool GetSaveExternalGeomData() const;

	void SetNoDecals(bool bVal);
	bool GetNoDecals() const;

	void SetAlwaysDraw(bool bVal);
	bool GetAlwaysDraw() const;

	void SetActorNode(bool bVal);
	bool GetActorNode() const;

	void SetFixedBound(bool bVal);
	bool GetFixedBound() const;

	void SetFadedIn(bool bVal);
	bool GetFadedIn() const;

	void SetIgnoreFade(bool bVal);
	bool GetIgnoreFade() const;

	void SetLODFadingOut(bool bVal);
	bool GetLODFadingOut() const;

	void SetHasMovingSound(bool bVal);
	bool GetHasMovingSound() const;

	void SetHasPropertyController(bool bVal);
	bool GetHasPropertyController() const;

	void SetHasBound(bool bVal);
	bool GetHasBound() const;

	void SetActorCulled(bool bVal);
	bool GetActorCulled() const;

	void SetIgnoresPicking(bool bVal);
	bool GetIgnoresPicking() const;



	void Update(NiUpdateData& arData);
	NiNode* GetParent() { return m_pkParent; };
	const NiNode* GetParent() const { return m_pkParent; };
	NiProperty* GetProperty(UInt32 iType) const;
	__forceinline NiBound* GetWorldBound() const;
	NiAVObject* GetObjectByNameEx(const char* apName);
	void Cull(NiCullingProcess* apCuller);

	NiNode* FindRootNode();
	
	bool HasPropertyController() const;

	void CreateWorldBoundIfMissing();
	void CreateWorldBoundIfMissing(bool abAlwaysDraw);
	void AttachProperty(NiPropertyPtr& aspProperty);
	void AddProperty(NiProperty* apProperty);
	void RemoveProperty(UInt32 auiPropertyType);
	void DetachProperty(NiProperty* apProperty);
	void UpdateProperties();
	void PushLocalProperties(NiPropertyState* apParentState, bool abCopyOnChange, NiPropertyState*& apOut);
	void UpdateObjectControllers(NiUpdateData& apData, bool abUpdateProperties);
	void UpdateWorldTransform();
	void AssignShaders(bool a = false, bool b = false);
	void StartAnimations();

	void CopyWorldRotateFromParent();

	bool IsInFrustum(NiCamera* apCamera) const;
	bool IsVisualObject() const;


	static void __cdecl UpdateGeomMorphTimeRecurse(NiAVObject* apNode);
};

ASSERT_OFFSET(NiAVObject, m_pkParent, 0x18);
ASSERT_OFFSET(NiAVObject, m_spCollisionObject, 0x1C);
ASSERT_OFFSET(NiAVObject, m_pWorldBound, 0x20);
ASSERT_OFFSET(NiAVObject, m_kPropertyList, 0x24);
ASSERT_SIZE(NiTPointerList<NiPointer<NiProperty>>, 0xC);
ASSERT_OFFSET(NiAVObject, m_uiFlags, 0x30);
ASSERT_OFFSET(NiAVObject, m_kLocal, 0x34);
ASSERT_OFFSET(NiAVObject, m_kWorld, 0x68);
ASSERT_SIZE(NiAVObject, 0x9C);