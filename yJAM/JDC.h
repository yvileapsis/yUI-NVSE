#pragma once

inline UInt32	g_JDC					= 1;

namespace JDC
{
	
	inline Float64	g_Distance			= 0;
	inline Float64	g_Speed				= 0.3;

	inline Float64	g_LengthMax			= 72;
	inline Float64	g_LengthMin			= 24;
	inline Float64	g_Width				= 8;
	inline Float64	g_OffsetMin			= 0;
	inline Float64	g_OffsetMax			= 256;

	inline UInt32	g_ModeHolstered		= 1;
	inline UInt32	g_ModeOut1st		= 5;
	inline UInt32	g_ModeOut3rd		= 5;
	inline UInt32	g_ModeSighting1st	= 3;
	inline UInt32	g_ModeSighting3rd	= 3;
	inline UInt32	g_ModeScope			= 0;

	inline UInt32	g_NoNodeSighting	= 1;
	inline UInt32	g_ShotgunAlt		= 1;
	inline UInt32	g_Dynamic			= 1;

	void Initialize();
	void Reset();
	void MainLoop();
}
