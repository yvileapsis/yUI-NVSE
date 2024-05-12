#pragma once

#include "NiPoint3.hpp"
#include "NiPoint2.hpp"
#include "NiSmartPointer.hpp"
#include "BGSDistantTreeBlock.hpp"

struct BGSTerrainChunk;
struct BGSDistantObjectBlock;

class BGSTerrainManager;

// 0x60
struct BGSTerrainNode {
	union Coordinate {
		UInt32 xy;
		struct {
			SInt16 x;
			SInt16 y;
		};
	};

	BGSTerrainManager*		pManager;
	UInt32					uiLODLevel;
	Coordinate				kBaseCellCoords;
	bool					bIsLvlMaxOrLower;
	UInt8					byte0D;
	UInt8					byte0E;
	UInt8					byte0F;
	BGSTerrainChunk*		pkTerrainChunk;
	BGSDistantObjectBlock*	pkObjectBlock;
	BGSDistantTreeBlock*	pkTreeBlock;
	UInt32					ukn1C;
	BGSTerrainNode*			pkChildren[4];
	BGSTerrainNode*			pkParent;
	NiPoint3				pos34;
	float					fDist;
	float					fSplitDist;
	float					fMorphStartDist;
	float					fMorphEndDist;
	UInt32					uiTerrainFadeTimer;
	bool					bForceFade;
	UInt32					uiLinkID;
	bool					maybeShouldShow_5C;
	bool					byte5D;
	bool					byte5E;
	bool					byte5F;

	SInt16 GetCellX();
	SInt16 GetCellY();
	BGSTerrainChunk* GetTerrainChunk();
	BGSDistantObjectBlock* GetObjectBlock();
	BGSDistantTreeBlock* GetTreeBlock();
	UInt32 GetLODLevel();
	float GetRange(const NiPoint2 apPos1, const NiPoint2 apPos2, float afScale);
	bool IsWithinLoadDistance(const NiPoint3& apPosition);
	bool IsWithinLowLoadDistance(const NiPoint3& apPosition);
	bool IsPlayerInRange();
	void RenderDistantBlocksRecurse(NiPoint3& apPlayerPos, bool abIsBlocks);
	void UpdateBlockVisibility();

	BGSDistantTreeBlock* GetDistantTreeBlock(NiPoint3* apPos);
};
static_assert(sizeof(BGSTerrainNode) == 0x60);