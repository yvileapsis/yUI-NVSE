#pragma once

#include "BackgroundCloneThread.hpp"
#include "IOManager.hpp"
#include "QueuedAnimIdle.hpp"
#include "QueuedHelmet.hpp"
#include "QueuedReplacementKFList.hpp"
#include "QueuedTexture.hpp"
#include "LoadedFile.hpp"

class Animation;
class AnimIdle;
class BSFileEntry;
class KFModel;
class Model;
class TESObjectREFR;

class ModelLoader {
public:
	ModelLoader();
	~ModelLoader();

	LockFreeCaseInsensitiveStringMap<Model*>*						pModelMap;
	LockFreeCaseInsensitiveStringMap<KFModel*>*						pKFModelMap;
	LockFreeMap<TESObjectREFR*, NiPointer<QueuedReference> >*		pQueuedReferenceMap1;
	LockFreeMap<TESObjectREFR*, NiPointer<QueuedReference> >*		pQueuedReferenceMap2;
	LockFreeMap<AnimIdle*, NiPointer<QueuedAnimIdle>>*				pQueuedAnimIdleMap;
	LockFreeMap<Animation*, NiPointer<QueuedReplacementKFList>>*	pQueuedReplacementKFListMap;
	LockFreeMap<TESObjectREFR*, NiPointer<QueuedHelmet>>*			pQueuedHelmetMap;
	LockFreeQueue<NiPointer<AttachDistant3DTask>>*					pAttachDistant3DTaskQueue;
	LockFreeMap<BSFileEntry*, NiPointer<QueuedTexture>>*			pQueuedTextureMap;
	LockFreeCaseInsensitiveStringMap<LoadedFile*>*					pLoadingFileMap;
	BackgroundCloneThread*											pBackgroundCloneThread;
	bool															bHasDelayedFree;

	static ModelLoader* GetSingleton();
};

ASSERT_SIZE(ModelLoader, 0x30)