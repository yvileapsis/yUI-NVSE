#pragma once

#include "NiRTTI.hpp"
#include "NiAVObject.hpp"
#include "NiGeometry.hpp"
#include "NiCamera.hpp"
#include "NiVisibleArray.hpp"
#include "NiFrustumPlanes.hpp"

class NiCullingProcess : public NiMemObject {
public:
	NIRTTI_ADDRESS(0x11F4990);

	NiCullingProcess(NiVisibleArray* apVisibleSet = nullptr);

	virtual const NiRTTI*				GetRTTI() const { return NiCullingProcess::ms_RTTI; }
	virtual NiNode*						IsNiNode() const { return nullptr; };
	virtual BSFadeNode*					IsFadeNode() const { return nullptr; };
	virtual BSMultiBoundNode*			IsMultiBoundNode() const { return nullptr; };
	virtual NiGeometry*					IsGeometry() const { return nullptr; };
	virtual NiTriBasedGeom*				IsTriBasedGeometry() const { return nullptr; };
	virtual NiTriStrips*				IsTriStrips() const { return nullptr; };
	virtual NiTriShape*					IsTriShape() const { return nullptr; };
	virtual BSSegmentedTriShape*		IsSegmentedTriShape() const { return nullptr; };
	virtual BSResizableTriShape*		IsResizableTriShape() const { return nullptr; };
	virtual NiParticles*				IsParticlesGeom() const { return nullptr; };
	virtual NiLines*					IsLinesGeom() const { return nullptr; };
	virtual bhkNiCollisionObject*		IsBhkNiCollisionObject() const { return nullptr; };
	virtual bhkBlendCollisionObject*	IsBhkBlendCollisionObject() const { return nullptr; };
	virtual bhkRigidBody*				IsBhkRigidBody() const { return nullptr; };
	virtual bhkLimitedHingeConstraint*	IsBhkLimitedHingeConstraint() const { return nullptr; };
	virtual								~NiCullingProcess() {};
	virtual void						Process(NiAVObject* apObject);
	virtual void						ProcessAlt(const NiCamera* apCamera, NiAVObject* apScene, NiVisibleArray* apVisibleSet);
	virtual void						AppendVirtual(NiGeometry* apGeom) {};

	bool				m_bUseVirtualAppend;
	NiVisibleArray*		m_pkVisibleSet;
	NiCamera*			m_pkCamera;
	NiFrustum			m_kFrustum;
	NiFrustumPlanes		m_kPlanes;

	void SetCamera(const NiCamera* apCamera);
	NiCamera* GetCamera() const;
	void SetFrustum(const NiFrustum* kFrustum);

	void DoCulling(NiAVObject* pkObject);
};

ASSERT_SIZE(NiCullingProcess, 0x90);