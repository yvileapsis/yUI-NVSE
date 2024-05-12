#pragma once
#include "BGSTerrainNode.hpp"
#include "BSSimpleArray.hpp"
#include "NiNode.hpp"

class TESWorldSpace;

class BGSTerrainManager {
public:
	TESWorldSpace*		pkWorld;
	BGSTerrainNode*		pkRootNode;
	NiPointer<NiNode>	spLODRoot;
	NiPointer<NiNode>	spWaterLODNode;
	BGSTerrainNode::Coordinate coordNW;
	BGSTerrainNode::Coordinate coordSE;
	UInt32				uiMaxLevel;
	UInt32				uiMinLevel;
	UInt32				uiRootLevel;
	UInt32				uiLODLevel;
	bool				bNeedsImmediateUpdate;
	bool				bHasLOD;
	UInt8				byte2A;
	UInt8				byte2B;
	BSSimpleArray<TESObjectREFR*> kTreeRefs;

	bool HasRootNode();

	NiNode* GetWaterLODNode();
	NiNode* GetLODRoot();
	UInt32 GetLODLevel();

	static BGSTerrainManager* __fastcall Create(BGSTerrainManager* apThis, void*, TESWorldSpace* apWorldSpace);

	static NiNode* GetRootLandLODNode();
	static NiNode* GetRootObjectLODNode();
	static NiNode* GetRootTreeLODNode();

	void ToggleObjectLOD(NiNode* apNode, bool abEnable, bool abCurrentCellOnly);
	void ToggleLandLOD(bool abEnable, bool abCurrentCellOnly);
	void ToggleTreeLOD(bool abEnable, bool abCurrentCellOnly);

	void UpdateMultiBoundVisibility(NiCamera* apCamera);

	void ResetMultiBounds() const;

	BGSDistantTreeBlock* GetDistantTreeBlock(NiPoint3* apPos);
	void HideTreeLOD(TESObjectREFR* apRefr, bool abRegisterRefr);

	void UpdateLODAnimations();
};
static_assert(sizeof(BGSTerrainManager) == 0x3C);