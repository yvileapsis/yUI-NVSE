#pragma once

#include "BaseFormComponent.hpp"

class BGSAmmoForm : public BaseFormComponent {
public:
	BGSAmmoForm();
	~BGSAmmoForm();

	TESForm* ammo; // 04	either TESAmmo or BGSListForm
};