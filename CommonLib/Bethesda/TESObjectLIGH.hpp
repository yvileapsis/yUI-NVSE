#pragma once
#include "TESBoundAnimObject.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "BGSMessageIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESWeightForm.hpp"
#include "TESValueForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "NiPoint3.hpp"

class TESSound;

// 0xC8
class TESObjectLIGH :
	public TESBoundAnimObject,			// 000
	public TESFullName,					// 030
	public TESModelTextureSwap,			// 03C
	public TESIcon,						// 05C
	public BGSMessageIcon,				// 068
	public TESScriptableForm,			// 078
	public TESWeightForm,				// 084
	public TESValueForm,				// 08C
	public BGSDestructibleObjectForm	// 094
{
public:
	TESObjectLIGH();
	~TESObjectLIGH();

	enum EnumFlags : UInt32
	{
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
		UInt32			uiRadius;
		union {
			struct {
				UInt8	ucRed;
				UInt8	ucGreen;
				UInt8	ucBlue;
				UInt8	ucAlpha;
			};
			UInt32		uiColor;
		};
		EnumFlags		eFlags;
		float			fFalloffExp;
		float			fFOV;
	};

	Data			kData;
	float			fFadeValue;
	TESSound*		pkSound;
	NiPoint3		kPointBC;

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		kData.eFlags = (EnumFlags) (bMod ? (kData.eFlags | pFlag) : (kData.eFlags & ~pFlag));
	}
};
static_assert(sizeof(TESObjectLIGH) == 0x0C8);