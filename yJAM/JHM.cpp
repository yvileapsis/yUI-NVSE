#include <JHM.h>

#include <GameProcess.h>
#include <GameUI.h>
#include <functions.h>

#include <ranges>
#include <unordered_set>

#include <SimpleINILibrary.h>


namespace JHM
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	SInt64				visible				= 0;
	UInt32				depth				= 0;

	std::map<TESObjectREFR*, UInt32>	g_HitMarkers;
	std::unordered_set<Tile*>			g_UsefulTiles;
	std::unordered_set<Tile*>			g_UselessTiles;

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
		g_JHM_ModeHit			= ini.GetOrCreate("JHM", "ModeHit", static_cast<double>(kHitMarkerNormal), nullptr);
		g_JHM_ModeDead			= ini.GetOrCreate("JHM", "ModeDead", static_cast<double>(kHitMarkerNormal), nullptr);
		g_JHM_ModeKill			= ini.GetOrCreate("JHM", "ModeKill", static_cast<double>(kHitMarkerOffset), nullptr);
		g_JHM_ModeEnemy			= ini.GetOrCreate("JHM", "ModeEnemy", static_cast<double>(kHitMarkerAltColor), nullptr);
		g_JHM_ModeCrit			= ini.GetOrCreate("JHM", "ModeCrit", static_cast<double>(kHitMarkerDouble), nullptr);
		g_JHM_ModeHeadshot		= ini.GetOrCreate("JHM", "ModeHead", static_cast<double>(kHitMarkerShake), nullptr);
		g_JHM_ModeCompanion		= ini.GetOrCreate("JHM", "ModeByCompanion", static_cast<double>(kHitMarkerHalfAlpha), nullptr);
		g_JHM_EnableOut			= ini.GetOrCreate("JHM", "EnableOut", 1.0, nullptr);
		g_JHM_EnableSighting	= ini.GetOrCreate("JHM", "EnableSighting", 1.0, nullptr);
		g_JHM_EnableScope		= ini.GetOrCreate("JHM", "EnableScope", 1.0, nullptr);
		g_JHM_Dynamic			= ini.GetOrCreate("JHM", "Dynamic", 0.0, nullptr);
		g_JHM_MaxTiles			= ini.GetOrCreate("JHM", "MaxTiles", 25.0, nullptr);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (tileMain->GetChild("JHM")) tileMain->GetChild("JHMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JHMContainer");

		tileMain->SetFloat(TraitNameToID("_JHMAlphaBase"), g_JHM_Alpha);
		tileMain->SetFloat(TraitNameToID("_JHMLengthBase"), g_JHM_Length);
		tileMain->SetFloat(TraitNameToID("_JHMWidthBase"), g_JHM_Width);
		tileMain->SetFloat(TraitNameToID("_JHMOffsetBase"), g_JHM_Offset);

		tileMain->SetFloat(TraitNameToID("_JHMJDCLength"), g_JHM_Dynamic & 1);
		tileMain->SetFloat(TraitNameToID("_JHMJDCOffset"), g_JHM_Dynamic & 2);

		tileMain->GradualSetFloat(TraitNameToID("_JHMGlobalShaker"), -0.05, 0.05, 0.15, GradualSetFloat::kGradualSetFloat_StartToEndPerpetual);

		tileMain->SetFloat(TraitNameToID("_JHMVisible"), visible = 0);
	}

	void Initialize()
	{
		initialized = true;
		tileMain = g_MenuHUDMain->tile->GetChild("JHM");
		if (!tileMain)
		{
			g_MenuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JHM\JHM.xml)");
			tileMain = g_MenuHUDMain->tile->GetChild("JHM");
		}
		if (!tileMain) return;
		RegisterEvent("JHM:Reset", 0, nullptr, 4);
		SetNativeEventHandler("JHM:Reset", reinterpret_cast<EventHandler>(Reset));
		DispatchEvent("JHM:Reset", nullptr);

		SetNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));
	}

	bool ProcessUselessTiles(Tile* tile)
	{
		if (!tile) return false;
		const auto val = tile->GetValue(kTileValue_alpha);
		if (!val) return false;
		if (val->num > 0) return true;
		g_UsefulTiles.emplace(tile);
		return false;
	}

	Tile* CreateTileForHitMarker()
	{
		Tile* tile;
		if (g_UsefulTiles.empty()) {
			tile = tileMain->GetChild("JHMContainer")->AddTileFromTemplate("JHMMarker");
		}
		else {
			const auto iter = g_UsefulTiles.begin();
			tile = *iter;
			g_UsefulTiles.erase(iter);
		}
		g_UselessTiles.emplace(tile);
		return tile;
	}

	void CreateHitMarker(UInt32 flags)
	{
		const auto tile = CreateTileForHitMarker();

		tile->SetFloat(TraitNameToID("_JHMDepth"), depth++);

		tile->SetFloat(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitMarkerAltColor));

		tile->SetFloat(TraitNameToID("_JHMModeOffset"), static_cast<bool>(flags & kHitMarkerOffset));
		tile->SetFloat(TraitNameToID("_JHMModeShake"), static_cast<bool>(flags & kHitMarkerShake));
		tile->SetFloat(TraitNameToID("_JHMModeRotate"), static_cast<bool>(flags & kHitMarkerRotate));

		if (flags & kHitMarkerHalfAlpha) tile->SetFloat(TraitNameToID("_JHMAlphaMult"), 0.5);

		tile->GradualSetFloat(TraitNameToID("_JHMCounter"), 0, 1, (flags & kHitMarkerDouble) ? 2 * g_JHM_Seconds : g_JHM_Seconds, GradualSetFloat::kGradualSetFloat_StartToEnd);
	}

	void MainLoop()
	{
		if (!initialized) return;

		if (GetPCUsingScope()) visible = -1 * static_cast<SInt64>(g_JHM_EnableScope);
		else if (GetPCUsingIronSights()) visible = g_JHM_EnableSighting;
		else visible = g_JHM_EnableOut;
		tileMain->SetFloat(TraitNameToID("_JHMVisible"), visible);

		if (!visible) { g_HitMarkers.clear(); return; }

		depth = 0;
		for (const auto& snd : g_HitMarkers | std::views::values) CreateHitMarker(snd);
		g_HitMarkers.clear();

		for (auto i = g_UselessTiles.begin(); i != g_UselessTiles.end(); ) if (!ProcessUselessTiles(*i)) g_UselessTiles.erase(i++); else ++i;

		tileMain->SetFloat(TraitNameToID("_JDCOffset"), g_MenuHUDMain->tile->GetChild("JDC")->GetValueFloat(TraitNameToID("_JDCOffset")));
		tileMain->SetFloat(TraitNameToID("_JDCLength"), g_MenuHUDMain->tile->GetChild("JDC")->GetValueFloat(TraitNameToID("_JDCLength")));
	}

	void OnHit(Actor* target, void* args)
	{
		if (!initialized || !visible) return;

		if (!target || target == g_player) return;

		UInt32 flags = 0;

		if (target->IsActor())
		{
			const auto hitData = target->baseProcess->GetLastHitData();
			if (!hitData || !hitData->source) return;
			if (hitData->source != g_player && !hitData->source->isTeammate) return;

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
