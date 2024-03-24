#pragma once

#include "NiNode.hpp"
#include "NiCamera.hpp"

class NiTexture;

class ScreenCustomSplatter {
public:

	static __forceinline NiNode* GetRootNode() { return *(NiNode**)0x11C7810; };
	
	static __forceinline NiCamera* GetCamera() { return *(NiCamera**)0x11C781C; };

	static __forceinline bool HasNodes() {
		if (!GetRootNode()) 
			return false;
		return GetRootNode()->GetChildCount() != 0;
	}

	static bool* const bEnabled;
	static UInt32* const uiCount;

	static NiNode* CreateGeometry(UInt32 auiCount, float afSizeMult, float afOpacityMult);
	static void Activate(UInt32 auiCount, float afDuration, float afSizeMult, float afOpacityMult, NiTexture* apAlphaTex, NiTexture* apColorTex, NiTexture* apFlareTex);
};