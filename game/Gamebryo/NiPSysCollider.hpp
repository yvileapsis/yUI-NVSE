#pragma once

#include "NiObject.hpp"
#include "NiPoint3.hpp"

class NiPSysSpawnModifier;
class NiPSysColliderManager;
class NiPSysData;

NiSmartPointer(NiPSysCollider);

class NiPSysCollider : public NiObject {
public:
	NiPSysCollider();
	virtual ~NiPSysCollider();

	virtual NiPSysCollider*		Resolve(float afInitialTime, float& arfCollisionTime, NiPSysData* apData, UInt16 ausCurParticle);
	virtual void				Update(float afTime, NiPSysData* apData, UInt16 ausCurParticle);
	virtual void				SetResolveConstants();

	float					m_fBounce;
	bool					m_bSpawnOnCollide;
	bool					m_bDieOnCollide;
	NiPSysSpawnModifier*	m_pkSpawnModifier;
	NiPoint3				m_kCollisionPoint;
	float					m_fCollisionTime;
	NiPSysColliderManager*	m_pkManager;
	NiPSysColliderPtr		m_spNext;

	CREATE_OBJECT(NiPSysCollider, 0xC2D5B0);

	static NiPSysCollider* __fastcall ResolveEx(NiPSysCollider* apThis, void*, float afInitialTime, float& arfCollisionTime, NiPSysData* apData, UInt16 ausCurParticle);
};

ASSERT_SIZE(NiPSysCollider, 0x2C);