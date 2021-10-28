#pragma once
//#include <ParamInfos1.h>
#include <GameAPI.h>

extern std::unordered_map <TESForm*, std::string> map_ySI_Item;
extern std::unordered_map <std::string, Tile*> map_ySI_Icon;
#define GetExtraType(xDataList, Type) (Extra ## Type*)xDataList.GetByType(kExtraData_ ## Type)

ParamInfo kParams_OneString_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};

DEFINE_COMMAND_PLUGIN(ySIGetTagTrait, , 0, 2, kParams_OneString_OneOptionalForm);

bool Cmd_ySIGetTagTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	//	ScriptVar* string_var;
	//	src = GetStringVar(string_var->data);
	char src[0x100];
	TESForm* form;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &form)) return true;
	if (!form && thisObj) form = thisObj->baseForm;
	//	if (map_ySI_Item.find(form) != map_ySI_Item.end()) {
	std::string tag;
	if (!form) return true;
	//	Console_Print(form->GetTheName());
	tag = map_ySI_Item[form];
	//	Console_Print(tag.c_str());
	if (map_ySI_Icon[tag] && map_ySI_Icon[tag]->GetValueName(src)) {
		AssignString(PASS_COMMAND_ARGS, map_ySI_Icon[tag]->GetValueName(src)->str);
	}

	//	}
	return true;
}