#pragma once

#include "NiD3DShaderConstantMap.hpp"
#include "NiD3DTextureStage.hpp"
#include "NiGeometry.hpp"
#include "NiD3DPixelShader.hpp"
#include "NiD3DVertexShader.hpp"
#include "NiTArray.hpp"
#include "NiD3DRenderStateGroup.hpp"

class NiDX9RenderState;

NiSmartPointer(NiD3DPass);

class NiD3DPass {
public:
	NiD3DPass();
	virtual ~NiD3DPass();

	virtual UInt32	SetupShaderPrograms(NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, const NiSkinPartition::Partition* pkPartition, NiGeometryBufferData* pkBuffData, const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, const NiTransform* kWorld, const NiBound* kWorldBound, UInt32 uiPass);
	virtual void	SetupRenderingPass(NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, NiGeometryBufferData* pkBuffData, const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, const NiBound& kWorldBound, unsigned int uiPass);
	virtual UInt32	PostProcessRenderingPass(UInt32 uiPass);

	char											m_szName[16];
	UInt32											m_uiCurrentStage;
	UInt32											m_uiStageCount;
	UInt32											m_uiTexturesPerPass;
	NiTObjectArray<NiPointer<NiD3DTextureStage>>	m_kStages;
	NiD3DRenderStateGroup*							m_pkRenderStateGroup;
	NiD3DShaderConstantMapPtr						m_spPixelConstantMap;
	char*											m_pcPixelShaderProgramFile;
	char*											m_pcPixelShaderEntryPoint;
	char*											m_pcPixelShaderTarget;
	NiD3DPixelShaderPtr								m_spPixelShader;
	NiD3DShaderConstantMapPtr						m_spVertexConstantMap;
	char*											m_pcVertexShaderProgramFile;
	char*											m_pcVertexShaderEntryPoint;
	char*											m_pcVertexShaderTarget;
	float											float58;
	NiD3DVertexShaderPtr							m_spVertexShader;
	bool											m_bSoftwareVP;
	bool											m_bRendererOwned;
	UInt32											m_uiRefCount;

	static NiDX9RenderState* GetD3DRenderState();
	static NiD3DRenderer* GetRenderer();
	static UInt32 GetMaxTextureBlendStages();
	void SetRenderState(D3DRENDERSTATETYPE auiRenderState, UInt32 auiValue, bool abSave = true);
	void SetBlendRenderState(UInt32 uiSrcBlend, UInt32 uiDestcBlend);
	void RemoveRenderState(UInt32 uiRenderState);

	void SetName(const char* apcName);
	const char* GetName() const;

	UInt32 SetStage(UInt32 uiStageNum, NiD3DTextureStage* pkStage);
	NiD3DTextureStage* GetStage(UInt32 auiStage) const {
		return m_kStages.GetAt(auiStage);
	}

	void SetVertexShader(NiD3DVertexShader* pkVertexShader);
	NiD3DVertexShader* GetVertexShader();
	void SetPixelShader(NiD3DPixelShader* pkPixelShader);
	NiD3DPixelShader* GetPixelShader();

	void SetPixelConstantMap(NiD3DShaderConstantMap* pMap);
	void SetVertexConstantMap(NiD3DShaderConstantMap* pMap);

	static void CreateNewPass(NiD3DPassPtr& aspPass);

	void ReturnPassToPool();

	void IncRefCount();

	void DecRefCount();
};

ASSERT_SIZE(NiD3DPass, 0x68);
ASSERT_OFFSET(NiD3DPass, m_uiStageCount, 0x18);