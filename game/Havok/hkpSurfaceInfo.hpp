#pragma once

struct hkpSurfaceInfo {
	enum SupportedState {
		UNSUPPORTED = 0,
		SLIDING		= 1,
		SUPPORTED	= 2
	};

	SupportedState	m_supportedState;
	hkVector4		m_surfaceNormal;
	hkVector4		m_surfaceVelocity;
	float			m_surfaceDistanceExcess;
	bool			m_surfaceIsDynamic;
};

ASSERT_SIZE(hkpSurfaceInfo, 0x40);