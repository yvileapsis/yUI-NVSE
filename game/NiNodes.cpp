#include <NiNodes.h>
#include <Utilities.hpp>

void TextureFormat::InitFromD3DFMT(UInt32 fmt)
{
	typedef void (* _D3DFMTToTextureFormat)(UInt32 d3dfmt, TextureFormat * dst);
	_D3DFMTToTextureFormat D3DFMTToTextureFormat = (_D3DFMTToTextureFormat)0x00E7C1E0;
	
	D3DFMTToTextureFormat(fmt, this);
}

static const UInt32 kNiObjectNET_SetNameAddr = 0x00A5B690;

NiExtraData* __fastcall NiObjectNET::GetExtraData(UInt32 vtbl) const
{
	for (UInt16 index = 0; index < m_extraDataListLen; index++)
	{
		const auto iter = this->m_extraDataList[index];
		if (*(UInt32*)iter == vtbl)
			return iter;
	}
	return nullptr;
}

NiExtraData* NiObjectNET::GetExtraData2(const char* name)
{
	return ThisCall<NiExtraData*>(0x006FF9C0, this, name);
}

bool NiObjectNET::AddExtraData(NiExtraData* xData)
{
	return ThisCall<bool>(0xA5BA40, this, xData);
}

void NiObjectNET::SetName(const char* newName)
{
	// uses cdecl, not stdcall
	_asm
	{
		push newName
		// mov ecx, this (already)
		call kNiObjectNET_SetNameAddr
		add esp, 4
	}
	// OBSE : ThisStdCall(kNiObjectNET_SetNameAddr, this, newName);
}

