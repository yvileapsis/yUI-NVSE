#pragma once
#include "Menu.hpp"

// 0x4014
class RouletteMenu : public Menu
{
public:
	RouletteMenu();
	~RouletteMenu();

	enum Buttons
	{
		kIncreaseBet = 0x6,
		kDecreaseBet
	};

	enum SlotsIds
	{
		kRoulette_Zero = 0x43,
		kRoulette_First12 = 0x93,
		kRoulette_1to18 = 0x94,
		kRoulette_Even = 0x95,
		kRoulette_Second12 = 0x96,
		kRoulette_Reds = 0x97,
		kRoulette_Blacks = 0x98,
		kRoulette_Third12 = 0x99,
		kRoulette_Odd = 0x9A,
		kRoulette_19to36 = 0x9B,
		kRoulette_Column1 = 0x9C,
		kRoulette_Column2 = 0x9D,
		kRoulette_Column3 = 0x9E,
	};

	struct PossibleBet
	{
		enum Color
		{
			kRoulette_Red = 0x1,
			kRoulette_Black = 0x2,
		};

		BSSimpleList<void*> includedNumbersList;
		UInt32 color;
		UInt32 payoutRatio;
		NiPoint3 pos;
		char str01C[8];
		char payoutText[64];
	};
	static_assert(sizeof(PossibleBet) == 0x64);

	int stage;
	union
	{
		Tile* pkTiles[11];

	};
	UInt32 time58;
	RouletteMenu::PossibleBet possibleBets[159];
	BSSimpleList<void*>*	placedBets;
	UInt32 rouletteWheelToTileIDArray[38];
	UInt32 tableNif;
	NiNode *pointsNif;
	UInt32 betNif;
	UInt32 chipNifs[10];
	UInt32 chipShadowNifs[10];
	NiNode *node3F70;
	NiNode *unk3F74;
	NiNode *unk3F78;
	NiNode *node3F7C;
	NiControllerManager *tableControllerManager;
	NiControllerSequence *tableSpinSequences[3];
	float distBetweenBet0And1;
	float halfDistBetweenBet0And1;
	float unk3F98;
	float unk3F9C;
	float unk3FA0;
	UInt32 unk3FA4;
	float unk3FA8;
	float unk3FAC;
	UInt32 unk3FB0;
	float unk3FB4;
	float unk3FB8;
	UInt32 unk3FBC;
	float unk3FC0;
	float unk3FC4;
	float unk3FC8;
	float unk3FCC;
	float unk3FD0;
	float unk3FD4_translateX;
	float unk3FD8_translateY;
	float unk3FDC;
	UInt32 nextPlacedBetID;
	UInt32 numPlacedBets;
	UInt32 betAmount;
	UInt32 totalBetAmount;
	UInt32 playerChipCount;
	UInt32 playerLuck;
	UInt32 unk3FF8;
	TESCasino *minBet;
	UInt32 maxBet;
	UInt32 winningSlot;
	UInt32 animToPlayIdx;
	UInt8 byte400C;
	UInt8 isLucky;
	UInt8 isUnlucky;
	UInt8 shouldBanFromCasino;
	int unk4010;
};
static_assert(sizeof(RouletteMenu) == 0x4014);