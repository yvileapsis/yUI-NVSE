#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESDescription.hpp"
#include "TESValueForm.hpp"
#include "TESWeightForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSMessageIcon.hpp"
#include "BGSPickupPutdownSounds.hpp"

// 0xB0
class TESObjectIMOD :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESModelTextureSwap,			// 03C
	public TESIcon,						// 05C
	public TESScriptableForm,			// 068
	public TESDescription,				// 074
	public TESValueForm,				// 07C
	public TESWeightForm,				// 084
	public BGSDestructibleObjectForm,	// 08C
	public BGSMessageIcon,				// 094
	public BGSPickupPutdownSounds		// 0A4
{
public:
	TESObjectIMOD();
	~TESObjectIMOD();

};
static_assert(sizeof(TESObjectIMOD) == 0xB0);