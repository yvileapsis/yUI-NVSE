#pragma once

#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"

class TESSound;

class BGSAddonNode : public TESBoundObject, public TESModelTextureSwap {
public:
	BGSAddonNode();
	virtual ~BGSAddonNode();

	struct AddonData {
		UInt16		usMasterParticleCap;
		Bitfield8	ucFlags;
	};

	UInt32		uiIndex;
	TESSound*	pSound;
	AddonData	kData;
	UInt32		uiMasterParticleLock;
};

ASSERT_SIZE(BGSAddonNode, 0x60);