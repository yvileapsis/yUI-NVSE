#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"

class TESSound;

// 0x60
class BGSAddonNode :
	public TESBoundObject,
	public TESModelTextureSwap
{
public:
	BGSAddonNode();
	virtual ~BGSAddonNode();

	struct AddonData
	{
		UInt16	usMasterParticleCap;
		Bitfield8 eFlags;
	};

	UInt32		uiIndex;
	TESSound*	pkSound;
	AddonData	kData;
	UInt32		uiMasterParticleLock;
};
static_assert(sizeof(BGSAddonNode) == 0x60);