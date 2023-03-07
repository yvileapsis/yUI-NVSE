#include "ConfigurationMenu.h"

#include "main.h"
#include "SafeWrite.h"

struct MenuItem
{
	const char* name;
};

class ConfigurationMenu1 : public Menu
{
	ListBox<MenuItem> listbox;
};


StartMenuOption* yCM1;

void __cdecl yCMCallback()
{
	Log(true, Log::kConsole) << "Test";
//	MenuButton_DownloadsClick();
	//	ThisCall(0x7D74F0, (void *)( 0x11DAAC0 + 0x0B4), 0);
	//	static StartMenu* g_StartMenu = StartMenu::GetSingleton();
	//	((Tile*)(&(g_StartMenu->settingsSubMenu) + 0xC))->SetFloat(TraitNameToID("_enabled"), 1);
	//	g_StartMenu->GetChild("yCM")->GetChild("yCM")->SetFloat(kTileValue_visible, 0, 0);
}

void __fastcall AddyCMToSettingsMenu(BSSimpleArray<StartMenuOption*>* settingsMenuOptions, void* edx, StartMenuOption** menuDownloads)
{
	//		(*HUDColorSetting)->onSelection = HUDColorSettingCallback;
	//	(*menuDownloads)->displayString = "Mods";
	//	(*menuDownloads)->callback = yCMCallback;
	//	(*menuDownloads)->data = StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu;

	yCM1 = StartMenuOption::Create("Mods", yCMCallback, StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu);
	settingsMenuOptions->Append(&yCM1);

	settingsMenuOptions->Append(menuDownloads);


	//yCM = StartMenuOption::Create("Mods", yCMCallback, StartMenuOption::kMainMenu);

}

namespace ConfigurationMenu
{
	void DeferredInit()
	{
		const auto tile = StartMenu::GetSingleton()->tile->InjectUIXML(R"(Data\menus\yCM.xml)");

	}

	void Init()
	{

		WriteRelCall(0x7CC9D4, UInt32(AddyCMToSettingsMenu));
		WriteRelJump(0x7CCA43, 0x7CCAAD);

		deferredInit.emplace_back(DeferredInit);

	//	SafeWriteBuf(0x7CBF77

	//	SafeWriteBuf(0x7CBF8C, "\x66\x0F\x1F\x44\x00\x00", 6);//
	//	SafeWriteBuf(0x7CB674, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB686, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB698, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6AA, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6BC, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6CE, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6E0, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CBE50, "\x81\x7A\x08\x50\x05\x7D\x00", 7);

	/*	SafeWriteBuf(0x7CC044, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC392, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC23E, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC0C3, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC044, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC416, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC582, "\x0F\x1F\x44\x00\x00", 5);*/
	//	SafeWriteBuf(0x7CBF59, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CC01F, "\x0F\x1F\x44\x00\x00", 5);

	}
}