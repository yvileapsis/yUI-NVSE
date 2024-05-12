#pragma once

#include "NiKeyBasedInterpolator.hpp"
#include "NiFloatData.hpp"
#include "NiFixedString.hpp"
#include "NiQuatTransform.hpp"

class BSTreadTransform;

class BSTreadTransfInterpolator : public NiKeyBasedInterpolator {
public:
	struct LinkInfo {
		NiQuatTransform kStart;
		NiQuatTransform kEnd;
		NiFixedString	strName;
	};

	LinkInfo*		pLinks;
	UInt32			uiNumLinks;
	float			fCurrLerp;
	float			fDiffLerp;
	UInt32			uiLastLerpIdx;
	NiFloatDataPtr	spLerpData;
};

ASSERT_SIZE(BSTreadTransfInterpolator, 0x24);