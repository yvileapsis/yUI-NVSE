#pragma once
// Complete
#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class TESForm;

// 0xC
class TESFullName : public BaseFormComponent {
public:
	TESFullName();
	~TESFullName();

	BSStringT<char>	kFullName;

	const char* GetFullName() const;
	static const char* GetFullName(const TESForm* apForm);
};
static_assert(sizeof(TESFullName) == 0xC);