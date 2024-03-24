#pragma once

#include "INISettingCollection.hpp"

class RendererSettingCollection : public INISettingCollection {
public:
	static RendererSettingCollection* GetSingleton();

	struct BlurShader {
		static bool * const bUseBlurShader;
	};

	struct BlurShaderHDR {
		static bool * const bDoHighDynamicRange;
	};

	struct Display {
		static bool * const bDrawShadows;
		static bool * const bEnableEyefinity;
		static bool * const bFullScreen;
		static bool * const bTransparencyMultisampling;

		static SInt32 * const iActorShadowCountExt;
		static SInt32 * const iActorShadowCountInt;
		static SInt32 * const iAdapter;
		static SInt32 * const iMaxAnisotropy;
		static SInt32 * const iMaxDecalsPerFrame;
		static SInt32 * const iMaxSkinDecalsPerFrame;
		static SInt32 * const iMultiSample;
		static SInt32 * const iShadowFilter;
		static SInt32 * const iShadowMapResolution;
		static SInt32 * const iShadowMode;
		static SInt32 * const iSizeH;
		static SInt32 * const iSizeW;
		static SInt32 * const iTexMipMapMinimum;
		static SInt32 * const iTexMipMapSkip;
		static SInt32 * const iWaterMultiSamples;

		static char * const sD3DDevice;

		static float * const fDecalLOD1;
		static float * const fDecalLOD2;
		static float * const fLightLODStartFade;
		static float * const fShadowLODStartFade;
		static float * const fSpecularLODStartFade;
	};

	struct Decals {
		static UInt32 * const uMaxDecals;
	};

	struct Grass {
		static bool * const b30GrassVS;

		static float * const fGrassStartFadeDistance;
	};

	struct Imagespace {
		static bool * const bDoDepthOfField;

		static SInt32 * const iRadialBlurLevel;
	};

	struct LOD {
		static float * const fLODFadeOutMultActors;
		static float * const fLODFadeOutMultItems;
		static float * const fLODFadeOutMultObjects;
	};

	struct TerrainManager {
		static float * const fBlockLoadDistance;
		static float * const fBlockLoadDistanceLow;
		static float * const fSplitDistanceMult;
		static float * const fTreeLoadDistance;
	};

	struct Water {
		static bool * const bAutoWaterSilhouetteReflections;
		static bool * const bForceHighDetailReflections;
		static bool * const bUseWaterDepth;
		static bool * const bUseWaterDisplacements;
		static bool * const bUseWaterReflectionBlur;
		static bool * const bUseWaterReflections;
		static bool * const bUseWaterRefractions;

		static SInt32 * const iWaterBlurAmount;
		static SInt32 * const iWaterReflectHeight;
		static SInt32 * const iWaterReflectWidth;
	};
};