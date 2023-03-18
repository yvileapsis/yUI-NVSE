#pragma once
#include <utility>

#include "SM_Setting.h"
#include "Utilities.h"

struct SM_Mod
{
	SM_Mod(std::string name, std::string description, signed int priority, std::string internalName, std::string internalCategory, std::string category)
	: name(std::move(name)), category(std::move(category)), description(std::move(description)), settingCategory(std::move(internalCategory)), settingName(std::move(internalName)),
	priority(priority), value(-1)
	{
	}

	std::string name;
	std::string category;
	std::string description;

	std::string settingPath;
	std::string settingCategory;
	std::string settingName;

	signed int priority;
	signed int value;

	const char* GetInternalName() { return settingName.c_str(); };
	const char* GetInternalCategory()
	{
		// if there's no internal category set, assume it's tweaks - done to avoid needlessly creating ~400 'Tweaks' strings since most tweak items will have that category
		if (!settingCategory.empty()) return settingCategory.c_str();
		return "Tweaks";
	};
	bool IsBoolean() { return !settingName.empty() && settingName[0] == 'b'; };

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}

	~SM_Mod() = default;
};
