#pragma once
#include "QueuedFileEntry.hpp"
#include "NiGeometry.hpp"
#include "BSMultiBoundNode.hpp"

struct BGSDistantObjectBlock;
class TESWorldSpace;

NiSmartPointer(BGSDistantObjectBlockLoadTask);

// 0x58
class BGSDistantObjectBlockLoadTask : public QueuedFileEntry {
public:
	bool					bQueuedTextures;
	bool					byte31;
	bool					byte32;
	bool					byte33;
	SInt32					iCellX;
	SInt32					iCellY;
	UInt32					uiLODLevel;
	BGSDistantObjectBlock*	pParent;
	TESWorldSpace*			pWorldSpace;
	NiGeometryPtr			spObjectGeometry;
	NiNodePtr				spRootNode;
	bool					byte50;
	bool					bIsStinger;
	bool					byte52;
	bool					byte53;
	bool					unk54;

	static BGSDistantObjectBlockLoadTask* Create(BGSDistantObjectBlock* apParentBlock,
		TESWorldSpace* apWorld,
		SInt32 x,
		SInt32 y,
		UInt32 auiLODLevel,
		bool abIsApocalypse,
		bool abIsHigh,
		bool abIsStinger = false);

	static void __fastcall RunEx(BGSDistantObjectBlockLoadTask* apThis);
};
static_assert(sizeof(BGSDistantObjectBlockLoadTask) == 0x58);