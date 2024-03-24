#pragma once

#include "NiD3DShader.hpp"
#include "NiDynamicEffectState.hpp"

class NiD3DDefaultShader : public NiD3DShader {
public:
	enum PassFogStatus {
		NO_FOG			= 0,
		BLACK_FOG		= 1,
		STANDARD_FOG	= 2,
	};

	void*							Unk3C;
	UInt32							m_uiMaxTextureIndex;
	UInt32							m_uiRemainingStages;
	UInt32							m_uiRemainingTextures;
	UInt32							m_uiOldLightingValue;
	PassFogStatus*					m_peFogPassArray;
	UInt32							m_uiFogPassArraySize;
	const NiDynamicEffectState*		m_pkLastState;

	NIRTTI_ADDRESS(0x126F554);
};

ASSERT_SIZE(NiD3DDefaultShader, 0x5C);