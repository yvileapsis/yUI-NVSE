#pragma once

#include "TESObject.hpp"

class NiNode;

class TESBoundObject : public TESObject {
public:
	TESBoundObject();
	~TESBoundObject();

	virtual NiNode* Clone3D(TESObjectREFR* apRequester);	// calls Fn53, for NPC calls ReadBones, for LevelledActor level them if necessary
	virtual bool	ReplaceModel(const char* apPath);

	struct Bounds
	{
		SInt16 x;
		SInt16 y;
		SInt16 z;
	};

	struct BOUND_DATA
	{
		Bounds BoundMin;
		Bounds BoundMax;
	};

	BOUND_DATA BoundData;	// 024
};

ASSERT_SIZE(TESBoundObject, 0x30)