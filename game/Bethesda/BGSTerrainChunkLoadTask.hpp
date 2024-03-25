#pragma once
#include "QueuedFileEntry.hpp"
#include "NiNode.hpp"
#include "NiGeometry.hpp"

struct BGSTerrainChunk;
class TESWorldSpace;

// 0x60
class BGSTerrainChunkLoadTask : public QueuedFileEntry {
public:
	bool				bQueuedTextures;
	SInt32				iCellX;
	SInt32				iCellY;
	UInt32				uiLODLevel;
	BGSTerrainChunk*	pTerrainChunk;
	TESWorldSpace*		pWorldSpace;
	NiGeometryPtr		spTerrainGeometry;
	NiGeometryPtr		spWaterGeometry;
	NiGeometryPtr		spWaterGeometry_Reflecting;
	NiNodePtr			spRootNode; // BSMultiBoundNode
	bool				bExit;
	int					byte5C;

	static BGSTerrainChunkLoadTask* Create(BGSTerrainChunk* apChunk, TESWorldSpace* apWorldSpace, SInt32 x, SInt32 y, UInt32 auiLODLevel);

	static void __fastcall RunEx(BGSTerrainChunkLoadTask* apThis);
};
static_assert(sizeof(BGSTerrainChunkLoadTask) == 0x60);
