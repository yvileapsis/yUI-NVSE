#pragma once

#include "BaseFormComponent.hpp"

class BGSPreloadable : public BaseFormComponent {
public:
	BGSPreloadable();
	~BGSPreloadable();

	virtual void	Fn_04(void); // pure virtual
};