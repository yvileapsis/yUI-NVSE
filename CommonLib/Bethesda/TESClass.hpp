#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESDescription.hpp"
#include "TESTexture.hpp"
#include "TESAttributes.hpp"

class TESClass :
	public TESForm,
	public TESFullName,		// 18
	public TESDescription,	// 24
	public TESTexture,		// 2C
	public TESAttributes	// 38
{
public:
	TESClass();
	~TESClass();

	enum EnumClassFlags
	{
		kFlag_Playable = 0x00000001,
		kFlag_Guard = 0x00000002,
	};

	enum EnumServices
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

	// corresponds to DATA chunk
	UInt32			uiTagSkills[4];		// 44
	EnumClassFlags	eFlags;				// 54
	EnumServices	eServices;			// 58
	UInt8			ucTeaches;			// 5C
	UInt8			ucTrainingLevel;	// 5D
	UInt8			pad5E[2];			// 5E
};
static_assert(sizeof(TESClass) == 0x60);