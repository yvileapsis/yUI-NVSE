#pragma once
#include "NiRefObject.hpp"
#include "NiPoint3.hpp"

class TESAnimGroup : public NiRefObject {
public:
	TESAnimGroup();
	virtual ~TESAnimGroup();

	enum EnumAnimGroupID : UInt8 {
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

	// 24
	struct AnimGroupInfo {
		const char* name;				// 00
		UInt8		supportsVariants;		// 04
		UInt8		pad[3];
		UInt32		sequenceType;
		UInt32		keyType;
		UInt32		unk10;
		UInt32		unk14[4];
	};

	struct __declspec(align(4)) AnimGroupSound
	{
		float		unk00;
		UInt8		soundID;
		UInt8		gap05[3];
		UInt32		unk08;
		TESSound*	sound;
	};


	UInt8			byte08[8];
	UInt16			groupID;
	UInt8			unk12[1];
	UInt32			numKeys;
	float*			keyTimes;
	NiPoint3		moveVector;
	UInt8			leftOrRight_whichFootToSwitch;
	UInt8			blend;
	UInt8			blendIn;
	UInt8			blendOut;
	UInt8			decal;
	UInt8			gap2D[3];
	char*			parentRootNode;
	UInt32			unk34;
	AnimGroupSound*	ptr38;

	static const char* StringForAnimGroupCode(UInt32 groupCode);
	static UInt32 AnimGroupForString(const char* groupName);

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
};
static_assert(sizeof(TESAnimGroup) == 0x3C);
extern std::span<TESAnimGroup::AnimGroupInfo> g_animGroups;