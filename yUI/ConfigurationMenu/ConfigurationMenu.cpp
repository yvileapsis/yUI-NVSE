#include "ConfigurationMenu.h"

#include "main.h"
#include "PluginAPI.h"
#include "SimpleINILibrary.h"

inline int g_FixReorderMCM = 1;

yCM g_yCM_Manager;

std::string MCMPath(UInt32 child, UInt32 grandchild, std::string src)
{

	std::string path = "StartMenu/MCM/";
	if (child == 0) {}
	else if (child == 1) {
		path += "MCM_Options/";
		if (grandchild != 0) {
			path += "Option" + std::to_string(grandchild) + "/";
		}
	}
	else if (child == 2) {
		path += "MCM_Scale/";
	}
	else if (child == 3) {
		path += "MCM_List/";
		if (grandchild != 0) {
			path += "Item" + std::to_string(grandchild) + "/";
		}
	}
	else if (child == 4) {
		path += "MCM_Trigger/";
	}
	else if (child == 5) {
		path += "MCM_ModList/";
		if (grandchild == 0) {}
		else if (grandchild <= 23) {
			path += "Mod" + std::to_string(grandchild) + "/";
		}
		else {
			path += "SubMenu" + std::to_string(grandchild - 23) + "/";
		}
	}
	else if (child == 9) {
		path += "MCM_Info/";
	}
	else if (child == 15) {
		path += "MCM_Input/";
	}
	else if (child == 17) {
		path += "MCM_Images/";
	}
	path += std::string(src);
	return path;
}


bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;
	std::string path = MCMPath(child, grandchild, src);
	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) *result = val->num;
	return true;
}

bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	float value = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src, &value)) return true;
	std::string path = MCMPath(child, grandchild, src);
	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) {
		*result = 1;
		val->parent->SetFloat(val->id, value);
	}
	return true;
}

bool Cmd_GetyCMString_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;
	std::string path = MCMPath(child, grandchild, src);
	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) AssignString(PASS_COMMAND_ARGS, val->str);
	return true;
}

UInt32 SetyCMStringParams;
bool Cmd_SetyCMString_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	char buffer[0x80];
	if (!ExtractFormatStringArgs(3, buffer, EXTRACT_ARGS_EX, SetyCMStringParams, &child, &grandchild, &src)) return true;
	std::string path = MCMPath(child, grandchild, src);
	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) {
		*result = 1;
		val->parent->SetString(val->id, buffer);
	}
	return true;
}

bool Cmd_SetUIFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100] = "\0";
	float value = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &value)) return true;
	if (const auto val = InterfaceManager::GetMenuComponentValue(src)) {
		*result = 1;
		val->parent->SetFloat(val->id, value);
	}
	return true;
}

yCM_Option_Value::yCM_Option_Value()
= default;

yCM_Option_Value::~yCM_Option_Value()
= default;

extern NVSECommandTableInterface* g_commandInterface;

void WriteMCMHooks()
{
/*
	RegisterCommand GetModINISetting (21C0)
	RegisterCommand SetModINISetting (21C1)
	RegisterCommand GetMCMFloat (21C2)
	RegisterCommand SetMCMFloat (21C3)
	RegisterCommand SetMCMString (21C4)
	RegisterCommand SetMCMFloatMass (21C5)
	RegisterCommand SetMCMStringMass (21C6)
	RegisterCommand SetMCMModList (21C7)
	RegisterCommand GetMCMListWidth (21C8)
 */

	g_commandInterface->GetByOpcode(0x21C2)->execute = Cmd_GetyCMFloat_Execute;
	g_commandInterface->GetByOpcode(0x21C3)->execute = Cmd_SetyCMFloat_Execute;
	g_commandInterface->GetByOpcode(0x21C4)->execute = Cmd_SetyCMString_Execute;
	SetyCMStringParams = g_commandInterface->GetByOpcode(0x21C4)->numParams;

	g_commandInterface->GetByOpcode(5284)->execute = g_commandInterface->GetByOpcode(5663)->execute;
	g_commandInterface->GetByOpcode(5285)->execute = g_commandInterface->GetByOpcode(5664)->execute;
	g_commandInterface->GetByOpcode(5286)->execute = g_commandInterface->GetByOpcode(5665)->execute;
	g_commandInterface->GetByOpcode(5346)->execute = g_commandInterface->GetByOpcode(5665)->execute;

}

std::unordered_map<std::string, UInt32> g_MCMChildOrder{
	{"MCM_Version", 0}, {"MCM_RGB", 25}, {"MCM_Images", 2}, {"MCM_Return", 3}, {"MCM_Input", 4}, {"MCM_Font", 5},
	{"MCM_Highlight", 6}, {"MCM_PressControl", 7}, {"MCM_Defaults", 8}, {"MCM_Back", 9}, {"MCM_Info", 10},
	{"MCM_ModTitle", 11}, {"MCM_Bracket", 12}, {"MCM_BG", 13}, {"MCM_ModList", 14}, {"MCM_Trigger", 15},
	{"MCM_List", 16}, {"MCM_Scale", 17}, {"MCM_Options", 18}
};

int SortMCMChildren(Tile* tile1, Tile* tile2)
{
	const auto pos1 = g_MCMChildOrder[std::string(tile1->name.CStr())];
	const auto pos2 = g_MCMChildOrder[std::string(tile2->name.CStr())];
	if (pos1 > pos2) return -1;
	if (pos1 < pos2) return 1;
	return 0;
}

std::unordered_map<std::string, UInt32> g_ValueChildOrder{
	{"RGB", 25}, {"toggle_check", 0}, {"toggle", 1}, {"suffix", 2}, {"prefix", 3}, {"text", 4}
};

int SortValueChildren(Tile* tile1, Tile* tile2)
{
	const auto pos1 = g_ValueChildOrder[std::string(tile1->name.CStr())];
	const auto pos2 = g_ValueChildOrder[std::string(tile2->name.CStr())];
	if (pos1 > pos2) return -1;
	if (pos1 < pos2) return 1;
	return 0;
}

bool fixReorderMCM = false;

void FixReorderMCM()
{
	if (StartMenu::GetSingleton())
	{
		if (fixReorderMCM)
		{
			fixReorderMCM = false;
			const auto tileMCM = StartMenu::GetSingleton()->tile->GetChild("MCM");
			tileMCM->children.Sort(SortMCMChildren);

			const auto tileMCMOptions = tileMCM->GetChild("MCM_Options");

			for (auto iter : tileMCMOptions->children)
			{
				if (iter->GetComponentValue("_optionID"))
				{
					const auto tileValue = iter->GetChild("value");
					tileValue->children.Sort(SortValueChildren);
				}
			}
		}
	}
	else fixReorderMCM = true;
}

StartMenuOption* yCM1;

void __cdecl yCMCallback()
{
	MenuButton_DownloadsClick();
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

void HandleINIs()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	auto iniPath = GetCurPath() + yUI_INI;
	if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }
	g_FixReorderMCM = ini.GetOrCreate("General", "bFixReorderMCM", 1, "; fix the issues where MCM is incompatible with newest xNVSE features like Get/SetUIFloatAlt commands and inline expressions. This is fixed by reordering MCM's .xml in-code to work with these commands and hooking deprecated commands to work through Get/SetUIFloatAlt.");
	if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	iniPath = GetCurPath() + R"(\Data\Config\MCMFix.ini)";
	if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }
	if (!g_FixReorderMCM) g_FixReorderMCM = ini.GetLongValue("General", "bFixReorderMCM", 0);
	if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }
}

namespace Fix::ReorderMCM
{
	void Init()
	{
		if (g_FixReorderMCM) FixReorderMCM();
		mainLoop.emplace_back(FixReorderMCM);
	}
}