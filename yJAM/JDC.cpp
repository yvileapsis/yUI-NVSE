#include <JDC.h>
#include <GameSettings.h>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace JDC
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	Float64				spreadCurrent		= 0;

	void Initialize()
	{
		initialized = true;
		tileMain = g_MenuHUDMain->tile->GetChild("JDC");
		if (!tileMain)
		{
			g_MenuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JDC\JDC.xml)");
			tileMain = g_MenuHUDMain->tile->GetChild("JDC");
		}
		if (!tileMain) return;
		RegisterEvent("JDC:Reset", 0, nullptr, 4);
		SetEventHandler("JDC:Reset", Reset);
		DispatchEvent("JDC:Reset", nullptr);
	}

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		g_JDC				= ini.GetOrCreate("JustMods", "JDC", 1.0, nullptr);
		g_Dynamic			= ini.GetOrCreate("JDC", "Dynamic", 1.0, nullptr);
		g_ShotgunAlt		= ini.GetOrCreate("JDC", "ShotgunAlt", 1.0, nullptr);
		g_NoNodeSighting	= ini.GetOrCreate("JDC", "NoNodeSighting", 1.0, nullptr);
		g_ModeHolstered		= ini.GetOrCreate("JDC", "ModeHolstered", 1.0, nullptr);
		g_ModeOut1st		= ini.GetOrCreate("JDC", "ModeOut1st", 5.0, nullptr);
		g_ModeOut3rd		= ini.GetOrCreate("JDC", "ModeOut3rd", 5.0, nullptr);
		g_ModeSighting1st	= ini.GetOrCreate("JDC", "ModeSighting1st", 3.0, nullptr);
		g_ModeSighting3rd	= ini.GetOrCreate("JDC", "ModeSighting3rd", 3.0, nullptr);
		g_ModeScope			= ini.GetOrCreate("JDC", "ModeScope", 0.0, nullptr);
		g_Distance			= ini.GetOrCreate("JDC", "Distance", 0.0, nullptr);
		g_Speed				= ini.GetOrCreate("JDC", "Speed", 0.3, nullptr);
		g_LengthMax			= ini.GetOrCreate("JDC", "LengthMax", 72.0, nullptr);
		g_LengthMin			= ini.GetOrCreate("JDC", "LengthMin", 24.0, nullptr);
		g_Width				= ini.GetOrCreate("JDC", "Width", 8.0, nullptr);
		g_OffsetMax			= ini.GetOrCreate("JDC", "OffsetMax", 256.0, nullptr);
		g_OffsetMin			= ini.GetOrCreate("JDC", "OffsetMin", 0.0, nullptr);
		
		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (!g_JDC)
		{
			RemoveEventHandler("yJAM:MainLoop", MainLoop);
			return;
		}

		SetEventHandler("yJAM:MainLoop", MainLoop);

		tileMain->SetFloat("_JDCDynamicOffset", g_Dynamic & 1);
		tileMain->SetFloat("_JDCDynamicLength", g_Dynamic & 2);

		tileMain->SetFloat("_JDCLengthMin", g_LengthMin);
		tileMain->SetFloat("_JDCLengthMax", g_LengthMax);

		tileMain->SetFloat("_JDCOffsetMin", g_OffsetMin);
		tileMain->SetFloat("_JDCOffsetMax", g_OffsetMax);

		tileMain->SetFloat("_JDCWidth", g_Width);

		tileMain->SetFloat("_JDCVisible", 0);
		g_TileReticleCenter->GetChild("reticle_center")->SetFloat(kTileValue_visible, 1);

		if (g_Distance == 0)
		{
			Float64 worldFOV = 0;
			GetNumericIniSetting("fDefaultWorldFOV:Display", &worldFOV);
			g_Distance = 0.2;
			if (worldFOV <= 160) g_Distance += 0.03 * (160 - worldFOV);
			if (worldFOV <= 100) g_Distance += 0.02 * (100 - worldFOV);
			if (worldFOV <= 65)	g_Distance += 0.05 * (65 - worldFOV);
		}
	}

	bool IsPlayerWeaponGood()
	{
		const auto weaponInfo = g_player->baseProcess->GetWeaponInfo();
		if (!weaponInfo) return false;
		if (!g_player->baseProcess->IsWeaponOut()) return false;
		if (!weaponInfo->weapon->IsRangedWeapon()) return false;
		// TODO: exclusion list
		return true;
	}

	bool IsPlayerWeaponShotgun()
	{
		if (!g_ShotgunAlt) return false;
		if (reinterpret_cast<ContChangesEntry*>(g_player->baseProcess->GetWeaponInfo())->GetWeaponNumProjectiles(g_player) < 2) return false;
		return true;
	}

	Float32 UpdateCurrentSpread(Float64 spreadTarget)
	{
		if (spreadCurrent > spreadTarget)
		{
			spreadCurrent -= 0.01 * g_Speed * (1 - 20 * (spreadTarget - spreadCurrent));
			if (spreadCurrent < spreadTarget) spreadCurrent = spreadTarget;
		}
		else if (spreadCurrent < spreadTarget)
		{
			spreadCurrent += 0.01 * g_Speed * (1 + 20 * (spreadTarget - spreadCurrent));
			if (spreadCurrent > spreadTarget) spreadCurrent = spreadTarget;
		}
		return spreadCurrent;
	}

	bool GetPCAiming()
	{
		return g_player->ironSightNode && g_player->baseProcess->IsWeaponOut() || g_NoNodeSighting && g_player->baseProcess->IsAiming();
	}

	void MainLoop()
	{
		if (!initialized) return;

		tileMain->SetFloat(tileMain->GetComponentValue("_JDCAlphaRC")->id, g_TileReticleCenter->GetChild("reticle_center")->GetFloat(kTileValue_alpha));

		if (true && g_TileReticleCenter->GetChild("reticle_center")->children.Head()) { // iHUDEditor
			tileMain->SetFloat(kTileValue_red, g_TileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetFloat(kTileValue_red));
			tileMain->SetFloat(kTileValue_blue, g_TileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetFloat(kTileValue_blue));
			tileMain->SetFloat(kTileValue_green, g_TileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetFloat(kTileValue_green));
		}

		tileMain->SetFloat(kTileValue_systemcolor, g_TileReticleCenter->GetChild("reticle_center")->GetFloat(kTileValue_systemcolor));

		UInt32 mode = 0;
		if (!IsPlayerWeaponGood())	mode = g_ModeHolstered;
		else if (GetPCUsingScope())	mode = g_ModeScope;
		else if (GetPCAiming())		mode = g_player->isInThirdPerson ? g_ModeSighting3rd : g_ModeSighting1st;
		else						mode = g_player->isInThirdPerson ? g_ModeOut3rd : g_ModeOut1st;


		tileMain->SetFloat(tileMain->GetComponentValue("_JDCScope")->id, GetPCUsingScope());

		UInt32 visibleReticle = 0;
		UInt32 visibleDot = 0;
		UInt32 visibleCrosshair = 0;

		if (mode == 0) {}
		else if (mode == 1) visibleReticle = 1;
		else if (mode == 2) visibleDot = 1;
		else if (mode == 3) visibleDot = 2;
		else if (mode == 4) visibleDot = 3;
		else if (mode == 5) visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;
		else if (mode == 6) { visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2; visibleDot = 1; }
		else if (mode == 7) { visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;	visibleDot = 2; }

		tileMain->SetFloat("_JDCVisible", visibleDot || visibleCrosshair);
		tileMain->SetFloat("_JDCVisibleDot", visibleDot);
		tileMain->SetFloat("_JDCVisibleReticle", visibleCrosshair);
		g_TileReticleCenter->GetChild("reticle_center")->SetFloat(kTileValue_visible, visibleReticle);

		Float64 totalSpread = 0.1;

		if (g_Dynamic)
		{
			const auto weaponSpread = g_player->GetCalculatedSpread(0);
			const auto playerSpread = g_player->GetCalculatedSpread(1);

			totalSpread = weaponSpread + 10 * playerSpread;

			if (totalSpread < 0) totalSpread = 0;
			if (totalSpread > 1.5) totalSpread = 1.5;
		}

		auto spreadTarget = g_Distance * tan(totalSpread);

		if (GetPCUsingIronSights())
		{
			Float64 fDefaultWorldFOV;
			GetNumericIniSetting("fDefaultWorldFOV:Display", &fDefaultWorldFOV);
			if (g_player->baseProcess && g_player->baseProcess->GetWeaponInfo() && g_player->baseProcess->GetWeaponInfo()->weapon)
				spreadTarget *= tan(0.5 * g_player->baseProcess->GetWeaponInfo()->weapon->sightFOV) / tan(0.5 * fDefaultWorldFOV);
		}

		tileMain->SetFloat("_JDCSpread", UpdateCurrentSpread(spreadTarget));
	}
}
