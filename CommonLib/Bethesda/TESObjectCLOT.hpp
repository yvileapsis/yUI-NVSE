#pragma once
#include "TESBoundObject.hpp"
#include "TESFullName.hpp"
#include "TESScriptableForm.hpp"
#include "TESEnchantableForm.hpp"
#include "TESValueForm.hpp"
#include "TESWeightForm.hpp"
#include "TESBipedModelForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSEquipType.hpp"

// 0x154
class TESObjectCLOT :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESScriptableForm,			// 03C
	public TESEnchantableForm,			// 048
	public TESValueForm,				// 058
	public TESWeightForm,				// 060
	public TESBipedModelForm,			// 068
	public BGSDestructibleObjectForm,	// 144
	public BGSEquipType					// 14C
{
public:
	TESObjectCLOT();
	~TESObjectCLOT() override;
};
static_assert(sizeof(TESObjectCLOT) == 0x154);