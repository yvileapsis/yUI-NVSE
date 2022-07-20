#include <JVO.h>

#include <GameData.h>
#include <GameUI.h>
#include <ranges>
#include <functions.h>

#include <SimpleINILibrary.h>
#include <unordered_set>

namespace JVO
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;
	
	SInt64				visible				= 0;
	UInt32				depth				= 0;

	std::unordered_set<Tile*>					g_UsefulTiles;
	std::unordered_map<Tile*, NiPoint3>			g_UselessTiles;

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

//		g_JDC = ini.GetOrCreate("JustMods", "JDC", 1.0, nullptr);
//		g_JDC_Dynamic = ini.GetOrCreate("JDC", "Dynamic", 1.0, nullptr);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (tileMain->GetChild("JVO")) tileMain->GetChild("JVOContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JVOContainer");

		tileMain->SetFloat(TraitNameToID("_JVODistanceVisible"), g_JVO_DistanceHandling);
		tileMain->SetFloat(TraitNameToID("_JVOTextVisible"), g_JVO_TextHandling);

		tileMain->SetFloat(TraitNameToID("_JVOAlphaBase"), g_JVO_Alpha);
		tileMain->SetFloat(TraitNameToID("_JVOAlphaMult"), g_JVO_AlphaMult);
		tileMain->SetFloat(TraitNameToID("_JVORadiusMax"), g_JVO_Radius);

		tileMain->SetFloat(TraitNameToID("_JVOWidthBase"), g_JVO_Width);
		tileMain->SetFloat(TraitNameToID("_JVOHeightBase"), g_JVO_Height);
		tileMain->SetFloat(TraitNameToID("_JVOOffsetWidth"), g_JVO_OffsetWidth);
		tileMain->SetFloat(TraitNameToID("_JVOOffsetHeight"), g_JVO_OffsetHeight);

		tileMain->SetFloat(TraitNameToID("_JVOFontBase"), g_JVO_Font);
		tileMain->SetFloat(TraitNameToID("_JVOFontYBase"), g_JVO_FontY);
	}

	void OnRender()
	{
		for (auto& [fst, snd] : g_UselessTiles)
		{
			NiPoint3 out;
			const bool onScreen = JG_WorldToScreen(&snd, out, g_JVO_OffscreenHandling);
			fst->SetFloat(kTileValue_visible, onScreen && (g_JVO_OffscreenHandling != 2));
			fst->SetFloat(TraitNameToID("_X"), out.x);
			fst->SetFloat(TraitNameToID("_Y"), out.y);
		}
	}

	void Initialize()
	{
		initialized = true;
		tileMain = g_MenuHUDMain->tile->GetChild("JVO");
		if (!tileMain)
		{
			g_MenuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JVO\JVO.xml)");
			tileMain = g_MenuHUDMain->tile->GetChild("JVO");
		}
		if (!tileMain) return;
		RegisterEvent("JVO:Reset", 0, nullptr, 4);
		SetNativeEventHandler("JVO:Reset", reinterpret_cast<EventHandler>(Reset));
		DispatchEvent("JVO:Reset", nullptr);
		SetNativeEventHandler("yJAM:JG:OnRender", reinterpret_cast<EventHandler>(OnRender));
	}

	Tile* CreateTileForVisualObjective(NiPoint3 target)
	{
		Tile* tile;
		if (g_UsefulTiles.empty()) {
			tile = tileMain->GetChild("JVOContainer")->AddTileFromTemplate("JVOMarker");
		}
		else {
			const auto iter = g_UsefulTiles.begin();
			tile = *iter;
			g_UsefulTiles.erase(iter);
		}
		g_UselessTiles[tile] = target;
		return tile;
	}

	void AddVisualObjective(UInt32 altMarker, TESObjectREFR* ref, TESObjectREFR* linkDoor = nullptr)
	{
		auto target = ref;
		if (linkDoor) target = linkDoor;

		NiPoint3 in = *target->GetPos();
		if (in.z == 0)
		{
			NiPoint2 ni2{ in.x, in.y };
			Float32 ni1;
			TES::GetSingleton()->GetTerrainHeight(&ni2, &ni1);
			in.z = ni1 > 0 ? ni1 : g_player->GetPos()->z;
		}
		if (target->GetNiNode())
		{
			if (const auto niblock = target->GetNifBlock(0, "Bip01 Head"))
			{
				in = niblock->m_worldTransform.pos;
				in.z += 20;
			}
			else
			// TODO: real GetObjectDimensions
			// if (const auto bounds = target->GetBoundingBox())
			//(bounds->centre.z + target->GetScale() * bounds->dimensions.z)
				in.z += 0.5 * GetObjectDimensions(target) + 20;
		}

		const auto tile = CreateTileForVisualObjective(in);

		// TODO: fix this so it doesn't rely on altMarker but rather somehow checks for target to be player marker
		const auto distance = !altMarker ? g_player->GetDistance(target) : g_player->GetDistance2D(target);

		bool inDistance = true;
		if (g_JVO_DistanceMax >= 0 && distance > g_JVO_DistanceMax) inDistance = false;
		if (g_JVO_DistanceMin >= 0 && distance < g_JVO_DistanceMin) inDistance = false;

		const bool inFocus = tile->GetValueFloat(TraitNameToID("_JVOInFocus"));


		if (g_JVO_AltColor) tile->SetFloat(TraitNameToID("_JVOHostile"), target->IsCrimeOrEnemy());

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
		tile->SetString(TraitNameToID("_JVODistance"), string.c_str());

		tile->SetFloat(TraitNameToID("_JVOInDistance"), inDistance);

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
		tile->SetString(TraitNameToID("_JVOText"), string.c_str());

		tile->SetFloat(TraitNameToID("_JVOAltMarker"), altMarker);
	}

	void MainLoop()
	{
		if (!initialized) return;

		if (GetPCUsingScope()) visible = -1 * static_cast<SInt64>(g_JVO_EnableScope);
		else if (GetPCUsingIronSights()) visible = g_JVO_EnableSighting;
		else visible = g_JVO_EnableOut;
		tileMain->SetFloat(TraitNameToID("_JVOVisible"), visible);

		if (!visible) return;

		tileMain->SetFloat(TraitNameToID("_JVOInCombat"), g_player->pcInCombat);
		tileMain->SetFloat(TraitNameToID("_JVOAlphaCW"), g_MenuHUDMain->tileHitPointsCompass->GetValueFloat(kTileValue_alpha));

		const auto targets = g_player->GetCurrentQuestObjectiveTargets();
		const auto target = g_player->GetPlacedMarkerOrTeleportLink();

		for (const auto fst : g_UselessTiles | std::views::keys) { g_UsefulTiles.emplace(fst); fst->SetFloat(kTileValue_visible, 0); }
		g_UselessTiles.clear();

		if (target)						AddVisualObjective(1, target);
		for (const auto i : *targets)	AddVisualObjective(0, i->target, i->teleportLinks.size ? i->teleportLinks.data->door : nullptr);
	}
}
