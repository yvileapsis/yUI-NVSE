#include <JDC.h>

#include <GameTiles.h>
#include <GameUI.h>
#include <GameObjects.h>
#include <GameSettings.h>
#include <GameProcess.h>
#include <cmath>

#include <SimpleINILibrary.h>

#include "main.h"

namespace JDC
{
	HUDMainMenu*		menuHUDMain			= nullptr;
	Tile*				tileMain			= nullptr;
	Tile*				tileReticleCenter	= nullptr;
	PlayerCharacter*	player				= nullptr;

	Float64				spreadCurrent		= 0;

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		g_JDC					= ini.GetOrCreate("JustMods", "JDC", 1.0, nullptr);
		g_JDC_Dynamic			= ini.GetOrCreate("JDC", "Dynamic", 1.0, nullptr);
		g_JDC_ShotgunAlt		= ini.GetOrCreate("JDC", "ShotgunAlt", 1.0, nullptr);
		g_JDC_NoNodeSighting	= ini.GetOrCreate("JDC", "NoNodeSighting", 1.0, nullptr);
		g_JDC_ModeHolstered		= ini.GetOrCreate("JDC", "ModeHolstered", 1.0, nullptr);
		g_JDC_ModeOut1st		= ini.GetOrCreate("JDC", "ModeOut1st", 5.0, nullptr);
		g_JDC_ModeOut3rd		= ini.GetOrCreate("JDC", "ModeOut3rd", 5.0, nullptr);
		g_JDC_ModeSighting1st	= ini.GetOrCreate("JDC", "ModeSighting1st", 3.0, nullptr);
		g_JDC_ModeSighting3rd	= ini.GetOrCreate("JDC", "ModeSighting3rd", 3.0, nullptr);
		g_JDC_ModeScope			= ini.GetOrCreate("JDC", "ModeScope", 0.0, nullptr);
		g_JDC_Distance			= ini.GetOrCreate("JDC", "Distance", 2.25, nullptr);
		g_JDC_Speed				= ini.GetOrCreate("JDC", "Speed", 0.3, nullptr);
		g_JDC_LengthMax			= ini.GetOrCreate("JDC", "LengthMax", 72.0, nullptr);
		g_JDC_LengthMin			= ini.GetOrCreate("JDC", "LengthMin", 24.0, nullptr);
		g_JDC_Width				= ini.GetOrCreate("JDC", "Width", 8.0, nullptr);
		g_JDC_OffsetMax			= ini.GetOrCreate("JDC", "OffsetMax", 256.0, nullptr);
		g_JDC_OffsetMin			= ini.GetOrCreate("JDC", "OffsetMin", 0.0, nullptr);
		
		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset(TESObjectREFR* wah, void* par)
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		tileMain->SetFloat(Tile::TraitNameToID("_JDCDynamicOffset"), g_JDC_Dynamic & 1);
		tileMain->SetFloat(Tile::TraitNameToID("_JDCDynamicLength"), g_JDC_Dynamic & 2);

		tileMain->SetFloat(Tile::TraitNameToID("_JDCLengthMin"), g_JDC_LengthMin);
		tileMain->SetFloat(Tile::TraitNameToID("_JDCLengthMax"), g_JDC_LengthMax);

		tileMain->SetFloat(Tile::TraitNameToID("_JDCOffsetMin"), g_JDC_OffsetMin);
		tileMain->SetFloat(Tile::TraitNameToID("_JDCOffsetMax"), g_JDC_OffsetMax);

		tileMain->SetFloat(Tile::TraitNameToID("_JDCWidth"), g_JDC_Width);

		tileMain->SetFloat(Tile::TraitNameToID("_JDCVisible"), 0);
		tileReticleCenter->GetChild("reticle_center")->SetFloat(kTileValue_visible, 1);

		/*
	fWorldFOV = GetNumericINISetting "fDefaultWorldFOV:Display"
	fDefaultDistanceMult = 0.20

	if fWorldFOV <= 160
		fDefaultDistanceMult = fDefaultDistanceMult + 0.03 * (160 - fWorldFOV)
	endif
	if fWorldFOV <= 100
		fDefaultDistanceMult = fDefaultDistanceMult + 0.02 * (100 - fWorldFOV)
	endif
	if fWorldFOV <= 65
		fDefaultDistanceMult = fDefaultDistanceMult + 0.05 * (65 - fWorldFOV)
	endif
		 */
	}

	void Initialize()
	{
		menuHUDMain = HUDMainMenu::GetSingleton();
		tileMain = menuHUDMain->tile->GetChild("JDC");
		if (!tileMain)
		{
			menuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JDC\JDC.xml)");
			tileMain = menuHUDMain->tile->GetChild("JDC");
		}
		if (!tileMain) return;
		tileReticleCenter = menuHUDMain->tileReticleCenter;
		player = PlayerCharacter::GetSingleton();
		SetNativeEventHandler("JDC:Reset", reinterpret_cast<EventHandler>(Reset));
		DispatchEvent("JDC:Reset", nullptr);
	}

	bool IsPlayerWeaponGood()
	{
		const auto weaponInfo = player->baseProcess->GetWeaponInfo();

		if (!weaponInfo) return false;
		if (!player->baseProcess->IsWeaponOut()) return false;
		if (!weaponInfo->weapon->IsRangedWeapon()) return false;
		// TODO: exclusion list
		return true;
	}

	bool IsPlayerWeaponShotgun()
	{
		if (!g_JDC_ShotgunAlt) return false;

		const auto weaponInfo = player->baseProcess->GetWeaponInfo();

		if (reinterpret_cast<ContChangesEntry*>(weaponInfo)->GetWeaponNumProjectiles(player) < 2) return false;
		return true;
	}

	Float32 UpdateCurrentSpread(Float64 spreadTarget)
	{
		if (spreadCurrent > spreadTarget)
		{
			spreadCurrent -= 0.01 * g_JDC_Speed * (1 - 20 * (spreadTarget - spreadCurrent));
			if (spreadCurrent < spreadTarget) spreadCurrent = spreadTarget;
		}
		else if (spreadCurrent < spreadTarget)
		{
			spreadCurrent += 0.01 * g_JDC_Speed * (1 + 20 * (spreadTarget - spreadCurrent));
			if (spreadCurrent > spreadTarget) spreadCurrent = spreadTarget;
		}
		return spreadCurrent;
	}

	bool GetPCUsingIronSights()
	{
		return player->ironSightNode && player->baseProcess->IsWeaponOut() || g_JDC_NoNodeSighting && player->baseProcess->IsAiming();
	}

	bool GetPCUsingScope()
	{
		return menuHUDMain->isUsingScope;
	}

	void MainLoop()
	{
		if (!player) return;

		tileMain->SetFloat(tileMain->GetComponentValue("_JDCAlphaRC")->id, menuHUDMain->tileReticleCenter->GetChild("reticle_center")->GetValueFloat(kTileValue_alpha));

		if (true && menuHUDMain->tileReticleCenter->GetChild("reticle_center")->children.Head()) { // iHUDEditor
			tileMain->SetFloat(kTileValue_red, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetValueFloat(kTileValue_red));
			tileMain->SetFloat(kTileValue_blue, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetValueFloat(kTileValue_blue));
			tileMain->SetFloat(kTileValue_green, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetValueFloat(kTileValue_green));
		}

		tileMain->SetFloat(kTileValue_systemcolor, tileReticleCenter->GetChild("reticle_center")->GetValueFloat(kTileValue_systemcolor));

		UInt32 mode = 0;
		if (!IsPlayerWeaponGood())
			mode = g_JDC_ModeHolstered;
		else if (GetPCUsingScope())
			mode = g_JDC_ModeScope;
		else if (GetPCUsingIronSights())
			if (!PlayerCharacter::GetSingleton()->isInThirdPerson)
				mode = g_JDC_ModeSighting1st;
			else
				mode = g_JDC_ModeSighting3rd;
		else
			if (!PlayerCharacter::GetSingleton()->isInThirdPerson)
				mode = g_JDC_ModeOut1st;
			else
				mode = g_JDC_ModeOut3rd;


		tileMain->SetFloat(tileMain->GetComponentValue("_JDCScope")->id, GetPCUsingScope());

		UInt32 visibleReticle = 0;
		UInt32 visibleDot = 0;
		UInt32 visibleCrosshair = 0;

		if (mode == 0) {
		}
		else if (mode == 1) {
			visibleReticle = 1;
		}
		else if (mode == 2) {
			visibleDot = 1;
		}
		else if (mode == 3) {
			visibleDot = 2;
		}
		else if (mode == 4) {
			visibleDot = 3;
		}
		else if (mode == 5) {
			visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;
		}
		else if (mode == 6) {
			visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;
			visibleDot = 1;
		}
		else if (mode == 7) {
			visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;
			visibleDot = 2;
		}

		tileMain->SetFloat(Tile::TraitNameToID("_JDCVisible"), visibleDot || visibleCrosshair);
		tileMain->SetFloat(Tile::TraitNameToID("_JDCVisibleDot"), visibleDot);
		tileMain->SetFloat(Tile::TraitNameToID("_JDCVisibleReticle"), visibleCrosshair);
		tileReticleCenter->GetChild("reticle_center")->SetFloat(kTileValue_visible, visibleReticle);

		Float64 totalSpread = 0.1;

		if (g_JDC_Dynamic)
		{
			const auto weaponSpread = PlayerCharacter::GetSingleton()->GetCalculatedSpread(0);
			const auto playerSpread = PlayerCharacter::GetSingleton()->GetCalculatedSpread(1);

			totalSpread = weaponSpread + 10 * playerSpread;

			if (totalSpread < 0) totalSpread = 0;
			if (totalSpread > 1.5) totalSpread = 1.5;
		}

		auto spreadTarget = g_JDC_Distance * tan(totalSpread);

		if (GetPCUsingIronSights())
		{
			Float64 fDefaultWorldFOV;
			GetNumericIniSetting("fDefaultWorldFOV:Display", &fDefaultWorldFOV);
			if (player->baseProcess && player->baseProcess->GetWeaponInfo() && player->baseProcess->GetWeaponInfo()->weapon)
				spreadTarget *= tan(0.5 * player->baseProcess->GetWeaponInfo()->weapon->sightFOV) / tan(0.5 * fDefaultWorldFOV);
		}

		tileMain->SetFloat(Tile::TraitNameToID("_JDCSpread"), UpdateCurrentSpread(spreadTarget));

	}

}
