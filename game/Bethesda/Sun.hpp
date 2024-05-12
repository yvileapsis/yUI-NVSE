#pragma once

#include "NiTriShape.hpp"
#include "SkyObject.hpp"
#include "NiBillboardNode.hpp"
#include "BSShaderAccumulator.hpp"
#include "NiDirectionalLight.hpp"
#include "SkyShaderProperty.hpp"

class NiPick;

class Sun : public SkyObject {
public:
	NiBillboardNodePtr		spSunBaseNode;
	NiBillboardNodePtr		spSunGlareNode;
	NiTriShapePtr			spSunBase;
	NiTriShapePtr			spSunQuery;
	NiPick*					pSunPick; // Unused
	NiDirectionalLightPtr	spLight;
	float					fGlareScale;
	bool					bDoOcclusionTests;
	BSShaderAccumulatorPtr	spSunAccumulator;

	void StartSunOcclusion();

	void CalculateSunGlare(UInt32 aiTestNumber, BSShaderAccumulator* apAccumulator, NiCamera* apCamera);

	float GetAlphaColor() {
		NiTriShape* pSunQuery = spSunQuery;
		return pSunQuery->GetShadeProperty<SkyShaderProperty>()->kVertexColor.a;
	}
};

ASSERT_SIZE(Sun, 0x2C);