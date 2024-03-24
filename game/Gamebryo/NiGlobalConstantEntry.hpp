#pragma once

#include "NiRefObject.hpp"
#include "NiFixedString.hpp"

class NiGlobalConstantEntry : public NiRefObject {
public:
	NiGlobalConstantEntry();
	virtual ~NiGlobalConstantEntry();

	enum AttributeType
	{
		ATTRIB_TYPE_UNDEFINED = 0x0,
		ATTRIB_TYPE_BOOL = 0x1,
		ATTRIB_TYPE_STRING = 0x2,
		ATTRIB_TYPE_UNSIGNEDINT = 0x3,
		ATTRIB_TYPE_FLOAT = 0x4,
		ATTRIB_TYPE_POINT2 = 0x5,
		ATTRIB_TYPE_POINT3 = 0x6,
		ATTRIB_TYPE_POINT4 = 0x7,
		ATTRIB_TYPE_MATRIX3 = 0x8,
		ATTRIB_TYPE_MATRIX4 = 0x9,
		ATTRIB_TYPE_COLOR = 0xA,
		ATTRIB_TYPE_TEXTURE = 0xB,
		ATTRIB_TYPE_FLOAT8 = 0xC,
		ATTRIB_TYPE_FLOAT12 = 0xD,
		ATTRIB_TYPE_ARRAY = 0xE,
		ATTRIB_TYPE_COUNT = 0xF,
	};

	NiFixedString	m_kKey;
	AttributeType	m_eType;
	UInt32			m_uiDataSize;
	void*			m_pvDataSource;
};