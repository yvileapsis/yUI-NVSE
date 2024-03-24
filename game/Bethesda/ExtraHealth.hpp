#pragma once

#include "BSExtraData.hpp"

class ExtraHealth : public BSExtraData {
public:
	ExtraHealth();
	virtual ~ExtraHealth();
	float health;

	static ExtraHealth* Create();
};