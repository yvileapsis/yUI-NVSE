#pragma once

#include "TESBoundAnimObject.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "TESScriptableForm.hpp"
#include "TESSound.hpp"
#include "TESModelTextureSwap.hpp"
#include "BGSOpenCloseForm.hpp"

class TESWaterForm;
class BGSTalkingActivator;

class TESObjectACTI : public TESBoundAnimObject {
public:
	TESObjectACTI();
	~TESObjectACTI();

	// bases
	TESFullName					fullName;			// 30
	TESModelTextureSwap			modelTextureSwap;	// 3C
	TESScriptableForm			scriptable;			// 5C
	BGSDestructibleObjectForm	destructible;		// 68
	BGSOpenCloseForm			openClose;			// 70

	TESSound* loopingSound;		// 74	SNAM
	TESSound* activationSound;	// 78	VNAM
	TESSound* radioTemplate;		// 7C	INAM
	TESWaterForm* waterType;			// 80	WNAM
	BGSTalkingActivator* radioStation;		// 84	RNAM
	BSStringT						activationPrompt;	// 88	XATO
};

ASSERT_SIZE(TESObjectACTI, 0x90);