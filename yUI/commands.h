#pragma once
//#include <ParamInfos1.h>
#include <GameAPI.h>
#include <GameObjects.h>
#include <CommandTable.h>
#include <file.h>
#include <functions.h>

extern std::unordered_map <TESForm*, std::string> g_SI_Items;
extern std::unordered_map <std::string, JSONEntryTag> g_SI_Tags;
extern bool				(*AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);

#define GetExtraType(xDataList, Type) (Extra ## Type*)xDataList.GetByType(kExtraData_ ## Type)

ParamInfo kParams_OneString_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};

DEFINE_COMMAND_PLUGIN(ySIGetTrait, , 0, 2, kParams_OneString_OneOptionalForm);

bool Cmd_ySIGetTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100];
	TESForm* form = nullptr;
	AssignString(PASS_COMMAND_ARGS, "");
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &form)) return true;
	if (!form && thisObj) form = thisObj->baseForm;
	if (!form) return true;
	const auto tochange = std::string(src);
	if (tochange == "tag" || tochange == "string") {
		const std::string tag = g_SI_Items[form];
		AssignString(PASS_COMMAND_ARGS, tag.c_str());
	} else if (tochange == "icon" || tochange == "filename") {
		const std::string tag = g_SI_Items[form];
		const std::string icon = g_SI_Tags[tag].filename;
		AssignString(PASS_COMMAND_ARGS, icon.c_str());
	}
	return true;
}

ParamInfo kParams_TwoStrings_OneOptionalForm[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
	{"Form", kParamType_AnyForm, 1},
};

DEFINE_COMMAND_PLUGIN(ySISetTrait, , 0, 3, kParams_TwoStrings_OneOptionalForm);

bool Cmd_ySISetTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100];
	char newstring[0x100];
	TESForm* form = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &newstring, &form)) return true;
	if (!form && thisObj) form = thisObj->baseForm;
	if (!form) return true;
	const auto tochange = std::string(src);
	if (tochange == "tag" || tochange == "string") {
		g_SI_Items[form] = std::string(newstring);
		*result = 1;
	} else if (tochange == "icon" || tochange == "filename") {
		const std::string tag = g_SI_Items[form];
		g_SI_Tags[tag].filename = std::string(newstring);
		*result = 1;
	}
	return true;
}

ParamInfo kParams_TwoStrings[] =
{
	{"String", kParamType_String, 0},
	{"String", kParamType_String, 0},
};


/*
DEFINE_COMMAND_PLUGIN(SwapTexatlas, , 0, 2, kParams_TwoStrings);

bool Cmd_SwapTexatlas_Execute(COMMAND_ARGS)
{
	*result = 0;
	char texatlas[0x100];
	char texatlasnew[0x100];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &texatlas, &texatlasnew)) return true;
	SetUIStringFull(texatlas, texatlasnew, kTileValue_texatlas);
	return true;
}

ParamInfo kParams_OneWorldspace_OneFloat[] =
{
	{"Worldspace", kParamType_WorldSpace, 0},
	{"Float", kParamType_Float, 0},
};

DEFINE_COMMAND_PLUGIN(SetWorldspaceDefaultWaterHeight, , 0, 2, kParams_OneWorldspace_OneFloat);

bool Cmd_SetWorldspaceDefaultWaterHeight_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace* worldspace = nullptr;
	float newwaterheight = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &worldspace, &newwaterheight)) return true;
	worldspace->defaultWaterHeight = newwaterheight;
	return true;
}
*/