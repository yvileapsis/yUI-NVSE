#pragma once
#include "BaseFormComponent.hpp"

// 0xC
class TESSoundFile : public BaseFormComponent {
public:
	TESSoundFile();
	~TESSoundFile();

	virtual void	Set(const char* str);

	BSStringT<char>	kSoundPath;	// 4
};
static_assert(sizeof(TESSoundFile) == 0xC);