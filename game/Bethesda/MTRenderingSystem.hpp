#pragma once

#include "BSCullingProcess.hpp"
#include "RenderingTaskThread.hpp"

class MTRenderManager {
public:
	enum Stage : SInt32 {
		// 0-16 are stages dedicated to water?
		MTRM_STAGE_NONE = -1,
		MTRM_STAGE_0 = 0,
		MTRM_STAGE_1 = 1,
		MTRM_STAGE_2 = 2,
		MTRM_STAGE_3 = 3,
		MTRM_STAGE_4 = 4,
		MTRM_STAGE_5 = 5,
		MTRM_STAGE_6 = 6,
		MTRM_STAGE_7 = 7,
		MTRM_STAGE_8 = 8,
		MTRM_STAGE_9 = 9,
		MTRM_STAGE_10 = 10,
		MTRM_STAGE_11 = 11,
		MTRM_STAGE_12 = 12,
		MTRM_STAGE_13 = 13,
		MTRM_STAGE_14 = 14,
		MTRM_STAGE_15 = 15,
		MTRM_STAGE_16 = 16,

		// 17-20 are light stages (for shadows)
		MTRM_LIGHT_0 = 17,
		MTRM_LIGHT_1 = 18,
		MTRM_LIGHT_2 = 19,
		MTRM_LIGHT_3 = 20,

		MTRM_WORLD = 21,
		MTRM_VIEWMODEL = 22,
		MTRM_INTERFACE = 23,
		MTRM_STAGE_COUNT
	};


	struct StageData {
		NiCameraPtr							spCamera;
		BSCompoundFrustum*					pFrustum;
		NiAVObjectPtr						spSceneRoot;
		NiTPointerList<NiAVObject>*			pGeomList[2];
		BSShaderAccumulatorPtr				spAccumulator;
		BSCullingProcess::BSCPCullingType	eCullMode;
		UInt32								uiAccumulatorCount;
	};

	RenderingTaskThread*	pTaskThreads[3];
	UInt32					eCurrentStage[3];
	BSSemaphore				kSemaphores[3];
	StageData				kStageData[MTRM_STAGE_COUNT];

	static MTRenderManager* const GetSingleton() {
		return (MTRenderManager*)0x1200088;
	}

	static void AddTask(NiCamera* apCamera, BSCompoundFrustum* apFrustum, NiNode* apSceneRoot, void* apGeometryList1, void* apGeometryList2, BSShaderAccumulator* apShaderAccum, BSCullingProcess::BSCPCullingType aeCullMode, UInt32 auiStage, UInt32 auiAccumulatorCount);

	static void AddTask(const NiCamera* apCamera, 
		BSCompoundFrustum* apFrustum, NiNode* apSceneRoot,
		void* apGeometryList1, void* apGeometryList2,
		BSShaderAccumulator* apShaderAccum, BSCullingProcess* apCuller, UInt32 aeCullMode, MTRenderManager::Stage auiStage, UInt32 auiAccumulatorCount);

	static void SetThreadStage(UInt32 auiThread, UInt32 auiThreadStage);
	static void RemoveThreadStage(UInt32 auiThread, UInt32 auiThreadStage);
	static void WaitToComplete();
	static void InitTaskThread(UInt32 auiThreadID, UInt32 auiPreferredCore, void(__cdecl* callback)(), const char* apName, SIZE_T dwStackSize);
	static void __cdecl TaskThreadCallback();
};