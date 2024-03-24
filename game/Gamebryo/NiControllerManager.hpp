#pragma once

#include "NiTimeController.hpp"
#include "NiTArray.hpp"
#include "NiTStringMap.hpp"
#include "NiControllerSequence.hpp"
#include "NiDefaultAVObjectPalette.hpp"
#include "NiTSet.hpp"
#include "BSAnimNoteListener.hpp"

class NiControllerManager : public NiTimeController {
public:
	NiTObjectArray<NiPointer<NiControllerSequence>>		m_kSequenceArray;
	NiTPrimitiveSet<NiControllerSequence*>				m_kActiveSequences;
	NiTStringPointerMap<NiControllerSequence*>			m_kIndexMap;
	BSAnimNoteListener*									pListener;
	bool												m_bCumulative;
	NiTObjectSet<NiPointer<NiControllerSequence> >		m_kTempBlendSeqs;
	NiDefaultAVObjectPalettePtr							m_spObjectPalette;

	NIRTTI_ADDRESS(0x11F36AC);

	NiControllerSequence* LookupSequence(NiFixedString& akName);
	NiControllerSequence* LookupSequence(const char* apName);

	void ActivateSequence(NiControllerSequence* apSequence, char acPriority, bool abStartOver, float afWeight, float afEaseInTime, NiControllerSequence* apTimeSyncSeq);
	bool DeactivateSequence(NiControllerSequence* apSequence, float afEaseOutTime);
};

ASSERT_SIZE(NiControllerManager, 0x7C);