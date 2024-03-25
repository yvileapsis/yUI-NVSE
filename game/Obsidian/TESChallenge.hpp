#pragma once
#include "TESForm.hpp"
#include "TESDescription.hpp"
#include "TESScriptableForm.hpp"
#include "BGSMessageIcon.hpp"

// 0x7C
class TESChallenge :
	public TESForm,
	public TESFullName,
	public TESDescription,
	public TESScriptableForm,
	public TESIcon,
	public BGSMessageIcon
{
	enum TESChallengeFlags
	{
		kChallenge_IsLocked = 0x1,
		kChallenge_IsCompleted = 0x2,
		kChallenge_NoRecur = 0x8,
	};

	struct ChallengeData
	{

		enum TESChallengeDataFlags
		{
			kChallengeData_StartDisabled = 0x1,
			kChallengeData_Recurring = 0x2,
			kChallengeData_ShowZeroProgress = 0x4,
		};

		UInt32 type;
		UInt32 threshold;
		UInt32 flags;
		UInt32 interval;
		UInt16 value1;
		UInt16 value2;
		UInt16 value3;
		UInt16 gap16;
	};

	TESChallenge::ChallengeData data;
	UInt32 amount;
	UInt32 challengeflags;
	TESForm *SNAM;
	TESForm *XNAM;
};
static_assert(sizeof(TESChallenge) == 0x7C);