#pragma once

#include "hkpWorldObject.hpp"

class hkpPhantomOverlapListener;
class hkpPhantomListener;
class hkAabb;

class hkpPhantom : public hkpWorldObject {
public:
	hkpPhantom();
	virtual ~hkpPhantom();

	virtual UInt32			getType() const;
	virtual void			calcAabb(hkAabb& aabb);
	virtual void			addOverlappingCollidable(hkpCollidable* collidable);
	virtual bool			isOverlappingCollidableAdded(const hkpCollidable* collidable);
	virtual void			removeOverlappingCollidable(hkpCollidable* collidable);
	virtual void			ensureDeterministicOrder();
	virtual hkpPhantom*		clone() const;
	virtual void			updateShapeCollectionFilter();
	virtual void			deallocateInternalArrays();

	hkArray<hkpPhantomOverlapListener*>		m_overlapListeners;
	hkArray<hkpPhantomListener*>			m_phantomListeners;
};

ASSERT_SIZE(hkpPhantom, 0xA4);