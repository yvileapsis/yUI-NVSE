#pragma once

class NiPixelData;
class NiFile;
class NiPixelFormat;

class NiImageReader : public NiMemObject {
public:
	NiImageReader();

	virtual ~NiImageReader();
	virtual bool CanReadFile(const char* pcFileExtension);
	virtual NiPixelData* ReadFile(NiFile* kIst, NiPixelData* pkOptDest);
	virtual bool ReadHeader(NiFile* kIst, UInt32* uiWidth, UInt32* uiHeight, NiPixelFormat* kFormat, bool* bMipmap, UInt32* uiFaces);
	
	bool gap4[124];
	_RTL_CRITICAL_SECTION m_kReadCriticalSection;
	bool gap98[104];
};

ASSERT_SIZE(NiImageReader, 0x100);