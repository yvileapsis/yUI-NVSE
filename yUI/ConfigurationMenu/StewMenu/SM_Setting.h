#pragma once
#include "Containers.h"
#include "Utilities.h"

struct SubSettingData
{
	struct DropdownData
	{
		std::string displayName;
		signed int value;
		std::string description;
	};

	enum ElementType
	{
		kSettingType_Boolean = 0,
		kSettingType_Slider,
		kSettingType_StringInput,
		kSettingType_NumberInput,
		kSettingType_Hotkey,
		kSettingType_Options,
	};

	enum
	{
		kSettingDataType_String,
		kSettingDataType_Integer,
		kSettingDataType_Float,
	};

	static ElementType StrToType(char c)
	{
		switch (c | 0x20)
		{
		case 'b': return kSettingType_Boolean;
		case 's': return kSettingType_Slider;
		case 'i': return kSettingType_StringInput;
		case 'n': return kSettingType_NumberInput;
		case 'o': return kSettingType_Options;
		case 'h': return kSettingType_Hotkey;
		}
		return kSettingType_Boolean;
	}

	static const char* SettingTypeToTemplate(signed int type)
	{
		switch (type)
		{
		case kSettingType_Boolean:
			return "SubSettingTemplate_Boolean";
		case kSettingType_Slider:
			return "SubSettingTemplate_Slider";
		case kSettingType_StringInput:
		case kSettingType_NumberInput:
		case kSettingType_Hotkey:
			return "SubSettingTemplate_Input";
		case kSettingType_Options:
			return "SubSettingTemplate_Dropdown";
		}
		return nullptr;
	}

	void Init()
	{
		type = kSettingType_Boolean;
		valueInt = 0;
		valueStr.clear();
		minValue = 0;
		maxValue = 0;
	}

	SubSettingData() {};

	void Free()
	{
		if (type == kSettingType_Options)
		{
			options.DeleteAll();
		}
	}

	ElementType type;
	union
	{
		float valueFloat;
		int valueInt;
		void* value;
	};
	union
	{
		struct
		{
			float minValue;
			float maxValue;
		};
		TList<DropdownData> options;
	};
	std::string valueStr;

	void SetType(ElementType _type) { this->type = _type; }
	signed int GetElementType() { return this->type; };
	bool IsInputField() { return type == kSettingType_NumberInput || type == kSettingType_StringInput; };
	bool IsHotkeyField() { return type == kSettingType_Hotkey; };
	bool IsCheckboxField() { return type == kSettingType_Boolean; };
	bool IsSlider() { return type == kSettingType_Slider; };
	bool IsDropdown() { return type == kSettingType_Options; };
	void AddOption(const char* displayString, signed int _value, const char* description)
	{
		auto option = (DropdownData*)GameHeapAlloc(sizeof(DropdownData));
		option->displayName = (displayString);
		option->description = (description);
		option->value = _value;
		this->options.Insert(option);
	};
	const char* GetDropdownValue()
	{
		auto iter = options.Head();
		if (iter)
		{
			do
			{
				if (iter->data->value == valueInt) { return iter->data->displayName.c_str(); }
			}
			while (iter = iter->next);
		}
		return "";
	}
	const char* GetDropdownDescription()
	{
		auto iter = options.Head();
		if (iter)
		{
			do
			{
				if (iter->data->value == valueInt) { return iter->data->description.c_str(); }
			} while (iter = iter->next);
		}
		return "";
	}

	void DropdownSelectNext(bool isNext)
	{
		auto iter = options.Head();
		auto prev = options.GetLastNode();
		if (iter)
		{
			do
			{
				if (iter->data->value == valueInt)
				{
					// found currently selected element
					if (isNext)
					{
						if (!(iter = iter->next))
						{
							iter = options.Head();
						}
					}
					else
					{
						iter = prev;
					}
					this->valueInt = iter->data->value;
					return;
				}

				prev = iter;
			}
			while (iter = iter->next);

			// the current value wasn't one of the available options... reset it to the first option
			this->valueInt = options.Head()->data->value;
		}
	}

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}
};

struct SM_Setting
{
	std::string name;
	std::string id;
	std::string description;

	SubSettingData data;

	std::string settingPath;
	std::string settingCategory;
	std::string settingName;


	SM_Setting(std::string name, std::string description, std::string id, std::string internalCategory)
		: name(name), id(id), settingCategory(internalCategory), description(description)
	{
		data.Init();
	}

	void Free()
	{
		data.Free();
	}

	const char* GetTemplate() { return SubSettingData::SettingTypeToTemplate(this->data.type); };
	signed int GetElementType() { return this->data.GetElementType(); };
	signed int GetDataType()
	{
		auto elementType = GetElementType();
		if (elementType == SubSettingData::kSettingType_Hotkey)
		{
			return SubSettingData::kSettingDataType_Integer;
		}
		if (elementType == SubSettingData::kSettingType_StringInput)
		{
			return SubSettingData::kSettingDataType_String;
		}

		if (elementType == SubSettingData::kSettingType_Slider || elementType == SubSettingData::kSettingType_NumberInput)
		{
			if (id[0] == 'f')
			{
				return SubSettingData::kSettingDataType_Float;
			}
		}

		return SubSettingData::kSettingDataType_Integer;
	};
	bool IsInputField() { return this->data.IsInputField(); };
	bool IsCheckboxField() { return this->data.IsCheckboxField(); };
	bool IsSlider() { return this->data.IsSlider(); };
	bool IsDropdown() { return this->data.IsDropdown(); };
	bool IsHotkeyField() { return this->data.IsHotkeyField(); };

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}
};

struct SubsettingList : TList<SM_Setting>
{
	void Destroy()
	{
		for (auto iter : *this)
		{
			auto item = iter;
			item->Free();
		}
		this->DeleteAll();
	}
};