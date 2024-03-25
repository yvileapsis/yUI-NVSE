#pragma once
#include "BSSimpleList.hpp"

class TESWeather;
class TESGlobal;

// 0xC
struct WeatherEntry {
	TESWeather*	pkWeather;
	UInt32		uiChance;
	TESGlobal*	pkChanceVar;

	void Set(TESWeather* apWtr, UInt32 apChn, TESGlobal* apGlb)
	{
		pkWeather = apWtr;
		uiChance = apChn;
		pkChanceVar = apGlb;
	}
};
static_assert(sizeof(WeatherEntry) == 0xC);