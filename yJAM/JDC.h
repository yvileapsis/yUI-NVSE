#pragma once

inline UInt32	g_JDC					= 1;
inline Float64	g_JDC_Distance			= 2.25;
inline Float64	g_JDC_Speed				= 0.3;
inline Float64	g_JDC_LengthMax			= 72;
inline Float64	g_JDC_LengthMin			= 24;
inline Float64	g_JDC_Width				= 8;
inline Float64	g_JDC_OffsetMin			= 0;
inline Float64	g_JDC_OffsetMax			= 256;
inline UInt32	g_JDC_ModeHolstered		= 1;
inline UInt32	g_JDC_ModeOut1st		= 5;
inline UInt32	g_JDC_ModeOut3rd		= 5;
inline UInt32	g_JDC_ModeSighting1st	= 3;
inline UInt32	g_JDC_ModeSighting3rd	= 3;
inline UInt32	g_JDC_ModeScope			= 0;
inline UInt32	g_JDC_NoNodeSighting	= 1;
inline UInt32	g_JDC_ShotgunAlt		= 1;
inline UInt32	g_JDC_Dynamic			= 1;

namespace JDC
{

	void Initialize();
	void MainLoop();
}
