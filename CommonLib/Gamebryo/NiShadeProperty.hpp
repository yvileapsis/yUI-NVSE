#pragma once

#include "NiProperty.hpp"

NiSmartPointer(NiShadeProperty);

class NiShadeProperty : public NiProperty {
public:
	NiShadeProperty();
	virtual ~NiShadeProperty();

	enum ShaderPropType : UInt32 {
		PROP_Lighting			= 1,
		PROP_DistantLOD			= 2,
		PROP_GeometryDecal		= 3,
		PROP_TallGrass			= 4,
		//						  5,
		PROP_SpeedTreeLeaf		= 6,
		//						  7,
		PROP_PPLighting			= 8,
		PROP_Hair				= 9,
		PROP_SpeedTreeBranch	= 10,
		PROP_SpeedTreeBillboard = 11,
		PROP_Lighting30			= 12,
		PROP_Sky				= 13,
		PROP_Water				= 14,
		PROP_Bolt				= 15,
		//						  16,
		PROP_Particle			= 17,
		PROP_Precipitation		= 18,
		PROP_Tile				= 19,
		//						  20,
		PROP_NoLighting			= 21,
		PROP_VolumetricFog		= 22,
		PROP_BloodSplatter		= 23,
		PROP_DistantTree		= 24,
	};

	enum ExtraFlags {
		EF_IsPlayer = 0x2,
	};

	Bitfield16		m_usFlags;
	ShaderPropType	m_eShaderType;

	CREATE_OBJECT(NiShadeProperty, 0xA814E0);
	NIRTTI_ADDRESS(0x11F5AE0);

	__forceinline const char* GetShaderType() { return NiShadeProperty::GetShaderType(m_eShaderType); };
	static const char* GetShaderType(UInt32 eType);

	bool IsLightShader() const { return m_eShaderType < 13; };
};

ASSERT_SIZE(NiShadeProperty, 0x20);