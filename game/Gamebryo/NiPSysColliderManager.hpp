#pragma once

#include "NiPSysModifier.hpp"
#include "NiPSysCollider.hpp"

NiSmartPointer(NiPSysColliderManager);

class NiPSysColliderManager : public NiPSysModifier {
public:
	NiPSysColliderManager();
	virtual ~NiPSysColliderManager();

	NiPSysColliderPtr m_spColliders;

	CREATE_OBJECT(NiPSysColliderManager, 0xC2D5B0)
};

ASSERT_SIZE(NiPSysColliderManager, 0x1C);