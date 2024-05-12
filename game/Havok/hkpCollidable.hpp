#pragma once

#include "hkpCdBody.hpp"
#include "hkpTypedBroadPhaseHandle.hpp"

class hkpCollidable : public hkpCdBody {
public:
	struct BoundingVolumeData {
		UInt32	m_min[3];
		UInt8	m_expansionMin[3];
		UInt8	m_expansionShift;
		UInt32	m_max[3];
		UInt8	m_expansionMax[3];
		UInt8	m_padding;
		UInt16	m_numChildShapeAabbs;
		UInt16	m_capacityChildShapeAabbs;
		void*	m_childShapeAabbs;
		void*	m_childShapeKeys;
	};


	SInt8						m_ownerOffset;
	UInt8						m_forceCollideOntoPpu;
	UInt16						m_shapeSizeOnSpu;
	hkpTypedBroadPhaseHandle	m_broadPhaseHandle;
	BoundingVolumeData			m_boundingVolumeData;
	float						m_allowedPenetrationDepth;
};

ASSERT_SIZE(hkpCollidable, 0x50);