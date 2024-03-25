#pragma once
#include "BSTextureSet.hpp"
#include "TESTexture.hpp"
#include "TESBoundObject.hpp"

class BGSTextureSet :
	public TESBoundObject,
	public BSTextureSet
{
public:
	enum	// texture types
	{
		kDiffuse = 0,
		kNormal,
		kEnvMask,
		kGlow,
		kParallax,
		kEnv
	};

	enum
	{
		kTexFlag_NoSpecMap = 0x0001,
	};

	struct DecalInfo
	{
		enum EnumFlags : UInt8
		{
			kFlag_Parallax		= 1 << 0,
			kFlag_AlphaBlend	= 1 << 1,
			kFlag_AlphaTest		= 1 << 2,
		};

		Float32		fMinWidth;			// 00
		Float32		fMaxWidth;			// 04
		Float32		fMinHeight;			// 08
		Float32		fMaxHeight;			// 0C
		Float32		fDepth;				// 10
		Float32		fShininess;			// 14
		Float32		fParallaxScale;		// 18
		UInt8		ucParallaxPasses;	// 1C
		EnumFlags	eFlags;				// 1D
		UInt8		pad1E[2];			// 1E
		UInt32		uiColor;			// 20
	};

	TESTexture	kTexturesA[6];
	DecalInfo*	pkDecalData;
	UInt16		m_usFlags;
	UInt32		unk88[6];
};
static_assert(sizeof(BGSTextureSet) == 0xA0);