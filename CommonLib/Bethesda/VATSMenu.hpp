#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"
#include "NiPoint2.hpp"

struct VATSTargetInfo
{
	enum ActionTypes
	{
		kAPCost_AttackUnarmed = 0x0,
		kAPCost_AttackOneHandMelee = 0x1,
		kAPCost_AttackTwoHandMelee = 0x2,
		kAPCost_AttackPistol = 0x3,
		kAPCost_AttackRifle = 0x4,
		kAPCost_AttackHandle = 0x5,
		kAPCost_AttackLauncher = 0x6,
		kAPCost_AttackGrenade = 0x7,
		kAPCost_AttackMine = 0x8,
		kAPCost_Reload = 0x9,
		kAPCost_Crouch = 0xA,
		kAPCost_Stand = 0xB,
		kAPCost_SwitchWeapon = 0xC,
		kAPCost_ToggleWeaponDrawn = 0xD,
		kAPCost_Heal = 0xE,
		kAPCost_OneHandThrown = 0x13,
		kAPCost_AttackThrown = 0x14,
		kAPCost_UnarmedAttackGround = 0x15,
		kAPCost_MAX = 0x16,
	};

	int actionType;
	UInt8 isSuccess;
	UInt8 byte05;
	UInt8 isMysteriousStrangerVisit;
	UInt8 byte07;
	UInt8 remainingShotsToFire;
	UInt8 count09;
	UInt8 gap0A[2];
	TESObjectREFR* ref;
	UInt32 avCode;
	ActorHitData* hitData;
	Float32 unk18;
	Float32 unk1C;
	Float32 apCost;
	UInt8 isMissFortuneVisit;
	UInt8 gap25[3];
};
static_assert(sizeof(VATSTargetInfo) == 0x28);


struct VATSTargetBodyPartData
{
	NiPoint2	screenPos;
	NiPoint3	relativePos;
	NiPoint3	pos;
	UInt32		bodyPartID;
	Float32		percentVisible;
	Float32		hitChance;
	UInt8		isOnScreen;
	UInt8		byte2D;
	UInt8		shouldCalculateBodyPartVisibilityPercents;
	UInt8		byte2F;
	Tile*		tile_body_part_percent;
	Float32		unk34;
	UInt8		byte38;
	UInt8		gap38[3];
};

// 0x144
class VATSMenu : public Menu
{
	VATSMenu();
	~VATSMenu();
public:
	enum Buttons
	{
		kVATS_queuedActions			= 0x1,
		kVATS_leftArrow				= 0x2,
		kVATS_rightArrow			= 0x3,
		kVATS_acceptButton			= 0x4,
		kVATS_returnButton			= 0x5,
		kVATS_bodyPartButton		= 0x6,
		kVATS_selectButton			= 0x7,
		kVATS_specialAttackButton	= 0x1E,
		kVATS_specialAttack2Button	= 0x1F,
	};

	UInt32		unk028;

	union
	{
		Tile*	pkTiles[31];
		struct
		{
			TileImage*	pkTile02C;
			TileImage*	pkTile030;
			TileImage*	pkTile034;
			TileImage*	pkTile038;
			TileImage*	pkTile03C;
			TileImage*	pkTile040;
			TileImage*	pkTile044;
			TileImage*	pkTile048;
			TileImage*	pkTile04C;
			TileImage*	pkTile050;
			TileText*	pkTile054;
			TileText*	pkTile058;
			TileText*	pkTile05C;
			TileText*	pkTile060;
			TileImage*	pkTile064;
			TileImage*	pkTile068;
			TileImage*	pkTile06C;
			TileImage*	pkTile070;
			TileText*	pkTile074;
			TileRect*	pkTile078;
			TileRect*	pkTile07C;
			TileRect*	pkTile080;
			TileImage*	pkTile084;
			TileRect*	pkTile088;
			TileImage*	pkTile08C;
			TileImage*	pkTile090;
			TileImage*	pkTile094;
			TileImage*	pkTile098;
			TileText*	pkTile09C;
			TileImage*	pkTile0A0;
			TileImage*	pkTile0A4;
		};
	};

	UInt32			unk0A8[2];
	ListBox<UInt32>	kQueuedActions;
	Float32			fActionPoints;
	Float32			fMaxActionPoints;
	UInt32			unk0E8;
	UInt32			unk0EC;
	Float32			fClipAmmo;
	Float32			fReserveAmmo;
	UInt8			bIsFullyZoomedIn;
	UInt8			unk0F9;
	UInt8			unk0FA;
	UInt8			unk0FB;
	UInt32			unk0FC;
	VATSTargetBodyPartData* currentBodyPartData;
	UInt8			hasConcentratedFire;
	UInt8			gap105[3];
	UInt32			unk108;
	UInt8			byte10C;
	UInt8			byte10D;
	UInt8			byte10E;
	UInt8			byte10F;
	UInt8			byte110;
	UInt8			pad111;
	UInt8			pad112;
	UInt8			pad113;
	UInt32			time114;
	UInt8			byte118;
	UInt8			pad119[3];
	VATSTargetInfo	kTargetInfo;

	static VATSMenu* GetSingleton() { return *reinterpret_cast<VATSMenu**>(0x11DB0D4); };
	static TESObjectREFR* GetTarget() { return *reinterpret_cast<TESObjectREFR**>(0x11F21CC); };
};
static_assert(sizeof(VATSMenu) == 0x144);
