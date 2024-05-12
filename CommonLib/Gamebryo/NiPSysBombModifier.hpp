#pragma once

#include "NiPSysModifier.hpp"
#include "NiAVObject.hpp"
#include "NiPoint3.hpp"

class NiPSysData;

NiSmartPointer(NiPSysBombModifier);

class NiPSysBombModifier : public NiPSysModifier {
public:
	NiPSysBombModifier();
	virtual ~NiPSysBombModifier();

	enum DecayType {
		NONE,
		LINEAR,
		EXPONENTIAL
	};

	enum SymmType {
		SPHERICAL,
		CYLINDRICAL,
		PLANAR
	};

	NiAVObject*		m_pkBombObj;
	NiPoint3		m_kBombAxis;
	float			m_fDecay;
	float			m_fDeltaV;
	DecayType		m_eDecayType;
	SymmType		m_eSymmType;

	CREATE_OBJECT(NiPSysBombModifier, 0xC2E4C0);

	static void __fastcall UpdateEx(NiPSysBombModifier* apThis, void*, float fTime, NiPSysData* pkData);
};

ASSERT_SIZE(NiPSysBombModifier, 0x38);