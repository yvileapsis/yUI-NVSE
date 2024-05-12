#pragma once

#include "hkReferencedObject.hpp"
#include "hkArray.hpp"

class hkaSkeleton;
class hkaRagdollRaycastInterface;
class hkpAabbPhantom;

class hkaDetectRagdollPenetration : public hkReferencedObject {
public:
	hkaDetectRagdollPenetration();
	virtual ~hkaDetectRagdollPenetration();

	struct Setup {
		const hkaSkeleton*			m_ragdollSkeleton;
		hkaRagdollRaycastInterface*	m_raycastInterface;
		hkpAabbPhantom*				m_ragdollPhantom;
	};

	enum BonePenetrationStatus {
		HK_NOP = 0,				// No penetration
		HK_YESP = 1,			// Yes, bone penetrates (penetration point lies on this bone)
		HK_YESP_DESCENDANT = 2	// Yes, bone is descendant of penetrated bones
	};

	Setup	m_setup;
	UInt32	m_numBones;
	hkArray<BonePenetrationStatus> m_boneStates;
	bool	m_ragdollIsOut;

};

ASSERT_SIZE(hkaDetectRagdollPenetration, 0x28)