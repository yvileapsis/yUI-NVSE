#pragma once

#include "Ni2DBuffer.hpp"
#include <d3d9.h>

class NiDX92DBufferData : public Ni2DBuffer::RendererData {
public:
	NiDX92DBufferData();
	virtual ~NiDX92DBufferData();

	virtual void						Unk3();
	virtual NiRTTI*						GetRTTI();
	virtual NiNode*						IsNiNode();
	virtual BSFadeNode*					IsFadeNode();
	virtual BSMultiBoundNode*			IsMultiBoundNode();
	virtual NiGeometry*					IsGeometry();
	virtual NiTriBasedGeom*				IsTriBasedGeometry();
	virtual NiTriStrips*				IsTriStrips();
	virtual NiTriShape*					IsTriShape();
	virtual BSSegmentedTriShape*		IsSegmentedTriShape();
	virtual BSResizableTriShape*		IsResizableTriShape();
	virtual NiParticles*				IsParticlesGeom();
	virtual NiLines*					IsLinesGeom();
	virtual bhkNiCollisionObject*		IsBhkNiCollisionObject();
	virtual bhkBlendCollisionObject*	IsBhkBlendCollisionObject();
	virtual bhkRigidBody*				IsBhkRigidBody();
	virtual bhkLimitedHingeConstraint*	IsBhkLimitedHingeConstraint();
	virtual bool						CanDisplayFrame();
	virtual bool						DisplayFrame();
	virtual bool						Shutdown();
	virtual bool						Recreate(LPDIRECT3DDEVICE9 pkD3DDevice9);
	virtual bool						SetRenderTarget(LPDIRECT3DDEVICE9 pkD3DDevice9, UInt32 uiTarget);
	virtual bool						SetDepthTarget(LPDIRECT3DDEVICE9 pkD3DDevice9);
	virtual UInt32						GetMSSALevel();
	virtual void						ReleaseReferences();

	LPDIRECT3DSURFACE9		m_pkSurface;
	LPDIRECT3DSURFACE9		m_pkSurface2;
	D3DMULTISAMPLE_TYPE		eMSAALevel;
};

ASSERT_SIZE(NiDX92DBufferData, 0x20);