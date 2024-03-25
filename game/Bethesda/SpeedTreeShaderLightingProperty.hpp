#pragma once
#include "BSShaderLightingProperty.hpp"

class SpeedTreeShaderLightingProperty : public BSShaderLightingProperty {
public:
	class UnknownTreeObject : public NiRefObject {
	public:
		DWORD unk8;
		WORD unkC;
	};

	NiObjectPtr						spUnk7C;
	NiObjectPtr						spUnk80;
	NiPointer<UnknownTreeObject>	spUnk84;
};
ASSERT_SIZE(SpeedTreeShaderLightingProperty, 0x88);