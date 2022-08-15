#pragma once
#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

inline int g_logLevel					= 1;

inline int g_yTM						= 1;
inline int g_yTM_Mode					= 1;
inline int g_yTM_MinMax					= 1;

inline int g_FixExplosionPushForce		= 1;
inline int g_AlterSpread				= 0;

inline int g_CorrectAmmoEffects			= 0;
inline int g_CorrectMeltdownEffects		= 0;
inline int g_CorrectWeaponEffects		= 0;

inline int g_ArmedUnarmed				= 1;

void handleINIOptions();