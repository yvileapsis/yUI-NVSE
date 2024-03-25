#pragma once
#include "TESBoundAnimObject.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESScriptableForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSOpenCloseForm.hpp"

class TESForm;
class TESSound;

// 0x90
class TESObjectDOOR :
	public TESBoundAnimObject,			// 00
	public TESFullName, 				// 30
	public TESModelTextureSwap,			// 3C
	public TESScriptableForm,			// 5C
	public BGSDestructibleObjectForm,	// 68
	public BGSOpenCloseForm				// 70 
{
public:
	TESObjectDOOR();
	~TESObjectDOOR() override;

	UInt32					unk74;
	TESSound*				pkOpenSound;
	TESSound*				pkCloseSound;
	TESSound*				pkRandomLoopingSound;
	UInt8					ucFlags;
	UInt8					byte85[3];
	BSSimpleList<TESForm*>	kRandomTeleports;
};
static_assert(sizeof(TESObjectDOOR) == 0x90);