#pragma once

#include "NiAdditionalGeometryData.hpp"

class BSPackedAdditionalGeometryData : public NiAdditionalGeometryData {
public:
	UInt32	m_uiShaderDeclarationIndex;
	DWORD	dword30; // Read from NIF

	NIRTTI_ADDRESS(0x11F4AAC);

	class NiBSPackedAGDDataBlock : public NiAdditionalGeometryData::NiAGDDataBlock {
	public:
		NiBSPackedAGDDataBlock();
		virtual ~NiBSPackedAGDDataBlock();

		bool byteD;
	};
};

ASSERT_SIZE(BSPackedAdditionalGeometryData, 0x34);
ASSERT_SIZE(BSPackedAdditionalGeometryData::NiBSPackedAGDDataBlock, 0x14);