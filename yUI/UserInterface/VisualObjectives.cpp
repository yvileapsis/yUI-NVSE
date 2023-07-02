#include <main.h>
#include <Menu.h>

#include <GameData.h>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace UserInterface::VisualObjectives
{

	bool		enable				= false;

	UInt32		key					= 49;
	bool		toggle				= true;

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

	bool		altColor			= true;

	Float64		radius				= 0.06;
	Float64		distanceMin			= -1.0;
	Float64		distanceMax			= -1.0;

	bool		enableOut			= true;
	bool		enableSighting		= true;
	bool		enableScope			= false;

	UInt32		font				= 0;
	Float64		fontY				= 0.0;

	Tile*		tileMain			= nullptr;
	
	SInt8		visible				= 0;
	UInt32		depth				= 0;
	bool		show				= true;
	bool		doonce				= false;

	Float32		offsetHUDMarker		= 0;
	Float32		offsetHUDText		= 0;
	Float32		offsetWorld			= 20;

	Float32		speed				= 0.1;

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
				in = niblock->m_worldTransform.pos;
			else
				in += target->GetCenter();
			in.z += offsetWorld;
		}
		if (in.z == 0)
		{
			NiPoint2 ni2{ in.x, in.y };
			Float32 ni1;
			TES::GetSingleton()->GetTerrainHeight(&ni2, &ni1);
			in.z = ni1 > 0 ? ni1 : g_player->GetPos()->z;
		}
		const auto tile = CreateTileForVisualObjective(in);

		tile->Set("_Hostile", depth++);

		// TODO: fix this so it doesn't rely on altMarker but rather somehow checks for target to be player marker
		const auto distance = !altMarker ? g_player->GetDistance(target) : g_player->GetDistance2D(target);

		bool inDistance = true;
		if (distanceMax >= 0 && distance > distanceMax) inDistance = false;
		if (distanceMin >= 0 && distance < distanceMin) inDistance = false;

		const bool inFocus = static_cast<bool>(tile->Get("_InFocus"));

		if (altColor) tile->Set("_Hostile", target->IsCrimeOrEnemy());

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
		tile->Set("_Distance", string);

		tile->Set("_InDistance", inDistance);

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
		tile->Set("_Text", string);

		tile->Set("_AltMarker", altMarker);
	}

	void MainLoop()
	{
		if (!enable) return;

		const SInt8 oldVisible = visible;

		if (g_HUDMainMenu->isUsingScope)		visible = -1 * static_cast<SInt8>(enableScope);
		else if (g_player->UsingIronSights())	visible = enableSighting;
		else									visible = enableOut;

		if (toggle)
		{
			if (!IsKeyPressed(key)) doonce = true;
			else if (doonce) { doonce = false; show = true - show; }
			if (!show) visible = 0;
		}
		else if (!IsKeyPressed(key)) visible = 0;

//		tileMain->SetFloat("_Visible", visible);

		if (visible != oldVisible) tileMain->SetGradual("_AlphaMult", oldVisible, abs(visible), speed, 0);

		if (!visible) return;

		tileMain->Set("_InCombat", g_player->pcInCombat);
		tileMain->Set("_AlphaCW", HUDMainMenu::GetOpacity());

		offsetHUDMarker = GetJIPAuxVarOrDefault("*_Offset", 0, 0);
		offsetHUDText = GetJIPAuxVarOrDefault("*_Offset", 1, 0);
		offsetWorld = GetJIPAuxVarOrDefault("*_Offset", 2, 20);

		tileMain->Set("_OffsetMarker", offsetHUDMarker);
		tileMain->Set("_OffsetText", offsetHUDText);

		for (const auto fst : tilesInUse | std::views::keys) { tilesFree.emplace(fst); fst->Set(kTileValue_visible, 0); }
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
			fst->Set(kTileValue_visible, onScreen || offscreenHandling != 2);
			fst->Set("_X", out.x);
			fst->Set("_Y", out.y);
		}
	}

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable				= ini.GetOrCreate("General", "bVisualObjectives", 1, "; enable 'Visual Objectives' feature. If required files are not found this will do nothing.");
		key					= ini.GetOrCreate("Visual Objectives", "iKey", 49, nullptr);
		toggle				= ini.GetOrCreate("Visual Objectives", "bToggle", true, nullptr);
		offscreenHandling	= ini.GetOrCreate("Visual Objectives", "iOffscreenHandling", 0, nullptr);
		distanceHandling	= ini.GetOrCreate("Visual Objectives", "iDistanceHandling", 2, nullptr);
		distanceSystem		= ini.GetOrCreate("Visual Objectives", "iDistanceSystem", 1, nullptr);
		textHandling		= ini.GetOrCreate("Visual Objectives", "iTextHandling", 1, nullptr);
		textSystem			= ini.GetOrCreate("Visual Objectives", "iTextSystem", 1, nullptr);
		alpha				= ini.GetOrCreate("Visual Objectives", "fAlpha", 0.0, nullptr);
		alphaMult			= ini.GetOrCreate("Visual Objectives", "fAlphaMult", 0.6, nullptr);
		height				= ini.GetOrCreate("Visual Objectives", "fHeight", 36.0, nullptr);
		width				= ini.GetOrCreate("Visual Objectives", "fWidth", 24.0, nullptr);
		offsetHeight		= ini.GetOrCreate("Visual Objectives", "fOffsetHeight", 0.02, nullptr);
		offsetWidth			= ini.GetOrCreate("Visual Objectives", "fOffsetWidth", 0.01, nullptr);
		altColor			= ini.GetOrCreate("Visual Objectives", "bAltColor", true, nullptr);
		radius				= ini.GetOrCreate("Visual Objectives", "fRadius", 0.06, nullptr);
		distanceMin			= ini.GetOrCreate("Visual Objectives", "fDistanceMin", -1.0, nullptr);
		distanceMax			= ini.GetOrCreate("Visual Objectives", "fDistanceMax", -1.0, nullptr);
		enableOut			= ini.GetOrCreate("Visual Objectives", "bEnableOut", true, nullptr);
		enableSighting		= ini.GetOrCreate("Visual Objectives", "bEnableSighting", true, nullptr);
		enableScope			= ini.GetOrCreate("Visual Objectives", "bEnableScope", false, nullptr);
		font				= ini.GetOrCreate("Visual Objectives", "fFont", 0.0, nullptr);
		fontY				= ini.GetOrCreate("Visual Objectives", "fFontY", 0.0, nullptr);

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

		if (tileMain->GetChild("JVO")) delete tileMain->GetChild("JVOContainer");
		tileMain->AddTileFromTemplate("JVOContainer");

		tileMain->Set("_DistanceVisible", distanceHandling);
		tileMain->Set("_TextVisible", textHandling);

		tileMain->Set("_AlphaBase", alpha);
		tileMain->Set("_AlphaMultInactive", alphaMult);
		tileMain->Set("_RadiusMax", radius);

		tileMain->Set("_WidthBase", width);
		tileMain->Set("_HeightBase", height);
		tileMain->Set("_OffsetWidth", offsetWidth);
		tileMain->Set("_OffsetHeight", offsetHeight);

		tileMain->Set("_FontBase", font);
		tileMain->Set("_FontYBase", fontY);
	}

	void MainLoopDoOnce()
	{
		if (!tileMain)
		{
			tileMain = g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\VisualObjectives.xml)");

			if (!tileMain)
			{
				Log() << "VisualObjectives.xml was not detected despite Visual Objectives being enabled! Visual Objectives will not function.";
				return;
			}
		}
		RegisterEvent("JVO:Reset", 0, nullptr, 4);
		SetEventHandler("JVO:Reset", Reset);
		Reset();
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}