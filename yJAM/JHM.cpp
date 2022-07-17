#include <JHM.h>

#include <GameTiles.h>
#include <GameUI.h>
#include <GameObjects.h>
#include <GameSettings.h>
#include <GameProcess.h>
#include <ranges>

#include <SimpleINILibrary.h>

namespace JHM
{
	HUDMainMenu*		menuHUDMain			= nullptr;
	Tile*				tileMain			= nullptr;
	Tile*				tileReticleCenter	= nullptr;
	PlayerCharacter*	player				= nullptr;

	SInt64				visible				= 0;
	UInt32				depth				= 0;

	std::map<TESObjectREFR*, UInt32> g_HitMarkers;
	std::map<Tile*, bool (*)(Tile* tile)> g_TileCallbacks;

	enum kHitMarkerFlags
	{
		kHitMarkerNothing			= 1 << 0,
		kHitMarkerNormal			= 1 << 1,
		kHitMarkerDouble			= 1 << 2,
		kHitMarkerOffset			= 1 << 3,
		kHitMarkerShake				= 1 << 4,
		kHitMarkerRotate			= 1 << 5,
		kHitMarkerAltColor			= 1 << 6,
		kHitMarkerHalfAlpha			= 1 << 7
	};

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		g_JHM					= ini.GetOrCreate("JustMods", "JHM", 1.0, nullptr);
		g_JHM_Seconds			= ini.GetOrCreate("JHM", "Seconds", 0.5, nullptr);
		g_JHM_Alpha				= ini.GetOrCreate("JHM", "Alpha", 400.0, nullptr);
		g_JHM_Length			= ini.GetOrCreate("JHM", "Length", 24.0, nullptr);
		g_JHM_Width				= ini.GetOrCreate("JHM", "Width", 8.0, nullptr);
		g_JHM_Offset			= ini.GetOrCreate("JHM", "Offset", 24.0, nullptr);
		g_JHM_ModeHit			= ini.GetOrCreate("JHM", "ModeHit", kHitMarkerNormal, nullptr);
		g_JHM_ModeDead			= ini.GetOrCreate("JHM", "ModeDead", kHitMarkerNothing, nullptr);
		g_JHM_ModeKill			= ini.GetOrCreate("JHM", "ModeKill", kHitMarkerOffset, nullptr);
		g_JHM_ModeEnemy			= ini.GetOrCreate("JHM", "ModeEnemy", kHitMarkerAltColor, nullptr);
		g_JHM_ModeCrit			= ini.GetOrCreate("JHM", "ModeCrit", kHitMarkerDouble, nullptr);
		g_JHM_ModeHeadshot		= ini.GetOrCreate("JHM", "ModeHead", kHitMarkerShake, nullptr);
		g_JHM_ModeCompanion		= ini.GetOrCreate("JHM", "ModeByCompanion", kHitMarkerHalfAlpha, nullptr);
		g_JHM_EnableOut			= ini.GetOrCreate("JHM", "EnableOut", 1.0, nullptr);
		g_JHM_EnableSighting	= ini.GetOrCreate("JHM", "EnableSighting", 1.0, nullptr);
		g_JHM_EnableScope		= ini.GetOrCreate("JHM", "EnableScope", 1.0, nullptr);
		g_JHM_Dynamic			= ini.GetOrCreate("JHM", "Dynamic", 0.0, nullptr);
		g_JHM_MaxTiles			= ini.GetOrCreate("JHM", "MaxTiles", 25.0, nullptr);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	bool DeleteIfAlphaNegative(Tile* tile)
	{
		if (!tile || !tile->GetComponentValue("alpha")) return false;
		if (tile->GetValueFloat(kTileValue_alpha) < 0)
		{
			tile->Destroy(true);
			return false;
		}
		return true;
	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (tileMain->GetChild("JHM")) tileMain->GetChild("JHM")->Destroy(true);
		tileMain->AddTileFromTemplate("JHMTemp");

		tileMain->SetFloat(Tile::TraitNameToID("_JHMAlphaBase"), g_JHM_Alpha);
		tileMain->SetFloat(Tile::TraitNameToID("_JHMLengthBase"), g_JHM_Length);
		tileMain->SetFloat(Tile::TraitNameToID("_JHMWidthBase"), g_JHM_Width);
		tileMain->SetFloat(Tile::TraitNameToID("_JHMOffsetBase"), g_JHM_Offset);

		tileMain->SetFloat(Tile::TraitNameToID("_JHMJDCLength"), g_JHM_Dynamic & 1);
		tileMain->SetFloat(Tile::TraitNameToID("_JHMJDCOffset"), g_JHM_Dynamic & 2);

		tileMain->GradualSetFloat(Tile::TraitNameToID("_JHMGlobalShaker"), -0.05, 0.05, 0.15, GradualSetFloat::kGradualSetFloat_StartToEndPerpetual);

		tileMain->SetFloat(Tile::TraitNameToID("_JHMVisible"), visible = 0);
	}

	void Initialize()
	{
		menuHUDMain = HUDMainMenu::GetSingleton();
		tileMain = menuHUDMain->tile->GetChild("JHM");
		if (!tileMain)
		{
			menuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JHM\JHM.xml)");
			tileMain = menuHUDMain->tile->GetChild("JHM");
		}
		if (!tileMain) return;
		tileReticleCenter = menuHUDMain->tileReticleCenter;
		player = PlayerCharacter::GetSingleton();

		SetNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHitHandler));
		SetNativeEventHandler("JHM:Reset", reinterpret_cast<EventHandler>(Reset));
		DispatchEvent("JHM:Reset", nullptr);
	}

	void CreateHitMarker(UInt32 flags)
	{
		const auto tile = tileMain->GetChild("JHM")->AddTileFromTemplate("JHMInjected");

		tile->SetFloat(Tile::TraitNameToID("_JHMDepth"), depth++);

		tile->SetFloat(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitMarkerAltColor));

		tile->SetFloat(Tile::TraitNameToID("_JHMModeOffset"), static_cast<bool>(flags & kHitMarkerOffset));
		tile->SetFloat(Tile::TraitNameToID("_JHMModeShake"), static_cast<bool>(flags & kHitMarkerShake));
		tile->SetFloat(Tile::TraitNameToID("_JHMModeRotate"), static_cast<bool>(flags & kHitMarkerRotate));

		if (flags & kHitMarkerHalfAlpha) tile->SetFloat(Tile::TraitNameToID("_JHMAlphaMult"), 0.5);

		tile->GradualSetFloat(Tile::TraitNameToID("_JHMCounter"), 0, 1, (flags & kHitMarkerDouble) ? 2 * g_JHM_Seconds : g_JHM_Seconds, GradualSetFloat::kGradualSetFloat_StartToEnd);

		g_TileCallbacks[tile] = DeleteIfAlphaNegative;
	}

	__forceinline bool GetPCUsingIronSights()
	{
		return player->ironSightNode && player->baseProcess->IsWeaponOut() || player->baseProcess->IsAiming();
	}

	__forceinline bool GetPCUsingScope()
	{
		return menuHUDMain->isUsingScope;
	}

	void MainLoop()
	{
		if (!player) return;

		if (GetPCUsingScope()) visible = -1 * static_cast<SInt64>(g_JHM_EnableScope);
		else if (GetPCUsingIronSights()) visible = g_JHM_EnableSighting;
		else visible = g_JHM_EnableOut;
		tileMain->SetFloat(Tile::TraitNameToID("_JHMVisible"), visible);

		if (!visible)
		{
			g_HitMarkers.clear();
			g_TileCallbacks.clear();
			return;
		}
		depth = 0;
		for (const auto& snd : g_HitMarkers | std::views::values) CreateHitMarker(snd);
		g_HitMarkers.clear();
		for (auto i = g_TileCallbacks.begin(); i != g_TileCallbacks.end(); ) if (!i->second(i->first)) g_TileCallbacks.erase(i++); else ++i;

		tileMain->SetFloat(Tile::TraitNameToID("_JDCOffset"), menuHUDMain->tile->GetChild("JDC")->GetValueFloat(Tile::TraitNameToID("_JDCOffset")));
		tileMain->SetFloat(Tile::TraitNameToID("_JDCLength"), menuHUDMain->tile->GetChild("JDC")->GetValueFloat(Tile::TraitNameToID("_JDCLength")));
	}

	void OnHitHandler(Actor* target, void* args)
	{
		if (!player || !visible) return;

		if (!target || target == player) return;

		UInt32 flags = 0;

		if (target->IsActor())
		{
			const auto hitData = target->baseProcess->GetLastHitData();
			if (!hitData) return;
			if (hitData->source != player && !hitData->source->isTeammate) return;

			flags |= g_JHM_ModeHit;
			if (target->lifeState == kLifeState_Dead || target->lifeState == kLifeState_Dying) flags |= g_JHM_ModeDead;
			if (target->lifeState == kLifeState_Alive && hitData->flags & ActorHitData::kFlag_IsFatal) flags |= g_JHM_ModeKill;
			if (hitData->flags & ActorHitData::kFlag_IsCritical) flags |= g_JHM_ModeCrit;
			if (hitData->hitLocation == BGSBodyPartData::eBodyPart_Brain || hitData->hitLocation ==
				BGSBodyPartData::eBodyPart_Head1 || hitData->hitLocation == BGSBodyPartData::eBodyPart_Head2) flags |= g_JHM_ModeHeadshot;
			if (target->IsCrimeOrEnemy()) flags |= g_JHM_ModeEnemy;
			if (hitData->source->isTeammate) flags |= g_JHM_ModeCompanion;
		}

		if (flags & kHitMarkerNothing) return;

		g_HitMarkers[target] |= flags;

	}
}
