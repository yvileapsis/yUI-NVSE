#pragma once

#include "BSTextureSet.hpp"
#include "TESTexture.hpp"
#include "TESBoundObject.hpp"

class BGSTextureSet : public TESBoundObject, BSTextureSet {
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
		enum
		{
			kFlag_Parallax = 0x01,
			kFlag_AlphaBlend = 0x02,
			kFlag_AlphaTest = 0x04,
		};

		float minWidth;
		float maxWidth;
		float minHeight;
		float maxHeight;
		float depth;
		float shininess;
		float parallaxScale;
		UInt8 parallaxPasses;
		UInt8 flags;
		UInt8 pad1E[2];
		UInt32 color;
	};

	TESTexture TexturesA[6];
	BGSTextureSet::DecalInfo* pDecalData;
	UInt16 m_usFlags;
	UInt32 unk88[6];
};

ASSERT_SIZE(BGSTextureSet, 0xA0);