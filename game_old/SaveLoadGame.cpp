#include <SaveLoadGame.h>

std::string GetSavegamePath()
{
	char path[0x104] = "\0";
	SaveGameManager::GetSingleton()->ConstructSavegamePath(path);
	return path;
}