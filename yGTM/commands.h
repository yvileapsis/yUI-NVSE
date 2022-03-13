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

static ParamInfo kParams_OneForm_OneOptionalForm[5] =
{
	{	"weapon", kParamType_AnyForm, 0 },
	{	"node", kParamType_String, 1 },
	{	"ammo", kParamType_AnyForm, 1 },
	{	"mods", kParamType_Integer, 1 },
	{	"condition", kParamType_Float, 1 }
};
DEFINE_COMMAND_PLUGIN(FireWeaponAlt, , 0, 2, kParams_OneForm_OneOptionalForm);