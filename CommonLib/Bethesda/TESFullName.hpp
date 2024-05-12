#pragma once
// Complete
#include "BaseFormComponent.hpp"
#include "BSString.hpp"

// 0xC
class TESFullName : public BaseFormComponent {
public:
	TESFullName();
	~TESFullName();

	BSStringT<char>	kFullName;
};
static_assert(sizeof(TESFullName) == 0xC);