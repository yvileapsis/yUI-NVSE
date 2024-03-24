#pragma once

#include "TESBoundObject.hpp"
#include "TESScriptableForm.hpp"
#include "TESEnchantableForm.hpp"
#include "TESValueForm.hpp"
#include "TESWeightForm.hpp"
#include "TESHealthForm.hpp"
#include "TESBipedModelForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSEquipType.hpp"
#include "BGSRepairItemList.hpp"
#include "BGSBipedModelList.hpp"
#include "BGSPickupPutdownSounds.hpp"

class TESObjectARMO : public TESBoundObject {
public:
	TESObjectARMO();
	~TESObjectARMO();

	// children
	TESFullName fullName;
	TESScriptableForm scriptable;
	TESEnchantableForm enchantable;
	TESValueForm value;
	TESWeightForm weight;
	TESHealthForm health;
	TESBipedModelForm bipedModel;
	BGSDestructibleObjectForm destuctible;
	BGSEquipType equipType;
	BGSRepairItemList repairItemList;
	BGSBipedModelList bipedModelList;
	BGSPickupPutdownSounds pickupPutdownSounds;
	UInt16 armorRating;
	UInt16 modifiesVoice;
	float damageThreshold;
	UInt32 armorFlags;
	UInt32 unk184;
	TESObjectARMO* audioTemplate;
	UInt8 overrideSounds;
	UInt8 pad18D[3];
};

ASSERT_SIZE(TESObjectARMO, 0x190);
ASSERT_OFFSET(TESObjectARMO, damageThreshold, 0x17C);