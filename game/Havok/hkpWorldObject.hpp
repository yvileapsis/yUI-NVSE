#pragma once

#include "hkReferencedObject.hpp"
#include "hkpLinkedCollidable.hpp"
#include "hkStringPtr.hpp"
#include "hkMultiThreadCheck.hpp"

class hkpWorld;

class hkpWorldObject : public hkReferencedObject {
public:
	hkpWorld*				m_world;
	void*					m_userData;
	hkpLinkedCollidable		m_collidable;
	hkMultiThreadCheck		m_multiThreadCheck;
	hkStringPtr				m_name;
	hkArray<void>			m_properties;
	hkReferencedObject*		m_aiData;
	void*					m_treeData;
};

ASSERT_SIZE(hkpWorldObject, 0x8C);