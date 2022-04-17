#include <SortingIcons.h>

#include "functions.h"

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
	char src[0x100] = "\0";
	char newstring[0x100] = "\0";
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
bool Cmd_SwapTexatlas_Execute(COMMAND_ARGS)
{
	*result = 0;
	char texatlas[0x100];
	char texatlasnew[0x100];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &texatlas, &texatlasnew)) return true;
	SetUIStringFull(texatlas, texatlasnew, kTileValue_texatlas);
	return true;
}
*/

bool Cmd_SetWorldspaceDefaultWaterHeight_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace* worldspace = nullptr;
	float newwaterheight = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &worldspace, &newwaterheight)) return true;

	for (auto iter : *worldspace->cellMap)
	{
		
	}
	worldspace->defaultWaterHeight = newwaterheight;
	return true;
}