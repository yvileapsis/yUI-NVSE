#pragma warning(disable: 6054)
#include <file.h>
#include <functions.h>
#include <ySI.h>

extern bool				(*AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);

#define GetExtraType(xDataList, Type) (Extra ## Type*)xDataList.GetByType(kExtraData_ ## Type)

extern bool Cmd_ySIGetTrait_Execute(COMMAND_ARGS)
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
		const std::string tag = SI::GetTagForItem(form);
		AssignString(PASS_COMMAND_ARGS, tag.c_str());
	} else if (tochange == "icon" || tochange == "filename") {
		const std::string tag = SI::GetTagForItem(form);
		const std::string icon = SI::g_Tags[tag].filename;
		AssignString(PASS_COMMAND_ARGS, icon.c_str());
	}
	return true;
}

extern bool Cmd_ySISetTrait_Execute(COMMAND_ARGS)
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
		SI::g_Items[form] = std::string(newstring);
		*result = 1;
	} else if (tochange == "icon" || tochange == "filename") {
		const std::string tag = SI::GetTagForItem(form);
		SI::g_Tags[tag].filename = std::string(newstring);
		*result = 1;
	}
	return true;
}


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