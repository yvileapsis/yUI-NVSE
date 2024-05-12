#pragma once

#include "BaseFormComponent.hpp"
class TESScriptableForm : public BaseFormComponent {
public:
	TESScriptableForm();
	~TESScriptableForm();

	Script* script;	// 004
	bool	resolved;	// 008	called during LoadForm, so scripts do not wait for TESForm_InitItem to be resolved
};

ASSERT_SIZE(TESScriptableForm, 0xC);