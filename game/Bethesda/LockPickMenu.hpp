#pragma once
#include "Menu.hpp"


class LockPickMenu : public Menu
{
public:
	~LockPickMenu();
	LockPickMenu();

	UInt32 stage;			// 28
	TileRect*	pkTile2C;		// 2C
	TileRect*	pkTile30;		// 30
	TileRect*	pkTile34;		// 34
	TileImage*	pkTile38;		// 38
	TileText*	pkTile3C;		// 3C
	TileImage*	pkTile40;		// 40
	TileImage*	pkTile44;		// 44
	TileText*	pkTile48;		// 48
	TileImage*	pkTile4C;		// 4C
	TileImage*	pkTile50;		// 50
	TileImage*	pkTile54;		// 54
	TileText*	pkTile58;		// 58
	TileText*	pkTile5C;		// 5C
	TileImage*	pkTile60;		// 60
	TileImage*	pkTile64;		// 64
	TileImage*	pkTile68;		// 68
	TESObjectREFR* targetRef;		// 6C
	UInt32					lockLevel;		// 70
	UInt32					skillLevel;		// 74
	Float32					fSkillLevel;	// 78
	UInt32					unk7C;			// 7C
	UInt32					unk80;			// 80
	Float32					flt84;			// 84
	UInt32					sweetSpotLen;	// 88
	Float32					cylinderAngle;	// 8C
	Float32					pickAngle;		// 90
	Float32					pinHealth;		// 94
	UInt8					isForceRotate;	// 98
	UInt8					shouldShowHelp;	// 99
	UInt8					pad9A[2];		// 99
	NiControllerSequence* ctrlSeq9C;		// 9C
	NiControllerSequence* ctrlSeqA0;		// A0
	NiControllerManager* ctrlManager;		// A4
	NiControllerSequence* ctrlSeqForward;	// A8
	NiControllerSequence* ctrlSeqBackward;	// AC
	NiControllerSequence* ctrlSeqLeft;		// B0
	NiPoint4				quaternionB4;	// B4
	NiPoint4				quaternionC4;	// C4
	NiPoint4				quaternionD4;	// D4

	static LockPickMenu* GetSingleton() { return *reinterpret_cast<LockPickMenu**>(0x11DA204); };
};
static_assert(sizeof(LockPickMenu) == 0xE4);