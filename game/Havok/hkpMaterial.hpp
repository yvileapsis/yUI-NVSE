#pragma once

class hkpMaterial {
	SInt8 m_responseType;
	float m_friction;
	float m_restitution;
};

ASSERT_SIZE(hkpMaterial, 0xC);