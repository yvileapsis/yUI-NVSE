#include <GameData.h>
#include <functions.h>
#include <SimpleINILibrary.h>

#include "main.h"
#include "Menus.h"
#include "PluginAPI.h"
#include <ranges>
#include <unordered_set>

namespace UserInterface::VisualObjectives
{

	inline UInt32	g_JVO = 0;
	inline UInt32	g_JVO_Key = 49;
	inline UInt32	g_JVO_Toggle = 1;

	inline UInt32	g_JVO_OffscreenHandling = 0;

	inline UInt32	g_JVO_DistanceHandling = 2;
	inline UInt32	g_JVO_DistanceSystem = 1;
	inline UInt32	g_JVO_TextHandling = 1;
	inline UInt32	g_JVO_TextSystem = 1;

	inline Float64	g_JVO_Alpha = 0.0;
	inline Float64	g_JVO_AlphaMult = 0.6;

	inline Float64	g_JVO_Height = 36.0;
	inline Float64	g_JVO_Width = 24.0;
	inline Float64	g_JVO_OffsetHeight = 0.02;
	inline Float64	g_JVO_OffsetWidth = 0.01;

	inline UInt32	g_JVO_AltColor = 1;

	inline Float64	g_JVO_Radius = 0.06;
	inline Float64	g_JVO_DistanceMin = -1.0;
	inline Float64	g_JVO_DistanceMax = -1.0;

	inline UInt32	g_JVO_EnableOut = 1;
	inline UInt32	g_JVO_EnableSighting = 1;
	inline UInt32	g_JVO_EnableScope = 0;

	inline UInt32	g_JVO_Font = 0;
	inline Float64	g_JVO_FontY = 0.0;

	Tile*				tileMain			= nullptr;
	
	SInt64				visible				= 0;
	UInt32				depth				= 0;
	bool				show				= true;
	bool				doonce				= false;

	Float32				offsetHUDMarker		= 0;
	Float32				offsetHUDText		= 0;
	Float32				offsetWorld			= 20;

	std::unordered_set<Tile*>					g_TilesFree;
	std::unordered_map<Tile*, NiPoint3>			g_TilesInUse;

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		g_JVO						= ini.GetOrCreate("JustMods", "JVO", 1.0, nullptr);
		g_JVO_Key					= ini.GetOrCreate("JVO", "Key", 49.0, nullptr);
		g_JVO_Toggle				= ini.GetOrCreate("JVO", "Toggle", 1.0, nullptr);
		g_JVO_OffscreenHandling		= ini.GetOrCreate("JVO", "OffscreenHandling", 0.0, nullptr);
		g_JVO_DistanceHandling		= ini.GetOrCreate("JVO", "DistanceHandling", 2.0, nullptr);
		g_JVO_DistanceSystem		= ini.GetOrCreate("JVO", "DistanceSystem", 1.0, nullptr);
		g_JVO_TextHandling			= ini.GetOrCreate("JVO", "TextHandling", 1.0, nullptr);
		g_JVO_TextSystem			= ini.GetOrCreate("JVO", "TextSystem", 1.0, nullptr);
		g_JVO_Alpha					= ini.GetOrCreate("JVO", "Alpha", 0.0, nullptr);
		g_JVO_AlphaMult				= ini.GetOrCreate("JVO", "AlphaMult", 0.6, nullptr);
		g_JVO_Height				= ini.GetOrCreate("JVO", "Height", 36.0, nullptr);
		g_JVO_Width					= ini.GetOrCreate("JVO", "Width", 24.0, nullptr);
		g_JVO_OffsetHeight			= ini.GetOrCreate("JVO", "OffsetHeight", 0.02, nullptr);
		g_JVO_OffsetWidth			= ini.GetOrCreate("JVO", "OffsetWidth", 0.01, nullptr);
		g_JVO_AltColor				= ini.GetOrCreate("JVO", "AltColor", 1.0, nullptr);
		g_JVO_Radius				= ini.GetOrCreate("JVO", "Radius", 0.06, nullptr);
		g_JVO_DistanceMin			= ini.GetOrCreate("JVO", "DistanceMin", -1.0, nullptr);
		g_JVO_DistanceMax			= ini.GetOrCreate("JVO", "DistanceMax", -1.0, nullptr);
		g_JVO_EnableOut				= ini.GetOrCreate("JVO", "EnableOut", 1.0, nullptr);
		g_JVO_EnableSighting		= ini.GetOrCreate("JVO", "EnableSighting", 1.0, nullptr);
		g_JVO_EnableScope			= ini.GetOrCreate("JVO", "EnableScope", 0.0, nullptr);
		g_JVO_Font					= ini.GetOrCreate("JVO", "Font", 0.0, nullptr);
		g_JVO_FontY					= ini.GetOrCreate("JVO", "FontY", 0.0, nullptr);

		ini.SaveFile(iniPath.c_str(), false);

	}


	Tile* CreateTileForVisualObjective(NiPoint3 target)
	{
		Tile* tile;
		if (!g_TilesFree.empty()) { const auto iter = g_TilesFree.begin(); tile = *iter; g_TilesFree.erase(iter); }
		else tile = tileMain->GetChild("JVOContainer")->AddTileFromTemplate("JVOMarker");
		g_TilesInUse[tile] = target;
		return tile;
	}

	void AddVisualObjective(UInt32 altMarker, TESObjectREFR* ref, TESObjectREFR* linkDoor = nullptr)
	{
		auto target = ref;
		if (linkDoor) target = linkDoor;

		NiPoint3 in = *target->GetPos();
		if (target->GetNiNode())
		{
			if (const auto niblock = target->GetNifBlock(0, "Bip01 Head"))
			{
				in = niblock->m_worldTransform.pos;
				in.z += offsetWorld;
			}
			else
			{
				const auto center = target->GetCenter();
				in.x += center.x;
				in.y += center.y;
				in.z += center.z + offsetWorld;
			}
		}
		if (in.z == 0)
		{
			NiPoint2 ni2{ in.x, in.y };
			Float32 ni1;
			TES::GetSingleton()->GetTerrainHeight(&ni2, &ni1);
			in.z = ni1 > 0 ? ni1 : g_player->GetPos()->z;
		}
		const auto tile = CreateTileForVisualObjective(in);

		tile->SetFloat("_JVOHostile", depth++);

		// TODO: fix this so it doesn't rely on altMarker but rather somehow checks for target to be player marker
		const auto distance = !altMarker ? g_player->GetDistance(target) : g_player->GetDistance2D(target);

		bool inDistance = true;
		if (g_JVO_DistanceMax >= 0 && distance > g_JVO_DistanceMax) inDistance = false;
		if (g_JVO_DistanceMin >= 0 && distance < g_JVO_DistanceMin) inDistance = false;

		const bool inFocus = tile->GetFloat("_JVOInFocus");

		if (g_JVO_AltColor) tile->SetFloat("_JVOHostile", target->IsCrimeOrEnemy());

		std::string string;
		if (inFocus + g_JVO_DistanceHandling - altMarker <= 1) {}
		else if (distance > 1000000)
			string = "Far away";
		else if (g_JVO_DistanceSystem == 0)
			string = std::to_string(static_cast<UInt32>(distance)) + " un.";
		else if (g_JVO_DistanceSystem == 1)
			string = std::to_string(static_cast<UInt32>(distance / 21.333)) + " ft.";
		else if (g_JVO_DistanceSystem == 2)
			string = std::to_string(static_cast<UInt32>(distance / 69.99104)) + " m.";
		tile->SetString("_JVODistance", string.c_str());

		tile->SetFloat("_JVOInDistance", inDistance);

		string = "";

		if (inFocus + g_JVO_TextHandling - altMarker <= 1) {}
		else if (g_JVO_TextSystem == 4)
			string = g_player->activeQuest->fullName.name.CStr();
		else if (g_JVO_TextSystem == 3)
			string = g_player->activeQuest->GetObjective(g_player->activeQuest->currentStage)->displayText.CStr();
		else if (g_JVO_TextSystem == 2)
			string = ref->GetTheName();
		else if (g_JVO_TextSystem == 1)
			string = target->GetTheName();
		else if (g_JVO_TextSystem == 0)
		{
			if (target->GetNiNode() && g_player->GetLineOfSight(target))
				string = target->GetTheName();
			else if (target->IsInInterior())
				string = target->GetParentCell()->GetTheName();
			else
				string = target->GetParentWorld()->GetTheName();
		}

		if (string == "<no name>") string = "";
		tile->SetString("_JVOText", string.c_str());

		tile->SetFloat("_JVOAltMarker", altMarker);
	}

	void MainLoop()
	{
		if (!g_JVO) return;

		if (g_HUDMainMenu->isUsingScope)		visible = -1 * static_cast<SInt64>(g_JVO_EnableScope);
		else if (g_player->UsingIronSights())	visible = g_JVO_EnableSighting;
		else									visible = g_JVO_EnableOut;

		if (g_JVO_Toggle)
		{
			if (!IsKeyPressed(g_JVO_Key)) doonce = true;
			else if (doonce) { doonce = false; show = true - show; }
			if (!show) visible = 0;
		}
		else if (!IsKeyPressed(g_JVO_Key)) visible = 0;

		tileMain->SetFloat("_JVOVisible", visible);

		if (!visible) return;

		tileMain->SetFloat("_JVOInCombat", g_player->pcInCombat);
		tileMain->SetFloat("_JVOAlphaCW", g_HUDMainMenu->tileHitPointsCompass->GetFloat(kTileValue_alpha));

		offsetHUDMarker = GetJIPAuxVarOrDefault("*_JVOOffset", 0, 0);
		offsetHUDText = GetJIPAuxVarOrDefault("*_JVOOffset", 1, 0);
		offsetWorld = GetJIPAuxVarOrDefault("*_JVOOffset", 2, 20);

		tileMain->SetFloat("_JVOOffsetMarker", offsetHUDMarker);
		tileMain->SetFloat("_JVOOffsetText", offsetHUDText);

		for (const auto fst : g_TilesInUse | std::views::keys) { g_TilesFree.emplace(fst); fst->SetFloat(kTileValue_visible, 0); }
		g_TilesInUse.clear();

		depth = 0;
		if (const auto target = g_player->GetPlacedMarkerOrTeleportLink())
			AddVisualObjective(1, target);
		for (const auto i : *g_player->GetCurrentQuestObjectiveTargets())
			AddVisualObjective(0, i->target, i->teleportLinks.size ? i->teleportLinks.data->door : nullptr);
	}

	void OnRender()
	{
		for (auto& [fst, snd] : g_TilesInUse)
		{
			NiPoint3 out;
			const bool onScreen = WorldToScreen(&snd, out, g_JVO_OffscreenHandling);
			fst->SetFloat(kTileValue_visible, onScreen || g_JVO_OffscreenHandling != 2);
			fst->SetFloat("_X", out.x);
			fst->SetFloat("_Y", out.y);
		}
	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (!g_JVO)
		{
			RemoveEventHandler("yJAM:MainLoop", MainLoop);
			RemoveEventHandler("yJAM:JG:OnRender", OnRender);
			return;
		}
		SetEventHandler("yJAM:JG:OnRender", OnRender);

		if (tileMain->GetChild("JVO")) tileMain->GetChild("JVOContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JVOContainer");

		tileMain->SetFloat("_JVODistanceVisible", g_JVO_DistanceHandling);
		tileMain->SetFloat("_JVOTextVisible", g_JVO_TextHandling);

		tileMain->SetFloat("_JVOAlphaBase", g_JVO_Alpha);
		tileMain->SetFloat("_JVOAlphaMult", g_JVO_AlphaMult);
		tileMain->SetFloat("_JVORadiusMax", g_JVO_Radius);

		tileMain->SetFloat("_JVOWidthBase", g_JVO_Width);
		tileMain->SetFloat("_JVOHeightBase", g_JVO_Height);
		tileMain->SetFloat("_JVOOffsetWidth", g_JVO_OffsetWidth);
		tileMain->SetFloat("_JVOOffsetHeight", g_JVO_OffsetHeight);

		tileMain->SetFloat("_JVOFontBase", g_JVO_Font);
		tileMain->SetFloat("_JVOFontYBase", g_JVO_FontY);
	}

	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JVO");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\prefabs\JVO\JVO.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JVO");
		}
		if (!tileMain) return;
		RegisterEvent("JVO:Reset", 0, nullptr, 4);
		SetEventHandler("JVO:Reset", Reset);
		DispatchEvent("JVO:Reset", nullptr);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;

		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		mainLoop.emplace_back(MainLoop);
	}
}