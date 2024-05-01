#pragma once
#include "TESForm.hpp"
#include "TESTexture.hpp"
#include "TESDescription.hpp"

// 60
class TESSkill :
	public TESForm,
	public TESDescription,
	public TESTexture
{
public:
	TESSkill();
	~TESSkill();

	UInt32			unk2C;
	UInt32			unk30;
	UInt32			unk34;
	Float32			unk38[2];
	TESDescription	kDesc2[4];
};
static_assert(sizeof(TESSkill) == 0x60);