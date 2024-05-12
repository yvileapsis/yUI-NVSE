#pragma once

#include "NiDepthStencilBuffer.hpp"
#include "NiDX9Renderer.hpp"
#include "NiSourceTexture.hpp"
#include "NiTListBase.hpp"
#include "ShadowSceneLight.hpp"
#include "NiSmartPointer.hpp"

NiSmartPointer(BSRenderedTexture);

enum RenderTargetTypes {
	BSTM_RT_HDR_DOWNSAMPLE = 0x0,
	BSTM_RT_HDR_DOWNSAMPLE_ANISOINTERMEDIATE = 0x1,
	BSTM_RT_HDR_BLURSWAP = 0x2,
	BSTM_RT_IMAGESPACE_SWAP = 0x3,
	BSTM_RT_MAIN_IMAGESPACE = 0x4,
	BSTM_RT_MAIN_FIRSTPERSON = 0x5,
	BSTM_RT_MAIN_MENUBG = 0x6,
	BSTM_RT_TESWATER_FFT = 0x7,
	BSTM_RT_TESWATER_GREY = 0x8,
	BSTM_RT_TESWATER_REFLECTION = 0x9,
	BSTM_RT_TESWATER_LOD_REFLECTION = 0xA,
	BSTM_RT_TESWATER_REFRACTION = 0xB,
	BSTM_RT_TESWATER_RAIN = 0xC,
	BSTM_RT_TESWATER_SCRAMBLEMAP = 0xD,
	BSTM_RT_TESWATER_IFFT_BUTTERFLYMAP = 0xE,
	BSTM_RT_TESWATER_HIGH_W = 0xF,
	BSTM_RT_TESWATER_HIGH_AMPLITUDE = 0x10,
	BSTM_RT_TESWATER_DEPTH = 0x11,
	BSTM_RT_TESWATER_NOISE_HEIGHTMAP = 0x12,
	BSTM_RT_TESWATER_NOISE_NORMALMAP = 0x13,
	BSTM_RT_TESWATER_DISPLACEMENT = 0x14,
	BSTM_RT_TESWATER_DEBUG_DISPLAY = 0x15,
	BSTM_RT_TESWATER_BLUR = 0x16,
	BSTM_RT_TREES_CANOPYSHADOWMASK = 0x17,
	BSTM_RT_BLURFULL_BUFFER = 0x18,
	BSTM_RT_BLURFULL_SWAP = 0x19,
	BSTM_RT_BLUR20_BUFFER = 0x1A,
	BSTM_RT_GETHIT_BUFFER = 0x1B,
	BSTM_RT_LOCALMAP_FINAL = 0x1C,
	BSTM_RT_LOCALMAP_COLOR = 0x1D,
	BSTM_RT_LOCALMAP_NORMALS = 0x1E,
	BSTM_RT_REFRACTION_NORMALS = 0x1F,
	BSTM_RT_VFOG = 0x20,
	BSTM_RT_SCENE_DEPTH_BUFFER = 0x21,
	BSTM_RT_VFOG_SWAP = 0x22,
	BSTM_RT_VELOCITY = 0x23,
	BSTM_RT_VELOCITY_SWAP = 0x24,
	BSTM_RT_DOF_BLUR = 0x25,
	BSTM_RT_DOF_BLUR_SWAP = 0x26,
	BSTM_RT_DOF_MASK = 0x27,
	BSTM_RT_DEPTH_RADIALBLUR_BLUR = 0x28,
	BSTM_RT_CUBEMAP_CAMERA = 0x29,
	BSTM_RT_SHADOWS_SHADOWMAP = 0x2A,
	BSTM_RT_SHADOWS_SHADOWMAPFRUSTUM = 0x2B,
	BSTM_RT_ENVMAP_WINDOWS = 0x2C,
	BSTM_RT_INTERFACE_RENDEREDMENU = 0x2D,
	BSTM_RT_INTERFACE_RENDEREDMENU_SWAP = 0x2E,
	BSTM_RT_INTERFACE_VATS = 0x2F,
	BSTM_RT_INTERFACE_TEMP = 0x30,
	BSTM_RT_NOISE_BUFFER = 0x31,
	BSTM_RT_DECAL_OCCLUSION = 0x33,
	BSTM_RT_MAX = 0x34
};

enum CreationFlags {
	BSTM_CF_NONE				= 0,
	BSTM_CF_UNK_1				= 1 << 0,
	BSTM_CF_UNK_2				= 1 << 1,
	BSTM_CF_NO_STENCIL			= 1 << 2,
	BSTM_CF_NO_RENDER_TARGET	= 1 << 3,
	BSTM_CF_CUBE_MAP			= 1 << 4,
	BSTM_CF_UNK_20				= 1 << 5,
	BSTM_CF_USE_MSAA			= 1 << 6,
	BSTM_CF_NO_DEPTH_DISCARD	= 1 << 7,
	BSTM_CF_NO_DEPTH			= 1 << 8,
	BSTM_CF_DONT_FREE			= 1 << 9,
	BSTM_CF_UNK_400				= 1 << 10,
};


/*
Stores and manages rendered textures. 
*/
struct BSTextureManager {
	enum eTextureType { // Is it the official name for RenderTargetTypes? Name from 0x10232BC
	};

	class RenderedTextureEntry : public BSMemObject {
	public:
		BSRenderedTexturePtr	spTexture;
		D3DFORMAT				eFormat;
		RenderedTextureEntry*	pNext;
		Bitfield32				uiFlags;
		bool					bLoaded;
		RenderTargetTypes		eRenderTargetType;
	};

	struct TextureLists {
		NiTPointerList<BSRenderedTexturePtr> kShadowMaps;
		NiTPointerList<BSRenderedTexturePtr> kWaterDepthMaps;
		NiTPointerList<BSRenderedTexturePtr> kWaterReflectionMaps;
		NiTPointerList<BSRenderedTexturePtr> kWaterBlurMaps;
		NiTPointerList<BSRenderedTexturePtr> kWaterNoiseMaps;
	};
	

	TextureLists							kUnusedShaderTextures;
	TextureLists							kUsedShaderTextures;
	NiTPointerList<RenderedTextureEntry*>	kUnusedTextures;
	NiTPointerList<RenderedTextureEntry*>	kUsedTextures;
	NiTPointerList<BSRenderedTexture*>		kUnusedShadowMaps;
	NiTPointerList<BSRenderedTexture*>		kUsedShadowMaps;
	NiDepthStencilBufferPtr					spDepthStencilBuffer;
	NiObjectPtr								spUnk0AC;

	static __forceinline BSTextureManager* GetSingleton() { return *(BSTextureManager**)0x11F91A8; }

	BSRenderedTexture* AcquireRenderTarget(NiDX9Renderer* apRenderer, UInt32 aeType, NiRefObjectPtr spTexture1, NiRefObjectPtr spTexture2, NiRefObjectPtr spTexture3);
	// Separate func to avoid NiPointer refcounting
	BSRenderedTexture* AcquireRenderTarget(NiDX9Renderer* apRenderer, UInt32 aeType);
	BSRenderedTexture* AcquireRenderTarget(NiDX9Renderer* apRenderer, UInt32 aeType, UInt32 unk, UInt32 uiBackgroundColor);

	// Looks like it cleans up textures from lists
	__declspec(noinline) void RetireTexture(BSRenderedTexture* apRenderedTexture);
	void RetireShadowMap(BSRenderedTexture* apShadowTexture);
	void FreeUnusedTextures(NiCamera* apCamera = nullptr, bool abIsMenuMode = false);

	void SetShadowCount(NiRenderer* apRenderer, UInt32 auiActorShadowCount);
	BSRenderedTexture* GetShadowMap();

	static BSRenderedTexture* CreateRenderedTexture(UInt32 uiWidth, UInt32 uiHeight, UInt32 auiFlags, D3DFORMAT eFormat, UInt32 auiMSAA);

	static BSRenderedTexture* CreateRenderedCubemap(UInt32 uiSize, UInt32 auiFlags, D3DFORMAT eFormat, UInt32 auiMSAA);

	static void __fastcall SetRenderTargetSizes(BSTextureManager* apThis, void*, NiDX9Renderer* apRenderer, RenderTargetTypes aeRenderTargetType, UInt32* apWidth, UInt32* apHeight, D3DFORMAT* apFormat, DWORD* a6, UInt32* apFlags, D3DMULTISAMPLE_TYPE* apMSAA);

	static UInt32 GetMaxAnisotropy();

	static void SetWaterNoiseSizes(UInt32 auiWidth, UInt32 auiHeight);
	static void GetWaterNoiseSizes(UInt32* auiWidth, UInt32* auiHeight);

	static NiSourceTexturePtr spDebugMipmapsTexture;
	__declspec(noinline) static NiTexture* GetDebugMipmapsTexture();

	static const NiTexture::FormatPrefs kDefaultFormatPrefs;
};

ASSERT_SIZE(BSTextureManager, 0xB0);