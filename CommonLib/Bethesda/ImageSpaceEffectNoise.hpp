#pragma once

#include "ImageSpaceEffect.hpp"
#include "NiPoint4.hpp"

class ImageSpaceEffectNoise : public ImageSpaceEffect {
public:
	ImageSpaceTexture	pNoiseMap;
	ImageSpaceTexture	pTexBuffer;
	ImageSpaceTexture	pPermutedTex2D;
	ImageSpaceTexture	pPermutedGradientTex;
	char				p[512];
	float				fTexRatio0[4];
	float				fTexRatio1[4];
	bool				bIsLoaded;

	static NiPoint4* pTexScrolls;
	static float* pTexScales;
	static float* pNoiseAmplitudes;

	static ImageSpaceEffectNoise*	GetSingleton();
	bool							IsLoaded();
	UInt32							InitializeUtilityBuffers(SInt32 aiSeed);
	void							ShufflePermuationTable();

	static void						SetNoiseScale(float afScale);
	static float					GetNoiseScale();
	static void						SetTexScale(float afValue, UInt32 auiLayer);
	static NiPoint4 				GetTexScale();
	static void						SetTexScroll(NiPoint4 afScroll, UInt32 auiLayer);
	static void						GetTexScrolls(NiPoint4* apfScroll0, NiPoint4* apfScroll1, NiPoint4* apfScroll2);
	static void						SetNoiseAmplitude(float afValue, UInt32 auiLayer);
	static NiPoint4 				GetNoiseAmplitude();
};

ASSERT_SIZE(ImageSpaceEffectNoise, 0x2BC);