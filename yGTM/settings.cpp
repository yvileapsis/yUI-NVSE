#include <settings.h>
#include <SimpleINILibrary.h>
#include <Utilities.h>

void HandleINIForPath(const std::string& iniPath, const bool isDefault = false)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());

	if (errVal == SI_FILE) { return; }
	
	if (isDefault)
	{
		g_yTM = ini.GetOrCreate("General", "bTimeMultChanges", 1, "; enable 'Game Time Mult' section of this .ini file.");
		g_yTM_Mode = ini.GetOrCreate("Game Time Mult", "bTimeMultMode", 1, "; select which mods are handled by yGTM, with 0 disabling handling of mods altogether, 1 handling mods that use SGTM command and 2 handling all mods.");
		g_yTM_MinMax = ini.GetOrCreate("Game Time Mult", "bTimeMultMinMax", 1, "; use multiplication of minimum and maximum local values instead of a multiplication of all local values. Provides a more sane range of TimeMult values.");
		g_FixExplosionPushForce = ini.GetOrCreate("General", "bFixExplosionPushForce", 1, "; fix force of the explosions to scale both with distance (like explosion damage) and with actual force of the explosion's baseform.");
		g_AlterSpread = ini.GetOrCreate("General", "iAlterSpread", 1, "; restore gamesettings controlling the spread value on weapon forms.");
		g_ArmedUnarmed = ini.GetOrCreate("General", "bArmedUnarmed", 1, "; Allow for Melee and Unarmed weapons to use ammo and shoot projectiles");
		g_CorrectAmmoEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectAmmoEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
		g_CorrectMeltdownEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectMeltdownEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
		g_CorrectWeaponEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectWeaponEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");

	}
	
	ini.SaveFile(iniPath.c_str(), false);

}

void handleINIOptions()
{
	auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yGTM.ini)";
	HandleINIForPath(iniPath, true);
}