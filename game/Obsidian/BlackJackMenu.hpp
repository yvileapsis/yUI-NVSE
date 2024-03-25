#pragma once
#include "Menu.hpp"

// 0xE70
class BlackJackMenu : public Menu
{
public:
	BlackJackMenu();
	~BlackJackMenu();

	enum EnumStages : UInt32
	{
		kBJCardsDealt = 0x4,
		kBJAfterStay = 0x8,
		kBJNoCardsDealt = 0xA,
		kBJExit = 0xD,
	};


	enum EnumSuits : UInt32
	{
		kBJHearts	= 0x0,
		kBJClubs	= 0x1,
		kBJDiamonds	= 0x2,
		kBJSpades	= 0x3,
	};

	enum Buttons
	{
		kCurrentBetDisplay = 0x0,
		kChipsDisplay = 0x1,
		kCasinoNameDisplay = 0x2,
		kHit = 0x3,
		kDouble_Deal = 0x4,
		kIncreaseBet = 0x5,
		kDecreaseBet = 0x6,
		kSurrender_MaxBet = 0x7,
		kStay_Exit = 0x8,
		kStatusText = 0x9,
	};

	struct BlackJackCard
	{
		UInt32			value;
		EnumSuits		suit;
		NiRefObject*	ref8;
		NiRefObject*	refC;
	};

	EnumStages				eStage;
	UInt32					unk2C;
	UInt32					unk30;
	Tile*					pkTiles34[10];
	UInt32					unk5C;
	Float32					fBlackjackPayout;
	Float32					fShufflePercent;
	UInt32					uiPlayerLuck;
	UInt32					uiCurrentBet;
	UInt32					unk70;
	UInt32					uiNumCurrency;
	UInt8					bIsLuckyWin;
	UInt8					bIsUnluckyLoss;
	UInt8					gap7A[2];
	UInt32					unk7C;
	UInt8					byte_somethingWinningsQuest;
	UInt8					gap81[3];
	UInt32					unk84[3];
	BlackJackCard			kCards[208];
	BSSimpleList<BlackJackCard*>* pkDealerHandCards;
	BSSimpleList<BlackJackCard*>* pkPlayerHandCards;
	BSSimpleList<BlackJackCard*>* pkPlayerSplitHandCards;
	UInt32					uiNumDecks;
	UInt32					uiNumRemainingCards;
	UInt8					unkDA4;
	UInt8					gapDA5[3];
	Float32					unkDA8;
	UInt8					dealerStandsOnSoft17;
	UInt8					gapDAD[3];
	UInt32					flags;
	TESModelTextureSwap*	pkBlackjackTable;
	NiRefObject*			pkHand1Nif;
	NiRefObject*			pkHand2Nif;
	NiRefObject*			pkDealerNif;
	TESModelTextureSwap*	pkChip1;
	TESModelTextureSwap*	pkChip5;
	TESModelTextureSwap*	pkChip10;
	TESModelTextureSwap*	pkChip25;
	TESModelTextureSwap*	pkChip100;
	TESModelTextureSwap*	pkChip500;
	TESModelTextureSwap*	pkChip1_Shadow;
	TESModelTextureSwap*	pkChip5_Shadow;
	TESModelTextureSwap*	pkChip10_Shadow;
	TESModelTextureSwap*	pkChip25_Shadow;
	TESModelTextureSwap*	pkChip100_Shadow;
	TESModelTextureSwap*	pkChip500_Shadow;
	NiControllerManager*	pkBlackjackTableControllerManager;
	NiControllerManager*	pkHand1ControllerManager;
	NiControllerManager*	pkHand2ControllerManager;
	NiControllerManager*	pkDealerControllerManager;
	NiControllerSequence*	pkPlayers_Discard;
	NiControllerSequence*	pkDealers_Discard;
	NiControllerSequence*	pkHand1_Discard;
	NiControllerSequence*	pkHand1_01;
	NiControllerSequence*	pkHand1_02;
	NiControllerSequence*	pkHand1_03;
	NiControllerSequence*	pkHand1_04;
	NiControllerSequence*	pkHand1_05;
	NiControllerSequence*	pkHand1_06;
	NiControllerSequence*	pkHand1_07;
	NiControllerSequence*	pkHand2_Discard;
	NiControllerSequence*	pkHand2_01;
	NiControllerSequence*	pkHand2_02;
	NiControllerSequence*	pkHand2_03;
	NiControllerSequence*	pkHand2_04;
	NiControllerSequence*	pkHand2_05;
	NiControllerSequence*	pkHand2_06;
	NiControllerSequence*	pkHand2_07;
	NiControllerSequence*	pkDealer_Discard;
	NiControllerSequence*	pkDealer_01;
	NiControllerSequence*	pkDealer_02;
	NiControllerSequence*	pkDealer_03;
	NiControllerSequence*	pkDealer_04;
	NiControllerSequence*	pkDealer_05;
	NiControllerSequence*	pkDealer_06;
	NiControllerSequence*	pkDealer_07;
	NiControllerSequence*	pkDealer_Reveal;
};
static_assert(sizeof(BlackJackMenu) == 0xE70);