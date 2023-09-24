#include <main.h>

#include <SimpleINILibrary.h>

#include <functions.h>
#include <SafeWrite.h>

namespace Fix::TouchpadScrolling
{
	inline int enable = 1;
	SInt32 scrollValue = 120;

	UInt32 keyUp = 264;
	UInt32 keyDown = 265; 

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() / yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bFixTouchpadScrolling", 1, "; fix the issue where New Vegas wouldn't recognize touchpad scrolling");
		scrollValue = ini.GetOrCreate("Touchpad Scrolling", "iScrollValue", 120, "; value by which game scrolls up or down per one scroll event");

		ini.SaveFile(iniPath.c_str(), false);
	}

	SInt32 scrollWheel = 0;


	SInt32 __fastcall GetMousewheel(OSInputGlobals* osinput, void* dummyedx, int a2)
	{
		if (IsKeyPressed(keyUp)) scrollWheel += scrollValue;
		if (IsKeyPressed(keyDown)) scrollWheel -= scrollValue;

		if (scrollWheel < 120 && scrollWheel > -120) return 0;
		const SInt32 result = scrollWheel / 120 * 120;
		scrollWheel = scrollWheel % 120;
		return result;
	}

	void Patch(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelCall(0x70CE9E, GetMousewheel);
			WriteRelCall(0x9459BB, GetMousewheel);
		}
		else
		{
			UndoSafeWrite(0x70CE9E);
			UndoSafeWrite(0x9459BB);
		}
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		Patch(enable);
	}
}
