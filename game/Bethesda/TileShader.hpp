#pragma once

#include "BSShader.hpp"

class TileShader : public BSShader {
public:
	virtual void Func_79();
	virtual void InitVertexShaders();
	virtual void InitPixelShaders();
	virtual bool InitPasses();
	virtual bool Func_83(); // Ret1

	NiD3DPassPtr				spPasses[1];
	NiD3DVertexShaderPtr		spVertexShaders[3]; // 1 = rotation, 2 = alpha
	NiD3DPixelShaderPtr			spPixelShaders[3];
	NiD3DShaderDeclarationPtr	spShaderDeclarations[2]; // 1 for vertex colors

	static void __fastcall SetupTexturesEx(TileShader* apThis, void*, NiPropertyState* apProperties);
};

ASSERT_SIZE(TileShader, 0x90);