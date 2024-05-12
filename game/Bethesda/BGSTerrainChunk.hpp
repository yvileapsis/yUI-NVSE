#pragma once

#include "NiGeometry.hpp"
#include "BSMultiBoundNode.hpp"
#include "BSShaderPPLightingProperty.hpp"
#include "BSSimpleList.hpp"

struct BGSTerrainNode;
class BGSTerrainChunkLoadTask;

struct BGSTerrainChunk {
	BGSTerrainChunk(BGSTerrainNode* apNode, UInt32 auiLODLevel, SInt16 x, SInt16 y);

	BGSTerrainNode*							pNode;
	NiGeometryPtr							spLandMesh;
	NiGeometryPtr							spWaterMesh;
	NiGeometryPtr							spWaterReflectMesh;
	BSMultiBoundNodePtr						spLandMultiBoundNode;
	BSMultiBoundNodePtr						spWaterMultiBoundNode;
	BSMultiBoundNodePtr						spWaterReflectMultiBoundNode;
	TESObjectREFR*							pWaterRef;
	TESObjectREFR*							pWaterReflectRef;
	NiPointer<BGSTerrainChunkLoadTask>		spChunkLoadTask;
	bool									bTaskFinished;
	bool									bIsInitialized;
	bool									bInitializedShaderProperty;
	bool									gap02B;
	NiPointer<BSShaderPPLightingProperty>	spShaderProperty;

	static bool* const bAttach;

	void InitializeShaderProperty();
	void AttachWaterLOD(bool abForce);
	void FinishLoadTask();

	static BSSimpleList<BGSTerrainChunk*>* const pTerrainChunks;

	static void ResetCullResults();
};

ASSERT_SIZE(BGSTerrainChunk, 0x30);