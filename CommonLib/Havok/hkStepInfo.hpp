#pragma once

struct hkStepInfo {
	float m_startTime;
	float m_endTime;
	float m_deltaTime;
	float m_invDeltaTime;
};

ASSERT_SIZE(hkStepInfo, 0x10);