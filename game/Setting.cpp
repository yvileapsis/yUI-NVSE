#include "Setting.h"

UInt32 Setting::GetType()
{
	if (!name) return kSetting_Other;

	switch(name[0]) 
	{
		case 'b': 
		case 'B': return kSetting_Bool;
		case 'c': 
		case 'C': return kSetting_Char;
		case 'i': 
		case 'I': return kSetting_Integer;
		case 'u': 
		case 'U': return kSetting_Unsigned;
		case 'f': 
		case 'F': return kSetting_Float;
		case 's':
		case 'S': return kSetting_String;
		case 'r': 
		case 'R': return kSetting_r;
		case 'a': 
		case 'A': return kSetting_a;
		default: return kSetting_Other;
	}
}

Float32 Setting::GetAsFloat()
{
	switch (GetType())
	{
	case kSetting_Bool:
	case kSetting_Integer: return data.i;
	case kSetting_Unsigned: return data.uint;
	case kSetting_Float: return data.f;
	default:
		return false;
	}
}

std::string Setting::GetAsString()
{
	if (GetType() == kSetting_String) return data.str;
	return "";
}

bool Setting::Set(double newVal)
{
	switch (GetType())
	{
	case kSetting_Bool:
		data.i = newVal ? 1 : 0;
		return true;
	case kSetting_Integer:
		data.i = newVal;
		return true;
	case kSetting_Unsigned:
		data.uint = newVal;
		return true;
	case kSetting_Float:
		data.f = newVal;
		return true;
	default:
		return false;
	}
}

bool Setting::Set(const char* str)
{
	if (GetType() == kSetting_String)
	{
		char * newVal = static_cast<char*>(GameHeapAlloc(strlen(str) + 1));
		if (!strcpy_s(newVal, strlen(str)+1, str)) {
			data.str = newVal;
			return true;
		}
	}
	return false;
}

GameSettingCollection * GameSettingCollection::GetSingleton()
{
	return *(GameSettingCollection**)0x011C8048;
}

INISettingCollection* INISettingCollection::GetSingleton()
{
	return *(INISettingCollection**)0x011F96A0;
}

INIPrefSettingCollection* INIPrefSettingCollection::GetSingleton()
{
	return *(INIPrefSettingCollection**)0x011F35A0;
}

class IniSettingFinder
{
public:
	const char* m_settingName;
	IniSettingFinder(const char* name) : m_settingName(name)
		{	}
	bool Accept(Setting* info)
	{
		return !StrCompare(m_settingName, info->name);
	}
};

Setting* GetINISetting(const std::string& settingName)
{
	Setting* setting = nullptr;
	IniSettingFinder finder(settingName.c_str());

	// check prefs first
	const auto coll = INIPrefSettingCollection::GetSingleton();
	if (coll) setting = coll->settingList.Find(finder);

	if (!setting)
	{
		const auto coll = INISettingCollection::GetSingleton();
		if (coll) setting = coll->settingList.Find(finder);
	}

	return setting;
}

Setting* GetGameSetting(const std::string& settingName)
{
	for (const auto iter : GameSettingCollection::GetSingleton()->settingMap)
		if (std::string(iter->key) == settingName) return iter->data;
	return nullptr;
}
