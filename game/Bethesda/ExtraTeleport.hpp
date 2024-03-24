#pragma once

#include "BSExtraData.hpp"

class TESObjectREFR;

class ExtraTeleport : public BSExtraData {
public:
	ExtraTeleport();
	~ExtraTeleport();

	struct Data
	{
		TESObjectREFR* linkedDoor;	// 00
		float			x;			// 04 x, y, z, zRot refer to teleport marker's position and rotation
		float			y;
		float			z;
		float			xRot;		// 10 angles in radians. x generally 0
		float			yRot;		// 14 y generally -0.0, no reason to modify
		float			zRot;		// 18
		UInt8			unk01C;		// 1C
		UInt8			pad01D[3];	// 1D
	};

	Data* data;

	static ExtraTeleport* Create();
};