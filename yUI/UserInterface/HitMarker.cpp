#include <main.h>

#include <Menus.h>
#include <SimpleINILibrary.h>

#include <unordered_set>
#include <ranges>

namespace UserInterface::HitMarker
{
	enum kHitMarkerFlags
	{
		kHitMarkerNothing	= 1 << 0,
		kHitMarkerNormal	= 1 << 1,
		kHitMarkerAltColor	= 1 << 2,
		kHitMarkerDouble	= 1 << 3,
		kHitMarkerHalfAlpha	= 1 << 4,
		kHitMarkerOffset	= 1 << 5,
		kHitMarkerShake		= 1 << 6,
		kHitMarkerRotate	= 1 << 7,
	};

	UInt32		enable			= 0;

	Float32		seconds			= 0.5;
	Float32		alpha			= 400.0;
	Float32		length			= 24.0;
	Float32		width			= 8.0;
	Float32		offset			= 24.0;
	UInt32		modeHit			= kHitMarkerNormal;
	UInt32		modeDead		= kHitMarkerNormal;
	UInt32		modeKill		= kHitMarkerOffset;
	UInt32		modeEnemy		= kHitMarkerAltColor;
	UInt32		modeCrit		= kHitMarkerDouble;
	UInt32		modeHeadshot	= kHitMarkerShake;
	UInt32		modeCompanion	= kHitMarkerHalfAlpha;
	UInt32		modeExplosion	= kHitMarkerDouble;
	UInt32		enableOut		= 1;
	UInt32		enableSighting	= 1;
	UInt32		enableScope		= 1;
	UInt32		dynamic			= 0;
	UInt32		maxTiles		= 25;

	Tile*		tileMain		= nullptr;

	SInt64		visible			= 0;
	UInt32		depth			= 0;

	std::map<TESObjectREFR*, UInt32>	hitMarkers;
	std::unordered_set<Tile*>			tilesFree;
	std::unordered_set<Tile*>			tilesInUse;

	bool ProcessTilesInUse(Tile* tile)
	{
		if (!tile) return false;
		const auto val = tile->GetValue("_counter");
		if (!val) return false;
		if (val->num < 1) return true;
		tilesFree.emplace(tile);
		return false;
	}

	Tile* CreateTileForHitMarker()
	{
		Tile* tile;
		if (tilesFree.empty()) {
			tile = tileMain->GetChild("JHMContainer")->AddTileFromTemplate("JHMMarker");
		}
		else {
			const auto iter = tilesFree.begin();
			tile = *iter;
			tilesFree.erase(iter);
		}
		tilesInUse.emplace(tile);
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

		tile->GradualSetFloat("_counter", 0, 1, flags & kHitMarkerDouble ? 2 * seconds : seconds, GradualSetFloat::StartToEnd);
	}

	void MainLoop()
	{
		if (!enable) return;

		if (g_HUDMainMenu->isUsingScope) visible = static_cast<SInt64>(enableScope);
		else if (g_player->UsingIronSights()) visible = enableSighting;
		else visible = enableOut;
		tileMain->SetFloat("_JHMVisible", visible);
		tileMain->SetFloat("_scope", g_player->UsingIronSights());

		if (!visible) { hitMarkers.clear(); return; }

		for (auto i = tilesInUse.begin(); i != tilesInUse.end(); ) if (!ProcessTilesInUse(*i)) tilesInUse.erase(i++); else ++i;

		depth = 0;
		for (const auto& snd : hitMarkers | std::views::values) CreateHitMarker(snd);
		hitMarkers.clear();

		if (const auto tileJDC = g_HUDMainMenu->tile->GetChild("JDC"))
		{
			tileMain->SetFloat("_JDCOffset", tileJDC->GetFloat("_JDCOffset"));
			tileMain->SetFloat("_JDCLength", tileJDC->GetFloat("_JDCLength"));
		}
	}

	void OnHit(Actor* target, void* args)
	{
		if (!enable || !visible) return;

		if (!target || target == g_player) return;

		UInt32 flags = 0;

		if (target->IsActor())
		{
			const auto hitData = target->baseProcess->GetLastHitData();
			if (!hitData || !hitData->source) return;
			if (hitData->source != g_player && !hitData->source->isTeammate) return;

			flags |= modeHit;
			if (target->lifeState == kLifeState_Dead || target->lifeState == kLifeState_Dying) flags |= modeDead;
			if (target->lifeState == kLifeState_Alive && hitData->flags & ActorHitData::kFlag_IsFatal) flags |= modeKill;
			if (hitData->flags & ActorHitData::kFlag_IsCritical) flags |= modeCrit;
			if (hitData->hitLocation == BGSBodyPartData::eBodyPart_Brain || hitData->hitLocation ==
				BGSBodyPartData::eBodyPart_Head1 || hitData->hitLocation == BGSBodyPartData::eBodyPart_Head2) flags |= modeHeadshot;
			if (target->IsCrimeOrEnemy()) flags |= modeEnemy;
			if (hitData->source->isTeammate) flags |= modeCompanion;
			if (hitData->explosion && hitData->explosion->IsExplosion()) flags |= modeExplosion;
		}

		if (flags & kHitMarkerNothing) return;

		hitMarkers[target] |= flags;
	}
	
	void HandleINI()
	{
		const auto iniPath = GetCurPath() + R"(\Data\Config\JustMods.ini)";
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable			= ini.GetOrCreate("JustMods", "JHM", 1.0, nullptr);
		seconds			= ini.GetOrCreate("JHM", "Seconds", 0.5, nullptr);
		alpha			= ini.GetOrCreate("JHM", "Alpha", 400.0, nullptr);
		length			= ini.GetOrCreate("JHM", "Length", 24.0, nullptr);
		width			= ini.GetOrCreate("JHM", "Width", 8.0, nullptr);
		offset			= ini.GetOrCreate("JHM", "Offset", 24.0, nullptr);
		modeHit			= ini.GetOrCreate("JHM", "ModeHit", static_cast<double>(kHitMarkerNormal), nullptr);
		modeDead		= ini.GetOrCreate("JHM", "ModeDead", static_cast<double>(kHitMarkerNormal), nullptr);
		modeKill		= ini.GetOrCreate("JHM", "ModeKill", static_cast<double>(kHitMarkerOffset), nullptr);
		modeEnemy		= ini.GetOrCreate("JHM", "ModeEnemy", static_cast<double>(kHitMarkerAltColor), nullptr);
		modeCrit		= ini.GetOrCreate("JHM", "ModeCrit", static_cast<double>(kHitMarkerDouble), nullptr);
		modeHeadshot	= ini.GetOrCreate("JHM", "ModeHead", static_cast<double>(kHitMarkerShake), nullptr);
		modeExplosion	= ini.GetOrCreate("JHM", "ModeExplosion", static_cast<double>(kHitMarkerDouble), nullptr);
		modeCompanion	= ini.GetOrCreate("JHM", "ModeByCompanion", static_cast<double>(kHitMarkerHalfAlpha), nullptr);
		enableOut		= ini.GetOrCreate("JHM", "EnableOut", 1.0, nullptr);
		enableSighting	= ini.GetOrCreate("JHM", "EnableSighting", 1.0, nullptr);
		enableScope		= ini.GetOrCreate("JHM", "EnableScope", 1.0, nullptr);
		dynamic			= ini.GetOrCreate("JHM", "Dynamic", 0.0, nullptr);
		maxTiles		= ini.GetOrCreate("JHM", "MaxTiles", 25.0, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			RemoveNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));
			return;
		}
		SetNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));

		if (tileMain->GetChild("JHM")) tileMain->GetChild("JHMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JHMContainer");

		tileMain->SetFloat("_JHMAlphaBase", alpha);
		tileMain->SetFloat("_JHMLengthBase", length);
		tileMain->SetFloat("_JHMWidthBase", width);
		tileMain->SetFloat("_JHMOffsetBase", offset);

		tileMain->SetFloat("_JHMJDCLength", dynamic & 1);
		tileMain->SetFloat("_JHMJDCOffset", dynamic & 2);

		tileMain->GradualSetFloat("_JHMGlobalShaker", -0.05, 0.05, 0.15, GradualSetFloat::StartToEndPerpetual);

		tileMain->SetFloat("_JHMVisible", visible = 0);
	}

	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JHM");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\HitMarker.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JHM");
		}
		if (!tileMain) return;
		RegisterEvent("JHM:Reset", 0, nullptr, 4);
		SetEventHandler("JHM:Reset", Reset);
		Reset();
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;

//		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
//		mainLoop.emplace_back(MainLoop);
	}
}
