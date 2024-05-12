#pragma once

#include "hkpCharacterProxyListener.hpp"

class bhkCharacterListener : public hkpCharacterProxyListener {
public:
	Bitfield32	uiFlags;
	UInt32		unk008;
	UInt32		unk00C;
	hkVector4	unk010;
	float		fCosMaxSlopeAngle;
	UInt32		eMaterial024;
	UInt32		eLayerType;
	UInt32		unk02C;
	hkVector4	unk030;
	hkVector4	hkSupportNorm;
	float		unk050;
	float		unk054;
	float		unk058;
	float		unk05C;
	float		fScaledHeight_060;
	float		fScaledHeightOverSinTimeCosMaxAngle;
	UInt8		byte068;
	bool		byte069;
	UInt32		uiContactPoints;
};

ASSERT_SIZE(bhkCharacterListener, 0x70);