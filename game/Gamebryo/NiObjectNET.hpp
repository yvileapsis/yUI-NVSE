#pragma once

#include "NiObject.hpp"
#include "NiFixedString.hpp"

class NiTimeController;
class NiExtraData;

class NiObjectNET : public NiObject {
public:
	NiObjectNET();
	virtual ~NiObjectNET();

	enum CopyType {
		COPY_NONE = 0x0,
		COPY_EXACT = 0x1,
		COPY_UNIQUE = 0x2,
	};

	NiFixedString					m_kName;
	NiPointer<NiTimeController>		m_spControllers;
	NiExtraData**					m_ppkExtra;
	UInt16							m_usExtraDataSize;
	UInt16							m_usMaxSize;

	const char* GetName() const { return m_kName.m_kHandle; };
	void SetName(const char* apString) { m_kName = apString; };
	void SetName(NiFixedString& arString) { m_kName = arString;	};

	NiTimeController* GetControllers() const { return m_spControllers; };
	NiTimeController* GetController(const NiRTTI* apRTTI) const;
	template <class ControllerType>
	ControllerType* GetController() const {
		return (ControllerType*)GetController(ControllerType::ms_RTTI);
	}
	NiExtraData* GetExtraData(const NiFixedString& kKey) const;
	NiExtraData* GetExtraData(const char* pcKey) const;
	bool AddExtraData(NiExtraData* apExtraData);
};

ASSERT_SIZE(NiObjectNET, 0x18);