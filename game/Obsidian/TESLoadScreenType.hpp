#include "TESForm.hpp"
#include "NiColor.hpp"

// 0x70
class TESLoadScreenType : public TESForm
{
	struct Data
	{
		enum TESLoadScreenTypes
		{
			kLoadScreenTypeNone = 0x0,
			kLoadScreenTypeXPProgress = 0x1,
			kLoadScreenTypeObjective = 0x2,
			kLoadScreenTypeTip = 0x3,
			kLoadScreenTypeStats = 0x4,
		};

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

		TESLoadScreenTypes eType;
		TESLoadScreenType::Data::Info kInfo1;
		TESLoadScreenType::Data::Info kInfo2;
		UInt32 uiStats;
	};

	TESLoadScreenType::Data kData;
};
static_assert(sizeof(TESLoadScreenType) == 0x70);