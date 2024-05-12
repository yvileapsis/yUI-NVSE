#pragma once

#include "NiMemObject.hpp"

class NiTextureCodec : public NiMemObject {
public:
	void(__cdecl* m_pfnAlphaBlockDecoder)(unsigned __int8* pucDest);
	void(__cdecl* m_pfnColorBlockDecoder)(unsigned __int8* pucDest);
	const UInt8* m_pucSrcPtr;
	UInt32 m_uiDestRowStride;
	UInt8 m_aaucExplicitAlphaLookup[256][2];
};
