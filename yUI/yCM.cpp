#include <yCM.h>

#include "PluginAPI.h"

yCM g_yCM_Manager;


bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;

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
	
	//	 = "StartMenu/MCM/*:" + std::to_string(child) + "/*:" + std::to_string(grandchild) + "/" + std::string(src);

	/*	if (child == 0 && grandchild == 0 && std::string(src) == "_MCM")
		{
			*result = 1;
			path = path + " " + std::to_string(*result);
			Console_Print(path.c_str());
			return true;
		}

		if (child == 0 && grandchild == 0 && std::string(src) == "_ActiveMod")
		{
			const ModInfo* pModInfo = g_dataHandler->LookupModByName("The Mod Configuration Menu.esp");
			*result = pModInfo->modIndex;
			path = path + " " + std::to_string(*result);
			Console_Print(path.c_str());
			return true;
		}
		*/
	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) *result = val->num;
	else
	{
		path = "GetMCMFloat " + std::to_string(child) + " " + std::to_string(grandchild) + " " + std::string(src);
		Console_Print(path.c_str());
	}

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
//	Console_Print("%d %d %d", child, grandchild, value);
//	Console_Print(src);
	std::string path = "StartMenu/MCM/";
	if (child == 0)
	{
		if (std::string(src) == "_ActiveMod")
		{
			g_yCM_Manager.activeMod = value;
		}
		if (std::string(src) == "_ActiveSubMenu")
		{
			g_yCM_Manager.activeSubMenu = value;
		}
		if (std::string(src) == "_ActiveOption")
		{
			g_yCM_Manager.activeSubMenu = value;
		}
	}
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

	auto path2 = path;
	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) {
		*result = 1;
		val->parent->SetFloat(val->id, value);
	} else
	{
//		Console_Print(path2.c_str());
		path = "SetMCMFloat " + std::to_string(child) + " " + std::to_string(grandchild) + " " + std::string(src) + " " + std::to_string(value);
		Console_Print(path.c_str());
	}
	return true;
}

yCM_Option_Value::yCM_Option_Value()
= default;

yCM_Option_Value::~yCM_Option_Value()
= default;

extern NVSECommandTableInterface* g_commandInterface;

void writeMCMHooks()
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
}
