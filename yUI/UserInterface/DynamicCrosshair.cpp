#include <main.h>

#include <Menu.h>
#include <Setting.h>
#include <SimpleINILibrary.h>

namespace UserInterface::DynamicCrosshair
{
	bool		enable			= false;

	enum kFlags
	{
		kNothing = 0,
		kVanilla,
		kDotSmall,
		kDotBig,
		kReticle,
		kCrosshair,
		kCrosshairDotSmall,
		kCrosshairDotBig
	};

	Float32		distance		= 0;
	Float32		speed			= 0.25;

	Float32		lengthMax		= 72;
	Float32		lengthMin		= 24;
	Float32		width			= 8;
	Float32		offsetMin		= 0;
	Float32		offsetMax		= 256;

	UInt32		modeHolstered	= kVanilla;
	UInt32		modeOut1st		= kCrosshair;
	UInt32		modeOut3rd		= kCrosshair;
	UInt32		modeSighting1st	= kDotBig;
	UInt32		modeSighting3rd	= kDotBig;
	UInt32		modeScope		= kNothing;

	bool		noNodeSighting	= true;
	bool		shotgunAlt		= true;
	UInt32		dynamic			= 1;



	Tile*		tileMain			= nullptr;
	Tile*		tileReticleCenter	= nullptr;

	Float32		spreadCurrent		= 0;

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
		if (!shotgunAlt) return false;
		if (g_player->baseProcess->GetWeaponInfo()->GetWeaponNumProjectiles(g_player) < 2) return false;
		return true;
	}

	Float32 UpdateCurrentSpread(Float32 spreadTarget)
	{
		if (spreadCurrent > spreadTarget)
		{
			spreadCurrent -= 0.01 * speed * (1 - 20 * (spreadTarget - spreadCurrent));
			if (spreadCurrent < spreadTarget) spreadCurrent = spreadTarget;
		}
		else if (spreadCurrent < spreadTarget)
		{
			spreadCurrent += 0.01 * speed * (1 + 20 * (spreadTarget - spreadCurrent));
			if (spreadCurrent > spreadTarget) spreadCurrent = spreadTarget;
		}
		return spreadCurrent;
	}

	void MainLoop()
	{
		if (!enable || MenuMode()) return;

		tileMain->Set(tileMain->GetValue("_AlphaRC")->id, tileReticleCenter->GetChild("reticle_center")->Get(kTileValue_alpha));

		if (true && tileReticleCenter->GetChild("reticle_center")->children.Head()) { // TODO:: iHUDEditor
			tileMain->Set(kTileValue_red, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->Get(kTileValue_red));
			tileMain->Set(kTileValue_blue, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->Get(kTileValue_blue));
			tileMain->Set(kTileValue_green, tileReticleCenter->GetChild("reticle_center")->children.Head()->data->Get(kTileValue_green));
		}

		tileMain->Set(kTileValue_systemcolor, tileReticleCenter->GetChild("reticle_center")->Get(kTileValue_systemcolor));

		UInt32 mode;
		if (!IsPlayerWeaponGood())				mode = modeHolstered;
		else if (g_HUDMainMenu->isUsingScope)	mode = modeScope;
		else if (g_player->UsingIronSights())	mode = g_player->isInThirdPerson ? modeSighting3rd : modeSighting1st;
		else									mode = g_player->isInThirdPerson ? modeOut3rd : modeOut1st;

		tileMain->Set("_scope", g_HUDMainMenu->isUsingScope);

		UInt32 visibleReticle = 0;
		UInt32 visibleDot = 0;
		UInt32 visibleCrosshair = 0;

		if		(mode == kNothing) {}
		else if (mode == kVanilla) visibleReticle = 1;
		else if (mode == kDotSmall) visibleDot = 1;
		else if (mode == kDotBig) visibleDot = 2;
		else if (mode == kReticle) visibleDot = 3;
		else if (mode == kCrosshair) visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;
		else if (mode == kCrosshairDotSmall) { visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2; visibleDot = 1; }
		else if (mode == kCrosshairDotBig) { visibleCrosshair = !IsPlayerWeaponShotgun() ? 1 : 2;	visibleDot = 2; }

		tileMain->Set("_visible", visibleDot || visibleCrosshair);
		tileMain->Set("_VisibleDot", visibleDot);
		tileMain->Set("_VisibleReticle", visibleCrosshair);
		tileReticleCenter->GetChild("reticle_center")->Set(kTileValue_visible, visibleReticle);

		Float64 totalSpread = 0.1;

		if (dynamic)
		{
			const auto weaponSpread = g_player->GetCalculatedSpread(0);
			const auto playerSpread = g_player->GetCalculatedSpread(1);

			totalSpread = weaponSpread + 6.75 * playerSpread;

			if (totalSpread < 0) totalSpread = 0;
			if (totalSpread > 1.5) totalSpread = 1.5;
		}

		auto spreadTarget = distance * tan(totalSpread);

		if (g_player->UsingIronSights())
		{
			Float64 fDefaultWorldFOV = GetINISetting("fDefaultWorldFOV:Display")->GetAsFloat();
			spreadTarget *= g_player->worldFOV / fDefaultWorldFOV;
		}

		tileMain->Set("_Spread", UpdateCurrentSpread(spreadTarget));
	}

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable			= ini.GetOrCreate("General", "bDynamicCrosshair", true, "; enable 'Dynamic Crosshair' feature. If required files are not found this will do nothing.");
		dynamic			= ini.GetOrCreate("Dynamic Crosshair", "iDynamic", 1, nullptr);
		shotgunAlt		= ini.GetOrCreate("Dynamic Crosshair", "bShotgunAlt", 1, nullptr);
		noNodeSighting	= ini.GetOrCreate("Dynamic Crosshair", "bNoNodeSighting", 1, nullptr);
		modeHolstered	= ini.GetOrCreate("Dynamic Crosshair", "iModeHolstered", 1, nullptr);
		modeOut1st		= ini.GetOrCreate("Dynamic Crosshair", "iModeOut1st", 5, nullptr);
		modeOut3rd		= ini.GetOrCreate("Dynamic Crosshair", "iModeOut3rd", 5, nullptr);
		modeSighting1st	= ini.GetOrCreate("Dynamic Crosshair", "iModeSighting1st", 3, nullptr);
		modeSighting3rd	= ini.GetOrCreate("Dynamic Crosshair", "iModeSighting3rd", 3, nullptr);
		modeScope		= ini.GetOrCreate("Dynamic Crosshair", "iModeScope", 0, nullptr);
		distance		= ini.GetOrCreate("Dynamic Crosshair", "fDistance", 0.0, nullptr);
		speed			= ini.GetOrCreate("Dynamic Crosshair", "fSpeed", 0.25, nullptr);
		lengthMax		= ini.GetOrCreate("Dynamic Crosshair", "fLengthMax", 72.0, nullptr);
		lengthMin		= ini.GetOrCreate("Dynamic Crosshair", "fLengthMin", 24.0, nullptr);
		width			= ini.GetOrCreate("Dynamic Crosshair", "fWidth", 8.0, nullptr);
		offsetMax		= ini.GetOrCreate("Dynamic Crosshair", "fOffsetMax", 256.0, nullptr);
		offsetMin		= ini.GetOrCreate("Dynamic Crosshair", "fOffsetMin", 0.0, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			std::erase(mainLoop, MainLoop);
			return;
		}
		mainLoop.emplace_back(MainLoop);

		tileMain->Set("_DynamicOffset", dynamic & 1);
		tileMain->Set("_DynamicLength", dynamic & 2);

		tileMain->Set("_LengthMin", lengthMin);
		tileMain->Set("_LengthMax", lengthMax);

		tileMain->Set("_OffsetMin", offsetMin);
		tileMain->Set("_OffsetMax", offsetMax);

		tileMain->Set("_Width", width);

		tileMain->Set("_Visible", 0);
		tileReticleCenter->GetChild("reticle_center")->Set(kTileValue_visible, 1);

		if (distance == 0)
		{
			const Float64 worldFOV = GetINISetting("fDefaultWorldFOV:Display")->GetAsFloat();
			distance = 0.2;
			if (worldFOV <= 160) distance += 0.03 * (160 - worldFOV);
			if (worldFOV <= 100) distance += 0.02 * (100 - worldFOV);
			if (worldFOV <= 65)	distance += 0.05 * (65 - worldFOV);
		}
	}

	void MainLoopDoOnce()
	{
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\DynamicCrosshair.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JDC");
		}
		if (!tileMain) return;

		tileReticleCenter = g_HUDMainMenu->tileReticleCenter;

		RegisterEvent("JDC:Reset", 0, nullptr, 4);
		SetEventHandler("JDC:Reset", Reset);
		Reset();
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}