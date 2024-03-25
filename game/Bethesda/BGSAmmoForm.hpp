#pragma once
#include "BaseFormComponent.hpp"

// 0x8
class BGSAmmoForm : public BaseFormComponent {
public:
	BGSAmmoForm();
	~BGSAmmoForm();

	TESForm* pkAmmo; // 04	either TESAmmo or BGSListForm
};
static_assert(sizeof(BGSAmmoForm) == 0x8);