#pragma once

#include "TESBoundObject.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESEnchantableForm.hpp"
#include "TESValueForm.hpp"
#include "TESWeightForm.hpp"
#include "TESDescription.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSMessageIcon.hpp"
#include "BGSPickupPutdownSounds.hpp"

class TESObjectBOOK : public TESBoundObject {
public:
	TESFullName fullName;
	TESModelTextureSwap model;
	TESIcon icon;
	TESScriptableForm scriptable;
	TESEnchantableForm enchantable;
	TESValueForm value;
	TESWeightForm weight;
	TESDescription description;
	BGSDestructibleObjectForm destuctible;
	BGSMessageIcon messageIcon;
	BGSPickupPutdownSounds sounds;
	UInt8 bookFlags;
	UInt8 skillCode;
	UInt8 byteC2;
	UInt8 byteC3;
};

ASSERT_SIZE(TESObjectBOOK, 0xC4)