#include <JHM.h>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace JHM
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	SInt64				visible				= 0;
	UInt32				depth				= 0;

	std::map<TESObjectREFR*, UInt32>	g_HitMarkers;
	std::unordered_set<Tile*>			g_TilesFree;
	std::unordered_set<Tile*>			g_TilesInUse;

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
		SetEventHandler("JHM:Reset", Reset);
		DispatchEvent("JHM:Reset", nullptr);
	}

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		g_JHM				= ini.GetOrCreate("JustMods", "JHM", 1.0, nullptr);
		g_Seconds			= ini.GetOrCreate("JHM", "Seconds", 0.5, nullptr);
		g_Alpha				= ini.GetOrCreate("JHM", "Alpha", 400.0, nullptr);
		g_Length			= ini.GetOrCreate("JHM", "Length", 24.0, nullptr);
		g_Width				= ini.GetOrCreate("JHM", "Width", 8.0, nullptr);
		g_Offset			= ini.GetOrCreate("JHM", "Offset", 24.0, nullptr);
		g_ModeHit			= ini.GetOrCreate("JHM", "ModeHit", static_cast<double>(kHitMarkerNormal), nullptr);
		g_ModeDead			= ini.GetOrCreate("JHM", "ModeDead", static_cast<double>(kHitMarkerNormal), nullptr);
		g_ModeKill			= ini.GetOrCreate("JHM", "ModeKill", static_cast<double>(kHitMarkerOffset), nullptr);
		g_ModeEnemy			= ini.GetOrCreate("JHM", "ModeEnemy", static_cast<double>(kHitMarkerAltColor), nullptr);
		g_ModeCrit			= ini.GetOrCreate("JHM", "ModeCrit", static_cast<double>(kHitMarkerDouble), nullptr);
		g_ModeHeadshot		= ini.GetOrCreate("JHM", "ModeHead", static_cast<double>(kHitMarkerShake), nullptr);
		g_ModeExplosion		= ini.GetOrCreate("JHM", "ModeExplosion", static_cast<double>(kHitMarkerDouble), nullptr);
		g_ModeCompanion		= ini.GetOrCreate("JHM", "ModeByCompanion", static_cast<double>(kHitMarkerHalfAlpha), nullptr);
		g_EnableOut			= ini.GetOrCreate("JHM", "EnableOut", 1.0, nullptr);
		g_EnableSighting	= ini.GetOrCreate("JHM", "EnableSighting", 1.0, nullptr);
		g_EnableScope		= ini.GetOrCreate("JHM", "EnableScope", 1.0, nullptr);
		g_Dynamic			= ini.GetOrCreate("JHM", "Dynamic", 0.0, nullptr);
		g_MaxTiles			= ini.GetOrCreate("JHM", "MaxTiles", 25.0, nullptr);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (!g_JHM)
		{
			RemoveNativeEventHandler("yJAM:MainLoop", reinterpret_cast<EventHandler>(MainLoop));
			RemoveNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));
			return;
		}

		SetNativeEventHandler("yJAM:MainLoop", reinterpret_cast<EventHandler>(MainLoop));
		SetNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));

		if (tileMain->GetChild("JHM")) tileMain->GetChild("JHMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JHMContainer");

		tileMain->SetFloat("_JHMAlphaBase", g_Alpha);
		tileMain->SetFloat("_JHMLengthBase", g_Length);
		tileMain->SetFloat("_JHMWidthBase", g_Width);
		tileMain->SetFloat("_JHMOffsetBase", g_Offset);

		tileMain->SetFloat("_JHMJDCLength", g_Dynamic & 1);
		tileMain->SetFloat("_JHMJDCOffset", g_Dynamic & 2);

		tileMain->GradualSetFloat("_JHMGlobalShaker", -0.05, 0.05, 0.15, GradualSetFloat::StartToEndPerpetual);

		tileMain->SetFloat("_JHMVisible", visible = 0);
	}

	bool ProcessTilesInUse(Tile* tile)
	{
		if (!tile) return false;
		const auto val = tile->GetValue("_counter");
		if (!val) return false;
		if (val->num < 1) return true;
		g_TilesFree.emplace(tile);
		return false;
	}

	Tile* CreateTileForHitMarker()
	{
		Tile* tile;
		if (g_TilesFree.empty()) {
			tile = tileMain->GetChild("JHMContainer")->AddTileFromTemplate("JHMMarker");
		}
		else {
			const auto iter = g_TilesFree.begin();
			tile = *iter;
			g_TilesFree.erase(iter);
		}
		g_TilesInUse.emplace(tile);
		return tile;
	}

	void CreateHitMarker(UInt32 flags)
	{
		const auto tile = CreateTileForHitMarker();

		tile->SetFloat("_JHMDepth", depth++);

		tile->SetFloat(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitMarkerAltColor));

		tile->SetFloat("_JHMModeOffset", static_cast<bool>(flags & kHitMarkerOffset));
		tile->SetFloat("_JHMModeShake", static_cast<bool>(flags & kHitMarkerShake));
		tile->SetFloat("_JHMModeRotate", static_cast<bool>(flags & kHitMarkerRotate));

		tile->SetFloat("_JHMAlphaMult", flags & kHitMarkerHalfAlpha ? 0.5 : 1);

		tile->GradualSetFloat("_counter", 0, 1, flags & kHitMarkerDouble ? 2 * g_Seconds : g_Seconds, GradualSetFloat::StartToEnd);
	}

	void MainLoop()
	{
		if (!initialized) return;

		if (GetPCUsingScope()) visible = static_cast<SInt64>(g_EnableScope);
		else if (GetPCUsingIronSights()) visible = g_EnableSighting;
		else visible = g_EnableOut;
		tileMain->SetFloat("_JHMVisible", visible);
		tileMain->SetFloat("_scope", GetPCUsingScope());

		if (!visible) { g_HitMarkers.clear(); return; }

		for (auto i = g_TilesInUse.begin(); i != g_TilesInUse.end(); ) if (!ProcessTilesInUse(*i)) g_TilesInUse.erase(i++); else ++i;

		depth = 0;
		for (const auto& snd : g_HitMarkers | std::views::values) CreateHitMarker(snd);
		g_HitMarkers.clear();

		if (const auto tileJDC = g_MenuHUDMain->tile->GetChild("JDC"))
		{
			tileMain->SetFloat("_JDCOffset", tileJDC->GetFloat("_JDCOffset"));
			tileMain->SetFloat("_JDCLength", tileJDC->GetFloat("_JDCLength"));
		}
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

			flags |= g_ModeHit;
			if (target->lifeState == kLifeState_Dead || target->lifeState == kLifeState_Dying) flags |= g_ModeDead;
			if (target->lifeState == kLifeState_Alive && hitData->flags & ActorHitData::kFlag_IsFatal) flags |= g_ModeKill;
			if (hitData->flags & ActorHitData::kFlag_IsCritical) flags |= g_ModeCrit;
			if (hitData->hitLocation == BGSBodyPartData::eBodyPart_Brain || hitData->hitLocation ==
				BGSBodyPartData::eBodyPart_Head1 || hitData->hitLocation == BGSBodyPartData::eBodyPart_Head2) flags |= g_ModeHeadshot;
			if (target->IsCrimeOrEnemy()) flags |= g_ModeEnemy;
			if (hitData->source->isTeammate) flags |= g_ModeCompanion;
			if (hitData->explosion && hitData->explosion->IsExplosion()) flags |= g_ModeExplosion;
		}

		if (flags & kHitMarkerNothing) return;

		g_HitMarkers[target] |= flags;

	}
}
