#pragma once

#include "BSFadeNode.hpp"
#include "NiScreenElements.hpp"
#include "NiSourceTexture.hpp"
#include "NiColor.hpp"
#include "NiCamera.hpp"
#include "SceneGraph.hpp"
#include "NiPointLight.hpp"
#include "ShadowSceneNode.hpp"
#include "BSShaderAccumulator.hpp"

class TileMenu;

class FORenderedMenu {
public:
	virtual				~FORenderedMenu();
	virtual void		Unk_01();
	virtual BSFadeNode*	GetMenuRoot();
	virtual void		Render();
	virtual void		Unk_04();
	virtual void		Update();
	virtual void		Unk_06();
	virtual void		Unk_07();
	virtual void		Unk_08();
	virtual void		Init();
	virtual void		ResetNifs();
	virtual void		Unk_0B();
	virtual void		OnMenuOpen();
	virtual void		OnMenuClose();
	virtual void		HandleStaticEffect(float);
	virtual void		HandleVerticalHoldEffect(float);
	virtual void		HandleShudderEffect(float);
	virtual void		HandlePulseEffect(float);
	virtual void		HandleScreenLightEffec(float);
	virtual void		HandleScanlines(float);
	virtual void		Unk_14();

	NiTriShapePtr			spScreenGeometry;
	BSFadeNodePtr			spMenuRoot;
	NiScreenElementsPtr		spScreenElements;
	NiTexturePtr			spSrcTexture;
	SceneGraphPtr			spSceneGraph;
	TileMenu*				pTileMenu;
	NiPointLightPtr			spMenuLight;
	ShadowSceneNodePtr		spShadowScene;
	NiCameraPtr				spCamera;
	bool					bIsInitialized;
	UInt8					byte029;
	UInt8					byte02A;
	UInt8					gap02B;
	NiColor					kScreenLightColor;
	float					fScreenLightBaseIntensity;
	float					unk03C;
	float					fBlurRadius;
	float					fBlurIntensity;
	float					fScanlineFrequency;
	UInt8					byte04C;
	UInt8					bIsShowStaticEffect;
	float					fStaticEffectStartTime;
	float					fBurstDuration;
	float					fBurstIntensity2;
	float					fBurstIntensity;
	bool					bIsShowVerticalHoldEffect;
	float					fVerticalHoldStartTime;
	float					fVerticalHoldDuration;
	float					fVerticalHoldSpeed;
	float					fUnk070;
	UInt8					bIsShowShudderHoldEffect;
	float					fShudderHoldStartTime;
	float					fShudderHoldDuration;
	float					fShudderHoldIntensity;
	float					fShudderHoldFrequency;
	float					fUnk088;
	float					fPulseBrightenIntensity;
	float					fPulseRadiusIntensity;
	UInt8					byte094;
	bool					bIsScanlineEffect;
	float					fScanLineStartTime;
	float					fInterval09C;
	float					fDistortDuration;
	float					fNextScanlineTime;
	NiPoint4				kTileColor;
	UInt8					bIsRenderedMenuOrPipboyOpen;
	UInt8					byte0B9;
	NiTexturePtr			spScanlineTexture;
	NiPoint4				kTileColor2;
	bool					bIsQuantityMenuShown;
	bool					bIsTutorialMenuShown;
	bool					bIsMessageMenuShown;
	BSShaderAccumulatorPtr  spShaderAccumulator;

	void SetTexture(NiTexture* apTexture);
	NiCamera* GetCamera();
	NiTriShape* GetScreenShape();

	static bool __fastcall CreateMenuLightEx(FORenderedMenu* apThis, void*, const char* apName, ShadowSceneNode* apScene);
	void ClearLight();
};

ASSERT_SIZE(FORenderedMenu, 0xD8);