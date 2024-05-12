#pragma once

#include "NiExtraData.hpp"
#include "BSSplatterData.hpp"

class BSCustomSplatterExtraData : public NiExtraData {
public:
	BSCustomSplatterExtraData();
	virtual ~BSCustomSplatterExtraData();


	BSSplatterData kData;

	static BSCustomSplatterExtraData* Create(BSSplatterData& arData);
	static NiFixedString* GetTag();

};

ASSERT_SIZE(BSCustomSplatterExtraData, 0x1C);

#if 1

class BSCustomSplatterExtraDataEx : public NiExtraData {
public:
	BSCustomSplatterExtraDataEx();
	virtual ~BSCustomSplatterExtraDataEx();


	BSSplatterBonusData kData;

	static BSCustomSplatterExtraDataEx* Create(BSSplatterBonusData& arData);
};

ASSERT_SIZE(BSCustomSplatterExtraDataEx, 0x34);

#endif