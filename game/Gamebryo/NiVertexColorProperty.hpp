#pragma once

#include "NiProperty.hpp"

NiSmartPointer(NiVertexColorProperty);

class NiVertexColorProperty : public NiProperty {
public:
	NiVertexColorProperty();
	~NiVertexColorProperty();

	enum SourceVertexMode
	{
		SOURCE_IGNORE = 0x0,
		SOURCE_EMISSIVE = 0x1,
		SOURCE_AMB_DIFF = 0x2,
		SOURCE_NUM_MODES = 0x3,
	};

	enum LightingMode
	{
		LIGHTING_E = 0x0,
		LIGHTING_E_A_D = 0x1,
		LIGHTING_NUM_MODES = 0x2,
	};

	UInt16 m_usFlags;

	CREATE_OBJECT(NiVertexColorProperty, 0xA5F280)
};

ASSERT_SIZE(NiVertexColorProperty, 0x1C)