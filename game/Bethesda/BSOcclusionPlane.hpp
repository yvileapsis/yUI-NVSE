#pragma once

#include "NiObject.hpp"
#include "NiPoint2.hpp"
#include "NiPoint3.hpp"
#include "NiColor.hpp"
#include "NiMatrix3.hpp"
#include "NiFrustumPlanes.hpp"
#include "BSMultiBoundShape.hpp"

class BSOcclusionPlane : public NiObject {
public:
	virtual UInt32	GetGeomShapeSize();
	virtual UInt32	GetDebugGeomShapeSize();

	NiPoint3							kCenter;
	NiPoint2							kHalfExtents;
	NiMatrix3							kRotation;
	BSMultiBoundShape::BSMBCullResult	eCullResult;
	NiFrustumPlanes						kCachedPlanes;
	NiPoint3							kCachedVertices[4];
	NiPoint3							kViewPoint;
	bool								bDirtyVertices;
	bool								bDirtyPlanes;
	bool								bDisabledPlanes[4];
	bool								bTwoSided;
	bool								UnkEB;
	BSOcclusionPlane*					pLinkedPlanes[4];

	NIRTTI_ADDRESS(0x1202804);

	void UpdateFrustumPlanes(NiPoint3 akViewPoint, NiFrustumPlanes& arPlanes);

	void UpdateVertices(NiPoint3* apPoints);

	bool WithinFrustum(NiFrustumPlanes& arPlanes, bool abViewHitPlane, NiPoint3 akViewIntersection);
	bool WithinFrustumDistFirst(NiFrustumPlanes& arPlanes, NiPoint3& arViewPoint);

	UInt32 Unk_B5AA00() const;

	void CreateDebugGeometry(NiPoint3 akPos, NiLines* apLines, NiTriShape* apShape, NiColorA akColor, bool abUnk);

	inline void ResetCullResult() { eCullResult = BSMultiBoundShape::BS_CULL_UNTESTED; };
};