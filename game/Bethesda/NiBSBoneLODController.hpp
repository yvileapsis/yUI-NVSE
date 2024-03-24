#pragma once

#include "NiTimeController.hpp"
#include "NiTSet.hpp"
#include "NiTArray.hpp"

NiSmartPointer(NiBSBoneLODController);

class NiBSBoneLODController : public NiTimeController {
public:
	NiBSBoneLODController();
	~NiBSBoneLODController();

	SInt32											m_iLOD;
	UInt32											m_uiNumLODs;
	NiTPrimitiveArray<NiTPrimitiveSet<NiNode*>*>	kNodeSets;

	CREATE_OBJECT(NiBSBoneLODController, 0xC537F0);
	NIRTTI_ADDRESS(0x12031B8);
};

ASSERT_SIZE(NiBSBoneLODController, 0x4C)