#pragma once

inline UInt32	g_JVO					= 1;
inline UInt32	g_JVO_Key				= 49;
inline UInt32	g_JVO_Toggle			= 1;

inline UInt32	g_JVO_OffscreenHandling	= 0;

inline UInt32	g_JVO_DistanceHandling	= 2;
inline UInt32	g_JVO_DistanceSystem	= 1;
inline UInt32	g_JVO_TextHandling		= 1;
inline UInt32	g_JVO_TextSystem		= 1;

inline Float64	g_JVO_Alpha				= 0.0;
inline Float64	g_JVO_AlphaMult			= 0.6;

inline Float64	g_JVO_Height			= 36.0;
inline Float64	g_JVO_Width				= 24.0;
inline Float64	g_JVO_OffsetHeight		= 0.02;
inline Float64	g_JVO_OffsetWidth		= 0.01;

inline UInt32	g_JVO_AltColor			= 1;

inline Float64	g_JVO_Radius			= 0.06;
inline Float64	g_JVO_DistanceMin		= -1.0;
inline Float64	g_JVO_DistanceMax		= -1.0;

inline UInt32	g_JVO_EnableOut			= 1;
inline UInt32	g_JVO_EnableSighting	= 1;
inline UInt32	g_JVO_EnableScope		= 0;

inline UInt32	g_JVO_Font				= 0;
inline Float64	g_JVO_FontY				= 0.0;

namespace JVO
{
	void Initialize();
	void MainLoop();
}
