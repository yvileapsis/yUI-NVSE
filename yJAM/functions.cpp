#include <functions.h>
#include <GameUI.h>
#include <GameData.h>

extern DataHandler* g_dataHandler;

void InitFunctions()
{
	g_MenuHUDMain = HUDMainMenu::GetSingleton();
	g_TileReticleCenter = g_MenuHUDMain->tileReticleCenter;
	g_Player = PlayerCharacter::GetSingleton();
}