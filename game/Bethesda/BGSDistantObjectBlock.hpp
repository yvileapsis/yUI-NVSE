#pragma once

#include "BGSDistantObjectBlockLoadTask.hpp"
#include "BGSTerrainNode.hpp"
#include "BSSegmentedTriShape.hpp"

class TESWorldSpace;

struct BGSDistantObjectBlock : BSMemObject {
	BGSDistantObjectBlock(BGSTerrainNode* apTerrainNode, UInt32 auiLODLevel, SInt32 aiX, SInt32 aiY, bool abIsLow);
	~BGSDistantObjectBlock();

	BGSTerrainNode*						pTerrainNode;
	BSSegmentedTriShapePtr				spSegmentedTriShape;
	BSMultiBoundNodePtr					spMultiboundNode;
	BSSegmentedTriShapePtr				spNode0C; // old ones?
	BSMultiBoundNodePtr					spNode10;
	NiNodePtr							spNode14;
	BGSDistantObjectBlockLoadTaskPtr	spDistantObjectBlockLoadTask;
	BGSDistantObjectBlockLoadTaskPtr	spTask2;
	bool								byte20;
	bool								bIsAddedToDistantBlocksList;
	bool								bPrepared;
	bool								bIsHigh;
	UInt8								byte24;
	UInt32								unk28;

	void Destroy(bool abFreeMemory);
	bool IsHigh() const;
	BSSegmentedTriShape* GetShape() const;
	BGSTerrainNode*	GetTerrainNode() const;
	BSMultiBoundNode* GetMultiBound(bool abUnk) const;

	void CreateNewTask(TESWorldSpace* apWorldSpace, UInt32 auiLODLevel, SInt16 aiCellX, SInt16 aiCellY, bool abIsHigh, bool abIsApocalypse = false, bool abIsStinger = false);
	void ShowRecurse(NiNode* apNode);
	void RemoveBlock();
	void LoadLow(TESWorldSpace* apWorldSpace, UInt32 auiLODLevel, SInt16 x, SInt16 y);
	void LoadHigh(TESWorldSpace* apWorldSpace, UInt32 auiLODLevel, SInt16 x, SInt16 y);
	void Unk_6F5CA0();
	void Prepare();

	static BSSimpleList<BGSDistantObjectBlock*>* const pObjectBlocks;

	static void ResetCullResults();

	void AddToList();
	void RemoveFromList();
	bool IsInList();
};
static_assert(sizeof(BGSDistantObjectBlock) == 0x2C);