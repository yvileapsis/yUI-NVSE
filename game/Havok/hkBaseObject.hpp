#pragma once

class hkBaseObject {
public:
	virtual ~hkBaseObject();
};

ASSERT_SIZE(hkBaseObject, 0x4);