#include <main.h>

#include <TESForm.h>
#include <Menu.h>
#include <SimpleINILibrary.h>

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

		tile->Set("_Depth", depth++);

		tile->Set(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitMarkerAltColor));

		tile->Set("_ModeOffset", static_cast<bool>(flags & kHitMarkerOffset));
		tile->Set("_ModeShake", static_cast<bool>(flags & kHitMarkerShake));
		tile->Set("_ModeRotate", static_cast<bool>(flags & kHitMarkerRotate));

		tile->Set("_AlphaMult", flags & kHitMarkerHalfAlpha ? 0.5 : 1);

		tile->SetGradual("_counter", 0, 1, flags & kHitMarkerDouble ? 2 * seconds : seconds, GradualSetFloat::StartToEnd);
	}

	void MainLoop()
	{
		if (!enable) return;

		if (g_HUDMainMenu->isUsingScope) visible = static_cast<SInt64>(enableScope);
		else if (g_player->UsingIronSights()) visible = enableSighting;
		else visible = enableOut;
		tileMain->Set("_Visible", visible);
		tileMain->Set("_scope", g_player->UsingIronSights());

		if (MenuMode() || !visible) { hitMarkers.clear(); return; }

		for (auto i = tilesInUse.begin(); i != tilesInUse.end(); ) if (!ProcessTilesInUse(*i)) tilesInUse.erase(i++); else ++i;

		depth = 0;
		for (const auto& snd : hitMarkers | std::views::values) CreateHitMarker(snd);
		hitMarkers.clear();

		if (const auto tileJDC = g_HUDMainMenu->tile->GetChild("JDC"))
		{
			tileMain->Set("_JDCOffset", tileJDC->Get("_Offset"));
			tileMain->Set("_JDCLength", tileJDC->Get("_Length"));
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
		const auto iniPath = GetCurPath() / yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable			= ini.GetOrCreate("General", "bHitMarker", true, "; enable 'Hit Marker' feature. If required files are not found this will do nothing.");
		seconds			= ini.GetOrCreate("Hit Marker", "fSeconds", 0.5, nullptr);
		alpha			= ini.GetOrCreate("Hit Marker", "fAlpha", 400.0, nullptr);
		length			= ini.GetOrCreate("Hit Marker", "fLength", 24.0, nullptr);
		width			= ini.GetOrCreate("Hit Marker", "fWidth", 8.0, nullptr);
		offset			= ini.GetOrCreate("Hit Marker", "fOffset", 24.0, nullptr);
		modeHit			= ini.GetOrCreate("Hit Marker", "iModeHit",kHitMarkerNormal, nullptr);
		modeDead		= ini.GetOrCreate("Hit Marker", "iModeDead", kHitMarkerNormal, nullptr);
		modeKill		= ini.GetOrCreate("Hit Marker", "iModeKill", kHitMarkerOffset, nullptr);
		modeEnemy		= ini.GetOrCreate("Hit Marker", "iModeEnemy", kHitMarkerAltColor, nullptr);
		modeCrit		= ini.GetOrCreate("Hit Marker", "iModeCrit", kHitMarkerDouble, nullptr);
		modeHeadshot	= ini.GetOrCreate("Hit Marker", "iModeHead", kHitMarkerShake, nullptr);
		modeExplosion	= ini.GetOrCreate("Hit Marker", "iModeExplosion", kHitMarkerDouble, nullptr);
		modeCompanion	= ini.GetOrCreate("Hit Marker", "iModeByCompanion", kHitMarkerHalfAlpha, nullptr);
		enableOut		= ini.GetOrCreate("Hit Marker", "bEnableOut", true, nullptr);
		enableSighting	= ini.GetOrCreate("Hit Marker", "bEnableSighting", true, nullptr);
		enableScope		= ini.GetOrCreate("Hit Marker", "bEnableScope", true, nullptr);
		dynamic			= ini.GetOrCreate("Hit Marker", "iDynamic", 0, nullptr);
		maxTiles		= ini.GetOrCreate("Hit Marker", "iMaxTiles", 25, nullptr);

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

		if (tileMain->GetChild("JHM")) delete tileMain->GetChild("JHMContainer");
		tileMain->AddTileFromTemplate("JHMContainer");

		tileMain->Set("_AlphaBase", alpha);
		tileMain->Set("_LengthBase", length);
		tileMain->Set("_WidthBase", width);
		tileMain->Set("_OffsetBase", offset);

		tileMain->Set("_DynamicLength", dynamic & 1);
		tileMain->Set("_DynamicOffset", dynamic & 2);

		tileMain->SetGradual("_GlobalShaker", -0.05, 0.05, 0.15, GradualSetFloat::StartToEndPerpetual);

		tileMain->Set("_Visible", visible = 0);
	}

	void MainLoopDoOnce()
	{
		if (!tileMain)
		{
			tileMain = g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\HitMarker.xml)");

			if (!tileMain)
			{
				Log() << "HitMarker.xml was not detected despite 'Hit Marker' being enabled! 'Hit Marker' will not function.";
				return;
			}
			Log() << "'Hit Marker' module enabled";
		}
		RegisterEvent("JHM:Reset", 0, nullptr, 4);
		SetEventHandler("JHM:Reset", Reset);
		Reset();
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}
