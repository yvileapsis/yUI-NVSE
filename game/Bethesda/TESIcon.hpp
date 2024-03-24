#pragma once

#include "TESTexture.hpp"

class TESIcon : public TESTexture {
public:
	TESIcon();
	~TESIcon();

	void SetPath(const char* newPath) { TextureName.Set(newPath); }
};