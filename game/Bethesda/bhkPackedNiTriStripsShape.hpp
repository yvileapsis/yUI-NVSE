#pragma once

#include "bhkShapeCollection.hpp"

class bhkPackedNiTriStripsShape : public bhkShapeCollection {
public:
	virtual UInt32 GetSubMaterial(UInt32 auiKey);
};