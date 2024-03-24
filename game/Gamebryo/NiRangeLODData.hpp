#pragma once

#include "NiLODData.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(NiRangeLODData);

class NiRangeLODData : public NiLODData {
public:
	NiRangeLODData();
	virtual ~NiRangeLODData();

	struct Range : NiMemObject {
		float m_fNear;
		float m_fFar;
		float m_fWorldNear;
		float m_fWorldFar;
	};

	NiPoint3	m_kCenter;
	NiPoint3	m_kWorldCenter;
	UInt32		m_uiNumRanges;
	Range*		m_pkRanges;

	CREATE_OBJECT(NiRangeLODData, 0xA97C00);

	void SetNumRanges(UInt32 auiNumRanges);

	static SInt32 __fastcall GetLODLevelEx(NiRangeLODData* apThis, void*, NiCamera* apCamera, NiLODNode* apLOD);
};

ASSERT_SIZE(NiRangeLODData, 0x28)