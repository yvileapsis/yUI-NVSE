#pragma once

#include "BSMultiBoundShape.hpp"

NiSmartPointer(BSMultiBound);

class BSMultiBound : public NiObject {
public:
	BSMultiBound();
	~BSMultiBound();

	UInt32 uiBoundFrameCount;
	BSMultiBoundShapePtr spShape;

	static bool bIgnoreMultiBounds;

	CREATE_OBJECT(BSMultiBound, 0xC361A0);

	BSMultiBoundShape* GetShape() const;
	void SetShape(BSMultiBoundShape* apShape);
};

ASSERT_SIZE(BSMultiBound, 0x10)