#pragma once

#include "BSRenderedTexture.hpp"
#include "NiDX9Renderer.hpp"
#include "ImageSpaceManager.hpp"
#include "BSShaderManager.hpp"

class NiDevImageConverter;

class BSRenderer {
public:
	static UInt32* const iSizeW;
	static UInt32* const iSizeH;
	static UInt32* const uiFrameBufferMode;

	static HWND* const pWindowHandle;

	static NiDX9Renderer* __cdecl CreateRenderer(HWND aHWND, HINSTANCE aHINSTANCE);

	static void SetImageConverter(NiDevImageConverter* apConverter);
	
	static void __fastcall DrawSkinnedPrimitive(NiDX9Renderer* apRenderer, void*, NiGeometryBufferData* apBuffData, const NiSkinPartition::Partition* apSkinPartition, NiGeometryData* apGeoData);
	static void ProcessImageSpaceShaders(NiDX9Renderer* apRenderer, BSRenderedTexture* apSourceTarget, BSRenderedTexture* apDestTarget);

	static __forceinline UInt32 GetFrameBufferMode() { return *(UInt32*)0x11C70CC; }
	static __forceinline bool IsMultisamplingEnabled() { return GetFrameBufferMode() >= 2; }
	static __forceinline bool IsTransparencyMultisamplingEnabled() { return *(bool*)0x11F9421; }
	static __forceinline bool IsSLI() { return *(bool*)0x11F9440; }
};