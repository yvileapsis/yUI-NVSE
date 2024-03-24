#pragma once

#include "TESBoundObject.hpp"
#include "TESFullName.hpp"
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

class TESObjectARMA : public TESBoundObject {
public:
	TESObjectARMA();
	~TESObjectARMA();

	TESFullName					fullName;				// 030
	TESScriptableForm			scriptable;				// 03C
	TESEnchantableForm			enchantable;			// 048
	TESValueForm				value;					// 058
	TESWeightForm				weight;					// 060
	TESHealthForm				health;					// 068
	TESBipedModelForm			bipedModelForm;			// 070
	BGSDestructibleObjectForm	destructible;			// 14C
	BGSEquipType				equipType;				// 154
	BGSRepairItemList			repairList;				// 15C
	BGSBipedModelList			bipedModelList;			// 164
	BGSPickupPutdownSounds		pickupPutdownSounds;	// 16C
	UInt32						unk178;					// 178
	UInt32						unk17C;					// 17C
};

ASSERT_SIZE(TESObjectARMA, 0x180);