#pragma once
#include <NiTypes.h>
#include <GameOSDepend.h>

struct Semaphore
{
	UInt32 count;
	HANDLE semaphore;
	UInt32 max;
};

struct MouseFilter
{
	float targetValue = 0;
	float remainingValue = 0;
	float lastAmount = 0;

	float smooth(float delta, float scale)
	{
		targetValue += delta;
		delta = (targetValue - remainingValue) * scale;
		lastAmount += (delta - lastAmount) * 0.5F;

		if (delta > 0.0F && delta > lastAmount || delta < 0.0F && delta < lastAmount)
		{
			delta = lastAmount;
		}

		remainingValue += delta;
		return delta;
	}

	void reset()
	{
		targetValue = 0;
		remainingValue = 0;
		lastAmount = 0;
	}
};


enum class IsDXKeyState
{
	IsHeld		= 0x0,
	IsPressed	= 0x1,
	IsDepressed	= 0x2,
	IsChanged	= 0x3,
};


enum AnimState
{
	kAnimState_Inactive		= 0x0,
	kAnimState_Animating	= 0x1,
	kAnimState_EaseIn		= 0x2,
	kAnimState_EaseOut		= 0x3,
	kAnimState_TransSource	= 0x4,
	kAnimState_TransDest	= 0x5,
	kAnimState_MorphSource	= 0x6,
};

enum class ControlCode
{
	Forward = 0x0,
	Backward = 0x1,
	Left = 0x2,
	Right = 0x3,
	Attack = 0x4,
	Activate = 0x5,
	Aim = 0x6,
	ReadyItem = 0x7,
	Crouch = 0x8,
	Run = 0x9,
	AlwaysRun = 0xA,
	AutoMove = 0xB,
	Jump = 0xC,
	TogglePOV = 0xD,
	MenuMode = 0xE,
	Rest = 0xF,
	VATS_ = 0x10,
	Hotkey1 = 0x11,
	AmmoSwap = 0x12,
	Hotkey3 = 0x13,
	Hotkey4 = 0x14,
	Hotkey5 = 0x15,
	Hotkey6 = 0x16,
	Hotkey7 = 0x17,
	Hotkey8 = 0x18,
	QuickSave = 0x19,
	QuickLoad = 0x1A,
	Grab = 0x1B,
	Escape = 0x1C,
	Console = 0x1D,
	Screenshot = 0x1E,
};

extern OSInputGlobals** g_inputGlobals;

//TESAnimGroup::AnimGroupInfo* GetGroupInfo(UInt8 groupId);
UInt32 GetSequenceType(UInt8 groupId);

TESForm* __stdcall LookupFormByRefID(UInt32 refID);
void FormatScriptText(std::string& str);

UInt16 GetActorRealAnimGroup(Actor* actor, UInt8 groupID);