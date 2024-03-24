#pragma once

#include "NiTimeController.hpp"

class NiInterpolator;
class NiBlendInterpolator;

NiSmartPointer(NiInterpController);

class NiInterpController : public NiTimeController {
public:
	NiInterpController();
	virtual ~NiInterpController();

	virtual UInt16					GetInterpolatorCount() const;
	virtual const char*				GetInterpolatorID(UInt16 ausIndex = 0) const;
	virtual UInt16					GetInterpolatorIndex(const char* apID) const;
	virtual UInt16					GetInterpolatorIndexFx(UInt16 ausIndex = 0) const;
	virtual NiInterpolator*			GetInterpolator(UInt16 ausIndex = 0) const;
	virtual void					SetInterpolator(NiInterpolator* apInterpolator, UInt16 ausIndex = 0);
	virtual void					ResetTimeExtrema();
	virtual UInt32					GetCtlrID() const;
	virtual NiInterpolator*			CreatePoseInterpolator(UInt16 ausIndex = 0);
	virtual void					SynchronizePoseInterpolator(NiInterpolator* apInterpolator, UInt16 ausIndex = 0);
	virtual NiBlendInterpolator*	CreateBlendInterpolator(UInt16 ausIndex = 0, bool abManagerControlled = false, bool abAccumulateAnimations = false, float afWeightThreshold = 0.0f, UInt8 aucArraySize = 2);
	virtual void					GuaranteeTimeRange(float afStartTime, float afEndTime);
	virtual bool					InterpolatorIsCorrectType(NiInterpolator* apInterpolator, UInt16 ausIndex = 0) const;

	NIRTTI_ADDRESS(0x11F36B4);
};

ASSERT_SIZE(NiInterpController, 0x34)