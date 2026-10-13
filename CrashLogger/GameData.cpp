#pragma once

#include <main.hpp>
#include <SafeWrite.hpp>
#include "BSFaceGenManager.hpp"
#include "BSFile.hpp"
#include "ModelLoader.hpp"
#include "NiTexture.hpp"
#include "ProcessLists.hpp"
#include "ScriptRunManager.hpp"
#include "ScriptRunner.hpp"
#include "TES.hpp"
#include "TESWorldSpace.hpp"
#include "AILinearTaskThreadManager.hpp"
#include "BSParallelTaskThreadManager.hpp"
#include "ShadowSceneNode.hpp"
#include "BGSSaveLoadGame.hpp"

namespace CrashLogger::GameData
{
	UInt16 textureCounters[8] = { 0 };

	static const char* AI_TASK_THREAD_1_EVENT_NAMES[] = {
		"Start",
		"Interface Update",
		"Animation Update",
		"Cell Animations Update",
		"Combat Update",
		"Magic Update",
		"Packages Update",
		"Actors Update",
		"Pre-Render Actions",
		"Post-Render Actions",
		"Player Followers Update",
		"Finished",
	};

	static const char* AI_TASK_THREAD_2_EVENT_NAMES[] = {
		"Start",
		"Detection Update",
		"World Animations Update",
		"Actor Movement Update",
		"Actor Ragdoll Anim Update",
		"Object Destruction Update",
		"World Update",
		"Finished",
	};

	static void __fastcall AddTexture(UInt32 size) {
		if (size <= 128)
			textureCounters[0]++;
		else if (size <= 256)
			textureCounters[1]++;
		else if (size <= 512)
			textureCounters[2]++;
		else if (size <= 1024)
			textureCounters[3]++;
		else if (size <= 2048)
			textureCounters[4]++;
		else if (size <= 4096)
			textureCounters[5]++;
		else if (size <= 8192)
			textureCounters[6]++;
	}

	static void TextureCounter() {
		NiTexture* head = NiTexture::GetListHead();
		while (head) {
			UInt32 height = head->GetHeight();
			UInt32 width = head->GetWidth();

			// Select larger dimension
			UInt32 dimension = height > width ? height : width;
			AddTexture(dimension);

			textureCounters[7]++;

			head = head->m_pkNext;
		}
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			_MESSAGE("\nPlayer Data:");
			try {
				AutoIndent indent;
				PlayerCharacter* pPlayer = PlayerCharacter::GetSingleton();
				if (pPlayer) {
					TESObjectCELL* pParentCell = pPlayer->GetParentCell();
					if (pParentCell) {
						const char* pDefaultCellName = reinterpret_cast<Setting*>(0x11CA130)->uValue.str;
						const char* pCellName = pParentCell->GetFullName();
						_MESSAGE("Cell:       %08X (\"%s\") - %s", pParentCell->GetFormID(), pParentCell->GetEditorID(), pCellName[0] ? pCellName : pDefaultCellName);
					}
					else
						_MESSAGE("Cell:       None");

					TESWorldSpace* pWorld = TES::GetSingleton()->pWorldSpace;
					if (pWorld)
						_MESSAGE("World:      %08X (\"%s\") - %s", pWorld->GetFormID(), pWorld->GetEditorID(), pWorld->GetFullName());
					else
						_MESSAGE("World:      None");

					const NiPoint3& rPosition = pPlayer->kPosition;
					const NiPoint3& rRotation = pPlayer->kRotation;
					_MESSAGE("Position:   X: %.2f Y: %.2f Z: %.2f", rPosition.x, rPosition.y, rPosition.z);
					_MESSAGE("Rotation:   X: %.2f Y: %.2f Z: %.2f", rRotation.x, rRotation.y, rRotation.z);
				}
				else {
					_MESSAGE("Failed to get player character.");
				}
			}
			catch (...) {
				_MESSAGE(" Failed to get player character info.");
			}

			try {
				ScriptRunManager* pMgr = ScriptRunManager::GetSingleton();
				_MESSAGE("\nCurrently running script:");
				if (pMgr && pMgr->pCurrentRunner) {
					ScriptRunner* pRunner = pMgr->pCurrentRunner;
					Script* pScript = pRunner->pRunningScript;

					{
						AutoIndent indent;
						if (pScript) {
							std::string_view strEDID = pScript->GetEditorID();
							if (pScript->IsTemporary()) {
								if (strEDID.empty())
									_MESSAGE("Script:	   Temporary Script");
								else
									_MESSAGE("Script:	   Temporary Script (\"%s\")", strEDID.data());
							}
							else {
								if (strEDID.empty())
									_MESSAGE("Script:	   %08X", pScript->GetFormID());
								else
									_MESSAGE("Script:	   %08X (\"%s\")", pScript->GetFormID(), strEDID.data());
							}
							
						}
						else
							_MESSAGE("Script:	   None");

						if (pRunner->pCurrentContainer)
							_MESSAGE("Container:    %08X (\"%s\")", pRunner->pCurrentContainer->GetFormID(), pRunner->pCurrentContainer->GetEditorID());
						else
							_MESSAGE("Container:    None");

						if (pRunner->pCurrentObject)
							_MESSAGE("Object:	   %08X (\"%s\")", pRunner->pCurrentObject->GetFormID(), pRunner->pCurrentObject->GetEditorID());
						else
							_MESSAGE("Object:	   None");

						_MESSAGE("Error Code:   %u", pRunner->eError);
					}
				}
				else {
					AutoIndent indent;
					_MESSAGE(" None");
				}
			}
			catch (...) {
				_MESSAGE(" Failed to get currently running script info.");
			}

			_MESSAGE("\nScene Stats:");
			{
				AutoIndent indent;
				ShadowSceneNode* pSSN = BSShaderManager::GetShadowSceneNode(0);
				if (pSSN) {
					_MESSAGE("Scene Lights: %i", pSSN->kLights.GetSize());
					_MESSAGE("Shadow Lights: %i", pSSN->kActorShadowCasters.GetSize());
				}
			}

			BGSSaveLoadGame* pSaveLoad = BGSSaveLoadGame::GetSingleton();
			if (pSaveLoad) {
				_MESSAGE("\nSave Load State:");
				AutoIndent indent;
				{
					BGSSaveLoadGame* pSaveLoad = BGSSaveLoadGame::GetSingleton();
					_MESSAGE("Global Flags:");
					{
						AutoIndent indent;
						_MESSAGE("Global Allow Changes: %i", pSaveLoad->GetGlobalAllowChanges());
						_MESSAGE("Save Game Loading: %i", pSaveLoad->GetSaveGameLoading());
						_MESSAGE("Save Game Saving: %i", pSaveLoad->GetSaveGameSaving());
						_MESSAGE("Initing Forms : %i", pSaveLoad->GetInitingForms());
						_MESSAGE("Defer Init Forms: %i", pSaveLoad->GetDeferInitForms());
						_MESSAGE("Positioning Player Character: %i", pSaveLoad->GetPositioningPlayerCharacter());
						_MESSAGE("Player Location Invalid: %i", pSaveLoad->GetPlayerLocationInvalid());
						_MESSAGE("Save Load Failed: %i", pSaveLoad->GetSaveLoadFailed());
					}
					_MESSAGE("\nThread Flags:");
					{
						AutoIndent indent;
						_MESSAGE("Thread Allow Changes: %i", pSaveLoad->GetThreadAllowChanges());
						_MESSAGE("Loading Moved Refs: %i", pSaveLoad->GetLoadingMovedRefs());
						_MESSAGE("Reconstructing Forms: %i", pSaveLoad->GetReconstructingForms());
						_MESSAGE("Allow Changes: %i", pSaveLoad->GetAllowChanges());
					}
				}
			}

			_MESSAGE("\nProcess Lists:");
			uint32_t uiTotal = 0;
			{
				AutoIndent indent;
				for (uint32_t i = 0; i < 4; i++) {
					uint32_t uiStart = ProcessLists::GetSingleton()->kAllProcessArrays.uiBeginOffsets[i];
					uint32_t uiEnd = ProcessLists::GetSingleton()->kAllProcessArrays.uiEndOffsets[i];
					uint32_t uiCount = uiEnd - uiStart;
					const char* pProcessName = "Invalid";
					switch (i) {
						case BaseProcess::kProcessLevel_High:
							pProcessName = "High:       ";
							break;
						case BaseProcess::kProcessLevel_MiddleHigh:
							pProcessName = "Middle High:";
							break;
						case BaseProcess::kProcessLevel_MiddleLow:
							pProcessName = "Middle Low: ";
							break;
						case BaseProcess::kProcessLevel_Low:
							pProcessName = "Low:        ";
							break;
						default:
							__assume(0);
					}
					_MESSAGE("%s %i", pProcessName, uiCount);
					uiTotal += uiCount;
				}
			}
			_MESSAGE("Total: %i", uiTotal);

			_MESSAGE("\nLoaded assets:");
			{
				TextureCounter();

				UInt32 uiFaceGenMeshes = 0;
				if (BSFaceGenManager::GetSingleton() && BSFaceGenManager::GetSingleton()->pModelMap)
					uiFaceGenMeshes = BSFaceGenManager::GetSingleton()->pModelMap->kEntryMap.GetCount();

				UInt32 uiModels = 0;
				UInt32 uiAnimations = 0;
				if (ModelLoader::GetSingleton()) {
					if (ModelLoader::GetSingleton()->pModelMap)
						uiModels = ModelLoader::GetSingleton()->pModelMap->uiCount;
					if (ModelLoader::GetSingleton()->pKFModelMap)
						uiAnimations = ModelLoader::GetSingleton()->pKFModelMap->uiCount;
				}

				UInt32 uiPlayingSounds = 0;
				UInt32 uiCachedSounds = 0;
				if (BSAudioManager::GetSingleton()) {
					uiPlayingSounds = BSAudioManager::GetSingleton()->kPlayingSounds.GetCount();
					uiCachedSounds = BSAudioManager::GetSingleton()->kCachedSounds.GetSize();
				}

				AutoIndent indent;
				_MESSAGE("Textures:       %i", textureCounters[7]);
				_MESSAGE("  <= 128:  %i", textureCounters[0]);
				_MESSAGE("  <= 256:  %i", textureCounters[1]);
				_MESSAGE("  <= 512:  %i", textureCounters[2]);
				_MESSAGE("  <= 1024: %i", textureCounters[3]);
				_MESSAGE("  <= 2048: %i", textureCounters[4]);
				_MESSAGE("  <= 4096: %i", textureCounters[5]);
				_MESSAGE("  <= 8192: %i", textureCounters[6]);
				_MESSAGE("Models:         %i", uiModels);
				_MESSAGE("FaceGen Models: %i", uiFaceGenMeshes);
				_MESSAGE("Animations:     %i", uiAnimations);
				_MESSAGE("Playing Sounds: %i", uiPlayingSounds);
				{
					AutoIndent indent;
					_MESSAGE("Cached Sounds: %i", uiCachedSounds);
				}
			}

			_MESSAGE("\nThreads States:");
			{
				AutoIndent indent;

				{
					AutoIndent indent;
					_MESSAGE("Main Thread");
					{
						AutoIndent indent;
						_MESSAGE("Rendering: %s", AILinearTaskThreadManager::IsMainRendering() ? "Yes" : "No");
						_MESSAGE("Queue 3D Tasks: %s", AILinearTaskThreadManager::ShouldQueue3DTask() ? "Yes" : "No");
					}
				}

				AILinearTaskThreadManager* pThreadMgr = AILinearTaskThreadManager::GetSingleton();
				if (pThreadMgr) {
					AutoIndent indent;
					_MESSAGE("AI Linear Task Threads");
					{
						AutoIndent indent;
						_MESSAGE("Running: %s", AILinearTaskThreadManager::IsRunningThreads() ? "Yes" : "No");

						// 2 threads
						const char* pThread0 = AI_TASK_THREAD_1_EVENT_NAMES[pThreadMgr->eThreadStage[0]];
						const char* pThread1 = AI_TASK_THREAD_2_EVENT_NAMES[pThreadMgr->eThreadStage[1]];

						_MESSAGE("Last Thread 0 Stage: %s", pThread0);
						_MESSAGE("Last Thread 1 Stage: %s", pThread1);
					}
				}

				BSTaskManagerThread* pTaskManagerThread = IOManager::GetSingleton()->ppThreads[0];
				{
					AutoIndent indent;
					_MESSAGE("Task Manager Thread");
					{
						AutoIndent indent;
						uint32_t uiQueued = IOManager::GetSingleton()->GetCount();
						uint32_t uiPostProcessQueued = IOManager::GetSingleton()->GetPostProcessQueueCount();
						_MESSAGE("Queued Tasks: %i", uiQueued);
						_MESSAGE("Post Process Tasks: %i", uiPostProcessQueued);
					}
				}

				BackgroundCloneThread* pCloneThread = ModelLoader::GetSingleton()->pBackgroundCloneThread;
				if (pCloneThread) {
					AutoIndent indent;
					_MESSAGE("Background Clone Thread");
					{
						AutoIndent indent;
						_MESSAGE("Running Count: %i", pCloneThread->iRunningCount);
						_MESSAGE("Process Count: %i", pCloneThread->pkProcessTaskQueue->GetCount());
					}
				}

				BSParallelTaskThreadManager* pParallelThreadMgr = BSParallelTaskThreadManager::GetSingleton();
				if (pParallelThreadMgr) {
					AutoIndent indent;
					_MESSAGE("Parallel Task Thread");
					{
						AutoIndent indent;
						_MESSAGE("Running: %s", pParallelThreadMgr->bActive ? "Yes" : "No");
					}
				}
			}
		}
		catch (...) {
			_MESSAGE("Failed to print game stats.\n");
		}
	}
}