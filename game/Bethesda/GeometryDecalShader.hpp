#pragma once

#include "BSShader.hpp"

class GeometryDecalShader : public BSShader {
public:
	virtual void Func_79();

	NiD3DPassPtr					spPasses[2];
	NiD3DVertexShaderPtr			spVertexShaders[2];
	NiD3DPixelShaderPtr				spPixelShaders[2];
	NiShaderConstantMapEntryPtr		spWorldViewProjTranspose;
	NiShaderConstantMapEntryPtr		spSkinModelViewProj;
	NiShaderConstantMapEntryPtr		spBones;

	static void __fastcall SetupTexturesEx(GeometryDecalShader* apThis, void*, NiPropertyState* apProperties);
};

ASSERT_SIZE(GeometryDecalShader, 0x90);