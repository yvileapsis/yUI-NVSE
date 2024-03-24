#pragma once

#include "NiObject.hpp"

class ImageSpaceModifierInstance : public NiObject {
public:
	virtual bool	IsExpired() const;
	virtual void	Apply();
	virtual bool	IsForm() const;
	virtual void	PrintInfo(const char* apBuffer) const;

	enum Flags {
		PERMANENT = 1,
	};

	bool			bIsHidden;
	float			fStrength;
	NiObjectPtr		spTarget;
	float			fAge;
	Bitfield32		uiFlags;

	NiNewRTTI(ImageSpaceModifierInstance, NiObject);

	void SetPermanent(bool abPermanent) { uiFlags.SetBit(PERMANENT, abPermanent); }
	bool IsPermanent() const { return uiFlags.IsSet(PERMANENT); }

	void SetHidden(bool abHidden) { bIsHidden = abHidden; }
	bool IsHidden() const { return bIsHidden; }

	void SetStrength(float afStrength) { fStrength = afStrength; }
	float GetStrength() const { return fStrength; }

	void SetTarget(NiObject* apTarget) { spTarget = apTarget; }
	NiObject* GetTarget() const { return spTarget; }

	void SetAge(float afAge) { fAge = afAge; }
	float GetAge() const { return fAge; }
};

ASSERT_SIZE(ImageSpaceModifierInstance, 0x1C);