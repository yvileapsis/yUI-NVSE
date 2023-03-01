#include "main.h"
#include "Menus.h"
#include "PluginAPI.h"
#include <string>

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
	if (const auto val = StringToTilePath(path.c_str())) *result = val->num;
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
	if (const auto val = StringToTilePath(path.c_str())) {
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
	if (const auto val = StringToTilePath(path.c_str())) AssignString(PASS_COMMAND_ARGS, val->str);
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
	if (const auto val = StringToTilePath(path.c_str())) {
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
	if (const auto val = StringToTilePath(src)) {
		*result = 1;
		val->parent->SetFloat(val->id, value);
	}
	return true;
}

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

