#pragma once

#include "NiImageConverter.hpp"
#include "NiTextureCodec.hpp"
#include "NiTList.hpp"

class NiDevImageConverter : public NiImageConverter {
public:
	virtual ~NiDevImageConverter();

	NiTextureCodec m_kCodec;
	NiTList<void*> m_kReaders;
	bool gap99C[100];

	static NiDevImageConverter* Create();
};

ASSERT_SIZE(NiDevImageConverter, 0xA00);