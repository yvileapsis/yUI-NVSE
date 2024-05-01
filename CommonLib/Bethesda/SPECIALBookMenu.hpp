#pragma once
#include "Menu.hpp"

class SPECIALBookMenu : public Menu
{
	struct SpecialBookButton;
	struct SpecialBookTextItem;
	struct SPECIALBookPage
	{
		BSSimpleArray<SpecialBookButton*> buttonArray;
		BSSimpleArray<SpecialBookTextItem*> textArray;
	};

public:
	enum Buttons
	{
		kNextPage = 0x0,
		kPreviousPage = 0x1,
		kIncreaseValue = 0x2,
		kDecreaseValue = 0x3,
		kExitMenu = 0x4,
		kClickable = 0x5,
		kIndexUp = 0x6,
		kIndexDown = 0x7,
	};

	Tile* unk028[8];

	UInt32 currPage;
	UInt32 finalPageSelectedSpecialIndex;
	Float32 time050;
	UInt8 isNotInChangePageAnim;
	UInt8 gap055[3];
	UInt32 numPoints;
	BSFadeNode* Babybook02Nif;
	SPECIALBookPage pages[9];
	NiRefObject* unk180[11];
	NiRefObject* unk1AC[4];
	NiRefObject* unk1BC[2];
	NiRefObject* unk1C4[2];
	NiRefObject* unk1CC[2];
	NiRefObject* unk1D4[2];
	NiRefObject* unk1DC[2];

	static SPECIALBookMenu* GetSingleton() { return *reinterpret_cast<SPECIALBookMenu**>(0x11DAA38); };
};
