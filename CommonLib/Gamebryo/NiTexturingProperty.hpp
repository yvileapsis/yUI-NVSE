#pragma once

#include "NiProperty.hpp"
#include "NiTextureTransform.hpp"

class NiTexture;

NiSmartPointer(NiTexturingProperty);

class NiTexturingProperty : public NiProperty {
public:
	NiTexturingProperty();
	virtual ~NiTexturingProperty();

	enum ApplyMode
	{
		APPLY_REPLACE = 0x0,
		APPLY_DECAL = 0x1,
		APPLY_MODULATE = 0x2,
		APPLY_DEPRECATED = 0x3,
		APPLY_DEPRECATED2 = 0x4,
		APPLY_MAX_MODES = 0x5,
	};

	enum FilterMode
	{
		FILTER_NEAREST = 0x0,
		FILTER_BILERP = 0x1,
		FILTER_TRILERP = 0x2,
		FILTER_NEAREST_MIPNEAREST = 0x3,
		FILTER_NEAREST_MIPLERP = 0x4,
		FILTER_BILERP_MIPNEAREST = 0x5,
		FILTER_ANISOTROPY = 0x6,
		FILTER_ANISOTROPY_2 = 0x7,
		FILTER_MAX_MODES = 0x8,
	};

	enum ClampMode
	{
		CLAMP_S_CLAMP_T = 0x0,
		CLAMP_S_WRAP_T = 0x1,
		WRAP_S_CLAMP_T = 0x2,
		WRAP_S_WRAP_T = 0x3,
		CLAMP_MAX_MODES = 0x4,
	};

	enum Flags : UInt16
	{
		MULTITEXTURE_MASK = 0x1,
		APPLYMODE_MASK = 0xE,
		APPLYMODE_POS = 0x1,
		DECALCOUNT_MASK = 0xFF0,
		DECALCOUNT_POS = 0x4,
	};

	class Map
	{
	public:
		void*				__vftable /*VFT*/;
		UInt16				m_usflags;
		NiTexture*			m_spTexture;
		NiTextureTransform* m_pkTextureTransform;
	};

	UInt16											m_usflags;
	NiTPrimitiveArray<NiTexturingProperty::Map*>	m_kMaps;
	NiTPrimitiveArray<void>*						m_pkShaderMaps;

	CREATE_OBJECT(NiTexturingProperty, 0xA6AD40)
};

ASSERT_SIZE(NiTexturingProperty, 0x30)