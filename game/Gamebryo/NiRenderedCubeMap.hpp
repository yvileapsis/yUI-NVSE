#pragma once

#include "NiRenderedTexture.hpp"

class NiDX9Renderer;

NiSmartPointer(NiRenderedCubeMap);

class NiRenderedCubeMap : public NiRenderedTexture {
public:
	NiRenderedCubeMap();
	virtual ~NiRenderedCubeMap();

	enum FaceID {
		FACE_POS_X,
		FACE_NEG_X,
		FACE_POS_Y,
		FACE_NEG_Y,
		FACE_POS_Z,
		FACE_NEG_Z,
		FACE_NUM
	};

	FaceID m_eFace;
	Ni2DBufferPtr m_aspFaceBuffers[6];

	NIRTTI_ADDRESS(0x12708E4);

	static NiRenderedCubeMap* CreateObject();

	static NiRenderedCubeMap* Create(const UInt32 uiSize, NiDX9Renderer* apRenderer) {
		NiTexture::FormatPrefs kPrefs;
		return CdeclCall<NiRenderedCubeMap*>(0xEE8330, uiSize, apRenderer, &kPrefs);
	};

	static NiRenderedCubeMap* Create(const UInt32 uiSize, NiDX9Renderer* apRenderer, const NiTexture::FormatPrefs& kPrefs) {
		return CdeclCall<NiRenderedCubeMap*>(0xEE8330, uiSize, apRenderer, &kPrefs);
	};

	static NiRenderedCubeMap* Create(Ni2DBuffer* pkPosXBuffer,
		Ni2DBuffer* pkNegXBuffer, Ni2DBuffer* pkPosYBuffer,
		Ni2DBuffer* pkNegYBuffer, Ni2DBuffer* pkPosZBuffer,
		Ni2DBuffer* pkNegZBuffer, NiDX9Renderer* pkRenderer);

	void SetCurrentCubeFace(UInt32 eFace) {
		m_eFace = (FaceID)eFace;
		m_spBuffer = m_aspFaceBuffers[eFace];
	}

	void SetCubeFaceBuffer(UInt32 eFace, Ni2DBuffer* pkBuffer) {
		m_aspFaceBuffers[eFace] = pkBuffer;
	}

	Ni2DBuffer* GetFaceBuffer(UInt32 eFace) {
		return m_aspFaceBuffers[eFace];
	}
};

ASSERT_SIZE(NiRenderedCubeMap, 0x64);