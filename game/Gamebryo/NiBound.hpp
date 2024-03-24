#pragma once

#include "NiPoint3.hpp"
#include "NiPlane.hpp"
#include "NiTransform.hpp"
#include "BSMemObject.hpp"

class NiCamera;

class NiBound : public BSMemObject {
public:
	NiBound() : m_kCenter(NiPoint3(0.0f,0.0f,0.0f)) , m_fRadius(0.0f) {};
	~NiBound() {};

	NiPoint3	m_kCenter;
	float		m_fRadius;

	static float	ms_fFuzzFactor;
	static float	ms_fTolerance;
	static NiBound	ms_kDefaultBound;

#ifdef USE_DX_COLLISION
	NiBound(const BoundingSphere& arSphere) {
		m_kCenter = arSphere.Center;
		m_fRadius = arSphere.Radius;
	}

	BoundingSphere* AsBoundingSphere() const {
		return (BoundingSphere*)(this);
	}
#endif

	bool operator==(const NiBound& arOther) const {
		return m_kCenter == arOther.m_kCenter && m_fRadius == arOther.m_fRadius;
	}

	int WhichSide(const NiPlane& Plane) const;
	void Merge(const NiBound* apBound);
	static void __fastcall MergeEx(NiBound* apThis, void*, const NiBound* apBound);
	static NiBound* GetGlobalWorldBound();
	bool IsInFrustum(NiCamera* apCamera) const;
	void ComputeFromData(SInt32 aiVertexCount, const NiPoint3* apVertexArray);
	void Update(const NiTransform& arTransform);
	void Update(const NiBound& arBound, const NiTransform& arTransform);
};

ASSERT_SIZE(NiBound, 0x010);