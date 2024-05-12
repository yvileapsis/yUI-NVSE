#pragma once

#include "NiRefObject.hpp"
#include "NiColor.hpp"
#include "NiDX9MaterialManager.hpp"
#include "NiTransform.hpp"

class NiAlphaProperty;
class NiDitherProperty;
class NiDitherProperty;
class NiDX9Renderer;
class NiDX9ShaderConstantManager;
class NiFogProperty;
class NiFogProperty;
class NiMaterialProperty;
class NiPropertyState;
class NiShadeProperty;
class NiSpecularProperty;
class NiSpecularProperty;
class NiStencilProperty;

class NiD3DRenderState : public NiRefObject {
public:
	NiD3DRenderState();
	virtual ~NiD3DRenderState();

	enum NiD3DSamplerState
	{
		NISAMP_INVALID = 0xFFFFFFFF,
		NISAMP_ADDRESSU = 0x0,
		NISAMP_ADDRESSV = 0x1,
		NISAMP_ADDRESSW = 0x2,
		NISAMP_BORDERCOLOR = 0x3,
		NISAMP_MAGFILTER = 0x4,
		NISAMP_MINFILTER = 0x5,
		NISAMP_MIPFILTER = 0x6,
		NISAMP_MIPMAPLODBIAS = 0x7,
		NISAMP_MAXMIPLEVEL = 0x8,
		NISAMP_MAXANISOTROPY = 0x9,
		NISAMP_SRGBTEXTURE = 0xA,
		NISAMP_ELEMENTINDEX = 0xB,
		NISAMP_DMAPOFFSET = 0xC,
		NISAMP_TOTAL_COUNT = 0xD,
	};

	virtual void							UpdateRenderState(const NiPropertyState* pkNew);
	virtual void							ApplyAlpha(const NiAlphaProperty* pkNew);
	virtual void							ApplyDitherMode(const NiDitherProperty* pkNew);
	virtual void							ApplyFog(const NiFogProperty* pkNew);
	virtual void							ApplyMaterial(const NiMaterialProperty* pkNew);
	virtual void							ApplyShading(const NiShadeProperty* pkNew);
	virtual void							ApplySpecular(const NiSpecularProperty* pkNew);
	virtual void							ApplyStencil(const NiStencilProperty* pkNew);
	virtual void							DisableAlpha();
	virtual void							SetBoneCount(UInt16 usNumBones);
	virtual void							vSetModelTransform(const NiTransform* kXform);
	virtual void							SetAntiAlias(bool bOn);
	virtual float							GetMaxFogValue();
	virtual float							GetMaxFogFactor();
	virtual void							SetMaxFogValue(float fFogVal);
	virtual void							GetCameraNearAndFar(float* fNear, float* fFar);
	virtual void							SetCameraNearAndFar(float fNear, float fFar);
	virtual bool							GetLeftHanded();
	virtual void							SetLeftHanded(bool bLeft);
	virtual void							PurgeMaterial(NiMaterialProperty* pkMaterial);
	virtual void							InitRenderState();
	virtual void							SetDestAndSrcBlends(D3DBLEND sourceBlend, D3DBLEND destBlend, int a4, int a5);
	virtual void							SetAlphaBlendEnable(DWORD aValue, UInt32 unk0 = 0, UInt32 unk1 = 0);
	virtual void							SetDepthBias(float afBias, bool abSave = false);
	// Do not use
	virtual void							vSetRenderState(D3DRENDERSTATETYPE eState, UInt32 uiValue, int, bool bSave);
	virtual UInt32							GetRenderState(D3DRENDERSTATETYPE eState);
	virtual void							InitShaderState();
	virtual void							SaveShaderState();
	virtual void							RestoreShaderState();
	virtual void							SetPixelShader(LPDIRECT3DPIXELSHADER9 hShader, bool abSave = false);
	virtual LPDIRECT3DPIXELSHADER9			GetPixelShader();
	virtual void							RestorePixelShader();
	virtual void							ClearPixelShader(LPDIRECT3DPIXELSHADER9 hShader);
	virtual void							SetVertexShader(LPDIRECT3DVERTEXSHADER9 hShader, bool abSave = false);
	virtual LPDIRECT3DVERTEXSHADER9			GetVertexShader();
	virtual void							RestoreVertexShader();
	virtual void							ClearVertexShader(LPDIRECT3DVERTEXSHADER9 hShader);
	virtual void							SetFVF(UInt32 uiFVF, bool bSave);
	virtual UInt32							GetFVF();
	virtual void							RestoreFVF();
	virtual void							ClearFVF(UInt32 uiFVF);
	// Do not use
	virtual void							vSetDeclaration(LPDIRECT3DVERTEXDECLARATION9 hDeclaration, bool bSave);
	virtual LPDIRECT3DVERTEXDECLARATION9	GetDeclaration();
	virtual void							RestoreDeclaration();
	virtual void							ClearDeclaration(LPDIRECT3DVERTEXDECLARATION9 hDeclaration);
	virtual void							InitTextureStageState();
	virtual void							SetTextureStageState(UInt32 uiStage, D3DTEXTURESTAGESTATETYPE eState, UInt32 uiValue, bool bSave);
	virtual UInt32							GetTextureStageState(UInt32 uiStage, D3DTEXTURESTAGESTATETYPE eState);
	virtual void							RestoreTextureStageState(UInt32 uiStage, D3DTEXTURESTAGESTATETYPE eState);
	virtual void							SetSamplerState(UInt32 uiStage, D3DSAMPLERSTATETYPE eState, UInt32 uiValue, bool bSave);
	virtual UInt32							GetSamplerState(UInt32 uiStage, NiD3DSamplerState eState);
	virtual void							RestoreSamplerState(UInt32 uiStage, NiD3DSamplerState eState);
	virtual void							InitTextures();
	// Do Not Use
	virtual void							vSetTexture(UInt32 uiStage, LPDIRECT3DBASETEXTURE9 pkTexture); 
	// Do Not Use
	virtual LPDIRECT3DBASETEXTURE9			vGetTexture(UInt32 uiStage);
	virtual void							ClearTexture(LPDIRECT3DBASETEXTURE9 pkTexture);
	virtual void							SetSoftwareVertexProcessing(bool bSoftware);
	virtual void							GetSoftwareVertexProcessing();
	virtual void							SetForceNormalizeNormals(bool bSet);
	virtual void							GetForceNormalizeNormals();
	virtual void							SetInternalNormalizeNormals(bool bSet);
	virtual void							GetInternalNormalizeNormals();
	virtual void							InitRenderModes();
	virtual void							ReadRenderStateCaps();

	struct NiRenderStateSetting {
		UInt32 m_uiCurrValue;
		UInt32 m_uiPrevValue;
	};

	UInt32							m_uiFlags;
	D3DVERTEXBLENDFLAGS				m_aD3DVertexBlendMappings[5];
	D3DBLEND						m_auiAlphaMapping[11];
	D3DCMPFUNC						m_auiAlphaTestMapping[8];
	UInt32							m_uiSrcAlphaCaps;
	UInt32							m_uiDestAlphaCaps;
	NiPointer<NiAlphaProperty>		m_spDisabledAlphaProperty;
	float							m_fCameraNear;
	float							m_fCameraFar;
	float							m_fCameraDepthRange;
	float							m_fMaxFogFactor;
	float							m_fMaxFogValue;
	NiColor							m_kCurrentFogColor;
	UInt32							m_uiCurrentFogColor;
	D3DCMPFUNC						m_auiStencilTestMapping[8];
	D3DSTENCILOP					m_auiStencilActionMapping[6];
	D3DCULL							m_auiCullModeMapping[4][2];
	UInt32							m_uiLeftHanded;
	NiDX9MaterialManager			m_kMaterials;
	D3DCMPFUNC						m_auiZBufferTestMapping[8];
	NiRenderStateSetting			m_akRenderStateSettings[256];
	bool							m_bSavedStates[256];
	NiRenderStateSetting			m_akTextureStageStateSettings[16][8];
	NiRenderStateSetting			m_akSamplerStateSettings[16][5];
	LPDIRECT3DBASETEXTURE9			m_apkTextureStageTextures[16];
	LPDIRECT3DVERTEXSHADER9			m_hCurrentVertexShader;
	LPDIRECT3DVERTEXSHADER9			m_hPreviousVertexShader;
	LPDIRECT3DPIXELSHADER9			m_hCurrentPixelShader;
	LPDIRECT3DPIXELSHADER9			m_hPreviousPixelShader;
	NiDX9ShaderConstantManager*		m_spShaderConstantManager;
	bool							m_bForceNormalizeNormals;
	bool							m_bInternalNormalizeNormals;
	LPDIRECT3DDEVICE9				m_pkD3DDevice;
	NiDX9Renderer*					m_pkD3DRenderer;

	void					SetModelTransform(const NiTransform& kXform);
	void					SetTexture(UInt32 uiStage, LPDIRECT3DBASETEXTURE9 pkTexture);
	LPDIRECT3DBASETEXTURE9	GetTexture(UInt32 uiStage);
	void					SetRenderState(D3DRENDERSTATETYPE eState, UInt32 uiValue, bool bSave = false);
};

ASSERT_SIZE(NiD3DRenderState, 0x1100);