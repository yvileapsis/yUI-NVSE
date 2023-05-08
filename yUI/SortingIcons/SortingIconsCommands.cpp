#include "SortingIcons.h"

#include <main.h>
#include <TESObjectREFR.h>
#include <PluginAPI.h>

namespace SortingIcons::Commands
{
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

	extern void Register()
	{
		g_nvseInterface->SetOpcodeBase(0x21D0);
		REG_CMD_STR(ySIGetTrait);
		REG_CMD(ySISetTrait);
	}

	extern bool Cmd_ySIGetTrait_Execute(COMMAND_ARGS)
	{
		*result = 0;
		char src[0x100] = "\0";
		TESForm* form = nullptr;
		AssignString(PASS_COMMAND_ARGS, "");
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &form)) return true;
		if (!form && thisObj) form = thisObj->baseForm;
		if (!form) return true;
		const auto tochange = std::string(src);
		if (tochange == "category" || tochange == "tag" || tochange == "string")
			AssignString(PASS_COMMAND_ARGS, Item::Get(form)->tag.c_str());
		else if (tochange == "icon" || tochange == "filename")
			AssignString(PASS_COMMAND_ARGS, Icon::Get(Item::Get(form))->filename.c_str());
		return true;
	}

	extern bool Cmd_ySISetTrait_Execute(COMMAND_ARGS)
	{
		*result = 0;
		char src[0x100] = "\0";
		char newstring[0x100] = "\0";
		TESForm* form = nullptr;
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &newstring, &form)) return true;
		if (!form && thisObj) form = thisObj->baseForm;
		if (!form) return true;
		const auto tochange = std::string(src);
		if (tochange == "tag" || tochange == "string") {
//			Icon::Set(form, Item::Get(newstring));
			*result = 1;
		}
		else if (tochange == "icon" || tochange == "filename") {
//			Icon::Get(form)->filename = std::string(newstring);
			*result = 1;
		}
		return true;
	}
}