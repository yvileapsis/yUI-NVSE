#pragma once

#include "TESBoundAnimObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESScriptableForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSOpenCloseForm.hpp"

class TESSound;

class TESObjectDOOR : public TESBoundAnimObject, public TESFullName, public TESModelTextureSwap, public TESScriptableForm,
						public BGSDestructibleObjectForm, public BGSOpenCloseForm {
public:
	TESObjectDOOR();
	~TESObjectDOOR();

	UInt32					unk74;
	TESSound*				pOpenSound;
	TESSound*				pCloseSound;
	TESSound*				pRandomLoopingSound;
	Bitfield8				ucFlags;
	BSSimpleList<TESForm*>	kRandomTeleports;
};

ASSERT_SIZE(TESObjectDOOR, 0x90)