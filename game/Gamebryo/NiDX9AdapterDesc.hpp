#pragma once

#include "NiDX9DeviceDesc.hpp"
#include "NiTPointerList.hpp"

class NiDX9AdapterDesc {
public:
	class ModeDesc {
	public:
		UInt32					m_uiWidth;
		UInt32					m_uiHeight;
		UInt32					m_uiBPP;
		D3DFORMAT				m_eD3DFormat;
		NiTPointerList<UInt32>	m_kRefreshRates;
	};

	UInt32							m_uiAdapter;
	D3DADAPTER_IDENTIFIER9			m_kAdapterIdentifier;
	NiTPrimitiveArray<ModeDesc*>	m_kModeList;
	NiDX9DeviceDesc*				m_pkHALDeviceDesc;
	NiDX9DeviceDesc*				m_pkREFDeviceDesc;
};

ASSERT_SIZE(NiDX9AdapterDesc, 0x468);