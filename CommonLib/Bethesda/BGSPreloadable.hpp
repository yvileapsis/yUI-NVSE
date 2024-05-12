#pragma once
#include "BaseFormComponent.hpp"

// 0x4
class BGSPreloadable : public BaseFormComponent {
public:
	BGSPreloadable();
	~BGSPreloadable();

	virtual void	Fn_04(); // pure virtual
};
static_assert(sizeof(BGSPreloadable) == 0x4);