#pragma once
#include <CommandTable.h>

ParamInfo kParams_OneString_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};
DEFINE_COMMAND_PLUGIN(ySIGetTrait, , 0, 2, kParams_OneString_OneOptionalForm);

ParamInfo kParams_TwoStrings_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};
DEFINE_COMMAND_PLUGIN(ySISetTrait, , 0, 3, kParams_TwoStrings_OneOptionalForm);

/*
ParamInfo kParams_TwoStrings[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
};
DEFINE_COMMAND_PLUGIN(SwapTexatlas, , 0, 2, kParams_TwoStrings);
*/

/*
ParamInfo kParams_OneWorldspace_OneFloat[] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Float", kParamType_Float, 0},
};

DEFINE_COMMAND_PLUGIN(SetWorldspaceDefaultWaterHeight, , 0, 2, kParams_OneWorldspace_OneFloat);
*/