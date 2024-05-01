#pragma once

#include "BSString.hpp"
#include "NiTriShape.hpp"
#include "SkyObject.hpp"

NiSmartPointer(Moon);

class Moon : public SkyObject {
public:
	virtual void Refresh(NiNode* niNode, const char* moonStr);

	enum UpdateStatus {
		US_NOT_REQUIRED = 0x0,
		US_WHEN_CULLED = 0x1,
		US_INITIALIZE = 0x2,
	};

	enum Phase {
		PH_FULL = 0x0,
		PH_3Q_WAN = 0x1,
		PH_HALF_WAN = 0x2,
		PH_1Q_WAN = 0x3,
		PH_NEW = 0x4,
		PH_1Q_WAX = 0x5,
		PH_HALF_WAX = 0x6,
		PH_3Q_WAX = 0x7,
		NUM_PHASES = 0x8,
	};

	NiNodePtr		spMoonNode;
	NiNodePtr		spShadowNode;
	NiTriShapePtr	spMoonMesh;
	NiTriShapePtr	spShadowMesh;
	BSStringT		strTextures[NUM_PHASES];
	float			fAngleFadeStart;
	float			fAngleFadeEnd;
	float			fAngleShadowEarlyFade;
	float			fSpeed;
	float			fZOffset;
	UInt32			uiSize;
	UpdateStatus	eUpdateMoonTexture;
	float			fAngle;
	float			fLastUpdateHour;
};

ASSERT_SIZE(Moon, 0x7C)