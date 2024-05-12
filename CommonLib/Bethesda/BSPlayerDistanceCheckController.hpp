#pragma once

#include "NiTimeController.hpp"



class BSPlayerDistanceCheckController : public NiTimeController {
public:
	BSPlayerDistanceCheckController();
	virtual ~BSPlayerDistanceCheckController();

	float fCheckDistance;
	void(__cdecl* pfnCallback)(NiAVObject* apObject);

	NIRTTI_ADDRESS(0x11DFEAC);

	static BSPlayerDistanceCheckController* CreateObject();

	void SetCallback(void(__cdecl* apfnCallback)(NiAVObject* apObject));
	void SetCheckDistance(float afCheckDistance);
	float GetCheckDistance() const;
};

ASSERT_SIZE(BSPlayerDistanceCheckController, 0x3C);