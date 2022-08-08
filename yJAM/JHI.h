#pragma once

inline UInt32	g_JHI					= 1;

namespace JHI
{
	enum kHitIndicatorFlags
	{
		kHitIndicatorNothing	= 1 << 0,
		kHitIndicatorNormal		= 1 << 1,
		kHitIndicatorAltColor	= 1 << 2,
		kHitIndicatorDouble		= 1 << 3,
		kHitIndicatorHalfAlpha	= 1 << 4,
		kHitIndicatorOffset		= 1 << 5,
		kHitIndicatorShakeVert	= 1 << 6,
		kHitIndicatorShakeHoriz	= 1 << 7,
	};

	inline Float64	g_Seconds			= 1.5;
	inline Float64	g_Alpha				= 400.0;
	inline Float64	g_Height			= 256.0;
	inline Float64	g_Width				= 256.0;
	inline Float64	g_Offset			= 0.0;
	inline UInt32	g_ModeHit			= kHitIndicatorNormal;
	inline UInt32	g_ModeDead			= kHitIndicatorNormal;
	inline UInt32	g_ModeKill			= kHitIndicatorOffset;
	inline UInt32	g_ModeEnemy			= kHitIndicatorAltColor;
	inline UInt32	g_ModeCrit			= kHitIndicatorDouble;
	inline UInt32	g_ModeHeadshot		= kHitIndicatorShakeHoriz;
	inline UInt32	g_ModeSelfDamage	= kHitIndicatorHalfAlpha;
	inline UInt32	g_ModeExplosion		= kHitIndicatorShakeVert;
	inline UInt32	g_ModeNoAttacker	= kHitIndicatorHalfAlpha;
	inline UInt32	g_ModeNoDamage		= kHitIndicatorNothing;
	inline UInt32	g_EnableOut			= 1;
	inline UInt32	g_EnableSighting	= 1;
	inline UInt32	g_EnableScope		= 1;
	inline UInt32	g_Rotate			= 2;

	void Initialize();
	void Reset();
	void MainLoop();
	void OnHit(Actor*, void*);
}
