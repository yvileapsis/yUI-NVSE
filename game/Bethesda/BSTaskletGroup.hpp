#pragma once

#include "BSTaskletGroupData.hpp"

class BSTaskletGroup {
public:
	BSTaskletGroup() : pData(nullptr) {};
	~BSTaskletGroup() {};

	BSTaskletGroupData* pData;
};