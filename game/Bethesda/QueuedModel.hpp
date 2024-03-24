#pragma once

#include "BSFadeNode.hpp"
#include "Model.hpp"
#include "QueuedFileEntry.hpp"
#include "TESModel.hpp"

NiSmartPointer(QueuedModel);

class QueuedModel : public QueuedFileEntry {
public:
	QueuedModel();
	~QueuedModel();

	virtual void Unk_0C(UInt32 arg0);

	enum Flags {
		ASSIGN_SHADERS		= 1 << 0,
		INCREASE_COUNTER	= 1 << 1,
		QUEUED_CHILDREN		= 1 << 2,
		CACHED_MODEL		= 1 << 3,
		FORCE_CREATE 		= 1 << 4,
		KEEP_UV				= 1 << 5,
	};


	ModelPtr		spModel;
	TESModel*		pTESModel;
	ENUM_LOD_MULT	eLODFadeMult;
	Bitfield8		ucFlags;
	float			flt040;
	UInt32			unk044;
};

ASSERT_SIZE(QueuedModel, 0x48);