#pragma once
#include "CommandTable.h"


static ParamInfo kParams_OneForm_OneOptionalForm[5] =
{
	{	"weapon", kParamType_AnyForm, 0 },
	{	"node", kParamType_String, 1 },
	{	"ammo", kParamType_AnyForm, 1 },
	{	"mods", kParamType_Integer, 1 },
	{	"condition", kParamType_Float, 1 }
};
DEFINE_COMMAND_PLUGIN(FireWeaponAlt, , 0, 2, kParams_OneForm_OneOptionalForm);