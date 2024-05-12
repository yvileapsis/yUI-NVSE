#pragma once
#include "TESBoundAnimObject.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "TESScriptableForm.hpp"
#include "TESSound.hpp"
#include "TESModelTextureSwap.hpp"
#include "BGSOpenCloseForm.hpp"

class TESWaterForm;
class BGSTalkingActivator;

class TESObjectACTI :
	public TESBoundAnimObject,
	public TESFullName,
	public TESModelTextureSwap,
	public TESScriptableForm,
	public BGSDestructibleObjectForm,
	public BGSOpenCloseForm
{
public:
	TESObjectACTI();
	~TESObjectACTI();

	TESSound*					pkLoopingSound;			// 74	SNAM
	TESSound*					pkActivationSound;		// 78	VNAM
	TESSound*					pkRadioTemplate;		// 7C	INAM
	TESWaterForm*				pkWaterType;			// 80	WNAM
	BGSTalkingActivator*		pkRadioStation;			// 84	RNAM
	BSStringT<char>				kActivationPrompt;		// 88	XATO
};
static_assert(sizeof(TESObjectACTI) == 0x90);