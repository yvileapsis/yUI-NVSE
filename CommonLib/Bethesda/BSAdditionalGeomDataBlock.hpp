#pragma once

class BSAdditionalGeomDataBlock {
public:
	BSAdditionalGeomDataBlock();
	~BSAdditionalGeomDataBlock();

	void*	pData;
	UInt32	uiStride;
	byte	unk08;
};

ASSERT_SIZE(BSAdditionalGeomDataBlock, 0xC);