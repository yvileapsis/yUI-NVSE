#pragma once

#include "NiIntegerExtraData.hpp"

NiSmartPointer(BSXFlags);

class BSXFlags : public NiIntegerExtraData {
public:
	BSXFlags();
	virtual ~BSXFlags();

	enum Flags : UInt32 {
		ANIMATED				= 1 << 0,
		HAVOK					= 1 << 1,
		RAGDOLL					= 1 << 2,
		COMPLEX					= 1 << 3,
		ADDON					= 1 << 4,
		EDITOR_MARKER			= 1 << 5,
		DYNAMIC					= 1 << 6,
		ARTICULATED				= 1 << 7,
		IK_TARGET				= 1 << 8,
		EXTERNAL_EMITTANCE		= 1 << 9,
	};

	inline void SetFlag(Flags aeFlag, bool abSet) {
		if (abSet)
			m_uiValue |= aeFlag;
		else
			m_uiValue &= ~aeFlag;
	}

	inline bool GetFlag(Flags aeFlag) const {
		return (m_uiValue & aeFlag) != 0;
	}

	CREATE_OBJECT(BSXFlags, 0xC43190);
	NIRTTI_ADDRESS(0x1202DE0);
};

ASSERT_SIZE(BSXFlags, 0x10)