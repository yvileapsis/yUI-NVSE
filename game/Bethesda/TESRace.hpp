#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESDescription.hpp"
#include "TESReactionForm.hpp"
#include "TESAttributes.hpp"
#include "TESSpellList.hpp"
#include "TESModel.hpp"
#include "TESTexture.hpp"
#include "BGSTextureModel.hpp"
#include "TESHair.hpp"
#include "TESEyes.hpp"
#include "NiTArray.hpp"
#include "BSSimpleArray.hpp"
#include "BSFaceGenManager.hpp"

class BGSVoiceType;

class TESRace : public TESForm {
public:
	// 2
	struct SkillMod
	{
		UInt8	actorValue;
		SInt8	mod;
	};

	enum
	{
		kFlag_Playable = 0x00000001,
		kFlag_Child = 0x00000004,
	};

	TESRace();
	~TESRace();

	TESFullName		fullName;				// 018
	TESDescription	desc;					// 024
	TESSpellList	spells;					// 02C
	TESReactionForm	reaction;				// 040

	SkillMod		skillMods[7];			// 050
	UInt8			pad05E[2];				// 05E
	float			height[2];				// 060 male/female
	float			weight[2];				// 068 male/female
	UInt32			raceFlags;				// 070

	TESAttributes	baseAttributes[2];		// 074 male/female
	BSSimpleList<TESHair*>	hairs;					// 08C
	TESHair* defaultHair[2];			// 094 male/female
	UInt8			defaultHairColor[2];	// 09C male/female
	UInt8			fill09E[2];				// 09E

	float unk0A0;
	float unk0A4;

	BSSimpleList<TESEyes*>	eyes;					// 0A8

	TESModel		faceModels[2][8];			// 0B0	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture		faceTextures[2][8];			// 230	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture		bodyPartsTextures[2][3];	// 2F0	male/female	UpperBody, LeftHand, RightHand
	TESModel		bodyModels[2][3];			// 338	male/female	UpperBody, LeftHand, RightHand
	BGSTextureModel	bodyTextures[2];			// 3C8	male/female	EGT file, not DDS.
	BSFaceGenManager::FaceGenDatas faceGenData;
	BSFaceGenManager::FaceGenDatas faceGenData2;
	UInt32 unk4F8;
	BGSVoiceType* voiceTypes[2];
	TESRace* ageRace[2];
	BSStringT editorID;
	BSSimpleArray<void*>	faceGenUndo;		// 4D4 - NiTPrimitiveArray<FaceGenUndo *>

	bool IsPlayable() const { return (raceFlags & kFlag_Playable) == kFlag_Playable; }
};

ASSERT_SIZE(TESRace, 0x524);