#pragma once

#include "NiExtraData.hpp"
#include "BSSimpleArray.hpp"

class NiAVObject;

NiSmartPointer(BSTextureData);

class BSTextureData : public NiExtraData {
public:
	BSTextureData();
	~BSTextureData();

	BSSimpleArray<NiAVObject*> objectArray;

	CREATE_OBJECT(BSTextureData, 0xC5EED0)
};

ASSERT_SIZE(BSTextureData, 0x1C);