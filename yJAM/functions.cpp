#include <functions.h>

#include <GameData.h>
#include <PluginAPI.h>

#include <dinput8.h>

extern DataHandler* g_dataHandler;
extern NVSEScriptInterface* g_scriptInterface;
extern NVSEArrayVarInterface* g_arrayInterface;
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

Script* JIPGetAuxVarOrDefault;
Float32 GetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 def)
{
	if (!JIPGetAuxVarOrDefault) JIPGetAuxVarOrDefault = CompileScript(R"(Begin Function { string_var auxvar, int index, int def }
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