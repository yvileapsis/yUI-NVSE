#pragma once

#include "NiMemObject.hpp"

class NiGeometryGroup;
class NiScreenTexture;
class NiSkinInstance;
class NiGeometryData;

class NiGeometryGroupManager : public NiMemObject {
public:
	enum RepackingBehavior {
		STATIC = 0x0,
		UNSHARED = 0x1,
		DYNAMIC = 0x2,
	};

	virtual						~NiGeometryGroupManager();
	virtual NiGeometryGroup*	CreateGroup(NiGeometryGroupManager::RepackingBehavior eBehavior);
	virtual bool				DestroyGroup(NiGeometryGroup* pkGroup);
	virtual bool				IsGroupEmpty(NiGeometryGroup* pkGroup);
	virtual bool				AddObjectToGroup(NiGeometryGroup* pkGroup, NiScreenTexture* pkScreenTexture);
	virtual bool				AddObjectToGroupAlt(NiGeometryGroup* pkGroup, NiGeometryData* pkData, NiSkinInstance* pkSkinInstance);
	virtual bool				RemoveObjectFromGroup(NiScreenTexture* pkScreenTexture);
	virtual bool				RemoveObjectFromGroupAlt(NiGeometryData* pkData, NiSkinInstance* pkSkinInstance);
};