#pragma once

#include "BSShader.hpp"
#include "ImageSpaceEffect.hpp"
#include "ImageSpaceShaderParam.hpp"

class ImageSpaceShader : public BSShader, public ImageSpaceEffect {
public:
	virtual void InitShaders(const char* apVertexShaderPath, const char* apPixelShaderPath, const char* apShaderName, const char* apVSTarget, const char* apPSTarget);
	virtual void SetupShaderDeclaration();
	virtual void Func_81();
	virtual void SetParameters(ImageSpaceShaderParam* apParam);
	virtual bool SetupPasses();
	virtual void Func_84();

	NiD3DVertexShaderPtr	spVertexShader;
	NiD3DPixelShaderPtr		spPixelShader;
	ImageSpaceShaderParam*	pParam;

	static void __fastcall SetRenderPassEx(ImageSpaceShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall InitShadersEx(ImageSpaceShader* apThis, void*, const char* apVertexShaderPath, const char* apPixelShaderPath, const char* apShaderName, const char* apVSTarget, const char* apPSTarget);
	static bool __fastcall SetupPassesEx(ImageSpaceShader* apThis);
};

ASSERT_SIZE(ImageSpaceShader, 0xD0);