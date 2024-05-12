#pragma once

#include "NiObject.hpp"
#include "NiPoint3.hpp"
#include "NiColor.hpp"

class BSMultiBound;
class NiFrustumPlanes;

NiSmartPointer(BSMultiBoundShape);

class BSMultiBoundShape : public NiObject {
public:
	virtual int		GetType();
	virtual void	GetRadius();
	virtual int		CheckBSBound(BSMultiBound* apTargetBound);
	virtual int		CheckBound(NiBound* apTargetBound);
	virtual bool	WithinFrustum(NiFrustumPlanes& arPlanes);
	virtual bool	CompletelyWithinFrustum(NiFrustumPlanes& arPlanes);
	virtual void	GetNiBound(NiBound& arBound);
	virtual void	CreateDebugGeometry(NiLines* apLines, NiTriShape* apGoemetry, NiColorA akColor);
	virtual UInt32	GetGeomShapeSize();
	virtual UInt32	GetDebugGeomShapeSize();
	virtual bool	GetPointWithin(NiPoint3& arPoint);
	virtual void	SetCenter(NiPoint3& arCenter);

	struct BoundVertices {
		NiPoint3 point[8];
	};

	enum BSMBCullResult {
		BS_CULL_UNTESTED = 0x0,
		BS_CULL_VISIBLE = 0x1,
		BS_CULL_CULLED = 0x2,
		BS_CULL_OCCLUDED = 0x3,
	};

	enum BSMBShapeType {
		BSMB_SHAPE_NONE		= 0,
		BSMB_SHAPE_AABB		= 1,
		BSMB_SHAPE_OBB		= 2,
		BSMB_SHAPE_SPHERE	= 3,
		BSMB_SHAPE_CAPSULE	= 4,
	};

	BSMBCullResult eCullResult;

	inline void ResetCullResult() {
		eCullResult = BS_CULL_UNTESTED;
	};
};

ASSERT_SIZE(BSMultiBoundShape, 0xC)