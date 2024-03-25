#pragma once
#include "BSShader.hpp"
#include "SkyShaderProperty.hpp"
#include "NiPoint4.hpp"

class SkyShader : public BSShader
{
public:
	enum VertexShader {
		VS_SKY = 0,
		VS_SKYT = 1,
		VS_SKYMM = 2,
		VS_SKYSTARS = 4,
		VS_SKYOCC = 5,
		VS_SKYCLOUDS = 6,
		VS_SKYCLOUDSFADE = 7,
		VS_SKYFAR = 8,
		VS_SKYCLOUDSI = 9,
		VS_SKYQUADSI = 10,
		VS_SKY_MAX = 11
	};

	enum PixelShader {
		PS_SKY = 0,
		PS_SKYTEX = 1,
		PS_SKYTEXFADE = 2,
		PS_SKYSUNOCCL = 3,
		PS_SKYSHORIZFADE = 4,
		PS_SKYSISUN = 5,
		PS_SKYSICLOUDS = 6,
		PS_SKYSI = 7,
		PS_SKY_MAX = 8
	};

	enum Pass {
		SP_OCCLUSION = 0,
		SP_SUNGLARE = 1,
		SP_MOON_STARS_MASK = 2,
		SP_STARS = 3,
		SP_CLOUDS = 4,
		SP_UNDERWATER = 5,
		SP_TEXTURE = 6,
		SP_SKY = 7,
		SP_SELFILLUM_QUAD = 8,
		SP_SUN = 9,
		SP_SELFILLUM_CLOUDS = 10,
		SP_MAX = 11
	};

	enum SkyBlendLevel {
		BOTTOM = 0,
		CENTER = 1,
		TOP = 2,
	};


	NiD3DPassPtr			spPasses[SP_MAX];
	NiD3DVertexShaderPtr	spVertexShaders[VS_SKY_MAX];
	NiD3DPixelShaderPtr		spPixelShaders[PS_SKY_MAX];
	float					fTexCoordYOff;
	NiPoint2				Params;

	static SkyShader* Create();
	static SkyShader* __stdcall CreateShader();
	void InitShaders();
	bool InitPasses();

	NiD3DPass* GetPass(UInt32 auID) { return spPasses[auID]; }
	NiD3DVertexShader* GetVertexShader(UInt32 auID) { return spVertexShaders[auID]; };
	NiD3DPixelShader* GetPixelShader(UInt32 auID) { return spPixelShaders[auID]; };

	void SetSkyTexture(SkyShaderProperty* apShaderProperty, UInt32 auiStageNumber, NiTexture* apTexture);

	static bool __fastcall InitializeEx(SkyShader* apThis);
	static void __fastcall SetupTexturesEx(SkyShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall UpdateConstantsEx(SkyShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetStencilEx(SkyShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetRenderPassEx(SkyShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall InitShaderConstantsEx(SkyShader* apThis);
	static void __fastcall ReinitializeEx(SkyShader* apThis);
	static void __fastcall ClearAllTextureStagesEx(SkyShader* apThis);

	static SkyShader* GetShader();
	static NiD3DPass* PickPass(BSShaderManager::RenderPassType aeRenderPass, SkyShader* apShader = nullptr);

	static void SetCloudTexCoordOffset(UInt32 auID, float afOffset);
	static float GetCloudTexCoordOffset(UInt32 auID);

	static NiCamera* GetCamera();

	struct VertexConstants {
		static NiColorA* GetBlendColor(UInt32 auID);
		static NiPoint4* EyePosition;
	};
};
static_assert(sizeof(SkyShader) == 0xF0);