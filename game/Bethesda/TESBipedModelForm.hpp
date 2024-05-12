#pragma once

#include "BaseFormComponent.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESModelRDT.hpp"
#include "BGSMessageIcon.hpp"

class TESBipedModelForm : public BaseFormComponent {
public:
	TESBipedModelForm();
	~TESBipedModelForm();

	// bit indices starting from lsb
	enum EPartBit {
		ePart_Head = 0,
		ePart_Hair,
		ePart_UpperBody,
		ePart_LeftHand,
		ePart_RightHand,
		ePart_Weapon,
		ePart_PipBoy,
		ePart_Backpack,
		ePart_Necklace,
		ePart_Headband,
		ePart_Hat,
		ePart_Eyeglasses,
		ePart_Nosering,
		ePart_Earrings,
		ePart_Mask,
		ePart_Choker,
		ePart_MouthObject,
		ePart_BodyAddon1,
		ePart_BodyAddon2,
		ePart_BodyAddon3
	};

	enum EPartBitMask {
		ePartBitMask_Full = 0x07FFFF,
	};

	enum ESlot {
		eSlot_Head = 0x1 << ePart_Head,
		eSlot_Hair = 0x1 << ePart_Hair,
		eSlot_UpperBody = 0x1 << ePart_UpperBody,
		eSlot_LeftHand = 0x1 << ePart_LeftHand,
		eSlot_RightHand = 0x1 << ePart_RightHand,
		eSlot_Weapon = 0x1 << ePart_Weapon,
		eSlot_PipBoy = 0x1 << ePart_PipBoy,
		eSlot_Backpack = 0x1 << ePart_Backpack,
		eSlot_Necklace = 0x1 << ePart_Necklace,
		eSlot_Headband = 0x1 << ePart_Headband,
		eSlot_Hat = 0x1 << ePart_Hat,
		eSlot_Eyeglasses = 0x1 << ePart_Eyeglasses,
		eSlot_Nosering = 0x1 << ePart_Nosering,
		eSlot_Earrings = 0x1 << ePart_Earrings,
		eSlot_Mask = 0x1 << ePart_Mask,
		eSlot_Choker = 0x1 << ePart_Choker,
		eSlot_MouthObject = 0x1 << ePart_MouthObject,
		eSlot_BodyAddon1 = 0x1 << ePart_BodyAddon1,
		eSlot_BodyAddon2 = 0x1 << ePart_BodyAddon2,
		eSlot_BodyAddon3 = 0x1 << ePart_BodyAddon3,

		// The first 20 bits are on (nothing else is used).
		eSlot_Full = 0xFFFFF
	};

	enum EBipedFlags {
		eBipedFlag_HasBackPack = 0x4,
		eBipedFlag_MediumArmor = 0x8,
		eBipedFlag_PowerArmor = 0x20,
		eBipedFlag_NonPlayable = 0x40,
		eBipedFlag_HeavyArmor = 0x80,
	};

	enum EBipedPath {
		ePath_Biped,
		ePath_Ground,
		ePath_Icon,
		ePath_Max
	};

	// missing part mask and flags
	UInt32					partMask;			// 004
	UInt8					bipedFlags;			// 008
	UInt8					fil009[3];			// 009 seen badly initialized (ArmorCombat)
	TESModelTextureSwap		bipedModel[2];		// 00C
	TESModelTextureSwap		groundModel[2];		// 04C
	TESIcon					icon[2];			// 08C
	BGSMessageIcon			messageIcon[2];		// 0A4
	TESModelRDT				modelRDT;			// 0C4
	// 0DC

	static UInt32 MaskForSlot(UInt32 mask);

	bool IsPowerArmor() const { return (bipedFlags & eBipedFlag_PowerArmor) == eBipedFlag_PowerArmor; }
	bool IsNonPlayable() const { return (bipedFlags & eBipedFlag_NonPlayable) == eBipedFlag_NonPlayable; }
	bool IsPlayable() const { return !IsNonPlayable(); }
	void SetPowerArmor(bool bPA) {
		if (bPA) {
			bipedFlags |= eBipedFlag_PowerArmor;
		}
		else {
			bipedFlags &= ~eBipedFlag_PowerArmor;
		}
	}
	void SetNonPlayable(bool bNP) {
		if (bNP) {
			bipedFlags |= eBipedFlag_NonPlayable;
		}
		else {
			bipedFlags &= ~eBipedFlag_NonPlayable;
		}
	}
	void  SetPath(const char* newPath, UInt32 whichPath, bool bfemalePath);
	const char* GetPath(UInt32 whichPath, bool bFemalePath);

	UInt32 GetSlotsMask() const;
	void SetSlotsMask(UInt32 mask);	// Limited by ePartBitMask_Full

	UInt32 GetBipedMask() const;
	void SetBipedMask(UInt32 mask);
};

ASSERT_SIZE(TESBipedModelForm, 0x0DC);