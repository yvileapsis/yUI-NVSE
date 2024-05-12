#pragma once

#include "NiRefObject.hpp"
#include "NiGPUProgram.hpp"
#include "NiShaderAttributeDesc.hpp"

class NiShaderConstantMap : public NiRefObject {
public:
	NiShaderConstantMap();
	virtual ~NiShaderConstantMap();

	virtual UInt32	AddEntry(const char* pszKey, UInt32 uiFlags, UInt32 uiExtra, UInt32 uiShaderRegister, UInt32 uiRegisterCount, const char* pszVariableName, UInt32 uiDataSize, UInt32 uiDataStride, const void* pvDataSource, bool bCopyData = false);
	virtual UInt32	AddEntryAlt(const char* pszKey, UInt32 uiExtra, UInt32 uiShaderRegister, const char* pszVariableName);
	virtual UInt32	AddAttributeEntry(const char* pszKey, UInt32 uiFlags, UInt32 uiExtra, UInt32 uiShaderRegister, UInt32 uiRegisterCount, const char* pszVariableName, UInt32 uiDataSize, UInt32 uiDataStride, const void* pvDataSource, bool bCopyData = false);
	virtual UInt32	AddConstantEntry(const char* pszKey, UInt32 uiFlags, UInt32 uiExtra, UInt32 uiShaderRegister, UInt32 uiRegisterCount, const char* pszVariableName, UInt32 uiDataSize, UInt32 uiDataStride, const void* pvDataSource, bool bCopyData = false);
	virtual UInt32	AddGlobalEntry(const char* pszKey, UInt32 uiFlags, UInt32 uiExtra, UInt32 uiShaderRegister, UInt32 uiRegisterCount, const char* pszVariableName, UInt32 uiDataSize, UInt32 uiDataStride, const void* pvDataSource, bool bCopyData = false);
	virtual UInt32	AddOperatorEntry(const char* pszKey, UInt32 uiFlags, UInt32 uiExtra, UInt32 uiShaderRegister, UInt32 uiRegisterCount, const char* pszVariableName);
	virtual UInt32	AddObjectEntry(const char* pszKey, UInt32 uiShaderRegister, const char* pszVariableName, UInt32 uiObjectIndex, NiShaderAttributeDesc::ObjectType eObjectType);
	virtual void	Func0009();

	NiGPUProgram::ProgramType m_eProgramType;
};