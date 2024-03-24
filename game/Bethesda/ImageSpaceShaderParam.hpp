#pragma once

#include "ImageSpaceEffectParam.hpp"
#include "NiTMap.hpp"
#include "NiD3DShaderConstantMap.hpp"
#include "NiRenderer.hpp"
#include "NiColor.hpp"

class ImageSpaceShaderParam : public ImageSpaceEffectParam {
public:
	ImageSpaceShaderParam();
	virtual ~ImageSpaceShaderParam();

	int													dword24;
	NiRenderer::ClearFlags								uiClearMode;
	NiColorA											kBackgroundColor;
	float*												pfVertexConstantGroup;
	UInt32												uiVertexGroupSize;
	float*												pfPixelConstantGroup;
	UInt32												uiPixelGroupSize;
	NiD3DShaderConstantMapPtr							spVertexConstantMap;
	NiD3DShaderConstantMapPtr							spPixelConstantMap;
	UInt16												word54;
	bool												byte56;
	NiTObjectArray<NiPointer<NiTexture>>				kInputTextures;
	NiTPrimitiveArray<NiTexturingProperty::FilterMode>	kInputFilterModes;
	NiTMap<UInt32, UInt32>								TMap78;

	static ImageSpaceShaderParam* Create(ImageSpaceShaderParam* apThis);

	void ClearConstantGroups();
	void ResizeConstantGroup(UInt32 auiVertexGroupSize, UInt32 auiPixelGroupSize);

	void SetVertexConstants(UInt32 auiIndex, float afVal1, float afVal2, float afVal3, float afVal4);
	void SetPixelConstants(UInt32 auiIndex, float afVal1, float afVal2, float afVal3, float afVal4);
};

ASSERT_SIZE(ImageSpaceShaderParam, 0x88);