#pragma once

#include "NiObject.hpp"
#include "NiFixedString.hpp"
#include "BSSimpleArray.hpp"

class hkConstraintCinfo;

class bhkRagdollTemplateData : public NiObject {
public:
	NiFixedString						strName;
	BSSimpleArray<hkConstraintCinfo*>	kConstraintInfos;
	BSSimpleArray<UInt32>				kConstraintTypes;
	float								fMass;
	float								fRestitution;
	float								fFriction;
	float								fRadius;
	float								fMaterial;
};

ASSERT_SIZE(bhkRagdollTemplateData, 0x40);