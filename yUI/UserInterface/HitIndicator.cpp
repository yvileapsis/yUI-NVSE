#include <main.h>
#include <functions.h>
#include <SimpleINILibrary.h>
#include <unordered_set>

#include "Menus.h"
#include "Tiles.h"

namespace UserInterface::HitIndicator
{

	inline UInt32	g_JHI = 0;

	enum kHitIndicatorFlags
	{
		kHitIndicatorNothing = 1 << 0,
		kHitIndicatorNormal = 1 << 1,
		kHitIndicatorAltColor = 1 << 2,
		kHitIndicatorDouble = 1 << 3,
		kHitIndicatorHalfAlpha = 1 << 4,
		kHitIndicatorOffset = 1 << 5,
		kHitIndicatorShakeVert = 1 << 6,
		kHitIndicatorShakeHoriz = 1 << 7,
	};

	inline Float64	g_Seconds = 1.5;
	inline Float64	g_Alpha = 400.0;
	inline Float64	g_Height = 256.0;
	inline Float64	g_Width = 256.0;
	inline Float64	g_Offset = 0.0;
	inline UInt32	g_ModeHit = kHitIndicatorNormal;
	inline UInt32	g_ModeDead = kHitIndicatorNormal;
	inline UInt32	g_ModeKill = kHitIndicatorOffset;
	inline UInt32	g_ModeEnemy = kHitIndicatorAltColor;
	inline UInt32	g_ModeCrit = kHitIndicatorDouble;
	inline UInt32	g_ModeHeadshot = kHitIndicatorShakeVert;
	inline UInt32	g_ModeSelf = kHitIndicatorHalfAlpha;
	inline UInt32	g_ModeExplosion = kHitIndicatorShakeHoriz;
	inline UInt32	g_ModeNoAttacker = kHitIndicatorHalfAlpha;
	inline UInt32	g_ModeNoDamage = kHitIndicatorNothing;
	inline UInt32	g_EnableOut = 1;
	inline UInt32	g_EnableSighting = 1;
	inline UInt32	g_EnableScope = 1;
	inline UInt32	g_Rotate = 2;

	bool				initialized						= false;
	Tile*				tileMain						= nullptr;

	SInt64				visible							= 0;
	UInt32				depth							= 0;
	Float32				angle							= 0;

	std::map<TESObjectREFR*, UInt32>					hitQueue;
	std::unordered_set<Tile*>							tileQueue;
	std::unordered_map<Tile*, TESObjectREFR*>			tileProcessing;



	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		g_JHI				= ini.GetOrCreate("JustMods", "JHI", 1, nullptr);
		g_Seconds			= ini.GetOrCreate("JHI", "Seconds", 2.5, nullptr);
		g_Alpha				= ini.GetOrCreate("JHI", "Alpha", 400.0, nullptr);
		g_Height			= ini.GetOrCreate("JHI", "Height", 256.0, nullptr);
		g_Width				= ini.GetOrCreate("JHI", "Width", 256.0, nullptr);
		g_Offset			= ini.GetOrCreate("JHI", "Offset", 0.0, nullptr);
//		g_ModeHit			= ini.GetOrCreate("JHI", "ModeHit", static_cast<double>(kHitMarkerNormal), nullptr);
		g_EnableOut			= ini.GetOrCreate("JHI", "EnableOut", 1, nullptr);
		g_EnableSighting	= ini.GetOrCreate("JHI", "EnableSighting", 1, nullptr);
		g_EnableScope		= ini.GetOrCreate("JHI", "EnableScope", 1, nullptr);
		g_Rotate			= ini.GetOrCreate("JHI", "Rotate", 2, nullptr);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	bool ProcessTilesInUse(Tile* tile, TESObjectREFR* target)
	{
		if (!tile) return false;
		if (g_Rotate == 2) tile->SetFloat("_RotateAngle", g_player->GetHeadingAngle(target));
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

		tile->SetFloat("_depth", depth++);

		tile->SetFloat(kTileValue_systemcolor, 1 + static_cast<bool>(flags & kHitIndicatorAltColor));

		tile->SetFloat("_ModeOffset", static_cast<bool>(flags & kHitIndicatorOffset));
		tile->SetFloat("_ModeShakeVert", static_cast<bool>(flags & kHitIndicatorShakeVert));
		tile->SetFloat("_ModeShakeHoriz", static_cast<bool>(flags & kHitIndicatorShakeHoriz));
		tile->SetFloat("_angle", g_player->GetHeadingAngle(target));
		tile->SetFloat("_alphaMult", flags & kHitIndicatorHalfAlpha ? 0.5 : 1);

		tile->GradualSetFloat("_counter", 0, 1, flags & kHitIndicatorDouble ? 2 * g_Seconds : g_Seconds, GradualSetFloat::StartToEnd);
	}

	void MainLoop()
	{
		if (!g_JHI) return;

		if (g_HUDMainMenu->isUsingScope) visible = static_cast<SInt64>(g_EnableScope);
		else if (g_player->UsingIronSights()) visible = g_EnableSighting;
		else visible = g_EnableOut;
		tileMain->SetFloat("_visible", visible);
		tileMain->SetFloat("_scope", g_player->UsingIronSights());

		if (!visible) { hitQueue.clear(); return; }

		if (g_Rotate == 1)
		{
			const auto newAngle = g_player->pos.z;
			tileMain->SetFloat("_angleWorld", angle - newAngle);
			angle = newAngle;
		}

		for (auto i = tileProcessing.begin(); i != tileProcessing.end(); ) if (!ProcessTilesInUse(i->first, i->second)) tileProcessing.erase(i++); else ++i;

		depth = 0;
		for (const auto& [fst, snd] : hitQueue) CreateHitMarker(fst, snd);
		hitQueue.clear();
	}

	void OnHit(const Actor* target, void* args)
	{
		if (!initialized || !visible) return;

		if (!target || target != g_player) return;

		UInt32 flags = 0;

		TESObjectREFR* source = nullptr;

		if (target->IsActor())
		{
			const auto hitData = target->baseProcess->GetLastHitData();
			if (!hitData) return;
			source = hitData->source;

			flags |= g_ModeHit;
			if (target->lifeState == kLifeState_Dead || target->lifeState == kLifeState_Dying) flags |= g_ModeDead;
			if (target->lifeState == kLifeState_Alive && hitData->flags & ActorHitData::kFlag_IsFatal) flags |= g_ModeKill;
			if (hitData->flags & ActorHitData::kFlag_IsCritical) flags |= g_ModeCrit;
			if (hitData->hitLocation == BGSBodyPartData::eBodyPart_Brain || hitData->hitLocation ==
				BGSBodyPartData::eBodyPart_Head1 || hitData->hitLocation == BGSBodyPartData::eBodyPart_Head2) flags |= g_ModeHeadshot;
			if (hitData->explosion && IS_TYPE(hitData->explosion, Explosion) && hitData->explosion->IsExplosion()) flags |= g_ModeExplosion;
			if (hitData->dmgMult <= 0) flags |= g_ModeNoDamage;
			if (!source) flags |= g_ModeNoAttacker;
			if (source && source->IsCrimeOrEnemy()) flags |= g_ModeEnemy;
			if (source == g_player) flags |= g_ModeSelf;
		}

		if (flags & kHitIndicatorNothing) return;
		hitQueue[source] |= flags;
	}


	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (!g_JHI)
		{
			RemoveNativeEventHandler("yJAM:MainLoop", reinterpret_cast<EventHandler>(MainLoop));
			RemoveNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));
			return;
		}

		SetNativeEventHandler("yJAM:MainLoop", reinterpret_cast<EventHandler>(MainLoop));
		SetNativeEventHandler("yJAM:JIP:OnHit", reinterpret_cast<EventHandler>(OnHit));

		if (tileMain->GetChild("JHI")) tileMain->GetChild("JHIContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JHIContainer");

		tileMain->SetFloat("_AlphaBase", g_Alpha);
		tileMain->SetFloat("_HeightBase", g_Height);
		tileMain->SetFloat("_WidthBase", g_Width);
		tileMain->SetFloat("_OffsetBase", g_Offset);

		tileMain->GradualSetFloat("_GlobalShaker", -0.05, 0.05, 0.15, GradualSetFloat::StartToEndPerpetual);

		tileMain->SetFloat("_visible", visible = 0);
		tileMain->SetFloat("_scope", 0);
	}

	void MainLoopDoOnce()
	{
		initialized = true;
		tileMain = g_HUDMainMenu->tile->GetChild("JHI");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\prefabs\JHI\JHI.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JHI");
		}
		if (!tileMain) return;
		RegisterEvent("JHI:Reset", 0, nullptr, 4);
		SetEventHandler("JHI:Reset", Reset);
		DispatchEvent("JHI:Reset", nullptr);
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;

		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		mainLoop.emplace_back(MainLoop);
	}
}
