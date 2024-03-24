#pragma once

#include "BSExtraData.hpp"

class ExtraWorn : public BSExtraData {
public:
	ExtraWorn();
	virtual ~ExtraWorn();

	static ExtraWorn* Create();
};