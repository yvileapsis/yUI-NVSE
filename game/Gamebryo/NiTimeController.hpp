#pragma once

#include "NiObject.hpp"
#include "NiUpdateData.hpp"

class NiObjectNET;
class NiAVObject;

class NiTimeController : public NiObject {
public:
	NiTimeController();
	virtual ~NiTimeController();

	virtual void				Start(float afTime = INFINITY);			// 35
	virtual void				Stop();									// 36
	virtual void				Update(NiUpdateData& arUpdateData);		// 37
	virtual void				SetTarget(NiObjectNET* apTarget);		// 38
	virtual bool				IsTransformController() const;			// 39
	virtual bool				IsVertexController() const;				// 40
	virtual float				ComputeScaledTime(float fTime);			// 41
	virtual void				OnPreDisplay() const;					// 42
	virtual bool				IsStreamable() const;					// 43
	virtual bool				TargetIsRequiredType() const;			// 44

	enum AnimType {
		APP_TIME,
		APP_INIT
	};

	enum CycleType {
		LOOP,
		REVERSE,
		CLAMP,
		MAX_CYCLE_TYPES
	};

	Bitfield16						m_usFlags;
	float							m_fFrequency;
	float							m_fPhase;
	float							m_fLoKeyTime;
	float							m_fHiKeyTime;
	float							m_fStartTime;
	float							m_fLastTime;
	float							m_fWeightedLastTime;
	float							m_fScaledTime;
	NiObjectNET*					m_pkTarget;
	NiPointer<NiTimeController>		m_spNext;

	NIRTTI_ADDRESS(0x11F49E4);

	NiTimeController* GetNext() const { return m_spNext; }
	void SetNext(NiTimeController* apNext) { m_spNext = apNext; }
	static void StartAnimations(NiAVObject* apObject);
};