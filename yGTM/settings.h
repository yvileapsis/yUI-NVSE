#pragma once
#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

inline int g_logLevel					= 1;


inline int g_FixExplosionPushForce		= 1;
inline int g_AlterSpread				= 0;

inline int g_CorrectAmmoEffects			= 0;
inline int g_CorrectMeltdownEffects		= 0;
inline int g_CorrectWeaponEffects		= 0;

inline int g_ArmedUnarmed				= 1;

void handleINIOptions();

g_FixExplosionPushForce = ini.GetOrCreate("General", "bFixExplosionPushForce", 1, "; fix force of the explosions to scale both with distance (like explosion damage) and with actual force of the explosion's baseform.");
g_AlterSpread = ini.GetOrCreate("General", "iAlterSpread", 1, "; restore gamesettings controlling the spread value on weapon forms.");
g_ArmedUnarmed = ini.GetOrCreate("General", "bArmedUnarmed", 1, "; Allow for Melee and Unarmed weapons to use ammo and shoot projectiles");
g_CorrectAmmoEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectAmmoEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
g_CorrectMeltdownEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectMeltdownEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
g_CorrectWeaponEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectWeaponEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
