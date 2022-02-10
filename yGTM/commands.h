#pragma once
#include "CommandTable.h"

static ParamInfo kParams_OneOptionalInt[1] =
{
	{	"int", kParamType_Integer, 1 },
};
DEFINE_COMMAND_PLUGIN(GetGlobalTimeMultiplierAlt, , 0, 2, kParams_OneOptionalInt);

static ParamInfo kParams_OneOptionalFloat_OneOptionalInt[2] =
{
	{	"float", kParamType_Float, 1 },
	{	"int", kParamType_Integer, 1 },
};
DEFINE_COMMAND_PLUGIN(SetGlobalTimeMultiplierAlt, , 0, 2, kParams_OneOptionalFloat_OneOptionalInt);