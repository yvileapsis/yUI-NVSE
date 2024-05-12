#pragma once
#include "Menu.hpp"

// 0x158
class SlotMachineMenu : public Menu
{
public:
	SlotMachineMenu();
	~SlotMachineMenu();

	enum Buttons
	{
		kSpin = 0x3,
		kIncreaseBet,
		kDecreaseBet,
		kPayoutList,
		kBetMax,
		kExit,
	};

	UInt32 stage;
	UInt32 unk2C[40];
	UInt32 betAmount;
	UInt32 unkCC[34];
};
static_assert(sizeof(SlotMachineMenu) == 0x158);