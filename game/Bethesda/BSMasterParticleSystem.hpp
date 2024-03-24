#pragma once

#include "NiNode.hpp"
#include "DList.hpp"

NiSmartPointer(BSMasterParticleSystem);

class BSMasterParticleSystem : public NiNode {
public:
	BSMasterParticleSystem();
	virtual ~BSMasterParticleSystem();

	NiTListItem<NiAVObjectPtr>		kEmitterObjList;
	UInt16							usActiveEmitterObjCount;
	UInt16							usMaxEmitterObj;
	UInt16							wordBC;
	NiTListItem<NiAVObjectPtr>*		kEmitterIterator;
	UInt32							uiIndex;
	DWORD							dwordC8;
	NiTPrimitiveArray<NiParticles*> kChildParticles;
	float							fMasterPSysTime;

	CREATE_OBJECT(BSMasterParticleSystem, 0xC5E5A0)
};

ASSERT_SIZE(BSMasterParticleSystem, 0xE0)