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

ParamInfo kParams_TwoStrings[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
};
