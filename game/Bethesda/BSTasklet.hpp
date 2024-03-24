#pragma once

#include "BSTaskletData.hpp"

class BSTasklet {
public:
	virtual ~BSTasklet();

	BSTaskletData* pData;
};