#pragma once
#include <internal/DecodedUI.h>
#include <GameTypes.h>
#include <unordered_set>
#include <utility>
#include "CommandTable.h"

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS);
bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS);

union Color
{
	UInt32 rgba;
	struct
	{
		UInt8 a, b, g, r;
	};
};

union RGB
{
	UInt32 rgb;
	struct
	{
		UInt8 b, g, r;
	};
};

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
	UInt32 activeMod;
	UInt32 activeSubMenu;
	UInt32 activeOption;
	std::unordered_map<UInt32, yCM_Option*>			mods;
};