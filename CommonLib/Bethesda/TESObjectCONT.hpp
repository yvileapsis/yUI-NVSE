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

// 0x9C
class TESObjectCONT :
	public TESBoundAnimObject,				// 00
	public TESContainer,					// 30
	public TESFullName,						// 3C
	public TESModelTextureSwap,				// 48
	public TESScriptableForm,				// 68
	public TESWeightForm,					// 74
	public BGSDestructibleObjectForm,		// 7C
	public BGSOpenCloseForm					// 84
{
public:
	TESObjectCONT();
	~TESObjectCONT();

	UInt32		unk88;
	TESSound*	pkOpenSound;				// 08C
	TESSound*	pkCloseSound;				// 090
	TESSound*	pkRandomLoopingSound;		// 094
	UInt8		flags;						// 098
	UInt8		pad099[3];
};
static_assert(sizeof(TESObjectCONT) == 0x9C);