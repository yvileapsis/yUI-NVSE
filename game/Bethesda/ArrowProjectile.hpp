#pragma once
#include "MobileObject.hpp"

// 0xC8
class ArrowProjectile : public MobileObject
{
	enum EnumStage
	{
		kArrow_MarkedForDelete = 0x3,
	};

	struct CollisionData
	{
		UInt32		type;
		NiPoint3	pt04;
		NiPoint3	pt10;
		NiPoint3	pt1C;
		TESObjectREFR*	refr;
		NiNode*		node2C;
		NiMatrix33	matrix30;
	};

	CollisionData*	pkCollisionData;
	EnumStage		eStage;
	Float32			unk90;
	Float32			fElapsedTime;
	Float32			fSpeed;
	Float32			unk9C;
	Float32			unkA0;
	Actor*			pkShooter;
	EnchantmentItem* pkArrowEnchantment;
	UInt32			unkAC;
	AlchemyItem*	pkPoison;
	NiPoint3		ptB4;
	UInt8			byteC0;
	UInt8			byteC1;
	UInt8			byteC2;
	UInt8			byteC3;
	UInt32			unkC4;
};
static_assert(sizeof(ArrowProjectile) == 0xC8);