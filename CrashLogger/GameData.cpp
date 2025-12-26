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

namespace CrashLogger::GameData
{
	UInt16 textureCounters[8] = { 0 };

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
					if (pParentCell)
						_MESSAGE("Cell:       %08X (\"%s\")", pParentCell->GetFormID(), pParentCell->GetEditorID());
					else
						_MESSAGE("Cell:       None");

					TESWorldSpace* pWorld = TES::GetSingleton()->pWorldSpace;
					if (pWorld)
						_MESSAGE("World:      %08X (\"%s\")", pWorld->GetFormID(), pWorld->GetEditorID());
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

			_MESSAGE("\nLoaded assets:");
			{
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
							pProcessName = "High";
							break;
						case BaseProcess::kProcessLevel_MiddleHigh:
							pProcessName = "Middle High";
							break;
						case BaseProcess::kProcessLevel_MiddleLow:
							pProcessName = "Middle Low";
							break;
						case BaseProcess::kProcessLevel_Low:
							pProcessName = "Low";
							break;
						default:
							__assume(0);
					}
					_MESSAGE("%s: %i", pProcessName, uiCount);
					uiTotal += uiCount;
				}
			}
			_MESSAGE("Total: %i", uiTotal);
		}
		catch (...) {
			_MESSAGE("Failed to print game stats.\n");
		}
	}
}