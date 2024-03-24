#pragma once

#include "NiTexturingProperty.hpp"
#include "NiD3DTextureStageGroup.hpp"
#include "NiDX9RenderState.hpp"

class NiGlobalConstantEntry;

NiSmartPointer(NiD3DTextureStage);

class NiD3DTextureStage {
public:
	NiTexturingProperty::FilterMode		m_eFilter;
	UInt32								m_uiStage;
	NiTexture*							m_pkTexture;
	UInt32								m_uiTextureFlags;
	NiD3DTextureStageGroup*				m_pkTextureStageGroup;
	UInt32								m_uiTextureTransformFlags;
	NiGlobalConstantEntry*				m_pkGlobalEntry;
	D3DMATRIX							m_kTextureTransformation;
	UInt16								m_usObjTextureFlags;
	bool								m_bTextureTransform;
	bool								m_bRendererOwned;
	UInt32								m_uiRefCount;
	bool								bUnk64;
	bool								bUnk65;
	bool								bUnk66;

	static float fMipMapLODBias;

	static NiDX9RenderState* GetRenderState();
	static NiDX9Renderer* GetRenderer();
	static LPDIRECT3DDEVICE9 GetD3DDevice();

	static void CreateNewStage(NiD3DTextureStagePtr& aspStage);

	void ApplyTexture();
	void ModifyClampMode(bool abNonPow2);
	void ApplyTextureTransform();
	bool ConfigureStage(bool abNonPow2 = false);
	void SetClampMode(NiTexturingProperty::ClampMode eClamp);
	void SetFilterMode(NiTexturingProperty::FilterMode eFilter);
	void SetStageProperties(UInt32 auiTextureIndex, NiTexturingProperty::ClampMode aeClampMode, UInt32 aeFilterValue, bool abUseAnisotropicFilter = true);


	static bool __fastcall ConfigureStageEx(NiD3DTextureStage* apThis, void*, bool abNonPow2);

	static void __fastcall SetFilterMode_Ex(NiD3DTextureStage* apThis, void*, NiTexturingProperty::FilterMode eFilter);

	void ReturnStageToPool();

	void IncRefCount();
	void DecRefCount();
};

ASSERT_SIZE(NiD3DTextureStage, 0x68);