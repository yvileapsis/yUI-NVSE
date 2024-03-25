#pragma once
#include "Tile.hpp"

// 1C
struct GradualSetTile
{
	//	0		From start to end in duration
	//	1		From start to end to start, in duration, perpetually
	//	2		From start to end to start, in duration, 4 times
	//	3		From start to end to start, 7 times, in duration
	//	4		From start to end in duration/6, end for duration*2/3, from end to start in duration/6
	//	5**		From start to end, in duration, perpetually (suitable for image rotation)
	enum EnumModes
	{
		StartToEnd				= 0,
		StartToEndPerpetual		= 1,
		StartToEndFourTimes		= 2,
		StartToEndSevenTimes	= 3,
		StartToEndToStart		= 4,
		StartToEndRotation		= 5
	};

	Float32			fStartValue;		// 00
	Float32			fEndValue;			// 04
	UInt32			uiStartTimeMS;		// 08
	Float32			fDurationMS;		// 0C
	Tile::EnumValue	eValueID;			// 10
	Tile*			pkTile;				// 14
	EnumModes		eChangeMode;		// 18	0-4, see 0xA081B5

	__forceinline static BSSimpleList<GradualSetTile>* QueuedGradualSetFloat() { return reinterpret_cast<BSSimpleList<GradualSetTile>*>(0x11F3348); }
};
static_assert(sizeof(GradualSetTile) == 0x1C);