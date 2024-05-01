#pragma once
#include "SpeedTreeShaderLightingProperty.hpp"

class SpeedTreeLeafShaderProperty : public SpeedTreeShaderLightingProperty
{
public:
	class LeafData : public NiRefObject {
	public:
		UInt32		unk04;
		float*		pfLeafBase; // pointer to float4[48]
	};

	NiPointer<LeafData> spLeafData;
	UInt16				unk8C;

	NIRTTI_ADDRESS(0x12007A0)
};
ASSERT_SIZE(SpeedTreeLeafShaderProperty, 0x90);