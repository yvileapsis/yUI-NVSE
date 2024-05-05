#pragma once

#include "NiGeometryGroup.hpp"

class NiVBDynamicSet;
class NiVBChip;

class NiDynamicGeometryGroup : public NiGeometryGroup {
public:
	NiDynamicGeometryGroup();
	virtual ~NiDynamicGeometryGroup();

	NiTPointerMap<UInt32, NiVBDynamicSet*>  m_kSets;
	NiTPointerMap<UInt32, NiVBChip*>		m_kChips;
	NiTPrimitiveArray<NiVBDynamicSet*>	  m_kMultiStreamSets;
	NiTPrimitiveArray<NiVBChip*>			m_kMultiStreamChips;
	UInt32								  m_uiNextIndex;
};

ASSERT_SIZE(NiDynamicGeometryGroup, 0x50);