#include <main.h>

#include <Menus.h>
#include <SimpleINILibrary.h>

#include <unordered_set>
#include <ranges>

namespace UserInterface::HitMarker
{
	bool		enable			= false;

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
	bool		enableOut		= true;
	bool		enableSighting	= true;
	bool		enableScope		= true;
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
		if (tilesFree.empty()) tile = tileMain->GetChild("JHMContainer")->AddTileFromTemplate("JHMMarker");
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

		tile->SetFloat("_Depth", depth++);

		tile->SetFloat(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitMarkerAltColor));

		tile->SetFloat("_ModeOffset", static_cast<bool>(flags & kHitMarkerOffset));
		tile->SetFloat("_ModeShake", static_cast<bool>(flags & kHitMarkerShake));
		tile->SetFloat("_ModeRotate", static_cast<bool>(flags & kHitMarkerRotate));

		tile->SetFloat("_AlphaMult", flags & kHitMarkerHalfAlpha ? 0.5 : 1);

		tile->GradualSetFloat("_counter", 0, 1, flags & kHitMarkerDouble ? 2 * seconds : seconds, GradualSetFloat::StartToEnd);
	}

	void MainLoop()
	{
		if (!enable) return;

		if (g_HUDMainMenu->isUsingScope) visible = static_cast<SInt64>(enableScope);
		else if (g_player->UsingIronSights()) visible = enableSighting;
		else visible = enableOut;
		tileMain->SetFloat("_Visible", visible);
		tileMain->SetFloat("_scope", g_player->UsingIronSights());

		if (!visible) { hitMarkers.clear(); return; }

		for (auto i = tilesInUse.begin(); i != tilesInUse.end(); ) if (!ProcessTilesInUse(*i)) tilesInUse.erase(i++); else ++i;

		depth = 0;
		for (const auto& snd : hitMarkers | std::views::values) CreateHitMarker(snd);
		hitMarkers.clear();

		if (const auto tileJDC = g_HUDMainMenu->tile->GetChild("JDC"))
		{
			tileMain->SetFloat("_JDCOffset", tileJDC->GetFloat("_Offset"));
			tileMain->SetFloat("_JDCLength", tileJDC->GetFloat("_Length"));
		}
	}

	void OnHit(Actor* target)
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
			// TODO:: remove vtable check, receiving trash (even without projectile fix)
			if (hitData->explosion && IS_TYPE(hitData->explosion, Explosion) && hitData->explosion->IsExplosion()) flags |= modeExplosion;
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

		enable			= ini.GetOrCreate("JustMods", "bHitMarker", true, nullptr);
		seconds			= ini.GetOrCreate("JHM", "fSeconds", 0.5, nullptr);
		alpha			= ini.GetOrCreate("JHM", "fAlpha", 400.0, nullptr);
		length			= ini.GetOrCreate("JHM", "fLength", 24.0, nullptr);
		width			= ini.GetOrCreate("JHM", "fWidth", 8.0, nullptr);
		offset			= ini.GetOrCreate("JHM", "fOffset", 24.0, nullptr);
		modeHit			= ini.GetOrCreate("JHM", "iModeHit",kHitMarkerNormal, nullptr);
		modeDead		= ini.GetOrCreate("JHM", "iModeDead", kHitMarkerNormal, nullptr);
		modeKill		= ini.GetOrCreate("JHM", "iModeKill", kHitMarkerOffset, nullptr);
		modeEnemy		= ini.GetOrCreate("JHM", "iModeEnemy", kHitMarkerAltColor, nullptr);
		modeCrit		= ini.GetOrCreate("JHM", "iModeCrit", kHitMarkerDouble, nullptr);
		modeHeadshot	= ini.GetOrCreate("JHM", "iModeHead", kHitMarkerShake, nullptr);
		modeExplosion	= ini.GetOrCreate("JHM", "iModeExplosion", kHitMarkerDouble, nullptr);
		modeCompanion	= ini.GetOrCreate("JHM", "iModeByCompanion", kHitMarkerHalfAlpha, nullptr);
		enableOut		= ini.GetOrCreate("JHM", "bEnableOut", true, nullptr);
		enableSighting	= ini.GetOrCreate("JHM", "bEnableSighting", true, nullptr);
		enableScope		= ini.GetOrCreate("JHM", "bEnableScope", true, nullptr);
		dynamic			= ini.GetOrCreate("JHM", "iDynamic", 0, nullptr);
		maxTiles		= ini.GetOrCreate("JHM", "iMaxTiles", 25, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			std::erase(onHit, OnHit);
			std::erase(mainLoop, MainLoop);
			return;
		}
		onHit.emplace_back(OnHit);
		mainLoop.emplace_back(MainLoop);

		if (tileMain->GetChild("JHM")) tileMain->GetChild("JHMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JHMContainer");

		tileMain->SetFloat("_AlphaBase", alpha);
		tileMain->SetFloat("_LengthBase", length);
		tileMain->SetFloat("_WidthBase", width);
		tileMain->SetFloat("_OffsetBase", offset);

		tileMain->SetFloat("_DynamicLength", dynamic & 1);
		tileMain->SetFloat("_DynamicOffset", dynamic & 2);

		tileMain->GradualSetFloat("_GlobalShaker", -0.05, 0.05, 0.15, GradualSetFloat::StartToEndPerpetual);

		tileMain->SetFloat("_Visible", visible = 0);
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
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
	}
}
