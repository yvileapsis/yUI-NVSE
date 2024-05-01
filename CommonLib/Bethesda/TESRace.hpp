#pragma once
#include "NiTArray.hpp"
#include "BSSimpleArray.hpp"

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESDescription.hpp"
#include "TESSpellList.hpp"
#include "TESReactionForm.hpp"

#include "TESAttributes.hpp"
#include "TESModel.hpp"
#include "TESTexture.hpp"
#include "BGSTextureModel.hpp"
#include "BSFaceGenManager.hpp"

class BGSVoiceType;
class TESHair;

class TESRace :
	public TESForm,
	public TESFullName,
	public TESDescription,
	public TESSpellList,
	public TESReactionForm
{
public:
	// 2
	struct SkillMod
	{
		UInt8	actorValue;
		SInt8	mod;
	};

	enum EnumRaceFlags : UInt32
	{
		kFlag_Playable	= 0x00000001,
		kFlag_Child		= 0x00000004,
	};

	TESRace();
	~TESRace() override;

	SkillMod				kSkillMods[7];				// 050
	UInt8					pad05E[2];					// 05E
	Float32					fHeight[2];					// 060 male/female
	Float32					fWeight[2];					// 068 male/female
	EnumRaceFlags			eRaceFlags;					// 070

	TESAttributes			kBaseAttributes[2];			// 074 male/female
	BSSimpleList<TESHair*>	kHairs;						// 08C
	TESHair*				pkDefaultHair[2];			// 094 male/female
	UInt8					defaultHairColor[2];		// 09C male/female
	UInt8					fill09E[2];					// 09E

	Float32					unk0A0;
	Float32					unk0A4;

	BSSimpleList<TESEyes*>	kEyes;						// 0A8

	TESModel				kFaceModels[2][8];			// 0B0	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture				kFaceTextures[2][8];		// 230	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture				kBodyPartsTextures[2][3];	// 2F0	male/female	UpperBody, LeftHand, RightHand
	TESModel				kBodyModels[2][3];			// 338	male/female	UpperBody, LeftHand, RightHand
	BGSTextureModel			kBodyTextures[2];			// 3C8	male/female	EGT file, not DDS.
	BSFaceGenManager::FaceGenDatas kFaceGenData[2];
	UInt32					unk4F8;
	BGSVoiceType*			kVoiceTypes[2];
	TESRace*				kAgeRace[2];
	BSStringT<char>			kEditorID;
	BSSimpleArray<void*>	kFaceGenUndo;				// 4D4 - NiTPrimitiveArray<FaceGenUndo *>

	bool IsPlayable() const { return (eRaceFlags & kFlag_Playable) == kFlag_Playable; }
};
static_assert(sizeof(TESRace) == 0x524);