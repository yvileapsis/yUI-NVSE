#pragma once

#include "NiCamera.hpp"
#include "NiScreenPolygon.hpp"
#include "NiScreenTexture.hpp"

NiSmartPointer(NiScreenSpaceCamera);

class NiScreenSpaceCamera : public NiCamera {
public:
	NiScreenSpaceCamera();
	virtual ~NiScreenSpaceCamera();

	NiTObjectArray<NiScreenPolygonPtr> m_kScreenPolygons;
	NiTObjectArray<NiScreenTexturePtr> m_kScreenTextures;

	CREATE_OBJECT(NiScreenSpaceCamera, 0xA91D80)
};

ASSERT_SIZE(NiScreenSpaceCamera, 0x134)