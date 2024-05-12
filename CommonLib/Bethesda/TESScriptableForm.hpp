#pragma once
#include "BaseFormComponent.hpp"
// Complete

class Script;

// 0xC
class TESScriptableForm : public BaseFormComponent {
public:
	TESScriptableForm();
	~TESScriptableForm();

	Script* pkScript;	// 004
	bool	bResolved;	// 008	called during LoadForm, so scripts do not wait for TESForm_InitItem to be resolved
	UInt8	pad[3];		// 009

};
static_assert(sizeof(TESScriptableForm) == 0xC);