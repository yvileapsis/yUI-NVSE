#pragma once

#include "NiObject.hpp"

class NiTextKey;

NiSmartPointer(BSAnimNote);

class BSAnimNote : public NiObject {
public:
	virtual ~BSAnimNote();

	virtual void	ParseTextKey(NiTextKey* apTextKey);
	
	enum Type {
		kGrabIK		= 1,
		kAnimNode	= 2,
	};

	Type	eType;
	float	fTime;

	NIRTTI_ADDRESS(0x11F3EF8);
};

ASSERT_SIZE(BSAnimNote, 0x10);