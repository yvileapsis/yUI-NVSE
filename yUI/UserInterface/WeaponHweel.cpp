#include <main.h>

#include <Form.h>
#include <Menus.h>
#include <SimpleINILibrary.h>

namespace UserInterface::WeaponHweel
{
	bool		enable = false;

	Tile*		tileMain = nullptr;

	UInt32		key = 35;
	UInt32		button = 128;
	bool		toggle = false;

	UInt32		imod = 4;


	Float64		alpha = 0.0;
	Float64		alphaMult = 0.75;

	Float64		height = 640.0;
	Float64		width = 640.0;

	Float64		offsetHeight = 0.5;
	Float64		offsetWidth = 0.5;

	UInt32 color = 0;

	bool stats = true;
	bool sounds = true;


	UInt32 function9 = 1;
	UInt32 function10 = 2;
	UInt32 function11 = 3;

	bool drop = true;
	Float64 timemult = 0.25;
	Float64 radiusMin = 50;
	Float64 radiusMax = -1;
	bool rumble = true;
	bool autoFill = false;

	UInt32		font = 0;
	Float64		fontY = 0.0;

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bHitMarker", true, "; enable 'Hit Marker' feature. If required files are not found this will do nothing.");

		ini.SaveFile(iniPath.c_str(), false);
	}

	void MainLoop() {


		tileMain->SetFloat(kTileValue_visible, true);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			return;
		}

		mainLoop.push_back(MainLoop);

//		tileMain->SetFloat("_Color", systemcolor);

		tileMain->SetFloat("_AlphaBase", alpha);
		tileMain->SetFloat("_AlphaMult", alphaMult);
		tileMain->SetFloat("_WidthBase", width);
		tileMain->SetFloat("_HeightBase", height);
		tileMain->SetFloat("_OffsetHorizontal", offsetHeight);
		tileMain->SetFloat("_OffsetVertical", offsetWidth);
		tileMain->SetFloat("_FontBase", font);
		tileMain->SetFloat("_FontYBase", fontY);
	}

	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JWH");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\WeaponHweel.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JWH");
		}
		if (!tileMain) return;
		RegisterEvent("JWH:Reset", 0, nullptr, 4);
		SetEventHandler("JWH:Reset", Reset);
		Reset();
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}