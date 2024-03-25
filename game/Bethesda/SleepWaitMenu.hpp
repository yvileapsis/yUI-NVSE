#pragma once
#include "Menu.hpp"


// 4C
class SleepWaitMenu : public Menu		// 1012
{
public:
	TileText*			tile28;		// 28
	TileImage*			tile2C;		// 2C
	TileText*			tile30;		// 30
	TileText*			tile34;		// 34
	TileImage*			tile38;		// 38
	TileImage*			tile3C;		// 3C
	UInt32				currentTimeWaited;			// 40
	bool				isStarted;		// 44
	bool				isRest;
	bool				restKeyDebounce;
	bool				pad47;
	Float32				selectedHours;	// 48

	__forceinline static SleepWaitMenu* GetSingleton() { return *reinterpret_cast<SleepWaitMenu**>(0x11DA920); }
};
static_assert(sizeof(SleepWaitMenu) == 0x4C);
