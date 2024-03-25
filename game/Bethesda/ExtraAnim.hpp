#pragma once
#include "BSExtraData.hpp"

// 0x10
class ExtraAnim : public BSExtraData {
public:
	ExtraAnim();
	virtual ~ExtraAnim();

	struct Animation
	{
	};	// 013C

	Animation* data;	// 0C
};
static_assert(sizeof(ExtraAnim) == 0x10);