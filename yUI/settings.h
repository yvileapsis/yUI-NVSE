#define INI_NAME "yUI.ini"
#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

int g_ySI = 1;
int g_yCM = 0;

/* plugins */
/*
extern HMODULE yUIHandle;
int GetOrCreateHotkey(CSimpleIniA* ini, char* name, char* comment);

void handleIniOptions()
{
	char iniPath[MAX_PATH];
	GetModuleFileNameA(yUIHandle, iniPath, MAX_PATH);
	strcpy((char*)(strrchr(iniPath, '\\') + 1), INI_NAME);

	CSimpleIniA ini;
	ini.SetUnicode();
	auto errVal = ini.LoadFile(iniPath);
	if (errVal == SI_Error::SI_FILE)
	{
		if (GetModuleHandle(MO2_VFS_DLL_NAME))
		{
			// user is using VFS and will probably not notice the ini if it were to generate in the MO2 overwrites folder, so don't create one
			MessageBoxA(NULL, "Tweaks INI not found, please download it or create an empty file nvse_stewie_tweaks.ini in \'Data\\NVSE\\Plugins\\\' !", "Stewie Tweaks", MB_ICONINFORMATION);
			return;
		}
	}

	ini.SetSortAlphabetically(ini.GetOrCreate("INI", "bSortAlphabetically", 0, "; sort the ini alphabetically"));
	ini.SetPrependNewKeys(ini.GetOrCreate("INI", "bPrependNewSettings", 1, "; add new settings to the top of the ini"));

	ini.SaveFile(iniPath, false);
}

int GetOrCreateHotkey(CSimpleIniA* ini, char* name, char* comment)
{
	int hotkey = ini->GetOrCreate("Hotkeys", name, 0, comment);
	if (hotkey < 0 || hotkey > 255) hotkey = 0;
	return hotkey;
}*/
