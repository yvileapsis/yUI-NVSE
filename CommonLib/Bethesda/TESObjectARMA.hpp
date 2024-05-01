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

// 0x180
class TESObjectARMA :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESScriptableForm,			// 03C
	public TESEnchantableForm,			// 048
	public TESValueForm,				// 058
	public TESWeightForm,				// 060
	public TESHealthForm,				// 068
	public TESBipedModelForm,			// 070
	public BGSDestructibleObjectForm,	// 14C
	public BGSEquipType,				// 154
	public BGSRepairItemList,			// 15C
	public BGSBipedModelList,			// 164
	public BGSPickupPutdownSounds		// 16C
{
public:
	TESObjectARMA();
	~TESObjectARMA();

	UInt32						unk178;					// 178
	UInt32						unk17C;					// 17C
};
static_assert(sizeof(TESObjectARMA) == 0x180);