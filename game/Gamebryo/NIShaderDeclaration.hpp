#pragma once

#include "NiObject.hpp"

NiSmartPointer(NiShaderDeclaration);

class  NiShaderDeclaration : public NiObject {
public:
	NiShaderDeclaration();
	virtual ~NiShaderDeclaration();

	enum ShaderParameter
	{
		SHADERPARAM_INVALID = 0xFFFFFFFF,
		SHADERPARAM_NI_POSITION = 0x0,
		SHADERPARAM_NI_BLENDWEIGHT = 0x1,
		SHADERPARAM_NI_BLENDINDICES = 0x2,
		SHADERPARAM_NI_NORMAL = 0x3,
		SHADERPARAM_NI_COLOR = 0x4,
		SHADERPARAM_NI_TEXCOORD0 = 0x5,
		SHADERPARAM_NI_TEXCOORD1 = 0x6,
		SHADERPARAM_NI_TEXCOORD2 = 0x7,
		SHADERPARAM_NI_TEXCOORD3 = 0x8,
		SHADERPARAM_NI_TEXCOORD4 = 0x9,
		SHADERPARAM_NI_TEXCOORD5 = 0xA,
		SHADERPARAM_NI_TEXCOORD6 = 0xB,
		SHADERPARAM_NI_TEXCOORD7 = 0xC,
		SHADERPARAM_NI_TANGENT = 0xD,
		SHADERPARAM_NI_BINORMAL = 0xE,
		SHADERPARAM_DATA_ZERO = 0xF,
		SHADERPARAM_DATA_ONE = 0x10,
		SHADERPARAM_SKIP_COUNT_MASK = 0x40000000,
		SHADERPARAM_EXTRA_DATA_MASK = 0x80000000,
	};

	enum ShaderParameterType
	{
		SPTYPE_FLOAT1 = 0x0,
		SPTYPE_FLOAT2 = 0x1,
		SPTYPE_FLOAT3 = 0x2,
		SPTYPE_FLOAT4 = 0x3,
		SPTYPE_UBYTECOLOR = 0x4,
		SPTYPE_UBYTE4 = 0x5,
		SPTYPE_SHORT2 = 0x6,
		SPTYPE_SHORT4 = 0x7,
		SPTYPE_NORMUBYTE4 = 0x8,
		SPTYPE_NORMSHORT2 = 0x9,
		SPTYPE_NORMSHORT4 = 0xA,
		SPTYPE_NORMUSHORT2 = 0xB,
		SPTYPE_NORMUSHORT4 = 0xC,
		SPTYPE_UDEC3 = 0xD,
		SPTYPE_NORMDEC3 = 0xE,
		SPTYPE_FLOAT16_2 = 0xF,
		SPTYPE_FLOAT16_4 = 0x10,
		SPTYPE_NONE = 0x11,
		SPTYPE_COUNT = 0x12,
	};

	enum ShaderParameterTesselator
	{
		SPTESS_DEFAULT = 0x0,
		SPTESS_PARTIALU = 0x1,
		SPTESS_PARTIALV = 0x2,
		SPTESS_CROSSUV = 0x3,
		SPTESS_UV = 0x4,
		SPTESS_LOOKUP = 0x5,
		SPTESS_LOOKUPPRESAMPLED = 0x6,
		SPTESS_COUNT = 0x7,
	};

	enum ShaderParameterUsage
	{
		SPUSAGE_POSITION = 0x0,
		SPUSAGE_BLENDWEIGHT = 0x1,
		SPUSAGE_BLENDINDICES = 0x2,
		SPUSAGE_NORMAL = 0x3,
		SPUSAGE_PSIZE = 0x4,
		SPUSAGE_TEXCOORD = 0x5,
		SPUSAGE_TANGENT = 0x6,
		SPUSAGE_BINORMAL = 0x7,
		SPUSAGE_TESSFACTOR = 0x8,
		SPUSAGE_POSITIONT = 0x9,
		SPUSAGE_COLOR = 0xA,
		SPUSAGE_FOG = 0xB,
		SPUSAGE_DEPTH = 0xC,
		SPUSAGE_SAMPLE = 0xD,
		SPUSAGE_COUNT = 0xE,
	};

	class ShaderRegisterEntry
	{
	public:
		UInt32						m_uiRegister;
		UInt32						m_uiPackingOffset;
		ShaderParameter				m_eInput;
		ShaderParameterType			m_eType;
		ShaderParameterTesselator	m_eTesselator;
		ShaderParameterUsage		m_eUsage;
		UInt32						m_uiUsageIndex;
	};

	class ShaderRegisterStream
	{
	public:
		bool					m_bValid;
		UInt32					m_uiEntryCount;
		ShaderRegisterEntry*	m_pkEntries;
		UInt32					m_uiStride;
	};

	virtual bool SetEntry(UInt32 uiEntry, UInt32 uiRegister, NiShaderDeclaration::ShaderParameter eInput, NiShaderDeclaration::ShaderParameterType eType, UInt32 uiStream);
	virtual bool RemoveEntry(UInt32 uiEntry, UInt32 uiStream = 0);
};

ASSERT_SIZE(NiShaderDeclaration, 0x8)