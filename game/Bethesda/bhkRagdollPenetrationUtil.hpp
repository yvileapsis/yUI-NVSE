#pragma once

#include "BSPrecisionTimer.hpp"
#include "BSSimpleArray.hpp"
#include "hkaDetectRagdollPenetration.hpp"
#include "hkaRagdollRaycastInterface.hpp"

ALIGN16 class bhkRagdollPenetrationUtil : public hkaRagdollRaycastInterface {
public:
	bhkRagdollPenetrationUtil();
	virtual ~bhkRagdollPenetrationUtil();

	struct ConstrainedSystem {
		void*								unk00;
		void*								unk04;
		void*								unk08;
		void*								unk0C;
		hkaDetectRagdollPenetration*		pDetectionUtil;
		void*								unk14;
		void*								unk18;
		void*								unk1C;
		bool								bPenetrating;
	};

	BSSimpleArray<ConstrainedSystem*>	kActiveSystems;
	UInt32								uiGroup;
	bool								bAttemptToInitializeNextUpdate;
	UInt32								unk1C;
	BSPrecisionTimer					kDetectionTimer;
};

ASSERT_SIZE(bhkRagdollPenetrationUtil, 0x30);