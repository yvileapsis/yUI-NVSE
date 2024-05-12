#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESDescription.hpp"
#include "TESTexture.hpp"
#include "TESAttributes.hpp"

class TESClass : public TESForm {
public:
	TESClass();
	~TESClass();

	enum
	{
		kFlag_Playable = 0x00000001,
		kFlag_Guard = 0x00000002,
	};

	enum
	{
		kService_Weapons = 0x00000001,
		kService_Armor = 0x00000002,
		kService_Clothing = 0x00000004,
		kService_Books = 0x00000008,
		kService_Food = 0x00000010,
		kService_Chems = 0x00000020,
		kService_Stimpacks = 0x00000040,
		kService_Lights = 0x00000080,	// ??
		kService_Misc = 0x00000400,
		kService_Potions = 0x00002000,	// probably deprecated
		kService_Training = 0x00004000,
		kService_Recharge = 0x00010000,
		kService_Repair = 0x00020000,
	};

	TESFullName		fullName;		// 18
	TESDescription	description;	// 24
	TESTexture		texture;		// 2C
	TESAttributes	attributes;		// 38

	// corresponds to DATA chunk
	UInt32			tagSkills[4];	// 44
	UInt32			classFlags;		// 54
	UInt32			services;		// 58
	UInt8			teaches;		// 5C
	UInt8			trainingLevel;	// 5D
	UInt8			pad5E[2];		// 5E
};

ASSERT_SIZE(TESClass, 0x60);