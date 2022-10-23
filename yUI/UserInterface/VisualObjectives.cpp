#include <main.h>
#include <Menus.h>

#include <GameData.h>
#include <functions.h>
#include <SimpleINILibrary.h>
#include <ranges>
#include <unordered_set>

namespace UserInterface::VisualObjectives
{

	UInt32		enable				= 0;
	UInt32		key					= 49;
	UInt32		toggle				= 1;

	UInt32		offscreenHandling	= 0;

	UInt32		distanceHandling	= 2;
	UInt32		distanceSystem		= 1;
	UInt32		textHandling		= 1;
	UInt32		textSystem			= 1;

	Float64		alpha				= 0.0;
	Float64		alphaMult			= 0.6;

	Float64		height				= 36.0;
	Float64		width				= 24.0;
	Float64		offsetHeight		= 0.02;
	Float64		offsetWidth			= 0.01;

	UInt32		altColor			= 1;

	Float64		radius				= 0.06;
	Float64		distanceMin			= -1.0;
	Float64		distanceMax			= -1.0;

	UInt32		enableOut			= 1;
	UInt32		enableSighting		= 1;
	UInt32		enableScope			= 0;

	UInt32		font				= 0;
	Float64		fontY				= 0.0;

	Tile*		tileMain			= nullptr;
	
	SInt64		visible				= 0;
	UInt32		depth				= 0;
	bool		show				= true;
	bool		doonce				= false;

	Float32		offsetHUDMarker		= 0;
	Float32		offsetHUDText		= 0;
	Float32		offsetWorld			= 20;

	std::unordered_set<Tile*>			tilesFree;
	std::unordered_map<Tile*, NiPoint3>	tilesInUse;


	Tile* CreateTileForVisualObjective(NiPoint3 target)
	{
		Tile* tile;
		if (!tilesFree.empty()) { const auto iter = tilesFree.begin(); tile = *iter; tilesFree.erase(iter); }
		else tile = tileMain->GetChild("JVOContainer")->AddTileFromTemplate("JVOMarker");
		tilesInUse[tile] = target;
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

		tile->SetFloat("_Hostile", depth++);

		// TODO: fix this so it doesn't rely on altMarker but rather somehow checks for target to be player marker
		const auto distance = !altMarker ? g_player->GetDistance(target) : g_player->GetDistance2D(target);

		bool inDistance = true;
		if (distanceMax >= 0 && distance > distanceMax) inDistance = false;
		if (distanceMin >= 0 && distance < distanceMin) inDistance = false;

		const bool inFocus = tile->GetFloat("_InFocus");

		if (altColor) tile->SetFloat("_Hostile", target->IsCrimeOrEnemy());

		std::string string;
		if (inFocus + distanceHandling - altMarker <= 1) {}
		else if (distance > 1000000)
			string = "Far away";
		else if (distanceSystem == 0)
			string = std::to_string(static_cast<UInt32>(distance)) + " un.";
		else if (distanceSystem == 1)
			string = std::to_string(static_cast<UInt32>(distance / 21.333)) + " ft.";
		else if (distanceSystem == 2)
			string = std::to_string(static_cast<UInt32>(distance / 69.99104)) + " m.";
		tile->SetString("_Distance", string.c_str());

		tile->SetFloat("_InDistance", inDistance);

		string = "";

		if (inFocus + textHandling - altMarker <= 1) {}
		else if (textSystem == 4)
			string = g_player->activeQuest->fullName.name.CStr();
		else if (textSystem == 3)
			string = g_player->activeQuest->GetObjective(g_player->activeQuest->currentStage)->displayText.CStr();
		else if (textSystem == 2)
			string = ref->GetTheName();
		else if (textSystem == 1)
			string = target->GetTheName();
		else if (textSystem == 0)
		{
			if (target->GetNiNode() && g_player->GetLineOfSight(target))
				string = target->GetTheName();
			else if (target->IsInInterior())
				string = target->GetParentCell()->GetTheName();
			else
				string = target->GetParentWorld()->GetTheName();
		}

		if (string == "<no name>") string = "";
		tile->SetString("_Text", string.c_str());

		tile->SetFloat("_AltMarker", altMarker);
	}

	void MainLoop()
	{
		if (!enable) return;

		if (g_HUDMainMenu->isUsingScope)		visible = -1 * static_cast<SInt64>(enableScope);
		else if (g_player->UsingIronSights())	visible = enableSighting;
		else									visible = enableOut;

		if (toggle)
		{
			if (!IsKeyPressed(key)) doonce = true;
			else if (doonce) { doonce = false; show = true - show; }
			if (!show) visible = 0;
		}
		else if (!IsKeyPressed(key)) visible = 0;

		tileMain->SetFloat("_Visible", visible);

		if (!visible) return;

		tileMain->SetFloat("_InCombat", g_player->pcInCombat);
		tileMain->SetFloat("_AlphaCW", g_HUDMainMenu->tileHitPointsCompass->GetFloat(kTileValue_alpha));

		offsetHUDMarker = GetJIPAuxVarOrDefault("*_Offset", 0, 0);
		offsetHUDText = GetJIPAuxVarOrDefault("*_Offset", 1, 0);
		offsetWorld = GetJIPAuxVarOrDefault("*_Offset", 2, 20);

		tileMain->SetFloat("_OffsetMarker", offsetHUDMarker);
		tileMain->SetFloat("_OffsetText", offsetHUDText);

		for (const auto fst : tilesInUse | std::views::keys) { tilesFree.emplace(fst); fst->SetFloat(kTileValue_visible, 0); }
		tilesInUse.clear();

		depth = 0;
		if (const auto target = g_player->GetPlacedMarkerOrTeleportLink())
			AddVisualObjective(1, target);
		for (const auto i : *g_player->GetCurrentQuestObjectiveTargets())
			AddVisualObjective(0, i->target, i->teleportLinks.size ? i->teleportLinks.data->door : nullptr);
	}

	void OnRender()
	{
		for (auto& [fst, snd] : tilesInUse)
		{
			NiPoint3 out;
			const bool onScreen = WorldToScreen(&snd, out, offscreenHandling);
			fst->SetFloat(kTileValue_visible, onScreen || offscreenHandling != 2);
			fst->SetFloat("_X", out.x);
			fst->SetFloat("_Y", out.y);
		}
	}

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + R"(\Data\Config\JustMods.ini)";
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable				= ini.GetOrCreate("JustMods", "bVisualObjectives", 1, nullptr);
		key					= ini.GetOrCreate("JVO", "iKey", 49, nullptr);
		toggle				= ini.GetOrCreate("JVO", "bToggle", true, nullptr);
		offscreenHandling	= ini.GetOrCreate("JVO", "iOffscreenHandling", 0, nullptr);
		distanceHandling	= ini.GetOrCreate("JVO", "iDistanceHandling", 2, nullptr);
		distanceSystem		= ini.GetOrCreate("JVO", "iDistanceSystem", 1, nullptr);
		textHandling		= ini.GetOrCreate("JVO", "iTextHandling", 1, nullptr);
		textSystem			= ini.GetOrCreate("JVO", "iTextSystem", 1, nullptr);
		alpha				= ini.GetOrCreate("JVO", "fAlpha", 0.0, nullptr);
		alphaMult			= ini.GetOrCreate("JVO", "fAlphaMult", 0.6, nullptr);
		height				= ini.GetOrCreate("JVO", "fHeight", 36.0, nullptr);
		width				= ini.GetOrCreate("JVO", "fWidth", 24.0, nullptr);
		offsetHeight		= ini.GetOrCreate("JVO", "fOffsetHeight", 0.02, nullptr);
		offsetWidth			= ini.GetOrCreate("JVO", "fOffsetWidth", 0.01, nullptr);
		altColor			= ini.GetOrCreate("JVO", "bAltColor", true, nullptr);
		radius				= ini.GetOrCreate("JVO", "fRadius", 0.06, nullptr);
		distanceMin			= ini.GetOrCreate("JVO", "fDistanceMin", -1.0, nullptr);
		distanceMax			= ini.GetOrCreate("JVO", "fDistanceMax", -1.0, nullptr);
		enableOut			= ini.GetOrCreate("JVO", "bEnableOut", true, nullptr);
		enableSighting		= ini.GetOrCreate("JVO", "bEnableSighting", true, nullptr);
		enableScope			= ini.GetOrCreate("JVO", "bEnableScope", false, nullptr);
		font				= ini.GetOrCreate("JVO", "fFont", 0.0, nullptr);
		fontY				= ini.GetOrCreate("JVO", "fFontY", 0.0, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			std::erase(mainLoop, MainLoop);
			std::erase(onRender, OnRender);
			return;
		}
		mainLoop.emplace_back(MainLoop);
		onRender.emplace_back(OnRender);

		if (tileMain->GetChild("JVO")) tileMain->GetChild("JVOContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JVOContainer");

		tileMain->SetFloat("_DistanceVisible", distanceHandling);
		tileMain->SetFloat("_TextVisible", textHandling);

		tileMain->SetFloat("_AlphaBase", alpha);
		tileMain->SetFloat("_AlphaMult", alphaMult);
		tileMain->SetFloat("_RadiusMax", radius);

		tileMain->SetFloat("_WidthBase", width);
		tileMain->SetFloat("_HeightBase", height);
		tileMain->SetFloat("_OffsetWidth", offsetWidth);
		tileMain->SetFloat("_OffsetHeight", offsetHeight);

		tileMain->SetFloat("_FontBase", font);
		tileMain->SetFloat("_FontYBase", fontY);
	}

	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JVO");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\VisualObjectives.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JVO");
		}
		if (!tileMain) return;
		RegisterEvent("JVO:Reset", 0, nullptr, 4);
		SetEventHandler("JVO:Reset", Reset);
		Reset();
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
	}
}