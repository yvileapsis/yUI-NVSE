#pragma once
#include "FORenderedMenu.hpp"
#include "BSSoundHandle.hpp"

class FOPipboyManager : public FORenderedMenu {
public:
	FOPipboyManager();
	~FOPipboyManager();

	NiNode* pipboyScreenNode0D8;
	NiNode* node_0DC;
	NiTriStrips* unk0C0;
	NiRefObject* unk0C4;
	NiTriStrips* pipboyLightButton[3];
	NiNodePtr spPipboyLightGlow[3];
	NiTriStrips* scrollKnobs[3];
	float unk10C[3];
	float pipboyKnobScrollPositions[3];
	float pipboyKnobScrollRates[3];
	float tabKnobMinPosition;
	UInt32 unk134;
	UInt32 unk138;
	UInt32 unk13C;
	float tabKnobMaxPosition;
	UInt8 byte144;
	UInt8 gap145[3];
	UInt32 unk148;
	UInt32 unk14C;
	UInt8 byte150;
	UInt8 gap151[3];
	BSSoundHandle pipboyHumLoopSound;
	UInt8 byte160;
	UInt8 byte161;
	UInt8 byte162;
	UInt8 gap163;
	float lightFadeStartTime;
	float lightEffectFadeDuration;
	UInt8 isResetPipboyManager;
	UInt8 gap16D[3];

	NiNode* GetPipboyLightGlow(UInt32 auiIndex);
};
static_assert(sizeof(FOPipboyManager) == 0x170);