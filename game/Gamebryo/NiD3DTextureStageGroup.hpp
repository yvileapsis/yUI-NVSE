#pragma once

class NiD3DTextureStageGroup {
public:
	bool	m_bRendererOwned;
	UInt32	m_uiNumSavedStages;
	UInt32	m_uiSavedStageArrayIter;
	UInt32	m_auiSavedStageArray[8];
	bool	m_abSavedStageValid[8];
	UInt32	m_uiNumStages;
	UInt32	m_uiStageArrayIter;
	UInt32	m_auiStageArray[8];
	bool	m_abStageValid[8];
	UInt32	m_uiNumSavedSamplers;
	UInt32	m_uiSavedSamplerArrayIter;
	UInt32	m_auiSavedSamplerArray[5];
	bool	m_abSavedSamplerValid[5];
	UInt32	m_uiNumSamplers;
	UInt32	m_uiSamplerArrayIter;
	UInt32	m_auiSamplerArray[5];
	bool	m_abSamplerValid[5];
	UInt32	m_uiNumUseMapValue;
	bool	m_abSamplerUseMapValue[5];

	UInt32 SetTextureStage(UInt32 uiStage);
	bool GetStageState(UInt32 uiState, UInt32* uiValue, bool* bSave);
};