#pragma once

#include "TESForm.hpp"

class BGSCameraPath : public TESForm {
public:
	BSSimpleList<int> bssimplelist_int18;
	BSSimpleList<int> bssimplelist_int20;
	char unk28;
	int unk2C;
	int unk30;
	int unk34;
};

ASSERT_SIZE(BGSCameraPath, 0x38);