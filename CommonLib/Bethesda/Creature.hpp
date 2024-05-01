#pragma once
#include "Actor.hpp"

// 0x1C0
class Creature : public Actor
{
public:
	Creature();
	~Creature();

	virtual void	Unk_137();

	UInt32			unk1B4[3];			// 1B4
};
static_assert(sizeof(Creature) == 0x1C0);
