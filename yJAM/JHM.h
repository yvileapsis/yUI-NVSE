#pragma once

enum kHitMarkerFlags
{
	kHitMarkerNothing			= 1 << 0,
	kHitMarkerNormal			= 1 << 1,
	kHitMarkerAltColor			= 1 << 2,
	kHitMarkerDouble			= 1 << 3,
	kHitMarkerHalfAlpha			= 1 << 4,
	kHitMarkerOffset			= 1 << 5,
	kHitMarkerShake				= 1 << 6,
	kHitMarkerRotate			= 1 << 7,
};

inline UInt32	g_JHM					= 1;
inline Float64	g_JHM_Seconds			= 0.5;
inline Float64	g_JHM_Alpha				= 400.0;
inline Float64	g_JHM_Length			= 24.0;
inline Float64	g_JHM_Width				= 8.0;
inline Float64	g_JHM_Offset			= 24.0;
inline UInt32	g_JHM_ModeHit			= kHitMarkerNormal;
inline UInt32	g_JHM_ModeDead			= kHitMarkerNormal;
inline UInt32	g_JHM_ModeKill			= kHitMarkerOffset;
inline UInt32	g_JHM_ModeEnemy			= kHitMarkerAltColor;
inline UInt32	g_JHM_ModeCrit			= kHitMarkerDouble;
inline UInt32	g_JHM_ModeHeadshot		= kHitMarkerShake;
inline UInt32	g_JHM_ModeCompanion		= kHitMarkerHalfAlpha;
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
