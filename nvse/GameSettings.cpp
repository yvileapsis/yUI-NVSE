#include "GameTypes.h"
#include "GameSettings.h"

	GameSettingCollection ** g_GameSettingCollection = (GameSettingCollection**)0x011C8048;
	IniSettingCollection ** g_IniSettingCollection = (IniSettingCollection**)0x011F96A0;
	IniSettingCollection ** g_IniPrefCollection = (IniSettingCollection**)0x011F35A0;

UInt32 Setting::GetType()
{
	if (!name) return kSetting_Other;

	switch(name[0]) 
	{
		case 'b': return kSetting_Bool;
		case 'c': return kSetting_c;
		case 'i': return kSetting_Integer;
		case 'u': return kSetting_Unsigned;
		case 'f': return kSetting_Float;
		case 's':
		case 'S':
			return kSetting_String;
		case 'r': return kSetting_r;
		case 'a': return kSetting_a;
		default:
			return kSetting_Other;
	}
}

bool Setting::Get(double& out)
{
	switch (GetType())
	{
	case kSetting_Bool:
	case kSetting_Integer:
		out = data.i;
		return true;
	case kSetting_Unsigned:
		out = data.uint;
		return true;
	case kSetting_Float:
		out = data.f;
		return true;
	default:
		return false;
	}
}

bool Setting::Get(const char * str)
{
	if (GetType() == kSetting_String)
	{
		str = data.str;
		return true;
	}
	return false;
}

const char * Setting::Get(void)
{
	if (GetType() == kSetting_String)
	{
		return data.str;
	}
	return NULL;
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
		char * newVal = (char*)FormHeap_Allocate(strlen(str)+1);
		if (!strcpy_s(newVal, strlen(str)+1, str)) {
			data.str = newVal;
			return true;
		}
	}
	return false;
}

bool GameSettingCollection::GetGameSetting(const char* settingName, Setting** out)
{
	return CALL_MEMBER_FN(&settingMap, Lookup)(settingName, out); // ***** hippo fix me ****
}

GameSettingCollection * GameSettingCollection::GetSingleton()
{
	return *g_GameSettingCollection;
}

IniSettingCollection* IniSettingCollection::GetSingleton()
{
	return *g_IniSettingCollection;
}

IniSettingCollection* IniSettingCollection::GetIniPrefs()
{
	return *g_IniPrefCollection;
}

class IniSettingFinder
{
public:
	const char* m_settingName;
	IniSettingFinder(const char* name) : m_settingName(name)
		{	}
	bool Accept(Setting* info)
	{
		if (!StrCompare(m_settingName, info->name))
			return true;
		else
			return false;
	}
};

bool GetIniSetting(const char* settingName, Setting** out)
{
	Setting* setting = NULL;
	IniSettingFinder finder(settingName);

	// check prefs first
	IniSettingCollection* coll = IniSettingCollection::GetIniPrefs();
	if (coll)
		setting = coll->settings.Find(finder);

	if (!setting)
	{
		coll = IniSettingCollection::GetSingleton();
		setting = coll->settings.Find(finder);
	}

	if (setting)
	{
		*out = setting;
		return true;
	}

	return false;
}

bool GetNumericGameSetting(char * settingName, double * result)
{
	bool bResult = false;
	*result = -1;

	if (strlen(settingName))
	{
		Setting* setting;
		GameSettingCollection* gmsts = GameSettingCollection::GetSingleton();
		if (gmsts && gmsts->GetGameSetting(settingName, &setting))
		{
			double val;
			if (setting->Get(val))
			{
				*result = val;
				bResult = true;
			}
		};
	}

	return bResult;
}

bool GetNumericIniSetting(const char * settingName, double * result)
{
	bool bResult = false;
	*result = -1;

	if (strlen(settingName))
	{
		Setting* setting;
		if (GetIniSetting(settingName, &setting))
		{
			double val;
			if (setting->Get(val))
			{
				*result = val;
				bResult = true;
			}
		};
	}

	return bResult;
}

Setting* GameSettingFromString(const std::string& settingName)
{
	Setting* setting = nullptr;
	if (const auto gmsts = GameSettingCollection::GetSingleton(); gmsts && gmsts->GetGameSetting(settingName.c_str(), &setting) && setting) return setting;
	return nullptr;
}

std::string GetStringGameSetting(Setting* setting)
{
	if (setting->GetType() == Setting::kSetting_String)	return setting->data.str;
	return "";
}

std::string GetStringFromGameSettingFromString(const std::string& settingName)
{
	if (const auto setting = GameSettingFromString(settingName); setting)
		return GetStringGameSetting(setting);
	return "";
}