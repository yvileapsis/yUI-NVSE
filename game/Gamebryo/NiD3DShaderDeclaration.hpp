#pragma once

#include "NiShaderDeclaration.hpp"
#include "NiSkinInstance.hpp"
#include "NiSkinPartition.hpp"

class NiDX9VertexBufferManager;
class NiVBChip;
class NiGeometryData;

NiSmartPointer(NiD3DShaderDeclaration);

class NiD3DShaderDeclaration : public NiShaderDeclaration {
public:
	NiD3DShaderDeclaration();
	virtual ~NiD3DShaderDeclaration();

	virtual bool					SetEntryAlt(UInt32 uiStream, UInt32 uiEntry, UInt32 uiRegister, NiShaderDeclaration::ShaderParameter eInput, NiShaderDeclaration::ShaderParameterType eType, NiShaderDeclaration::ShaderParameterUsage eUsage, UInt32 uiIndex, NiD3DShaderDeclaration::ShaderParameterTesselator eTess);
	virtual void					SetConstantData(UInt32 uiStartRegister, UInt32 uiNumRegisters, float* pfData);
	virtual void					ClearAllEntries(UInt32 uiStream);
	virtual UInt32					GetVertexStride(UInt32 uiStream);
	virtual void					SetSoftwareVertexProcessing();
	virtual bool					GetSoftwareVertexProcessing();
	virtual NiVBChip*				PackUntransformedVB(NiGeometryData* pkData, NiSkinInstance* pkSkin, NiSkinPartition::Partition* pkPartition, UInt16 usDirtyFlags, NiVBChip* pkOldVBChip, UInt32 uiStream, void* pvLockedBuff);
	virtual bool					GenerateVertexParameters(LPDIRECT3DVERTEXDECLARATION9* pkDeclaration, UInt32* uiNumStreams);
	virtual LPDIRECT3DVERTEXDECLARATION9	GetD3DDeclaration();

	NiD3DRenderer*								m_pkRenderer;
	NiDX9VertexBufferManager*					m_pkVBManager;
	LPDIRECT3DDEVICE9								m_pkD3DDevice;
	UInt32										m_uiDeclSize;
	UInt32										m_uiCurrDeclIndex;
	UInt32										m_uiMaxStreamEntryCount;
	UInt32										m_uiStreamCount;
	ShaderRegisterStream*						m_pkStreamEntries;
	bool										m_bModified;

	static UInt32 GetTypeSize(UInt32 aeType);
};

ASSERT_SIZE(NiD3DShaderDeclaration, 0x02C);