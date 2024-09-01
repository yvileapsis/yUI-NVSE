#pragma once

#include <main.hpp>
#include <SafeWrite.hpp>
#include <BSFile.hpp>
#include <NiTexture.hpp>
#include <ModelLoader.hpp>
#include <BSFaceGenManager.hpp>

namespace CrashLogger::AssetTracker
{
	std::stringstream output;

	UInt16 textureCounters[8] = { 0 };

	static void AddTexture(UInt32 size) {
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

	extern void Process(EXCEPTION_POINTERS* info)
	{

		TextureCounter();

		UInt32 uiFaceGenMeshes = 0;
		if (BSFaceGenManager::GetSingleton()->pModelMap)
			uiFaceGenMeshes = BSFaceGenManager::GetSingleton()->pModelMap->kEntryMap.GetCount();

		output << "Loaded assets:"	 << '\n';
		output << "Textures:       " << textureCounters[7] << '\n';
		output << "  <=128:  "		 << textureCounters[0] << '\n';
		output << "  <=256:  "		 << textureCounters[1] << '\n';
		output << "  <=512:  "		 << textureCounters[2] << '\n';
		output << "  <=1024: "		 << textureCounters[3] << '\n';
		output << "  <=2048: "		 << textureCounters[4] << '\n';
		output << "  <=4096: "		 << textureCounters[5] << '\n';
		output << "  <=8192: "		 << textureCounters[6] << '\n';
		output << "Models:         " << ModelLoader::GetSingleton()->pModelMap->uiQueuedCount << '\n';
		output << "FaceGen Models: " << uiFaceGenMeshes << '\n';
		output << "Animations:     " << ModelLoader::GetSingleton()->pKFModelMap->uiQueuedCount << '\n';
	}

	extern std::stringstream& Get() { output.flush(); return output; }
}