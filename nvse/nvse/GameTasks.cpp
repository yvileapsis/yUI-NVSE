#include "GameTasks.h"
#include "GameAPI.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "Hooks_Gameplay.h"

UInt32 kModelLoader_QueueReference = 0x00444850;
UInt32 * kBSTaskCounter = (UInt32*) 0x0011C3B38;

ModelLoader *ModelLoader::GetSingleton()
{
	return *(ModelLoader**)0x011C3B3C;
}

__declspec(naked) void ModelLoader::QueueReference(TESObjectREFR *refr, UInt32 arg2, UInt32 arg3)
// arg2 is encoded based on the parent cell and arg3 is most likely a boolean
{
	__asm	jmp		kModelLoader_QueueReference
}

UInt32 *BSTask::GetCounterSingleton()
{
	return kBSTaskCounter;
}

