#include <functions.h>

#include <GameData.h>
#include <PluginAPI.h>

#include <dinput8.h>

extern DataHandler* g_dataHandler;
extern NVSEScriptInterface* g_scriptInterface;
extern NVSEArrayVarInterface* g_arrayInterface;
extern NVSEEventManagerInterface* g_eventInterface;
extern DIHookControl* g_DIHook;

void InitFunctions()
{
	HMODULE jg = GetModuleHandle("johnnyguitar");
	if (jg) JG_WorldToScreen = (_JG_WorldToScreen) GetProcAddress(jg, "JG_WorldToScreen");

	g_MenuHUDMain = HUDMainMenu::GetSingleton();
	g_TileReticleCenter = g_MenuHUDMain->tileReticleCenter;
}

bool IsKeyPressed(UInt32 key, UInt32 flags)
{
	return g_DIHook->IsKeyPressed(key, flags);
}

void DisableKey(UInt32 key, bool disable, UInt32 mask)
{
	g_DIHook->SetKeyDisableState(key, disable, mask);
}

Script* LNIsButtonPressed;
bool IsButtonPressed(UInt32 button)
{
	if (!LNIsButtonPressed) LNIsButtonPressed = CompileScript(R"(Begin Function { int button }
		SetFunctionValue (IsButtonPressed button)
	end)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(LNIsButtonPressed, nullptr, nullptr, &element, 1, button);
	return element.num;
}

UInt32 GetControl(UInt32 whichControl, UInt32 type)
{
	const auto globs = OSInputGlobals::GetSingleton();

	if (whichControl >= globs->kMaxControlBinds)
		return 0xFF;

	UInt32	result;

	switch (type)
	{
	case kControlType_Keyboard:
		result = globs->keyBinds[whichControl];
		break;

	case kControlType_Mouse:
		result = globs->mouseBinds[whichControl];

		if (result != 0xFF) result += 0x100;
		break;

	case kControlType_Joystick:
		result = globs->joystickBinds[whichControl];
		break;

	default:
		result = 0xFF;
		break;
	}

	return result;
}

NVSEArrayElement nativeHandlerFunctionValue{};
void SetNativeHandlerFunctionBool(bool trueorfalse)
{
	nativeHandlerFunctionValue.dataType = NVSEArrayVarInterface::kType_Numeric;
	nativeHandlerFunctionValue.num = trueorfalse;
	g_eventInterface->SetNativeHandlerFunctionValue(nativeHandlerFunctionValue);
}

Script* JIPToggleVanityWheel;
bool ToggleVanityWheel(bool toggle)
{
	if (!JIPToggleVanityWheel) JIPToggleVanityWheel = CompileScript(R"(Begin Function { int toggle }
		ToggleVanityWheel toggle
	end)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPToggleVanityWheel, nullptr, nullptr, &element, 1, toggle);
	return element.num;
}

Script* JIPGetAuxVarOrDefault;
Float32 GetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 def)
{
	if (!JIPGetAuxVarOrDefault) JIPGetAuxVarOrDefault = CompileScript(R"(Begin Function { string_var auxvar, int index, float def }
		if PlayerRef.AuxiliaryVariableGetType (auxvar) index
			SetFunctionValue (PlayerRef.AuxiliaryVariableGetFloat (auxvar) index)
		else
			SetFunctionValue def
		endif
	End)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPGetAuxVarOrDefault, nullptr, nullptr, &element, 3, auxvar, index, *(UInt32*)&def);
	return element.num;
}

Script* JIPSetAuxVarOrDefault;
Float32 SetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 value)
{
	if (!JIPSetAuxVarOrDefault) JIPSetAuxVarOrDefault = CompileScript(R"(Begin Function { string_var auxvar, int index, float value }
		PlayerRef.AuxiliaryVariableSetFloat (auxvar) value index
	End)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPSetAuxVarOrDefault, nullptr, nullptr, &element, 3, auxvar, index, *(UInt32*)&value);
	return element.num;
}

TList<void*> activationPromptTList;
bool GetCannibalPrompt(TESObjectREFR* ref)
{
	activationPromptTList.RemoveAll();
	activationPromptTList.Init();
	ApplyPerkModifiers(0x1B, g_player, ref, &activationPromptTList, ref);
	return activationPromptTList.Empty();
}