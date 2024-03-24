#pragma once

#include "TESBoundAnimObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "BGSMessageIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESWeightForm.hpp"
#include "TESValueForm.hpp"
#include "BGSDestructibleObjectForm.hpp"

class TESSound;

class TESObjectLIGH : public TESBoundAnimObject, public TESFullName, public TESModelTextureSwap, public TESIcon, 
	public BGSMessageIcon, public TESScriptableForm, public TESWeightForm, public TESValueForm, public BGSDestructibleObjectForm {
public:
	TESObjectLIGH();
	~TESObjectLIGH();

	enum Flags {
		DYNAMIC				= 1 << 0,
		CAN_CARRY			= 1 << 1,
		NEGATIVE			= 1 << 2,
		FLICKER				= 1 << 3,
		DEEP_COPY			= 1 << 4,
		OFF_BY_DEFAULT		= 1 << 5,
		FLICKER_SLOW		= 1 << 6,
		PULSE				= 1 << 7,
		PULSE_SLOW			= 1 << 8,
		SPOTLIGHT			= 1 << 9,
		SPOTSHADOW			= 1 << 10,
		//	JIP Only
		COLOR_SHIFT			= 1 << 11,
		COLOR_SHIFT_SLOW	= 1 << 12,
	};

	struct Data {
		SInt32			iTime;
		UInt32			iRadius;
		union {
			struct {
				UInt8	ucRed;
				UInt8	ucGreen;
				UInt8	ucBlue;
				UInt8	ucAlpha;
			};
			UInt32 uiColor;
		};
		Bitfield32		uiLightFlags;
		float			fFalloffExp;
		float			fFOV;
	};

	Data			kData;
	float			fFadeValue;
	TESSound*		pSound;
	NiPoint3		kPointBC;
};

ASSERT_SIZE(TESObjectLIGH, 0xC8);