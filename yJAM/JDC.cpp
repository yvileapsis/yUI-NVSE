#include <JDC.h>

#include <GameTiles.h>
#include <GameUI.h>
#include <GameObjects.h>
#include <numbers>
#include <cmath>

#include "GameProcess.h"
#include "GameSettings.h"
#include "settings.h"

namespace JDC
{
	Float64				spreadCurrent	= 0;

	HUDMainMenu*		menuHUDMain		= nullptr;
	Tile*				tileJDC			= nullptr;
	PlayerCharacter*	player			= nullptr;

	void Initialize()
	{
		menuHUDMain = HUDMainMenu::GetSingleton();
		tileJDC = menuHUDMain->tile->GetChild("JDC");
		player = PlayerCharacter::GetSingleton();

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCDynamicOffset")->id, g_JDC_Dynamic & 1);
		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCDynamicLength")->id, g_JDC_Dynamic & 2);

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCLengthMin")->id, g_JDC_LengthMin);
		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCLengthMax")->id, g_JDC_LengthMax);

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCOffsetMin")->id, g_JDC_OffsetMin);
		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCOffsetMax")->id, g_JDC_OffsetMax);

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCWidth")->id, g_JDC_Width);

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

		const auto tileReticleCenter = menuHUDMain->tileReticleCenter;

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCAlphaRC")->id, menuHUDMain->tileReticleCenter->GetChild("reticle_center")->GetValueFloat(kTileValue_alpha));

		if (1 && menuHUDMain->tileReticleCenter->GetChild("reticle_center")->children.Head()) { // iHUDEditor
			tileJDC->SetFloat(kTileValue_red, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetValueFloat(kTileValue_red));
			tileJDC->SetFloat(kTileValue_blue, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetValueFloat(kTileValue_blue));
			tileJDC->SetFloat(kTileValue_green, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->GetValueFloat(kTileValue_green));
		}

		tileJDC->SetFloat(kTileValue_systemcolor, tileReticleCenter->GetChild("reticle_center")->GetValueFloat(kTileValue_systemcolor));

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


		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCScope")->id, GetPCUsingScope());

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

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCVisible")->id, visibleDot || visibleCrosshair);
		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCVisibleDot")->id, visibleDot);
		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCVisibleReticle")->id, visibleCrosshair);
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

		tileJDC->SetFloat(tileJDC->GetComponentValue("_JDCSpread")->id, UpdateCurrentSpread(spreadTarget));

	}

}
