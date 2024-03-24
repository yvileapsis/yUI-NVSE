#pragma once

#include <d3d9.h>
#include "NiTPointerList.hpp"
#include "NiTPointerMap.hpp"

class NiDX9DeviceDesc {
public:
	class DisplayFormatInfo {
	public:
		class DepthStencilInfo {
		public:
			bool	m_bValid;
			UInt8	m_aucMultiSampleQuality[16];
		};

		class RenderTargetInfo {
		public:
			DepthStencilInfo	m_akWindowedDSFormats[2];
			UInt32				unk;
			DepthStencilInfo	m_akFullscreenDSFormats[2];
		};

		D3DFORMAT										m_eFormat;
		bool											m_bWindowed;
		bool											m_bFullscreen;
		NiTPointerMap<D3DFORMAT, RenderTargetInfo*>*	m_kRenderTargets;
	};

	D3DDEVTYPE								m_kD3DDevType;
	D3DCAPS9								m_kD3DCaps9;
	NiTPointerList<DisplayFormatInfo*>		m_kScreenFormats;
	bool									m_bRenderWindowed;
};

ASSERT_SIZE(NiDX9DeviceDesc, 0x144);