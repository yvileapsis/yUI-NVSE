#pragma once

#include "NiExtraData.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(BSBound);

class BSBound : public NiExtraData {
public:
	BSBound();
	virtual ~BSBound();

#ifdef USE_DX_COLLISION
	union {
		struct {
			BoundingBox box;
		};
		NiPoint3 center;
		NiPoint3 extents;
	};
#else
	NiPoint3 center;
	NiPoint3 extents;
#endif

	CREATE_OBJECT(BSBound, 0xC55010);
	NIRTTI_ADDRESS(0x12031E8);
};

ASSERT_SIZE(BSBound, 0x24)