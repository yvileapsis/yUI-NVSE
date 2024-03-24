#pragma once

struct NiViewport
{
	float m_left;
	float m_right;
	float m_top;
	float m_bottom;
};

ASSERT_SIZE(NiViewport, 0x10)