#pragma once

#include "MiddleLowProcess.hpp"
#include "NiPoint3.hpp"

class QueuedFile;
class NiNode;
class MagicItem;
class BSFaceGenAnimationData;
class MagicTarget;
class AnimData;
class BSAnimGroupSequence;
class BSBound;
class BSFaceGenNiNode;
class NiTriShape;

struct PackageInfo {
	TESPackage*		pPackage;
	TESPackageData* pPackageData;
	TESObjectREFR*	pTargetRef;
	UInt32			unk0C;
	float			unk10;
	UInt32			uiFlags;
};


class MiddleHighProcess : public MiddleLowProcess {
public:
	MiddleHighProcess();
	~MiddleHighProcess();

	struct	Unk138 {
		UInt32	unk000;
		UInt32	unk004;		// Semaphore
		// ... ?
	};

	struct	Unk148 {
		UInt32	unk000;
		UInt32	unk004;
		UInt32	unk008;
		UInt16	unk00C;
		UInt8	unk00E;
		UInt8	fil00F;
	};

	virtual void	SetAnimation(UInt32 newAnimation);
	virtual void	Unk_209();
	virtual void	Unk_20A();
	virtual void	Unk_20B();
	virtual void	Unk_20C();
	virtual void	Unk_20D();
	virtual void	Unk_20E();
	virtual void	Unk_20F();
	virtual void	Unk_210();
	virtual void	Unk_211();
	virtual void	Unk_212();
	virtual void	Unk_213();
	virtual void	Unk_214();
	virtual void	Unk_215();
	virtual void	Unk_216();
	virtual void	Unk_217();
	virtual void	Unk_218();
	virtual void	Unk_219();
	virtual void	Unk_21A();
	virtual void	Unk_21B();

	BSSimpleList<TESForm*>				unk0C8;				// 0C8
	BSSimpleList<UInt32>				unk0D0;				// 0D0
	UInt32								unk0D8[3];			// 0D8
	PackageInfo							interruptPackage;	// 0E4
	UInt8								unk0FC[12];			// 0FC	Saved as one, might be Pos/Rot given size
	UInt32								unk108;				// 108
	TESIdleForm* idleForm10C;		// 10C
	UInt32								unk110;				// 110  EntryData, also handled as part of weapon code. AmmoInfo.
	WeaponInfo* weaponInfo;		// 114
	AmmoInfo* ammoInfo;			// 118
	QueuedFile* unk11C;			// 11C
	UInt8								byt120;				// 120
	UInt8								byt121;				// 121
	UInt8								byt122;				// 122
	UInt8								fil123;				// 123
	UInt8								isUsingOneHandGrenade;
	UInt8								isUsingOneHandMine;
	UInt8								isUsingOneHandThrown;
	UInt8								isWearingHeavyArmor;
	UInt8								isWearingPowerArmorTorso;
	UInt8								isWearingPowerArmorHelmet;
	UInt8								isWearingBackpack;
	UInt8								gap12B;
	NiNode* node12C;			// 12C
	NiNode* projectileNode;	// 130
	UInt8								byt134;				// 134
	bool								isWeaponOut;		// 135
	UInt8								byt136;				// 136
	UInt8								byt137;				// 137
	bhkCharacterController* charCtrl;			// 138
	UInt8								knockedState;		// 13C
	UInt8								unk13D[3];			// 13D
	UInt32								unk140[8];			// 140
	MagicItem* magicItem160;		// 160
	UInt32								unk164[3];			// 164
	float								actorAlpha;			// 170
	UInt32								unk174;				// 174
	BSFaceGenAnimationData* unk178;			// 178
	UInt8								byte17C;			// 17C
	UInt8								byte17D;			// 17D
	UInt8								byte17E;			// 17E
	UInt8								byte17F;			// 17F
	UInt32								unk180[3];			// 180
	UInt8								byte18C;			// 18C
	UInt8								byte18D[3];			// 18D
	UInt32								unk190[10];			// 190
	void* unk1B8;			// 1B8
	MagicTarget* magicTarget1BC;	// 1BC
	AnimData* animData;			// 1C0
	BSAnimGroupSequence* animSequence[3];	// 1C4
	float								unk1D0;				// 1D0
	float								unk1D4;				// 1D4
	UInt8								byte1D8;			// 1D8
	UInt8								byte1D9;			// 1D9
	UInt8								gap1DA[2];			// 1DA
	NiNode* limbNodes[15];		// 1DC
	NiNode* unk218;			// 218
	NiNode* weaponNode;		// 21C
	void* ptr220;			// 220
	BSBound* boundingBox;		// 224
	UInt8								isAiming;			// 228
	UInt8								byte229;			// 229
	UInt16								byte22A;			// 22A
	UInt32								unk22C;				// 22C
	BSSimpleList<void*>					queuedEquipItems;	// 230
	float								rads238;			// 238
	float								waterRadsSec;		// 23C
	ActorHitData* lastHitData;		// 240 - last damage taken by actor
	UInt32								unk244;				// 244
	BSFaceGenNiNode* unk248;			// 248
	BSFaceGenNiNode* unk24C;			// 24C
	NiTriShape* unk250;			// 250
	ActorHitData* lastTargetHitData; // 254 - last damage dealt by actor
	UInt32								unk258;				// 258
};

ASSERT_SIZE(MiddleHighProcess, 0x25C);