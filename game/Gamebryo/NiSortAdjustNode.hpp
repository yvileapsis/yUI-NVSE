#pragma once

#include "NiNode.hpp"

NiSmartPointer(NiSortAdjustNode);
NIRTTI_ADDRESS(NiSortAdjustNode, 0x11F5ED8);

class NiSortAdjustNode : public NiNode {
public:
	NiSortAdjustNode();
	~NiSortAdjustNode();

	enum SortingMode
	{
		SORTING_INHERIT,
		SORTING_OFF,
		SORTING_MAX
	};

	SortingMode m_eSortingMode;

	CREATE_OBJECT(NiSortAdjustNode, 0xA952A0);
};

ASSERT_SIZE(NiSortAdjustNode, 0xB0)