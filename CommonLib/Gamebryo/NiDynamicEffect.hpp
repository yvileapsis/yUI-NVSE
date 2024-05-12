#pragma once

#include "NiAVObject.hpp"
#include "NiTPointerList.hpp"

class NiNode;

typedef NiTPointerList<NiNode*> NiNodeList;

class NiDynamicEffect : public NiAVObject {
public:
	NiDynamicEffect();
	virtual ~NiDynamicEffect();

	enum EffectType {
		AMBIENT_LIGHT	= 0,
		POINT_LIGHT		= 2,
		DIR_LIGHT		= 3,
		SPOT_LIGHT		= 4,
		TEXTURE_EFFECT	= 5,
	};

	bool		m_bOn;
	UInt8		m_ucEffectType;
	union {
		struct JIP {
			bool		bResetTraits;
			Bitfield8   ucExtraFlags;
		};
		bool	bCastShadows;
		bool	bCanCarry;
	};
	SInt32		m_iIndex;
	UInt32		m_uiPushCount;
	UInt32		m_uiRevID;
	NiNodeList	m_pkShadowGenerator;
	NiNodeList	m_kUnaffectedNodeList;
};

ASSERT_SIZE(NiDynamicEffect, 0xC4)