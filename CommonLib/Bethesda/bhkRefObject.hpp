#pragma once

#include "NiObject.hpp"
#include "hkReferencedObject.hpp"

class NiAVObject;

NiSmartPointer(bhkRefObject);

class bhkRefObject : public NiObject {
public:
	bhkRefObject();
	virtual ~bhkRefObject();

	virtual void SethkObject(hkReferencedObject* aphkObject);	// 35
	virtual void Link(bool abAttach);							// 36			

	hkReferencedObject* phkObject;

	NiAVObject* GetAVObject(NiRTTI* apRTTI = nullptr) const {
		return ThisStdCall<NiAVObject*>(0xC85C80, this, apRTTI);
	};
};

ASSERT_SIZE(bhkRefObject, 0xC);