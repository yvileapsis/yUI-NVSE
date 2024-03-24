#pragma once

#include "BSExtraData.hpp"

class ExtraAnim : public BSExtraData {
public:
	ExtraAnim();
	virtual ~ExtraAnim();

	struct Animation
	{
	};	// 013C

	Animation* data;	// 0C
};	// 10