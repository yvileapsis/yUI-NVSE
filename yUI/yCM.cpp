#include <yCM.h>

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100];
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;

	std::string path;

	if (child == 0) {
		path = "StartMenu/MCM/" + std::string(src);
	}
	else if (child == 1) {
		if (grandchild == 0) {
			path = "StartMenu/MCM/MCM_Options/" + std::string(src);
		}
		else {
			path = "StartMenu/MCM/MCM_Options/Option" + std::to_string(grandchild) + "/" + std::string(src);
		}
	}
	else if (child == 2) {
		path = "StartMenu/MCM/MCM_Scale/" + std::string(src);
	}
	else if (child == 3) {
		if (grandchild == 0) {
			path = "StartMenu/MCM/MCM_List/" + std::string(src);
		}
		else {
			path = "StartMenu/MCM/MCM_List/Item" + std::to_string(grandchild) + "/" + std::string(src);;
		}
	}
	else if (child == 4) {
		path = "StartMenu/MCM/MCM_Trigger/" + std::string(src);
	}
	else if (child == 5) {
		if (grandchild == 0) {
			path = "StartMenu/MCM/MCM_ModList/" + std::string(src);
		}
		else if (grandchild <= 23) {
			path = "StartMenu/MCM/MCM_ModList/Mod" + std::to_string(grandchild) + "/" + std::string(src);
		}
		else {
			path = "StartMenu/MCM/MCM_ModList/SubMenu" + std::to_string(grandchild - 23) + "/" + std::string(src);
		}
	}
	else if (child == 9) {
		path = "StartMenu/MCM/MCM_Info/" + std::string(src);
	}
	else if (child == 15) {
		path = "StartMenu/MCM/MCM_Input/" + std::string(src);
	}
	else if (child == 17) {
		path = "StartMenu/MCM/MCM_Images/" + std::string(src);
	}
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

	return true;
}

UInt32 activeMod;
UInt32 activeSubMenu;
UInt32 activeOption;

bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100];
	SInt64 child = 0;
	SInt64 grandchild = 0;
	float value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src, &value)) return true;
	Console_Print("%d %d %d", child, grandchild, value);
	Console_Print(src);
	std::string path;

	if (child == 0) {
		path = "StartMenu/MCM/" + std::string(src);

		if (std::string(src) == "_ActiveMod")
		{
			activeMod = value;
		}

		if (std::string(src) == "_ActiveSubMenu")
		{
			activeSubMenu = value;
		}

		if (std::string(src) == "_ActiveOption")
		{
			activeSubMenu = value;
		}
		
	}
	else if (child == 1) {
		if (grandchild == 0) {
			path = "StartMenu/MCM/MCM_Options/" + std::string(src);
		}
		else {
			path = "StartMenu/MCM/MCM_Options/Option" + std::to_string(grandchild) + "/" + std::string(src);
		}


		
	}
	else if (child == 2) {
		path = "StartMenu/MCM/MCM_Scale/" + std::string(src);
	}
	else if (child == 3) {
		if (grandchild == 0) {
			path = "StartMenu/MCM/MCM_List/" + std::string(src);
		}
		else {
			path = "StartMenu/MCM/MCM_List/Item" + std::to_string(grandchild) + "/" + std::string(src);;
		}
	}
	else if (child == 4) {
		path = "StartMenu/MCM/MCM_Trigger/" + std::string(src);
	}
	else if (child == 5) {
		if (grandchild == 0) {
			path = "StartMenu/MCM/MCM_ModList/" + std::string(src);
		}
		else if (grandchild <= 23) {
			path = "StartMenu/MCM/MCM_ModList/Mod" + std::to_string(grandchild) + "/" + std::string(src);
		}
		else {
			path = "StartMenu/MCM/MCM_ModList/SubMenu" + std::to_string(grandchild - 23) + "/" + std::string(src);
		}
	}
	else if (child == 9) {
		path = "StartMenu/MCM/MCM_Info/" + std::string(src);
	}
	else if (child == 15) {
		path = "StartMenu/MCM/MCM_Input/" + std::string(src);
	}
	else if (child == 17) {
		path = "StartMenu/MCM/MCM_Images/" + std::string(src);
	}

	Tile::Value* val = InterfaceManager::GetMenuComponentValue(path.c_str());
	if (val) {
		*result = 1;
		val->parent->SetFloat(val->id, value);
	}
	return true;
}

yCM_Option_Value::yCM_Option_Value()
{
	floatValue = 0;
}

yCM_Option_Value::~yCM_Option_Value()
{
}
