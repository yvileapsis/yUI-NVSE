#pragma once
#include "FORenderedMenu.hpp"

class FORenderedMenuRaceSex : public FORenderedMenu
{
public:
	FORenderedMenuRaceSex();
	~FORenderedMenuRaceSex();

	enum RaceMenuType
	{
		kRaceConsole = 0x0,
		kRaceScript = 0x1,
		kBarber = 0x2,
		kPlasticSurgery = 0x3,
	};

	NiNode* unk0D8[4];
	NiNode* buttonLights[4];
	NiNode* unk0F8;
	NiNode* playerNode;
	NiNode* node100;
	NiCamera* camera;
	NiRefObject* PlayerFaceLights01;
	NiNode* playerParentNode;
	NiPoint3 playerNodeTranslate;
	NiMatrix33 playerNodeRotate;
	NiPoint3 bip01Translate;
	UInt8 isPlayerNodeVisible;
	UInt8 gap14D[3];
	UInt32 unk150;
	UInt32 unk154;
	UInt32 unk158;
	UInt32 unk15C;
	UInt32 unk160;
	UInt32 unk164;
	UInt32 unk168;
	RaceMenuType menuType;
};
static_assert(sizeof(FORenderedMenuRaceSex) == 0x170);