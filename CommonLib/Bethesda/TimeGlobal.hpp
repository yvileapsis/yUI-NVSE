#pragma once
#include "BSTimer.hpp"

struct TimeGlobal : BSTimer
{
	static TimeGlobal*				GetSingleton() { return reinterpret_cast<TimeGlobal*>(0x11F6394); };
	__forceinline void				Set(const Float32 value, const char isImmediateChange) { ThisCall<void>(0xAA4DB0, this, value, isImmediateChange); }
	static __forceinline Float32	Get() { return *reinterpret_cast<Float32*>(0x11AC3A0); }
	static __forceinline Float32	GetTarget() { return *reinterpret_cast<Float32*>(0x11AC3A4); }
};