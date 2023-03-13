#pragma once
#include "StewMenuSubsettingItem.h"
#include "Stootils.h"
#include "Utilities.h"

struct StewMenuItem
{
	StewMenuItem(const char* name, const char* description, signed int priority, const char* internalName, const char* internalCategory, const char* category)
	{
		this->name = (name);
		this->description = (description);
		this->internalName = (internalName);
		this->internalCategory = (internalCategory);
		this->category = (category);
		this->SetPriority(priority);
		this->value = -1;
		subsettings.Init(nullptr);
	}

	std::string name;
	std::string description;
	std::string internalName;
	std::string internalCategory;
	std::string category;
	signed int priority;
	signed int value;
	SubsettingList subsettings;

	void Free()
	{
		subsettings.Destroy();
	}

	const char* GetName() { return name.c_str(); };
	const char* GetDescription() { return description.c_str(); };
	const char* GetInternalName() { return internalName.c_str(); };
	const char* GetInternalCategory()
	{
		// if there's no internal category set, assume it's tweaks - done to avoid needlessly creating ~400 'Tweaks' strings since most tweak items will have that category
		if (!internalCategory.empty()) return internalCategory.c_str();
		return "Tweaks";
	};
	const char* GetCategory() { return category.c_str(); };
	signed int GetPriority() { return priority; };
	void SetPriority(signed int _priority) { this->priority = _priority; };
	bool IsBoolean() { return !internalName.empty() && internalName[0] == 'b'; };
	SubsettingList* GetSubsettings() { return &subsettings; };
	void AddSubsetting(StewMenuSubsettingItem* item) { subsettings.Append(item); };

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}
};
