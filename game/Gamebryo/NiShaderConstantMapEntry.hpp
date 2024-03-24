#pragma once

#include "NiRefObject.hpp"
#include "NiFixedString.hpp"

NiSmartPointer(NiShaderConstantMapEntry);

class NiShaderConstantMapEntry : public NiRefObject {
public:
	bool			bActive;
	NiFixedString	m_kKey;
	UInt32			m_uiInternal;
	UInt32			m_uiFlags;
	UInt32			m_uiExtra;
	UInt32			m_uiShaderRegister;
	UInt32			m_uiRegisterCount;
	NiFixedString	m_kVariableName;
	UInt32			m_uiDataSize;
	UInt32			m_uiDataStride;
	void*			m_pvDataSource;
	bool			m_bOwnData;
	bool			m_bVariableHookupValid;
	bool			m_bColumnMajor;
};

ASSERT_SIZE(NiShaderConstantMapEntry, 0x38);