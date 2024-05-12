#pragma once

#include "NiD3DShaderDeclaration.hpp"

NiSmartPointer(NiDX9ShaderDeclaration);

class NiDX9ShaderDeclaration : public NiD3DShaderDeclaration {
public:
	NiDX9ShaderDeclaration();
	virtual ~NiDX9ShaderDeclaration();

	D3DVERTEXELEMENT9*				m_pkElements;
	LPDIRECT3DVERTEXDECLARATION9	m_hVertexDecl;
	bool							m_bSoftwareVB;
	D3DDECLTYPE						ms_aeTypes[18];
	D3DDECLMETHOD					ms_aeMethods[7];
	D3DDECLUSAGE					ms_aeUsage[14];

	static NiDX9ShaderDeclaration* Create(NiDX9Renderer* apRenderer, UInt32 auiMaxStreamEntryCount, UInt32 auiStreamCount);
	UInt32 AddDeclarationEntry(NiShaderDeclaration::ShaderRegisterEntry* apEntry, UInt32 auiStream);
};

ASSERT_SIZE(NiDX9ShaderDeclaration, 0x0D4);