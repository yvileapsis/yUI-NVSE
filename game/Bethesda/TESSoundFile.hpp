#pragma once

#include "BaseFormComponent.hpp"

class TESSoundFile : public BaseFormComponent {
public:
	TESSoundFile();
	~TESSoundFile();

	virtual void	Set(const char* str);

	BSStringT	path;	// 4
};