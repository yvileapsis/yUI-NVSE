#pragma once

#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class TESTexture : public BaseFormComponent {
public:
	TESTexture();
	~TESTexture();

	virtual UInt32	Unk_04(void);
	virtual void	GetNormalMap(BSStringT* str);
	virtual char*	GetPathRoot(void);

	BSStringT TextureName;

	const char* GetTextureName();
	UInt32 GetTextureNameLength();
};