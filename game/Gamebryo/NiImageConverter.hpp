#pragma once

#include "NiRefObject.hpp"
#include "NiNIFImageReader.hpp"

NiSmartPointer(NiImageConverter);

class NiImageConverter : public NiRefObject {
public:
	virtual ~NiImageConverter();

	bool gap8[120];
	NiNIFImageReader m_kReader;
	NiImageConverterPtr ms_spConverter;
	char* ms_pcPlatformSubDir;
	bool gap750[45];
};
