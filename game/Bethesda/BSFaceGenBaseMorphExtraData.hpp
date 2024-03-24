#pragma once

#include "NiExtraData.hpp"
#include "NiPoint3.hpp"

class BSFaceGenBaseMorphExtraData : public NiExtraData {
public:
	NiPoint3*	pBaseVerts;
	UInt32		uiVertsCount;
	UInt32		uiVertsModelCount;

	NIRTTI_ADDRESS(0x11D5BA0);
};

ASSERT_SIZE(BSFaceGenBaseMorphExtraData, 0x18)