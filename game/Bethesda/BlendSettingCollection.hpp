#pragma once

#include "INISettingCollection.hpp"

class BlendSettingCollection : public INISettingCollection {
public:
	static BlendSettingCollection* GetSingleton();

	static char* const DEFAULT_sBody;
	static char* const DEFAULT_sHead;
	static char* const DEFAULT_sLCalf;
	static char* const DEFAULT_sLFoot;
	static char* const DEFAULT_sLForeArm;
	static char* const DEFAULT_sLHand;
	static char* const DEFAULT_sLThigh;
	static char* const DEFAULT_sLUpperArm;
	static char* const DEFAULT_sPonyTail;
	static char* const DEFAULT_sQuiver;
	static char* const DEFAULT_sRCalf;
	static char* const DEFAULT_sRFoot;
	static char* const DEFAULT_sRForeArm;
	static char* const DEFAULT_sRHand;
	static char* const DEFAULT_sRThigh;
	static char* const DEFAULT_sRUpperArm;
	static char* const DEFAULT_sShield;
	static char* const DEFAULT_sSpine1;
	static char* const DEFAULT_sSpine2;
	static char* const DEFAULT_sTail;
	static char* const HIT_sBody;
	static char* const HIT_sHead;
	static char* const HIT_sHitSpot;
	static char* const HIT_sHitStart;
	static char* const HIT_sLForeArm;
	static char* const HIT_sLHand;
	static char* const HIT_sLUpperArm;
	static char* const HIT_sRForeArm;
	static char* const HIT_sRHand;
	static char* const HIT_sRUpperArm;
	static char* const HIT_sSpine1;
	static char* const HIT_sSpine2;
	static char* const QUADHIT_sBody;
	static char* const QUADHIT_sHead;
	static char* const QUADHIT_sHitSpot;
	static char* const QUADHIT_sHitStart;
	static char* const QUADHIT_sLCalf;
	static char* const QUADHIT_sLForeArm;
	static char* const QUADHIT_sLThigh;
	static char* const QUADHIT_sLUpperArm;
	static char* const QUADHIT_sRCalf;
	static char* const QUADHIT_sRForeArm;
	static char* const QUADHIT_sRThigh;
	static char* const QUADHIT_sRUpperArm;
	static char* const QUADHIT_sSpine1;
	static char* const QUADHIT_sSpine2;
	static float* const DEFAULT_fHighRot;
	static float* const DEFAULT_fHighTrans;
	static float* const DEFAULT_fLowRot;
	static float* const DEFAULT_fLowTrans;
	static float* const DEFAULT_fPassOutForce;
	static float* const DEFAULT_fPassOutTime;
	static float* const HIT_fBFMax;
	static float* const HIT_fBFMin;
	static float* const HIT_fGetUpTime;
	static float* const HIT_fHFMax;
	static float* const HIT_fHFMin;
	static float* const HIT_fKnockDownTime;
	static float* const HIT_fMinHeirGain;
	static float* const HIT_fMinVelGain;
};