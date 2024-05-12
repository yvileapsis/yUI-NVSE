#pragma once
#include "Menu.hpp"

// 44
class QuantityMenu : public Menu		// 1016
{
public:

	TileRect*	pkTile28;		// 28
	TileImage*	pkTile2C;		// 2C	QM_DecreaseArrow
	TileImage*	pkTile30;		// 30	QM_IncreaseArrow
	TileText*	pkTile34;		// 34
	TileImage*	pkTile38;		// 38
	TileImage*	pkTile3C;		// 3C
	Float32				currentQtt;		// 40

	static QuantityMenu* GetSingleton() { return *reinterpret_cast<QuantityMenu**>(0x11DA618); };
};
