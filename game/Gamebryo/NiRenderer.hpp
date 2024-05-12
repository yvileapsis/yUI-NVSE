#pragma once

#include "NiAccumulator.hpp"
#include "NiCriticalSection.hpp"

class NiShader;
class NiDynamicEffectState;
class NiRenderedCubeMap;
class BSShaderAccumulator;
class NiDX9Renderer;
class NiPropertyState;
class NiRenderTargetGroup;

class NiRenderer : public NiObject {
public:
	NiRenderer();
	virtual ~NiRenderer();

	enum ClearFlags {
		CLEAR_BACKBUFFER = 0x1,
		CLEAR_STENCIL = 0x2,
		CLEAR_ZBUFFER = 0x4,
		CLEAR_NONE = 0,
		CLEAR_ALL = CLEAR_BACKBUFFER | CLEAR_STENCIL | CLEAR_ZBUFFER
	};

	enum EFrameState
	{
		FRAMESTATE_OUTSIDE_FRAME = 0x0,
		FRAMESTATE_INSIDE_FRAME = 0x1,
		FRAMESTATE_INSIDE_OFFSCREEN_FRAME = 0x2,
		FRAMESTATE_WAITING_FOR_DISPLAY = 0x3,
		FRAMESTATE_INTERNAL_FRAME = 0x4,
	};

	virtual void	Unk_23();

	NiPointer<BSShaderAccumulator>	m_spBSShaderAccum;
	NiPropertyState*				m_pkCurrProp;
	NiDynamicEffectState*			m_pkCurrEffects;
	NiShader*						m_spErrorShader;
	void*							m_spInitialDefaultMaterial;
	void*							m_spCurrentDefaultMaterial;
	NiAccumulator*					m_spAccum;
	float							unk024;
	float							unk028;
	NiRenderer*						ms_pkRenderer;
	NiRect<float>					m_kDisplaySafeZone;
	char							unk040[62];
	NiCriticalSection				m_kRendererLock;
	char							unk0A0[94];
	NiCriticalSection				m_kPrecacheCriticalSection;
	char							unk120[95];
	NiCriticalSection				m_kSourceDataCriticalSection;
	char							unk1AC[92];
	EFrameState						m_eSavedFrameState;
	EFrameState						m_eFrameState;
	UInt32							m_uiFrameID;
	bool							m_bRenderTargetGroupActive;
	bool							m_bBatchRendering;
	int								unk20C[29];

	static NiDX9Renderer* GetSingleton() {
		return *(NiDX9Renderer**)0x11F4748;
	}

	bool GetInsideFrameState() const {
		return m_eFrameState == FRAMESTATE_INSIDE_FRAME || m_eFrameState == FRAMESTATE_INSIDE_OFFSCREEN_FRAME;
	}

	bool Check_RenderTargetGroupActive(bool bExpectedValue) const {
		return NiRenderer::GetInsideFrameState() && m_bRenderTargetGroupActive == bExpectedValue;
	}

	void LockRenderer();
	void UnlockRenderer();

	static bool EndOffScreenFrame();

	static void DisplayFrame();
};

ASSERT_SIZE(NiRenderer, 0x280)