#pragma once

inline UInt32	g_JHM					= 1;

namespace JHM
{
	enum kHitMarkerFlags
	{
		kHitMarkerNothing = 1 << 0,
		kHitMarkerNormal = 1 << 1,
		kHitMarkerAltColor = 1 << 2,
		kHitMarkerDouble = 1 << 3,
		kHitMarkerHalfAlpha = 1 << 4,
		kHitMarkerOffset = 1 << 5,
		kHitMarkerShake = 1 << 6,
		kHitMarkerRotate = 1 << 7,
	};

	inline Float64	g_Seconds			= 0.5;
	inline Float64	g_Alpha				= 400.0;
	inline Float64	g_Length			= 24.0;
	inline Float64	g_Width				= 8.0;
	inline Float64	g_Offset			= 24.0;
	inline UInt32	g_ModeHit			= kHitMarkerNormal;
	inline UInt32	g_ModeDead			= kHitMarkerNormal;
	inline UInt32	g_ModeKill			= kHitMarkerOffset;
	inline UInt32	g_ModeEnemy			= kHitMarkerAltColor;
	inline UInt32	g_ModeCrit			= kHitMarkerDouble;
	inline UInt32	g_ModeHeadshot		= kHitMarkerShake;
	inline UInt32	g_ModeCompanion		= kHitMarkerHalfAlpha;
	inline UInt32	g_ModeExplosion		= kHitMarkerDouble;
	inline UInt32	g_EnableOut			= 1;
	inline UInt32	g_EnableSighting	= 1;
	inline UInt32	g_EnableScope		= 1;
	inline UInt32	g_Dynamic			= 0;
	inline UInt32	g_MaxTiles			= 25;

	void Initialize();
	void Reset();
	void MainLoop();
	void OnHit(Actor*, void*);
}
