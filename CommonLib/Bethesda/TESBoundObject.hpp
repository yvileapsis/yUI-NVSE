#pragma once
#include "TESObject.hpp"

class NiNode;

// 0x30
class TESBoundObject : public TESObject
{
public:
	TESBoundObject();
	~TESBoundObject() override;

	virtual NiNode* Clone3D_2(TESObjectREFR* apRequester);	// calls Fn53, for NPC calls ReadBones, for LevelledActor level them if necessary
	virtual bool	ReplaceModel(const char* apPath);

	struct Bounds
	{
		SInt16 x;
		SInt16 y;
		SInt16 z;
	};

	Bounds BoundMin;	// 024
	Bounds BoundMax;	// 027
};
static_assert(sizeof(TESBoundObject) == 0x30);