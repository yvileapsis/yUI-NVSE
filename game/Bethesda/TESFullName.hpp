#pragma once

#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class TESFullName : public BaseFormComponent {
public:
	TESFullName();
	~TESFullName();

	BSStringT	strFullName;
};

ASSERT_SIZE(TESFullName, 0xC);