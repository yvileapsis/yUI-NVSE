#pragma once

#include "BSShader.hpp"

class WaterShader : public BSShader {
public:
	NiD3DPassPtr			spPasses[74];
	NiD3DVertexShaderPtr	spVertexShaders[3];
	NiD3DPixelShaderPtr		spPixelShaders[38];
	NiRefObjectPtr			spObject238;
	NiRefObjectPtr			spObject23C;
	int						dword240;
	NiRefObjectPtr			spObject244;
	NiRefObjectPtr			spObject248;
	int						dword24C;

	static WaterShader* GetShader();
	static NiD3DPass* PickPass(BSShaderManager::RenderPassType aeRenderPass, WaterShader* apShader = nullptr);

	static __forceinline void SetIsUnderwater(bool abIsUnderwater) { *(bool*)0x11FF104 = abIsUnderwater; };
	static __forceinline bool IsUnderwater() { return *(bool*)0x11FF104; };

	static __forceinline void SetSunPosition(float x, float y, float z, float w) {
		*(NiPoint4*)0x11FF398 = NiPoint4(x, y, z, w);
	};

	static __forceinline void SetSunColor(float r, float g, float b, float a) {
		*(NiColorA*)0x11FF3A8 = NiColorA(r, g, b, a);
	};

	static void __fastcall SetupTexturesEx(WaterShader* apThis, void*, NiPropertyState* apProperties);
};

ASSERT_SIZE(WaterShader, 0x250);