#include <functions.h>

#include <GameData.h>
#include <PluginAPI.h>

extern DataHandler* g_dataHandler;
extern NVSEScriptInterface* g_scriptInterface;
extern NVSEArrayVarInterface* g_arrayInterface;

void InitFunctions()
{
	HMODULE jg = GetModuleHandle("johnnyguitar");
	if (jg)
	{
		JG_WorldToScreen = (_JG_WorldToScreen) GetProcAddress(jg, "JG_WorldToScreen");
	}
	g_MenuHUDMain = HUDMainMenu::GetSingleton();
	g_TileReticleCenter = g_MenuHUDMain->tileReticleCenter;
}

Script* JIPObjectDimensions;

Float32 GetObjectDimensions(TESObjectREFR* object)
{
	if (!JIPObjectDimensions) JIPObjectDimensions = CompileExpression(R"((this).GetObjectDimensions Z)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPObjectDimensions, object, nullptr, &element, 0);
	return element.num;
}