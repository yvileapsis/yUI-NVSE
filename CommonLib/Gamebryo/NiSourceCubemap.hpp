#pragma once

#include "NiSourceTexture.hpp"
#include "NiTexture.hpp"

NiSmartPointer(NiSourceCubeMap);

class NiDX9Renderer;

class NiSourceCubeMap : public NiSourceTexture {
public:
	NiSourceCubeMap();
	~NiSourceCubeMap();

	enum FaceID : UInt32
	{
		FACE_POS_X = 0x0,
		FACE_NEG_X = 0x1,
		FACE_POS_Y = 0x2,
		FACE_NEG_Y = 0x3,
		FACE_POS_Z = 0x4,
		FACE_NEG_Z = 0x5,
		FACE_NUM = 0x6,
	};

	FaceID	m_eFace;

	CREATE_OBJECT(NiSourceCubeMap, 0xA60F30);
	NIRTTI_ADDRESS(0x11F4454);

	static NiSourceCubeMap* Create(const char* pcPosXFilename, const char* pcNegXFilename, const char* pcPosYFilename, const char* pcNegYFilename, const char* pcPosZFilename, const char* pcNegZFilename,
		NiDX9Renderer* pkRenderer, const char* apName, NiTexture::FormatPrefs* kPrefs) {
		return CdeclCall<NiSourceCubeMap*>(0xA61120, pcPosXFilename, pcNegXFilename, pcPosYFilename, pcNegYFilename, pcPosZFilename, pcNegZFilename, pkRenderer, apName, &kPrefs);
	}
};

ASSERT_SIZE(NiSourceCubeMap, 0x4C);