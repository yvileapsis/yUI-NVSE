#include "TESForm.hpp"
#include "NiColor.hpp"

// 0x70
class TESLoadScreenType : public TESForm
{
	enum EnumTypes
	{
		kLoadScreenTypeNone = 0x0,
		kLoadScreenTypeXPProgress = 0x1,
		kLoadScreenTypeObjective = 0x2,
		kLoadScreenTypeTip = 0x3,
		kLoadScreenTypeStats = 0x4,
	};

	struct Data
	{
		struct Info
		{
			UInt32 x;
			UInt32 y;
			UInt32 width;
			UInt32 height;
			Float32 rotateAngle;
			UInt32 font1;
			NiColor fontcolor1;
			UInt32 justification;
		};

		EnumTypes	eType;
		Info		kInfo1;
		Info		kInfo2;
		UInt32		uiStats;
	};

	Data kData;
};
static_assert(sizeof(TESLoadScreenType) == 0x70);