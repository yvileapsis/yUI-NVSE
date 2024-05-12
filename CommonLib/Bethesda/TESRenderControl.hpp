#pragma once
#include "NiNode.hpp"
#include "NiColor.hpp"

NiSmartPointer(NiCamera);
NiSmartPointer(NiDirectionalLight);
NiSmartPointer(BSShaderAccumulator);

class TESRenderControl {
public:
	virtual void Unk_00();
	virtual void Unk_01();
	virtual void Unk_02();
	virtual ~TESRenderControl();
	virtual void Unk_04(NiAVObject* apObject);
	virtual void Unk_05(NiAVObject* apObject);
	virtual void Unk_06();
	virtual void Unk_07();
	virtual void Unk_08(float afDeltaTime, bool);
	virtual void Render();
	virtual void Unk_10();
	virtual void UpdateHavok(float afDeltaTime);
	virtual void Unk_12();

	HWND					pWindowHandle;
	DWORD					dword8;
	float					fHeight;
	float					fWidth;
	NiNodePtr				spNode14;
	NiDirectionalLightPtr	spDirLight;
	NiNodePtr				spRootNode;
	NiCameraPtr				spCamera;
	DWORD					dword24;
	DWORD					dword28;
	DWORD					spObject2C;
	DWORD					spObject30;
	DWORD					dword34;
	DWORD					dword38;
	DWORD					dword3C;
	DWORD					dword40;
	DWORD					dword44;
	DWORD					dword48;
	BYTE					byte4C;
	DWORD					spHavokWorld;
	bool					bHasSetBackgroundColor;
	NiColor					kBackgroundColor;
	bool					bWireFrame;
	BYTE					byte65;
	BSShaderAccumulatorPtr	spAccumulator;
};
static_assert(sizeof(TESRenderControl) == 0x6C);