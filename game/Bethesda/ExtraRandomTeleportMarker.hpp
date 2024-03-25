#pragma once
#include "BSExtraData.hpp"

class TESObjectREFR;

class ExtraRandomTeleportMarker : public BSExtraData {
public:
	ExtraRandomTeleportMarker();
	~ExtraRandomTeleportMarker();

	TESObjectREFR* teleportRef;
};
static_assert(sizeof(ExtraRandomTeleportMarker) == 0x10);