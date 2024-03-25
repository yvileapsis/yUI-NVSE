#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "TESTexture.hpp"

// 0x220
class TESCasino :
	public TESForm,
	public TESFullName
{
	struct GambleData
	{
		float shufflePercent;
		float blackjackPayout;
		UInt32 reelStops[7];
		UInt32 numDecks;
		UInt32 maxWinnings;
		UInt32 currencyRefID;
		UInt32 winningsQuestRefID;
		UInt32 flags;
	};
	static_assert(sizeof(GambleData) == 0x38);

	TESModelTextureSwap chip1;
	TESModelTextureSwap chip5;
	TESModelTextureSwap chip10;
	TESModelTextureSwap chip25;
	TESModelTextureSwap chip100;
	TESModelTextureSwap chip500;
	TESModelTextureSwap chipRoulette;
	TESModelTextureSwap slotMachine;
	TESModelTextureSwap blackjackTable;
	TESModelTextureSwap rouletteTable;
	TESIcon slotReel[7];
	TESTexture blackjackDeck[4];
	GambleData casinoData;
};
static_assert(sizeof(TESCasino) == 0x220);