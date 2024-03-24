#pragma once

#include "NiAccumulator.hpp"
#include "NiGeometry.hpp"
#include "NiTPointerList.hpp"

typedef NiTPointerList<NiGeometry*> NiSortedObjectList;

NiSmartPointer(NiBackToFrontAccumulator);

class NiBackToFrontAccumulator : public NiAccumulator {
public:
	NiBackToFrontAccumulator();
	virtual ~NiBackToFrontAccumulator();

	virtual void Sort();

	NiSortedObjectList	m_kItems;
	NiSortedObjectList* m_pGeometryList;
	SInt32				m_iNumItems;
	SInt32				m_iMaxItems;
	NiGeometry**		m_ppkItems;
	float*				m_pfDepths;
	SInt32				m_iCurrItem;

	float ChoosePivot(int l, int r) const;
	void SortObjectsByDepth(int l, int r);
};

ASSERT_SIZE(NiBackToFrontAccumulator, 0x30);