#pragma once

#include "NiGeometryData.hpp"

NiSmartPointer(NiLinesData);

class NiLinesData : public NiGeometryData {
public:
	NiLinesData();
	virtual ~NiLinesData();

	bool* m_pkFlags;

	CREATE_OBJECT(NiLinesData, 0xA8A700);

	void SetStartingPoint(const NiPoint3& kPoint);
	void SetEndingPoint(const NiPoint3& kPoint);
};

ASSERT_SIZE(NiLinesData, 0x44)