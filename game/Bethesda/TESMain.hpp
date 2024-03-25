#pragma once
#include "NiDX9Renderer.hpp"
#include "SceneGraph.hpp"
#include "BSShaderAccumulator.hpp"
#include "BSPackedTaskQueue.hpp"

class OSSoundGlobals;

class NiPointLight;

class Sun;

class BSInputManager;
class BSAudioManager;

class BSClearZNode;
class BSPackedTaskQueue;
class BGSCameraShot;

class ImageSpaceManager;
class ImageSpaceModifierInstanceForm;
class TESProjectile;
class TESIdleForm;

enum VATS_MODE_ENUM {
	VATS_MODE_NONE = 0x0,
	VATS_TARGET_SELECT = 0x1,
	VATS_LIMB_SELECT = 0x2,
	VATS_PLAYBACK = 0x4,
	VATS_MODE_COUNT = 0x5,
};

struct VATSCameraData {
	BSSimpleList<UInt32>			kTargetsList; // BSSimpleList<VATSQueuedAction>
	VATS_MODE_ENUM					eMode;
	UInt32							unk0C;
	BGSCameraShot*					pCamShot;
	float							flt14;
	float							flt18;
	TESProjectile*					pProjectile;
	TESProjectile*					pProjectile20;
	TESIdleForm*					pAttackAnim;
	ImageSpaceModifierInstanceForm* pISModInstForm;
	ImageSpaceModifierInstanceForm*	pISModInstRB;
	UInt32							unk30;
	ShadowSceneLightPtr				spLight;
	bool							byte38;
	UInt32							uiNumKills;
	UInt32							unk40;
	UInt32							unk44;

	bool IsPlaying() const { return eMode == VATS_PLAYBACK; }
	bool HasMode() const { return eMode != VATS_MODE_NONE; };

	void SetMode(VATS_MODE_ENUM aeMode, bool abForce);
	void SetLight(NiPointLight* apLight, bool abRemove);
	ShadowSceneLight* GetLight() const { return spLight; };
	void RemoveLight();
};


class TESMain {
public:
	bool					bOneMore;
	bool					bQuitGame;
	bool					bExitToMainMenu;
	bool					bGameActive;
	bool					unk04;
	bool					unk05;
	bool					bIsFlyCam;
	bool					bFreezeTime;
	HWND					hWnd;
	HINSTANCE				hInstance;
	UInt32					uiThreadID;
	HANDLE					hMainThread;
	UInt32* PackedTaskHeap;
	UInt32					unk1C;
	BSInputManager* pInput;
	BSAudioManager* pSound;
	BSPackedTaskQueue		kTaskQueue;
	UInt32					SecondaryPackedTaskHeap;
	UInt32					unk54;
	UInt32					unk58;
	UInt32					unk5C;
	BSPackedTaskQueue		kSecondaryTaskQueue;
	BSShaderAccumulatorPtr	spWorldAccum;
	BSShaderAccumulatorPtr	sp1stPersonAccum;
	BSShaderAccumulatorPtr	spAimDOFAccumulator;
	BSShaderAccumulatorPtr	spScreenSplatterAccum;
	BSShaderAccumulatorPtr	sp3DMenuAccumulator;
	bool					bShouldSetViewPort;
	NiCameraPtr				spFirstPersonCamera;

//	static PerfCounter SimulationCounter;
//	static PerfCounter InputCounter;
//	static PerfCounter FrameCounter;

	static double dSimulationTime;
	static double dRenderTime;

	static NiDX9RendererPtr* const spRenderer;

	static float fAimProgress;

	static bool* const bOffsetViewModelLights;
	static float* const fFrameAnimTime;
	static bool bFixViewModelShake;

	static std::stack<ShadowSceneLightPtr> kLightsToRender;

	static __forceinline VATSCameraData* GetVATSCameraData() { return (VATSCameraData*)0x11F2250; };
	static __forceinline BSMultiBoundNode* GetSkyRoot() { return *(BSMultiBoundNode**)0x11DEB34; };
	static __forceinline NiNode* GetObjectLODRoot() { return *(NiNode**)0x11DEA18; };
	static __forceinline BSClearZNode* GetLODRoot() { return *(BSClearZNode**)0x11DEA14; };
	static __forceinline SceneGraph* GetWorldRoot() { return *(SceneGraph**)0x11DEB7C; };
	static __forceinline NiNode* GetWeatherRoot() { return *(NiNode**)0x11DEDA4; };
	static __forceinline NiCamera* GetWorldRootCamera() { return GetWorldRoot()->spCamera.m_pObject; };
	static __forceinline NiNode* Get1stCamNode() { return *(NiNode**)0x11E07D0; };
	static __forceinline float GetFrameAnimTime() { return *(float*)0x11DEA30; };
	static __forceinline NiDX9Renderer* GetRenderer() { return *spRenderer; };

	BSRenderedTexture* SetupMainISTexture(NiDX9Renderer* apRenderer, BSRenderedTexture* apTexture, bool abDisableIS, bool abIsMSAA);

	void RenderWorldSceneGraph(Sun* pSun, int, bool bIsWireframe, bool bIsInVATS) const;

	void UpdateWater() const;

	static void __fastcall Swap(TESMain* apThis);
	static void __fastcall RenderDefault(TESMain* apThis, void*, BSRenderedTexture* apRenderedTexture2, bool bIsPipBoyMode);
	static void __fastcall RenderWireframe(TESMain* apThis, void*, BSRenderedTexture* apRenderedTexture2, bool bIsPipBoyMode);
	static void __fastcall RenderPipBoy(TESMain* apThis, void*, BSRenderedTexture* apRenderedTexture2, bool abDisableIS);

	static __forceinline TESMain* GetSingleton() { return *(TESMain**)0x11DEA0C; };

	static __forceinline BSShaderAccumulator* GetShaderAccumulator() { return GetSingleton()->spWorldAccum.m_pObject; };

	static __forceinline void StartAccumulating(NiCamera* apCamera, BSShaderAccumulator* apAccumulator);

	static __forceinline void SetMainISTexture(BSRenderedTexture* apTexture) { *(BSRenderedTexture**)0x11F9438 = apTexture; };
	static __forceinline BSRenderedTexture* GetMainISTexture() { return (*(BSRenderedTexture**)0x11F9438); };
	static __forceinline void SetBackgroundTexture(BSRenderedTexture* apTexture) { *(BSRenderedTexturePtr*)0x11DED3C = apTexture; };
	static __forceinline BSRenderedTexture* GetBackgroundTexture() { return (*(BSRenderedTexturePtr*)0x11DED3C).m_pObject; };



	static UInt32 GetFrameID() { return *(UInt32*)0x11A2FE0; }

	Sun* PrepareScene() const;
	void RenderShadowMaps() const;
	bool Render_VATS_8749B0();
	void Render_VATS_874AC0(bool bIsInVATS);
	static void SetOffScreenRTGroup(BSRenderedTexture* apTexture, UInt32 auiClearMode);
	void FinishRender(BSRenderedTexture* apRenderedTexture1, ImageSpaceManager* apImageSpaceManager, NiDX9Renderer* apRenderer, Sun* apSun);
	void SetupViewModel() const;
	void RenderViewModel(NiDX9Renderer* apRenderer, NiAVObject* apPipBoyNode, Sun* apSun, BSRenderedTexture* apRenderedTexture);
	void RenderPostProcess(BSRenderedTexture* apRenderedTexture1, NiDX9Renderer* apRenderer, BSRenderedTexture* apRenderedTexture2) const;
	void RenderAimDOF(BSRenderedTexture* apTexture) const;
	void RenderDepthOfField(BSShaderAccumulator* apAccumulator, BSRenderedTexture* apRenderedTexture);
	void RenderScreenSplatter(NiDX9Renderer* apRenderer) const;
	void InitRefraction(BSShaderAccumulator* apAccumulator, BSRenderedTexture* apTexture);
	void RenderPipBoyScreen(NiDX9Renderer* apRenderer) const;
	void CreateBackgroundTexture(ImageSpaceManager* apImageSpaceManager, NiDX9Renderer* apRenderer, bool bIsMSAA, BSRenderedTexture* apTexture);

	bool __fastcall HandleMouseMove(TESMain* apThis, void*, SInt32 x, SInt32 y);
};
ASSERT_SIZE(TESMain, 0xA4);

namespace ToggleCellNodesFlags {
#define g_ToggleCellNodesFlags *(UInt32*)0x11CA08C

	static bool GetFlag(UInt8 flag) { return (g_ToggleCellNodesFlags & (1 << flag)) != 0; };
	static void SetFlag(UInt8 flag, bool bSet) { if (bSet) g_ToggleCellNodesFlags |= (1 << flag); else g_ToggleCellNodesFlags &= ~(1 << flag); };
}