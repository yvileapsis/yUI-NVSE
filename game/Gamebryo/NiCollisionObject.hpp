#pragma once

#include "NiObject.hpp"

class NiAVObject;
class NiUpdateData;

NiSmartPointer(NiCollisionObject);

class NiCollisionObject : public NiObject {
public:
	NiCollisionObject();
	virtual ~NiCollisionObject();

	virtual void		SetSceneGraphObject(NiAVObject* apSceneObject);
	virtual void		UpdateWorldData(NiUpdateData& arData);
	virtual void		RecreateWorldData();
	virtual void		LoadBoundingVolume(void* apData);
	virtual void		Convert(UInt32 uiVersion, void* apData);

	NiAVObject* m_pkSceneObject;
};

ASSERT_SIZE(NiCollisionObject, 0xC);