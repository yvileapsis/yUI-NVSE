#pragma once
#include "ActiveEffect.hpp"

class ConcussionEffect : public ActiveEffect
{
public:
	ConcussionEffect();
	~ConcussionEffect();

	float	unk48;		//  48
};
static_assert(sizeof(ConcussionEffect) == 0x4C);