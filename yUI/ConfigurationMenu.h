#pragma once
#include <GameUI.h>
#include <CommandTable.h>
#include <unordered_set>


bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS);
bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS);

union yCM_Option_Value
{
	bool boolValue;
	float floatValue{};
	struct
	{
		float floatValue1;
		float floatValue2;
		float floatValue3;
	};
	Color rgbValue;
	std::string stringValue;

	yCM_Option_Value();
	~yCM_Option_Value();
};

class yCM_Option
{
public:
	std::string						name;
	bool							enable;
	UInt8							type;
	bool							hightlight;
	yCM_Option_Value*				value;
};

class yCM_SubMenu
{
public:
	std::string									name;
	UInt8										columns;
	UInt8										number;
	std::unordered_map<UInt32, yCM_Option*>		options;
};

class yCM_Mod
{
public:
	std::string									name;
	std::unordered_map<UInt32, yCM_Option*>		submenus;
};

class yCM
{
public:
	UInt32 activeMod = 0;
	UInt32 activeSubMenu = 0;
	UInt32 activeOption = 0;
	std::unordered_map<UInt32, yCM_Option*>			mods;
};

void WriteMCMHooks();
void FixReorderMCM();