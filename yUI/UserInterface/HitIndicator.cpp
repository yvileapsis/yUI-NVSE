#include <main.h>
#include <functions.h>
#include <SimpleINILibrary.h>

#include <Menu.h>
#include <Tile.h>

namespace UserInterface::HitIndicator
{

	bool		enable			= false;

	enum kHitIndicatorFlags
	{
		kHitIndicatorNothing	= 1 << 0,
		kHitIndicatorNormal		= 1 << 1,
		kHitIndicatorAltColor	= 1 << 2,
		kHitIndicatorDouble		= 1 << 3,
		kHitIndicatorHalfAlpha	= 1 << 4,
		kHitIndicatorOffset		= 1 << 5,
		kHitIndicatorShakeVert	= 1 << 6,
		kHitIndicatorShakeHoriz	= 1 << 7,
	};

	Float32		seconds			= 1.5;
	Float32		alpha			= 400.0;
	Float32		height			= 256.0;
	Float32		width			= 256.0;
	Float32		offset			= 0.0;
	UInt32		modeHit			= kHitIndicatorNormal;
	UInt32		modeDead		= kHitIndicatorNormal;
	UInt32		modeKill		= kHitIndicatorOffset;
	UInt32		modeEnemy		= kHitIndicatorAltColor;
	UInt32		modeCrit		= kHitIndicatorDouble;
	UInt32		modeHeadshot	= kHitIndicatorShakeVert;
	UInt32		modeSelf		= kHitIndicatorHalfAlpha;
	UInt32		modeExplosion	= kHitIndicatorShakeHoriz;
	UInt32		modeNoAttacker	= kHitIndicatorHalfAlpha;
	UInt32		modeNoDamage	= kHitIndicatorNothing;
	bool		enableOut		= true;
	bool		enableSighting	= true;
	bool		enableScope		= true;
	UInt32		rotate			= 2;

	Tile*		tileMain		= nullptr;

	SInt64		visible			= 0;
	UInt32		depth			= 0;
	Float32		angle			= 0;

	std::map<TESObjectREFR*, UInt32>			hitQueue;
	std::unordered_set<Tile*>					tileQueue;
	std::unordered_map<Tile*, TESObjectREFR*>	tileProcessing;

	bool ProcessTilesInUse(Tile* tile, TESObjectREFR* target)
	{
		if (!tile) return false;
		if (rotate == 2) tile->Set("_RotateAngle", g_player->GetHeadingAngle(target));
		const auto val = tile->GetValue("_counter");
		if (!val) return false;
		if (val->num < 1) return true;
		tileQueue.emplace(tile);
		return false;
	}

	Tile* CreateTileForHitMarker(TESObjectREFR* target)
	{
		Tile* tile;
		if (tileQueue.empty())
			tile = tileMain->GetChild("JHIContainer")->AddTileFromTemplate("JHIMarker");
		else {
			const auto iter = tileQueue.begin();
			tile = *iter;
			tileQueue.erase(iter);
		}
		tileProcessing[tile] = target;
		return tile;
	}

	void CreateHitMarker(TESObjectREFR* target, UInt32 flags)
	{
		const auto tile = CreateTileForHitMarker(target);

		tile->Set("_depth", depth++);

		tile->Set(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitIndicatorAltColor));

		tile->Set("_ModeOffset", static_cast<bool>(flags & kHitIndicatorOffset));
		tile->Set("_ModeShakeVert", static_cast<bool>(flags & kHitIndicatorShakeVert));
		tile->Set("_ModeShakeHoriz", static_cast<bool>(flags & kHitIndicatorShakeHoriz));
		tile->Set("_angle", g_player->GetHeadingAngle(target));
		tile->Set("_alphaMult", flags & kHitIndicatorHalfAlpha ? 0.5 : 1);

		tile->SetGradual("_counter", 0, 1, flags & kHitIndicatorDouble ? 2 * seconds : seconds, GradualSetFloat::StartToEnd);
	}

	void MainLoop()
	{
		if (!enable) return;

		if (g_HUDMainMenu->isUsingScope) visible = static_cast<SInt64>(enableScope);
		else if (g_player->UsingIronSights()) visible = enableSighting;
		else visible = enableOut;
		tileMain->Set("_visible", visible);
		tileMain->Set("_scope", g_player->UsingIronSights());

		if (MenuMode() || !visible) { hitQueue.clear(); return; }

		if (rotate == 1)
		{
			const auto newAngle = g_player->pos.z;
			tileMain->Set("_angleWorld", angle - newAngle);
			angle = newAngle;
		}

		for (auto i = tileProcessing.begin(); i != tileProcessing.end(); ) if (!ProcessTilesInUse(i->first, i->second)) tileProcessing.erase(i++); else ++i;

		depth = 0;
		for (const auto& [fst, snd] : hitQueue) CreateHitMarker(fst, snd);
		hitQueue.clear();
	}

	void OnHit(Actor* target)
	{
		if (!enable || !visible) return;

		if (!target || target != g_player) return;

		UInt32 flags = 0;

		TESObjectREFR* source = nullptr;

		if (target->IsActor())
		{
			const auto hitData = target->baseProcess->GetLastHitData();
			if (!hitData) return;
			source = hitData->source;

			flags |= modeHit;
			if (target->lifeState == kLifeState_Dead || target->lifeState == kLifeState_Dying) flags |= modeDead;
			if (target->lifeState == kLifeState_Alive && hitData->flags & ActorHitData::kFlag_IsFatal) flags |= modeKill;
			if (hitData->flags & ActorHitData::kFlag_IsCritical) flags |= modeCrit;
			if (hitData->hitLocation == BGSBodyPartData::eBodyPart_Brain || hitData->hitLocation ==
				BGSBodyPartData::eBodyPart_Head1 || hitData->hitLocation == BGSBodyPartData::eBodyPart_Head2) flags |= modeHeadshot;
			if (hitData->explosion && IS_TYPE(hitData->explosion, Explosion) && hitData->explosion->IsExplosion()) flags |= modeExplosion;
			if (hitData->dmgMult <= 0) flags |= modeNoDamage;
			if (!source) flags |= modeNoAttacker;
			if (source && source->IsCrimeOrEnemy()) flags |= modeEnemy;
			if (source == g_player) flags |= modeSelf;
		}

		if (flags & kHitIndicatorNothing) return;
		hitQueue[source] |= flags;
	}

	
	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable			= ini.GetOrCreate("General", "bHitIndicator", 1, "; enable 'Hit Indicator' feature. If required files are not found this will do nothing.");
		seconds			= ini.GetOrCreate("Hit Indicator", "fSeconds", 2.5, nullptr);
		alpha			= ini.GetOrCreate("Hit Indicator", "fAlpha", 400.0, nullptr);
		height			= ini.GetOrCreate("Hit Indicator", "fHeight", 256.0, nullptr);
		width			= ini.GetOrCreate("Hit Indicator", "fWidth", 256.0, nullptr);
		offset			= ini.GetOrCreate("Hit Indicator", "fOffset", 0.0, nullptr);
		modeHit			= ini.GetOrCreate("Hit Indicator", "iModeHit", kHitIndicatorNormal, nullptr);
		modeDead		= ini.GetOrCreate("Hit Indicator", "iModeDead", kHitIndicatorNormal, nullptr);
		modeKill		= ini.GetOrCreate("Hit Indicator", "iModeKill", kHitIndicatorOffset, nullptr);
		modeEnemy		= ini.GetOrCreate("Hit Indicator", "iModeEnemy", kHitIndicatorAltColor, nullptr);
		modeCrit		= ini.GetOrCreate("Hit Indicator", "iModeCrit", kHitIndicatorDouble, nullptr);
		modeHeadshot	= ini.GetOrCreate("Hit Indicator", "iModeHeadshot", kHitIndicatorShakeVert, nullptr);
		modeSelf		= ini.GetOrCreate("Hit Indicator", "iModeSelf", kHitIndicatorHalfAlpha, nullptr);
		modeExplosion	= ini.GetOrCreate("Hit Indicator", "iModeExplosion", kHitIndicatorShakeHoriz, nullptr);
		modeNoAttacker	= ini.GetOrCreate("Hit Indicator", "iModeNoAttacker", kHitIndicatorHalfAlpha, nullptr);
		modeNoDamage	= ini.GetOrCreate("Hit Indicator", "iModeNoDamage", kHitIndicatorNothing, nullptr);
		enableOut		= ini.GetOrCreate("Hit Indicator", "bEnableOut", true, nullptr);
		enableSighting	= ini.GetOrCreate("Hit Indicator", "bEnableSighting", true, nullptr);
		enableScope		= ini.GetOrCreate("Hit Indicator", "bEnableScope", true, nullptr);
		rotate			= ini.GetOrCreate("Hit Indicator", "iRotate", 2, nullptr);

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

		if (tileMain->GetChild("JHI")) delete tileMain->GetChild("JHIContainer");
		tileMain->AddTileFromTemplate("JHIContainer");

		tileMain->Set("_AlphaBase", alpha);
		tileMain->Set("_HeightBase", height);
		tileMain->Set("_WidthBase", width);
		tileMain->Set("_OffsetBase", offset);

		tileMain->SetGradual("_GlobalShaker", -0.05, 0.05, 0.15, GradualSetFloat::StartToEndPerpetual);

		tileMain->Set("_visible", visible = 0);
		tileMain->Set("_scope", 0);
	}

	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JHI");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\HitIndicator.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JHI");
		}
		if (!tileMain) return;
		RegisterEvent("JHI:Reset", 0, nullptr, 4);
		SetEventHandler("JHI:Reset", Reset);
		Reset();
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;

		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}
