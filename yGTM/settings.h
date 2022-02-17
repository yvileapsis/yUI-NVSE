#pragma once
#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

inline int g_logLevel					= 1;

inline int g_yTM						= 1;
inline int g_yTM_Mode					= 1;
inline int g_yTM_MinMax					= 1;

inline int g_FixExplosionPushForce		= 1;
inline int g_RestoreSpreadGameSettings	= 1;

void handleINIOptions();