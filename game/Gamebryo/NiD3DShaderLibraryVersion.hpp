#pragma once

class NiD3DShaderLibraryVersion {
public:
	UInt32 m_uiSystemPixelShaderVersion;
	UInt32 m_uiMinPixelShaderVersion;
	UInt32 m_uiRequestedPixelShaderVersion;
	UInt32 m_uiSystemVertexShaderVersion;
	UInt32 m_uiMinVertexShaderVersion;
	UInt32 m_uiRequestedVertexShaderVersion;
	UInt32 m_uiSystemUserVersion;
	UInt32 m_uiMinUserVersion;
	UInt32 m_uiRequestedUserVersion;
	UInt32 m_uiPlatform;
};

ASSERT_SIZE(NiD3DShaderLibraryVersion, 0x28);