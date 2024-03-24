#pragma once

#include "AStarQueue.hpp"

class ScrapHeap;

class AStarSearch {
public:
	AStarSearch();
	virtual ~AStarSearch();

	struct Unk10 {
		void* unk000[2039];
		UInt32 byte1FDC;
	};

	ScrapHeap*	pScrapHeapBuffer;
	UInt32		unk004;
	UInt32		unk008;
	UInt32		unk00C;
	Unk10		struct010;
	AStarQueue	kQueue;
	UInt32		unk2048;
	AStarNode*	pNode;
	UInt32		unk2050;
	bool		bIsRunningInTasklet;
};

ASSERT_SIZE(AStarSearch, 0x2058);