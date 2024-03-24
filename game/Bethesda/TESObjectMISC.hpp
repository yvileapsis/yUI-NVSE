#pragma once

#include "TESBoundObject.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESValueForm.hpp"
#include "TESWeightForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSMessageIcon.hpp"
#include "BGSPickupPutdownSounds.hpp"

class TESObjectMISC : public TESBoundObject {
public:
	TESObjectMISC();
	~TESObjectMISC();

	TESFullName fullName;
	TESModelTextureSwap modelSwap;
	TESIcon icon;
	TESScriptableForm scriptable;
	TESValueForm value;
	TESWeightForm weight;
	BGSDestructibleObjectForm destructible;
	BGSMessageIcon messageIcon;
	BGSPickupPutdownSounds pickupPutdown;
	UInt32 unkA8;
};

ASSERT_SIZE(TESObjectMISC, 0xAC);