#pragma once

#include "BSFadeNode.hpp"
#include "NiScreenElements.hpp"
#include "NiSourceTexture.hpp"
#include "NiColor.hpp"
#include "NiCamera.hpp"
;
class SceneGraph;
class TileMenu;
class ShadowSceneNode;
class NiPointLight;
class BSShaderAccumulator;

class FORenderedMenu
{
public:

	FORenderedMenu();

	virtual ~FORenderedMenu();
	virtual void	Unk_01();
	virtual BSFadeNode* GetMenuRoot();
	virtual void	Render();
	virtual void	Unk_04();
	virtual void	Update();
	virtual void	Unk_06();
	virtual void	Unk_07();
	virtual void	Unk_08();
	virtual void	Init();
	virtual void	ResetNifs();
	virtual void	Unk_0B();
	virtual void	OnMenuOpen();
	virtual void	OnMenuClose();
	virtual void	HandleStaticEffect(Float32 msPassed);
	virtual void	HandleVerticalHoldEffect(Float32 msPassed);
	virtual void	HandleShudderEffect(Float32 msPassed);
	virtual void	Unk_11(Float32 msPassed);
	virtual void	Unk_12(Float32 msPassed);
	virtual void	HandleScanlines(Float32 msPassed);
	virtual void	Unk_14();

	NiPointer<NiTriShape>		spScreenGeometry;
	NiPointer<BSFadeNode>		spMenuRoot;
	NiScreenElements*			pScreenElements;
	NiPointer<NiSourceTexture>	spSrcTexture;
	SceneGraph*					pkSceneGraph;
	TileMenu*					pkTileMenu;
	NiPointer<NiPointLight>		spMenuLight;
	ShadowSceneNode*			pkShadowScene;
	NiPointer<NiCameraPtr>		spCamera;

	UInt8					bIsInitialized;
	UInt8					byte029;
	UInt8					byte02A;
	UInt8					gap02B;
	NiColor					kScreenLightColor;
	Float32					fScreenLightBaseIntensity;
	Float32					unk03C;
	Float32					fBlurRadius;
	Float32					fBlurIntensity;
	Float32					fScanlineFrequency;
	UInt8					byte04C;
	UInt8					bIsShowStaticEffect;
	UInt8					gap04E[2];
	Float32					fStaticEffectStartTime;
	Float32					fBurstDuration;
	Float32					fBurstIntensity2;
	Float32					fBurstIntensity;
	UInt8					cIsShowVerticalHoldEffect;
	UInt8					gap061[3];
	Float32					fVerticalHoldStartTime;
	Float32					fVerticalHoldDuration;
	Float32					fVerticalHoldSpeed;
	Float32					unk070;
	UInt8					ucIsShowShudderHoldEffect;
	UInt8					gap075[3];
	Float32					fShudderHoldStartTime;
	Float32					fShudderHoldDuration;
	Float32					fshudderHoldIntensity;
	Float32					fShudderHoldFrequency;
	Float32					unk088;
	Float32					fPulseBrightenIntensity;
	Float32					fPulseRadiusIntensity;
	UInt8					byte094;
	UInt8					bIsScanlineEffect;
	UInt8					gap096[2];
	Float32					fScanLineStartTime;
	Float32					fInterval09C;
	UInt32					fDistortDuration;
	Float32					fNextScanlineTime;
	NiColorA				kTileColor;
	UInt8					ucIsRenderedMenuOrPipboyOpen;
	UInt8					byte0B9;
	UInt8					gap0BA[2];
	UInt32					uiScanlineTexture;
	NiColorA				kTileColor2;
	bool					bIsQuantityMenuShown;
	bool					bIsTutorialMenuShown;
	bool					bIsMessageMenuShown;
	UInt8					gap0D3;

	BSShaderAccumulator* pkShaderAccumulator;

	void SetTexture(NiTexture* apTexture);
	NiCamera* GetCamera();
	NiTriShape* GetScreenShape();

	static bool __fastcall CreateMenuLightEx(FORenderedMenu* apThis, void*, const char* apName, ShadowSceneNode* apScene);
	void ClearLight();
};
static_assert(sizeof(FORenderedMenu) == 0xD8);