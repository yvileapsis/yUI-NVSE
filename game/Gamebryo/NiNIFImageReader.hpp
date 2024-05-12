#pragma once

#include "NiImageReader.hpp"
#include "NiStream.hpp"

class NiNIFImageReader : public NiImageReader {
public:
	virtual ~NiNIFImageReader();

	NiStream m_kStream;
};
