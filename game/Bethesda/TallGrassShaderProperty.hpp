#pragma once

#include "BSShaderLightingProperty.hpp"
#include "TESGrassAreaParam.hpp"
#include "BSString.hpp"

class NiTexture;
class IOTask;
class NiStencilProperty;
class BSMultiBoundNode;
class TallGrassShaderProperty;
struct GrassInstance;

NiSmartPointer(TallGrassShaderProperty);

class TallGrassShaderProperty : public BSShaderLightingProperty {
public:
	GrassInstance*			pGrassInstance;
	NiObjectPtr				spObject80;
	NiPointer<NiTexture>	spTexture;
	BSStringT				strTexturePath;
	int						unk90;
	int						unk98;


	NiSmartPointer(CachedGeometry);

	class CachedGeometry : public NiRefObject {
	public:
		NiGeometryDataPtr				spGeoData;
		float*							pfUnkDataC;
		DWORD							dword10;
		NiSourceTexturePtr				spTexture;
		NiPropertyPtr					spAlphaPropertyHDR;
		NiPropertyPtr					spAlphaProperty;
		NiPointer<NiStencilProperty>	spStencilProperty;
		NiPointer<IOTask>				spLoadTask;
		DWORD							vertCount28;
		DWORD							vertCount2C;
		bool							bIsStrips;
		bool							bLightFade;
		bool							bCreated;
		bool							byte34;
		UInt16							uiTriangleCount;
	};

	class CachedGrass {
	public:
	};

	CREATE_OBJECT(TallGrassShaderProperty, 0xB60F50);

	static void __fastcall GetViewerStringsEx(TallGrassShaderProperty* apThis, void*, NiViewerStringsArray* apStrings);

	static RenderPassArray* __fastcall GetRenderPassesEx(TallGrassShaderProperty* apThis, void*, NiGeometry* apGeometry, UInt32 auiEnabledPasses, WORD* pusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
};

ASSERT_SIZE(TallGrassShaderProperty, 0x98)

struct GrassInstance {
	NiPointer<NiGeometry>						spGrassGeometry;
	NiPointer<BSMultiBoundNode>					spMBNode;
	TallGrassShaderProperty::CachedGeometryPtr	spCachedGeometry;
	TallGrassShaderPropertyPtr					spGrassShaderProperty;
	UInt16										usInstanceCount;	// Amount of slots in pInstanceData
	UInt16										usGrassCount;		// Amount of filled slots in pInstanceData
	NiPoint4*									pInstanceData;		// Positions of all grass instances
	void*										dword18;
	GrassParam									Params;
	SInt32										iCellX;
	SInt32										iCellY;
	bool										bLowDetail;

	static GrassInstance* __fastcall CreateEx(GrassInstance* apThis, void*,
		TallGrassShaderProperty::CachedGeometry* apCachedGeo, SInt32 aiX, SInt32 aiY, bool abLowDetail);
};
ASSERT_SIZE(GrassInstance, 0x48u);