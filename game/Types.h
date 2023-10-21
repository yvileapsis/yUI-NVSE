#pragma once
#include <NiNodes.h>
#include <OSDepend.h>

#include "Utilities.hpp"

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


// 02C+
class TESAnimGroup : NiRefObject
{
public:

	enum AnimGroupID : UInt8
	{
		kAnimGroup_Idle = 0x0,
		kAnimGroup_DynamicIdle,
		kAnimGroup_SpecialIdle,
		kAnimGroup_Forward,
		kAnimGroup_Backward,
		kAnimGroup_Left,
		kAnimGroup_Right,
		kAnimGroup_FastForward,
		kAnimGroup_FastBackward,
		kAnimGroup_FastLeft,
		kAnimGroup_FastRight,
		kAnimGroup_DodgeForward,
		kAnimGroup_DodgeBack,
		kAnimGroup_DodgeLeft,
		kAnimGroup_DodgeRight,
		kAnimGroup_TurnLeft,
		kAnimGroup_TurnRight,
		kAnimGroup_Aim,
		kAnimGroup_AimUp,
		kAnimGroup_AimDown,
		kAnimGroup_AimIS,
		kAnimGroup_AimISUp,
		kAnimGroup_AimISDown,
		kAnimGroup_Holster,
		kAnimGroup_Equip,
		kAnimGroup_Unequip,
		kAnimGroup_AttackLeft,
		kAnimGroup_AttackLeftUp,
		kAnimGroup_AttackLeftDown,
		kAnimGroup_AttackLeftIS,
		kAnimGroup_AttackLeftISUp,
		kAnimGroup_AttackLeftISDown,
		kAnimGroup_AttackRight,
		kAnimGroup_AttackRightUp,
		kAnimGroup_AttackRightDown,
		kAnimGroup_AttackRightIS,
		kAnimGroup_AttackRightISUp,
		kAnimGroup_AttackRightISDown,
		kAnimGroup_Attack3,
		kAnimGroup_Attack3Up,
		kAnimGroup_Attack3Down,
		kAnimGroup_Attack3IS,
		kAnimGroup_Attack3ISUp,
		kAnimGroup_Attack3ISDown,
		kAnimGroup_Attack4,
		kAnimGroup_Attack4Up,
		kAnimGroup_Attack4Down,
		kAnimGroup_Attack4IS,
		kAnimGroup_Attack4ISUp,
		kAnimGroup_Attack4ISDown,
		kAnimGroup_Attack5,
		kAnimGroup_Attack5Up,
		kAnimGroup_Attack5Down,
		kAnimGroup_Attack5IS,
		kAnimGroup_Attack5ISUp,
		kAnimGroup_Attack5ISDown,
		kAnimGroup_Attack6,
		kAnimGroup_Attack6Up,
		kAnimGroup_Attack6Down,
		kAnimGroup_Attack6IS,
		kAnimGroup_Attack6ISUp,
		kAnimGroup_Attack6ISDown,
		kAnimGroup_Attack7,
		kAnimGroup_Attack7Up,
		kAnimGroup_Attack7Down,
		kAnimGroup_Attack7IS,
		kAnimGroup_Attack7ISUp,
		kAnimGroup_Attack7ISDown,
		kAnimGroup_Attack8,
		kAnimGroup_Attack8Up,
		kAnimGroup_Attack8Down,
		kAnimGroup_Attack8IS,
		kAnimGroup_Attack8ISUp,
		kAnimGroup_Attack8ISDown,
		kAnimGroup_AttackLoop,
		kAnimGroup_AttackLoopUp,
		kAnimGroup_AttackLoopDown,
		kAnimGroup_AttackLoopIS,
		kAnimGroup_AttackLoopISUp,
		kAnimGroup_AttackLoopISDown,
		kAnimGroup_AttackSpin,
		kAnimGroup_AttackSpinUp,
		kAnimGroup_AttackSpinDown,
		kAnimGroup_AttackSpinIS,
		kAnimGroup_AttackSpinISUp,
		kAnimGroup_AttackSpinISDown,
		kAnimGroup_AttackSpin2,
		kAnimGroup_AttackSpin2Up,
		kAnimGroup_AttackSpin2Down,
		kAnimGroup_AttackSpin2IS,
		kAnimGroup_AttackSpin2ISUp,
		kAnimGroup_AttackSpin2ISDown,
		kAnimGroup_AttackPower,
		kAnimGroup_AttackForwardPower,
		kAnimGroup_AttackBackPower,
		kAnimGroup_AttackLeftPower,
		kAnimGroup_AttackRightPower,
		kAnimGroup_AttackCustom1Power,
		kAnimGroup_AttackCustom2Power,
		kAnimGroup_AttackCustom3Power,
		kAnimGroup_AttackCustom4Power,
		kAnimGroup_AttackCustom5Power,
		kAnimGroup_PlaceMine,
		kAnimGroup_PlaceMineUp,
		kAnimGroup_PlaceMineDown,
		kAnimGroup_PlaceMineIS,
		kAnimGroup_PlaceMineISUp,
		kAnimGroup_PlaceMineISDown,
		kAnimGroup_PlaceMine2,
		kAnimGroup_PlaceMine2Up,
		kAnimGroup_PlaceMine2Down,
		kAnimGroup_PlaceMine2IS,
		kAnimGroup_PlaceMine2ISUp,
		kAnimGroup_PlaceMine2ISDown,
		kAnimGroup_AttackThrow,
		kAnimGroup_AttackThrowUp,
		kAnimGroup_AttackThrowDown,
		kAnimGroup_AttackThrowIS,
		kAnimGroup_AttackThrowISUp,
		kAnimGroup_AttackThrowISDown,
		kAnimGroup_AttackThrow2,
		kAnimGroup_AttackThrow2Up,
		kAnimGroup_AttackThrow2Down,
		kAnimGroup_AttackThrow2IS,
		kAnimGroup_AttackThrow2ISUp,
		kAnimGroup_AttackThrow2ISDown,
		kAnimGroup_AttackThrow3,
		kAnimGroup_AttackThrow3Up,
		kAnimGroup_AttackThrow3Down,
		kAnimGroup_AttackThrow3IS,
		kAnimGroup_AttackThrow3ISUp,
		kAnimGroup_AttackThrow3ISDown,
		kAnimGroup_AttackThrow4,
		kAnimGroup_AttackThrow4Up,
		kAnimGroup_AttackThrow4Down,
		kAnimGroup_AttackThrow4IS,
		kAnimGroup_AttackThrow4ISUp,
		kAnimGroup_AttackThrow4ISDown,
		kAnimGroup_AttackThrow5,
		kAnimGroup_AttackThrow5Up,
		kAnimGroup_AttackThrow5Down,
		kAnimGroup_AttackThrow5IS,
		kAnimGroup_AttackThrow5ISUp,
		kAnimGroup_AttackThrow5ISDown,
		kAnimGroup_Attack9,
		kAnimGroup_Attack9Up,
		kAnimGroup_Attack9Down,
		kAnimGroup_Attack9IS,
		kAnimGroup_Attack9ISUp,
		kAnimGroup_Attack9ISDown,
		kAnimGroup_AttackThrow6,
		kAnimGroup_AttackThrow6Up,
		kAnimGroup_AttackThrow6Down,
		kAnimGroup_AttackThrow6IS,
		kAnimGroup_AttackThrow6ISUp,
		kAnimGroup_AttackThrow6ISDown,
		kAnimGroup_AttackThrow7,
		kAnimGroup_AttackThrow7Up,
		kAnimGroup_AttackThrow7Down,
		kAnimGroup_AttackThrow7IS,
		kAnimGroup_AttackThrow7ISUp,
		kAnimGroup_AttackThrow7ISDown,
		kAnimGroup_AttackThrow8,
		kAnimGroup_AttackThrow8Up,
		kAnimGroup_AttackThrow8Down,
		kAnimGroup_AttackThrow8IS,
		kAnimGroup_AttackThrow8ISUp,
		kAnimGroup_AttackThrow8ISDown,
		kAnimGroup_Counter,
		kAnimGroup_stomp,
		kAnimGroup_BlockIdle,
		kAnimGroup_BlockHit,
		kAnimGroup_Recoil,
		kAnimGroup_ReloadWStart,
		kAnimGroup_ReloadXStart,
		kAnimGroup_ReloadYStart,
		kAnimGroup_ReloadZStart,
		kAnimGroup_ReloadA,
		kAnimGroup_ReloadB,
		kAnimGroup_ReloadC,
		kAnimGroup_ReloadD,
		kAnimGroup_ReloadE,
		kAnimGroup_ReloadF,
		kAnimGroup_ReloadG,
		kAnimGroup_ReloadH,
		kAnimGroup_ReloadI,
		kAnimGroup_ReloadJ,
		kAnimGroup_ReloadK,
		kAnimGroup_ReloadL,
		kAnimGroup_ReloadM,
		kAnimGroup_ReloadN,
		kAnimGroup_ReloadO,
		kAnimGroup_ReloadP,
		kAnimGroup_ReloadQ,
		kAnimGroup_ReloadR,
		kAnimGroup_ReloadS,
		kAnimGroup_ReloadW,
		kAnimGroup_ReloadX,
		kAnimGroup_ReloadY,
		kAnimGroup_ReloadZ,
		kAnimGroup_JamA,
		kAnimGroup_JamB,
		kAnimGroup_JamC,
		kAnimGroup_JamD,
		kAnimGroup_JamE,
		kAnimGroup_JamF,
		kAnimGroup_JamG,
		kAnimGroup_JamH,
		kAnimGroup_JamI,
		kAnimGroup_JamJ,
		kAnimGroup_JamK,
		kAnimGroup_JamL,
		kAnimGroup_JamM,
		kAnimGroup_JamN,
		kAnimGroup_JamO,
		kAnimGroup_JamP,
		kAnimGroup_JamQ,
		kAnimGroup_JamR,
		kAnimGroup_JamS,
		kAnimGroup_JamW,
		kAnimGroup_JamX,
		kAnimGroup_JamY,
		kAnimGroup_JamZ,
		kAnimGroup_Stagger,
		kAnimGroup_Death,
		kAnimGroup_Talking,
		kAnimGroup_PipBoy,
		kAnimGroup_JumpStart,
		kAnimGroup_JumpLoop,
		kAnimGroup_JumpLand,
		kAnimGroup_HandGrip1,
		kAnimGroup_HandGrip2,
		kAnimGroup_HandGrip3,
		kAnimGroup_HandGrip4,
		kAnimGroup_HandGrip5,
		kAnimGroup_HandGrip6,
		kAnimGroup_JumpLoopForward,
		kAnimGroup_JumpLoopBackward,
		kAnimGroup_JumpLoopLeft,
		kAnimGroup_JumpLoopRight,
		kAnimGroup_PipBoyChild,
		kAnimGroup_JumpLandForward,
		kAnimGroup_JumpLandBackward,
		kAnimGroup_JumpLandLeft,
		kAnimGroup_JumpLandRight,

		kAnimGroup_Max						// = 0x0FFF,	// Temporary until known
	};

	// derived from NiRefObject
	TESAnimGroup();
	~TESAnimGroup();

	virtual void Destructor(bool arg0);

	bool IsAttack()
	{
		return ThisStdCall(0x4937E0, this);
	}

	bool IsAttackIS();
	bool IsAttackNonIS();


	bool IsAim()
	{
		auto idMinor = groupID & 0xFF;
		return idMinor >= kAnimGroup_Aim && idMinor <= kAnimGroup_AimISDown;
	}

	bool IsReload()
	{
		auto idMinor = groupID & 0xFF;
		return idMinor >= kAnimGroup_ReloadWStart && idMinor <= kAnimGroup_ReloadZ;
	}

	// 24
	struct AnimGroupInfo
	{
		const char* name;
		bool supportsVariants;
		UInt8 pad[3];
		UInt32 sequenceType;
		UInt32 keyType;
		UInt32 unk10;
		UInt32 unk14[4];
	};

	AnimGroupInfo* GetGroupInfo() const;
	UInt8 GetBaseGroupID() const;

	struct __declspec(align(4)) AnimGroupSound
	{
		float unk00;
		UInt8 soundID;
		UInt8 gap05[3];
		UInt32 unk08;
		TESSound* sound;
	};


	UInt8 byte08[8];
	UInt16 groupID;
	UInt8 unk12[1];
	UInt32 numKeys;
	float* keyTimes;
	NiPoint3 moveVector;
	UInt8 leftOrRight_whichFootToSwitch;
	UInt8 blend;
	UInt8 blendIn;
	UInt8 blendOut;
	UInt8 decal;
	UInt8 gap2D[3];
	char* parentRootNode;
	UInt32 numPtr38s;
	AnimGroupSound* ptr38;

	static const char* StringForAnimGroupCode(UInt32 groupCode);
	static UInt32 AnimGroupForString(const char* groupName);
};
static_assert(sizeof(TESAnimGroup) == 0x3C);



//TESAnimGroup::AnimGroupInfo* GetGroupInfo(UInt8 groupId);
UInt32 GetSequenceType(UInt8 groupId);

TESForm* __stdcall LookupFormByRefID(UInt32 refID);
void FormatScriptText(std::string& str);

UInt16 GetActorRealAnimGroup(Actor* actor, UInt8 groupID);


struct Timer
{
	UInt8 disableCounter;		// 00
	UInt8 gap01[3];				// 01
	float fpsClamp;				// 04
	float fpsClampRemainder;	// 08
	float secondsPassed;		// 0C
	float lastSecondsPassed;	// 10
	UInt32 msPassed;			// 14
	UInt32 unk18;				// 18
	byte isChangeTimeMultSlowly;// 1C
	byte unk1D;					// 1D
	byte unk1E;					// 1E
	byte unk1F;					// 1F
};

struct TimeGlobal : Timer
{
	float unk20;  // 020
	float unk24;  // 024
	float unk28;  // 028

	static TimeGlobal* GetSingleton() { return reinterpret_cast<TimeGlobal*>(0x11F6394); };
	__forceinline void				Set(const Float32 value, const char isImmediateChange) { ThisCall<void>(0xAA4DB0, this, value, isImmediateChange); }
	static __forceinline Float32	Get() { return *reinterpret_cast<Float32*>(0x11AC3A0); }
	static __forceinline Float32	GetTarget() { return *reinterpret_cast<Float32*>(0x11AC3A4); }
};

struct ToBeNamed
{
	char		m_path[0x104];	// 0000
	BSFile* m_file;			// 0104
	UInt32		m_unk0108;		// 0108
	UInt32		m_offset;		// 010C
};