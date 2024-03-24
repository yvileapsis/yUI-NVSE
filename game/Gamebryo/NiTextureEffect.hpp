#pragma once

#include "NiDynamicEffect.hpp"
#include "NiTexture.hpp"
#include "NiTexturingProperty.hpp"

NiSmartPointer(NiTextureEffect);

class NiTextureEffect : public NiDynamicEffect {
public:
	NiTextureEffect();
	~NiTextureEffect();

	enum CoordGenType
	{
		WORLD_PARALLEL = 0x0,
		WORLD_PERSPECTIVE = 0x1,
		SPHERE_MAP = 0x2,
		SPECULAR_CUBE_MAP = 0x3,
		DIFFUSE_CUBE_MAP = 0x4,
		NUM_COORD_GEN = 0x5,
	};

	enum TextureType
	{
		PROJECTED_LIGHT = 0x0,
		PROJECTED_SHADOW = 0x1,
		ENVIRONMENT_MAP = 0x2,
		FOG_MAP = 0x3,
		TEXTURE_TYPE_MAX = 0x4,
	};

	NiMatrix3 m_kModelProjMat;
	NiPoint3 m_kModelProjTrans;
	NiMatrix3 m_kWorldProjMat;
	NiPoint3 m_kWorldProjTrans;
	NiTexturePtr m_spTexture;
	NiTexturingProperty::FilterMode m_eFilter;
	NiTexturingProperty::ClampMode m_eClamp;
	NiTextureEffect::TextureType m_eTextureMode;
	NiTextureEffect::CoordGenType m_eCoordMode;
	bool m_bPlaneEnable;
	NiPlane m_kModelPlane;
	NiPlane m_kWorldPlane;

	CREATE_OBJECT(NiTextureEffect, 0xA929B0)
};

ASSERT_SIZE(NiTextureEffect, 0x15C);