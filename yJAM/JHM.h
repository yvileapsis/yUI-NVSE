#pragma once

inline UInt32	g_JHM					= 1;
inline Float64	g_JHM_Seconds			= 0.5;
inline Float64	g_JHM_Alpha				= 400.0;
inline Float64	g_JHM_Length			= 24.0;
inline Float64	g_JHM_Width				= 8.0;
inline Float64	g_JHM_Offset			= 24.0;
inline UInt32	g_JHM_ModeHit			= 1;
inline UInt32	g_JHM_ModeDead			= 1;
inline UInt32	g_JHM_ModeKill			= 3;
inline UInt32	g_JHM_ModeEnemy			= 1;
inline UInt32	g_JHM_ModeCrit			= 4;
inline UInt32	g_JHM_ModeHeadshot		= 2;
inline UInt32	g_JHM_ModeCompanion		= 0;
inline UInt32	g_JHM_EnableOut			= 1;
inline UInt32	g_JHM_EnableSighting	= 1;
inline UInt32	g_JHM_EnableScope		= 1;
inline UInt32	g_JHM_Dynamic			= 0;
inline UInt32	g_JHM_MaxTiles			= 25;

namespace JHM
{
	void Initialize();
	void MainLoop();
	void OnHitHandler(Actor*, void*);
}
