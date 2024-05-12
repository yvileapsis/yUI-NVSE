#pragma once

#include "TESForm.hpp"
#include "TESTexture.hpp"
#include "TESDescription.hpp"

class TESSkill : public TESForm {
public:
	TESSkill();
	~TESSkill();

	TESDescription description;
	TESTexture texture;
	UInt32 unk2C;
	UInt32 unk30;
	UInt32 unk34;
	float unk38[2];
	TESDescription desc2[4];
};

ASSERT_SIZE(TESSkill, 0x60);