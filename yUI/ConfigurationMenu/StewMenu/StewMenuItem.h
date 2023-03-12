#pragma once
#include "StewMenuSubsettingItem.h"
#include "Stootils.h"
#include "Utilities.h"

struct StewMenuItem
{
	StewMenuItem(const char* name, const char* description, signed int priority, const char* internalName, const char* internalCategory, const char* category)
	{
		this->name = GameHeapStrdup(name);
		this->description = GameHeapStrdup(description);
		this->internalName = GameHeapStrdup(internalName);
		this->internalCategory = GameHeapStrdup(internalCategory);
		this->category = GameHeapStrdup(category);
		this->SetPriority(priority);
		this->value = -1;
		subsettings.Init(nullptr);
	}

	const char* name;
	const char* description;
	const char* internalName;
	const char* internalCategory;
	const char* category;
	signed int priority;
	signed int value;
	SubsettingList subsettings;

	void Free()
	{
		if (name) GameHeapFree(name);
		if (description) GameHeapFree(description);
		if (internalName) GameHeapFree(internalName);
		if (internalCategory) GameHeapFree(internalCategory);
		if (category) GameHeapFree(category);
		subsettings.Destroy();
	}

	const char* GetName() { return name; };
	const char* GetDescription() { return description; };
	const char* GetInternalName() { return internalName; };
	const char* GetInternalCategory()
	{
		// if there's no internal category set, assume it's tweaks - done to avoid needlessly creating ~400 'Tweaks' strings since most tweak items will have that category
		if (internalCategory) return internalCategory;
		return "Tweaks";
	};
	const char* GetCategory() { return category; };
	signed int GetPriority() { return priority; };
	void SetPriority(signed int _priority) { this->priority = _priority; };
	bool IsBoolean() { return internalName && *internalName == 'b'; };
	SubsettingList* GetSubsettings() { return &subsettings; };
	void AddSubsetting(StewMenuSubsettingItem* item) { subsettings.Append(item); };

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}
};
