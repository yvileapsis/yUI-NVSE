#pragma once

#include "NiObject.hpp"

class NiCamera;
class NiLODNode;

NiSmartPointer(NiLODData);

class NiLODData : public NiObject {
public:
	NiLODData();
	virtual ~NiLODData();

	virtual SInt32		GetLODLevel(const NiCamera* pkCamera, NiLODNode* pkLOD);
	virtual void		UpdateWorldData(NiLODNode* pkLOD);
	virtual NiLODData*	Duplicate();
	virtual SInt32		GetLODIndex(SInt32 iLODLevel);
};

ASSERT_SIZE(NiLODData, 0x8)