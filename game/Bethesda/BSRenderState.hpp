#pragma once

#include "BSShaderProperty.hpp"

enum BSCompareFunctions {
	BSCMP_ALWAYS = 0x0,
	BSCMP_LESS = 0x1,
	BSCMP_EQUAL = 0x2,
	BSCMP_LESSEQUAL = 0x3,
	BSCMP_GREATER = 0x4,
	BSCMP_NOTEQUAL = 0x5,
	BSCMP_GREATEREQUAL = 0x6,
	BSCMP_NEVER = 0x7,
};

enum BSBlendFunctions {
	BSBLEND_ONE = 0x0,
	BSBLEND_ZERO = 0x1,
	BSBLEND_SRCCOLOR = 0x2,
	BSBLEND_INVSRCCOLOR = 0x3,
	BSBLEND_DESTCOLOR = 0x4,
	BSBLEND_INVDESTCOLOR = 0x5,
	BSBLEND_SRCALPHA = 0x6,
	BSBLEND_INVSRCALPHA = 0x7,
	BSBLEND_DESTALPHA = 0x8,
	BSBLEND_INVDESTALPHA = 0x9,
	BSBLEND_SRCALPHASAT = 0xA,
};


enum BSStencilBufferOperations {
	BSSTENCILOP_KEEP = 0x0,
	BSSTENCILOP_ZERO = 0x1,
	BSSTENCILOP_REPLACE = 0x2,
	BSSTENCILOP_INCR = 0x3,
	BSSTENCILOP_DECR = 0x4,
	BSSTENCILOP_INVERT = 0x5,
};

enum RenderStateCounterType : UInt32 {
	kZBuffer = 0x0,
	kZWrite = 0x1,
	kZFunc = 0x2,
	kDepthBias = 0x3,
	kAlphaTest = 0x4,
	kAlphaFunc = 0x5,
	kAlphaBlend = 0x6,
	kAlphaBlends = 0x7,
	kBlendOp = 0x8,
	kStencilEnable = 0x9,
	kStencilActions = 0xA,
	kStencilFuncs = 0xB,
	kStencilWrite = 0xC,
	kScissorTest = 0xD,
	kClipPlane = 0xF,
	kColorWrite = 0x10,
	kColorWrite123 = 0x11,
	kFillMode = 0x12,
	kCullMode = 0x13,
	kTMSAA = 0x14,
	COUNT
};


class BSRenderState {
public:
	static RenderStateCounterType* const RenderStateCounters;
	static DWORD* const RenderStateStatus;

	static BSBlendFunctions SelectBlendType(D3DBLEND aeType);

	static void InitHooks();
	static void SetAlphaAntiAliasing(bool abEnable, bool abMarkStatus);
	static void SetAlphaBlendEnable(bool abEnable, bool abMarkStatus);
	static void ResetAlphaBlendEnable(bool abMarkStatus);

	static void SetAlphaTestEnable(bool abEnable, bool abMarkStatus);
	static void ResetAlphaTestEnable(bool abMarkStatus);

	static void SetAlphaFuncAndRef(BSCompareFunctions aeCompareType, DWORD aiAlphaRef, bool abMarkStatus);
	static void ResetAlphaFuncAndRef(bool abMarkStatus);
	static void SetSrcAndDstBlends(BSBlendFunctions aeSrcType, BSBlendFunctions aeDestType, bool abMarkStatus);
	static void ResetSrcAndDstBlends(bool abMarkStatus);

	static void SetFillMode(bool abWireframe, bool abMarkStatus);
	static void ResetFillMode(bool abMarkStatus);

	
	static void SetColorWriteEnable(UInt32 aMask, bool abMarkStatus);
	static void ResetColorWriteEnable(bool abMarkStatus);


	static void SetScissorTestEnable(bool abEnable, bool abMarkStatus);
	static void SetScissorRectangle(SInt32 left, SInt32 top, SInt32 right, SInt32 bottom);
	static void AdjustScissorRectangle(SInt32 left, SInt32 top, SInt32 right, SInt32 bottom, bool abMarkStatus);
	static void ResetScissorRectangle();

	static void ResetZBuffer(bool abMarkStatus);

	static void ResetZFunc(bool abMarkStatus);
	static void SetZFunc(BSCompareFunctions aeFunc, bool abMarkStatus);

	static void SetZWriteEnable(bool abEnable, bool abMarkStatus);
	static void ResetZWriteEnable(bool abMarkStatus);

	static void SetZEnable(D3DZBUFFERTYPE aeBufferType, bool abMarkStatus);

	static void SetDepthBias(float afDepthBias, bool abMarkStatus);
	static void ResetDepthBias(bool abMarkStatus);

	static void SetStencilEnable(bool abEnable, bool abMarkStatus);
	static void SetStencilFunctions(BSCompareFunctions aeCompareType, int aiStencilRef, DWORD aiStencilMask, bool abMarkStatus);
	static void SetStencilOperations(BSStencilBufferOperations stencilFail, BSStencilBufferOperations stencilZFail, BSStencilBufferOperations stencilPass, bool abMarkStatus);
	static void SetStencilWriteMask(DWORD aMask, bool abMarkStatus);
	static void ResetStencil(bool abMarkStatus);

	static void SetClipPlaneEnable(bool abPlane0, bool abPlane1, bool abPlane2, bool abPlane3, bool abPlane4, bool abPlane5, bool abMarkStatus);
	static void ResetClipPlane(bool abMarkStatus);

	static void AddStatus(RenderStateCounterType type, bool abMark);
	static void RemoveStatus(RenderStateCounterType type, bool abMark);
	static void DecreaseStatus(RenderStateCounterType type);
	static void ResetStatus(RenderStateCounterType type);
	static UInt32 GetStatus(RenderStateCounterType type);

	static void SetRenderState(D3DRENDERSTATETYPE aeRenderStateType, DWORD value, bool abMarkStatus);

	static RenderStateCounterType GetStateCounter(D3DRENDERSTATETYPE aeRenderStateType);
};