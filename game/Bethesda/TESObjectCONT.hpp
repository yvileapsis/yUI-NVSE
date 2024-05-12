#pragma once

#include "TESBoundAnimObject.hpp"
#include "TESContainer.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESScriptableForm.hpp"
#include "TESWeightForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSOpenCloseForm.hpp"

class TESSound;

class TESObjectCONT : public TESBoundAnimObject, public TESContainer, public TESFullName, public TESModelTextureSwap,
	public TESScriptableForm, public TESWeightForm, public BGSDestructibleObjectForm, public BGSOpenCloseForm {
public:
	TESObjectCONT();
	~TESObjectCONT();

	UInt32		unk88;
	TESSound*	openSound;
	TESSound*	closeSound;
	TESSound*	randomLoopingSound;
	UInt8		flags;
	UInt8		pad099[3];
};

ASSERT_SIZE(TESObjectCONT, 0x9C);