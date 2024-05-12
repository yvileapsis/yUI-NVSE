#pragma once

#include "NiTMap.hpp"
#include "BSShaderProperty.hpp"
#include "BSSimpleList.hpp"
#include "NiObject.hpp"
#include "PersistentPassList.hpp"
#include "NiTPointerList.hpp"
// TODO: do something regarding perfcounter
//#include <PerfCounter.hpp>

class BSShader;
class NiTexture;

// BSSimpleArray<BSBatchRenderer::PassGroup, 1024> - PS3, FO3

class BSBatchRenderer : public NiObject {
public:
	virtual void RegisterPassGeometryGroup(BSShaderProperty::RenderPass* apRenderPass, UInt32 auiPassEnum);
	virtual void Unk_24(BSShaderManager::RenderPassType aeMinPass, BSShaderManager::RenderPassType aeMaxPass, bool abBlendAlpha);

	struct GeometryGroup {
		GeometryGroup(BSShaderManager::RenderPassType aeMaxPass = BSShaderManager::BSSM_FINAL_PASS);
		~GeometryGroup();

		enum {
			GG_NO_SORT = 0x1,
		};

		Bitfield8							ucFlags;
		UInt16								usCount;
		BSShaderManager::RenderPassType		eMaxPass;
		BSBatchRenderer*					pBatchRenderer;
		PersistentPassList					kPassList;
		float								fDepth;

		void RegisterPass(BSShaderProperty::RenderPass* apRenderPass, bool abFirst);
		void Render(bool bBlendAlpha);
		void ClearAndFreePasses();

		void SetSort(bool abSort);
		bool HasNoSort();
	};

	struct PassGroup {
		PersistentPassList	kPassLists[5];
		UInt32				uiCount;

		void AddHead(UInt32 auiList, BSShaderProperty::RenderPass* apRenderPass);
		void AddTail(UInt32 auiList, BSShaderProperty::RenderPass* apRenderPass);
		void Clear(bool abWarnNotEmpty);
	};

	enum GroupType : SInt32 {
		GROUP_NONE = -1,
		GROUP_LOD = 0,
		GROUP_UNK_1 = 1,
		GROUP_BLENDED_DECAL_SINGLE = 2,
		GROUP_BLENDED_DECAL_MULTI = 3,
		GROUP_BLENDED_DECAL_SINGLE_DEBUG = 4,
		GROUP_BLENDED_DECAL_MULTI_DEBUG = 5,
		GROUP_REFRACT = 6,
		GROUP_UNK_7 = 7,
		GROUP_REFRACT_CLEAR = 8,
		GROUP_UNK_9 = 9,
		GROUP_UNK_10 = 10,
		GROUP_DEPTH_WORLD = 11,
		GROUP_DEPTH_PLAYER = 12,
		GROUP_COUNT
	};

	enum AlphaGroupType : UInt32 {
		ALPHA_GROUP_SURFACE = 0,
		ALPHA_GROUP_UNDERWATER = 1,
		ALPHA_GROUP_COUNT,
	};

	enum TransparencyType : UInt32 {
		ALPHA_NONE = 0,
		ALPHA_STENCIL = 1,
		ALPHA_TEST_STENCIL = 2,
		ALPHA_TEST_NO_STENCIL = 3,
		ALPHA_NO_TMSAA = 4,
		ALPHA_MAX = 5,
	};

	PassGroup*										pPassGroups;
	NiTMap<BSShaderManager::RenderPassType, UInt32>	kPassMap; // PassType -> PassGroup index
	DWORD											uiMaxPass;
	BSShaderManager::RenderPassType					ePassStart;
	BSShaderManager::RenderPassType					ePassEnd;
	DWORD*											pTextureBlendStagesList;
	BSSimpleList<UInt16>							kCurrentBatch;
	SInt32											iGroupingAlphasGroupCount;
	bool											bAutoClearPasses;
	GeometryGroup*									pGeometryGroups[GROUP_COUNT];
	GeometryGroup*									pUngroupedAlphas[ALPHA_GROUP_COUNT];
	NiTPointerList<GeometryGroup*>					kGroupingAlphasGroup[ALPHA_GROUP_COUNT];
	GeometryGroup*									pGroupedAlphas[ALPHA_GROUP_COUNT];

	static BSShaderManager::RenderPassType uiRenderPassToIgnore;
	static bool bShowOnlySelectedPass;
	static const char* kGroupNames[GROUP_COUNT];
	static bool kBlockedGroups[GROUP_COUNT];

	static const char* kAlphaGroupNames[ALPHA_GROUP_COUNT];
	static bool kBlockedAlphaGroups[ALPHA_GROUP_COUNT];

// TODO: do something regarding perfcounter
//	static PerfCounter renderPassImmediatelyCounter;

	static std::vector<BSShaderProperty::RenderPass*> kSavedRenderPasses;

	static BSBatchRenderer* Create(UInt32 auiMaxPass, UInt32 aeBatchRendererCreationType);

	static void BeginPass(BSShaderManager::RenderPassType auiPass, BSShader* apShader);
	static void __cdecl RenderPassImmediately(BSShaderProperty::RenderPass* apRenderPass, UInt32 auiCurrentPass, bool abTestAlpha, bool abBlendAlpha, bool abSetupDrawmode);
	static void RenderPassImmediately_Skinned(BSShaderProperty::RenderPass* apRenderPass, bool abTestAlpha, bool abBlendAlpha, bool abSetupDrawmode);
	static void RenderPassImmediately_Standard(BSShaderProperty::RenderPass* apRenderPass, bool abTestAlpha, bool abBlendAlpha, bool abSetupDrawmode);
	static void EndPass();

	void RegisterAlphaPass(BSShaderProperty::RenderPass* apPass, AlphaGroupType auiGroup);
	void RegisterGeometryGroupPass(BSShaderProperty::RenderPass* apPass, GroupType aGroup, bool abFirst);
	double GetNextAlphaDepth(UInt32 auiGroup, NiCamera* apCamera, bool abSortByClosestPoint) const;
	void RenderNextAlpha(AlphaGroupType auiGroup, NiCamera* apCamera, bool abSortByClosestPoint, float a5);
	void SortAlphaPasses(NiCamera* apCamera, bool abSortByClosestPoint);
	static SInt32 __cdecl SortingFunction(BSShaderProperty::RenderPass** apPassPtrOne, BSShaderProperty::RenderPass** apPassPtrTwo);
	void ClearAlphaLists(AlphaGroupType auiGroupIndex);

	static void __fastcall RegisterPassGeometryGroupEx(BSBatchRenderer* apThis, void*, BSShaderProperty::RenderPass* apRenderPass, BSShaderManager::RenderPassType auiPassEnum);

	static bool HasPassStarted();
	static void SetHasPassStarted(bool abHasStarted);

	static BSShader* GetLastShader();
	static void SetLastShader(BSShader* apShader);
	static BSShaderManager::RenderPassType GetLastPass();
	static void SetLastPass(UInt32 auiPass);
	static UInt32 GetCurrentPassType();
	static void SetCurrentPassType(UInt32 auiPass);
	static BSShaderProperty::RenderPass* GetCurrentPass();
	static void SetCurrentPass(BSShaderProperty::RenderPass* apPass);

	static NiTexture* GetStageTexture(UInt32 auiStage);
	static void SetStageTexture(UInt32 auiStage, NiTexture* apTexture);

	void ClearRenderPasses();
	void StartGroupingAlphas(NiBound* apBound, bool abZPrePass, NiCamera* apCamera, bool abSortByClosestPoint);

	bool Unk_B99900(BSShaderManager::RenderPassType* apuiPass, TransparencyType* apuiTransparencyType);
	bool Unk_B9A090(BSShaderManager::RenderPassType* apuiPass, TransparencyType* apuiTransparencyType, bool* apbIsEnabled);
	bool SetTransparencyType(UInt32 auiGroup, TransparencyType* apuiTransparencyType);

	bool GetPassesWithinRange(BSShaderManager::RenderPassType aeStartRange, BSShaderManager::RenderPassType aeEndRange);
	bool GetWaterPassesWithinRange(BSShaderManager::RenderPassType aeStartRange, BSShaderManager::RenderPassType aeEndRange);
};
ASSERT_SIZE(BSBatchRenderer, 0x98);