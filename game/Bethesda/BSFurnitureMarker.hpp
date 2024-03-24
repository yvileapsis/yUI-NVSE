#pragma once

#include "NiExtraData.hpp"
#include "FurnitureMark.hpp"

NiSmartPointer(BSFurnitureMarker);

class BSFurnitureMarker : public NiExtraData {
public:
	BSFurnitureMarker();
	~BSFurnitureMarker();

	NiTObjectArray<FurnitureMark> markers;

	CREATE_OBJECT(BSFurnitureMarker, 0xC54210)
};

ASSERT_SIZE(BSFurnitureMarker, 0x1C)