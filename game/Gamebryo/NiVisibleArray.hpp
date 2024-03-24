#pragma once

class NiGeometry;

class NiVisibleArray {
public:
	NiGeometry** m_ppkArray;
	UInt32 m_uiCurrentSize;
	UInt32 m_uiAllocatedSize;
	UInt32 m_uiGrowBy;
};