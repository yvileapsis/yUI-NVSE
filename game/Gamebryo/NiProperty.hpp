#pragma once

#include "NiObjectNET.hpp"

class NiUpdateData;

NiSmartPointer(NiProperty);

class NiProperty : public NiObjectNET {
public:
	NiProperty();
	virtual ~NiProperty();

	virtual UInt32	Type();
	virtual void	Update(NiUpdateData& apUpdateData);

	enum PropertyType {
		ALPHA,
		CULLING,
		MATERIAL,
		SHADE,
		STENCIL,
		TEXTURING,
		MAX_TYPES
	};

	NIRTTI_ADDRESS(0x11F4420);
};

ASSERT_SIZE(NiProperty, 0x18);