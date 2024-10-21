#pragma once

#include "hkReferencedObject.hpp"
#include "hkpLinkedCollidable.hpp"
#include "hkStringPtr.hpp"
#include "hkMultiThreadCheck.hpp"
#include "hkSimpleProperty.hpp"

class hkpWorld;
class hkMotionState;

class hkpWorldObject : public hkReferencedObject {
public:
	hkpWorldObject();
	virtual ~hkpWorldObject();

	virtual UInt32			setShape(const hkpShape* shape);
	virtual hkMotionState*	getMotionState();

	hkpWorld*					m_world;
	void*						m_userData;
	hkpLinkedCollidable			m_collidable;
	hkMultiThreadCheck			m_multiThreadCheck;
	hkStringPtr					m_name;
	hkArray<hkSimpleProperty>	m_properties;
	hkReferencedObject*			m_aiData;
	void*						m_treeData;

	const char* GetName() const {
		return m_name.c_str();
	};

	hkSimplePropertyValue* getProperty(hkSimplePropertyValue* result, UInt32 key) const;
};

ASSERT_SIZE(hkpWorldObject, 0x8C);