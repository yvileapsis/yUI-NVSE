#pragma once
#include "Menu.hpp"

class CharGenMenu : public Menu
{
	CharGenMenu();
	~CharGenMenu();
public:
	UInt32				isTagSkills;
	Tile*				tile02C;
	Tile*				tile030;
	Tile*				tileActorValueIcon;
	Tile*				tileActorValueDescription;
	Tile*				tileNumSkills;
	Tile*				tileReset;
	Tile*				tileDone;
	SInt32				currPoints;
	SInt32				numPoints;
	Float32				listIndex;
	UInt8				isModValues;
	UInt8				showInitialTaggedSkills;
	UInt8				gap056[2];
	ListBox<UInt32>		actorValues;

	static CharGenMenu* GetSingleton() { return *reinterpret_cast<CharGenMenu**>(0x11D920C); };
};
static_assert(sizeof(CharGenMenu) == 0x88);