#pragma once

#include "bhkSerializable.hpp"

class bhkWorldObject : public bhkSerializable {
public:
	bhkWorldObject();
	virtual ~bhkWorldObject();

	virtual void	UpdateCollisionFilter();				// 49
	virtual void	ForceAdd(bhkWorld* apWorld);			// 50
	virtual NiNode* CreateDisplayGeometry(NiNode* apNode);	// 51
	virtual void	DestroyDisplayGeometry();				// 52

	enum Flags {
		WIND							= 1 << 0,
		DYNAMIC							= 1 << 1,
		FIXED_CONSTRAINTS				= 1 << 2,
		UNK_3							= 1 << 3,
		BATCH_ADD						= 1 << 4,
		BATCH_REMOVE					= 1 << 5,
		DISABLED						= 1 << 6,
		UNK_7							= 1 << 7,
		STICKING_TARGET					= 1 << 8,
		KEYFRAMED_BY_LOADEDAREABOUND	= 1 << 9,
	};

	Bitfield32 uiFlags;
};

ASSERT_SIZE(bhkWorldObject, 0x14)