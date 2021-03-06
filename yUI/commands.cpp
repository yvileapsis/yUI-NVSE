#include <SortingIcons.h>

#include "functions.h"
#include "PluginAPI.h"

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
	if (tochange == "category" || tochange == "tag" || tochange == "string") {
		const std::string tag = SI::GetCategoryForItem(form);
		AssignString(PASS_COMMAND_ARGS, tag.c_str());
	} else if (tochange == "icon" || tochange == "filename") {
		const std::string tag = SI::GetCategoryForItem(form);
		const std::string icon = SI::g_Categories[tag].filename;
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
		SI::g_ItemToCategory[form] = std::string(newstring);
		*result = 1;
	} else if (tochange == "icon" || tochange == "filename") {
		const std::string tag = SI::GetCategoryForItem(form);
		SI::g_Categories[tag].filename = std::string(newstring);
		*result = 1;
	}
	return true;
}