#pragma once

#include "hkReferencedObject.hpp"
#include "hkSmallArray.hpp"
#include "hkStringPtr.hpp"

class hkpConstraintOwner;
class hkpConstraintData;
class hkpModifierConstraintAtom;
class hkpEntity;
class hkpConstraintListener;
class hkConstraintInternal;

class hkpConstraintInstance : public hkReferencedObject {
public:
	hkpConstraintInstance();
	virtual ~hkpConstraintInstance();

	hkpConstraintOwner*						m_owner;
	hkpConstraintData*						m_data;
	hkpModifierConstraintAtom*				m_constraintModifiers;
	hkpEntity*								m_entities[2];
	UInt8									m_priority;
	bool									m_wantRuntime;
	UInt8									m_destructionRemapInfo;
	hkSmallArray<hkpConstraintListener*>	m_listeners;
	hkStringPtr								m_name;
	void*									m_userData;
	hkConstraintInternal*					m_internal;
	UInt32									m_uid;

	const char* GetName() const {
		return m_name;
	}
};

ASSERT_SIZE(hkpConstraintInstance, 0x38);