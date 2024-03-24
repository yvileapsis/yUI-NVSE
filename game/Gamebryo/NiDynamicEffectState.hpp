#pragma once

#include "NiRefObject.hpp"
#include "NiDynamicEffect.hpp"

class NiDynamicEffectState : public NiRefObject {
public:
	NiDynamicEffectState();
	virtual ~NiDynamicEffectState();

	class ListItem {
	public:
		ListItem*			m_pNext;
		NiDynamicEffect*	m_pItem;
	};

	bool		m_bMultiTexture;
	ListItem*	m_pLighBSSimpleList;
	ListItem*	m_pProjLighBSSimpleList;
	ListItem*	m_pProjShadowList;
};

ASSERT_SIZE(NiDynamicEffectState, 0x18)