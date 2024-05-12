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
class ItemChange;

class MiddleHighProcess : public MiddleLowProcess
{
public:

	struct PackageInfo {
		TESPackage*		pPackage;
		TESPackageData* pPackageData;
		TESObjectREFR*	pTargetRef;
		UInt32			unk0C;
		float			unk10;
		UInt32			uiFlags;
	};

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

	MiddleHighProcess();
	~MiddleHighProcess() override;
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
	TESIdleForm*						idleForm10C;		// 10C
	UInt32								unk110;				// 110  EntryData, also handled as part of weapon code. AmmoInfo.
	ItemChange*							weaponInfo;			// 114
	ItemChange*							ammoInfo;			// 118
	QueuedFile*							unk11C;				// 11C
	NiRefObject*						niRefObject;		// 120
	UInt8								isUsingOneHandGrenade;
	UInt8								isUsingOneHandMine;
	UInt8								isUsingOneHandThrown;
	UInt8								isWearingHeavyArmor;
	UInt8								isWearingPowerArmorTorso;
	UInt8								isWearingPowerArmorHelmet;
	UInt8								isWearingBackpack;
	UInt8								gap12B;
	NiNode*								node12C;			// 12C
	NiNode*								projectileNode;		// 130
	UInt8								byt134;				// 134
	bool								isWeaponOut;		// 135
	UInt8								byt136;				// 136
	UInt8								byt137;				// 137
	bhkCharacterController*				charCtrl;			// 138
	UInt8								knockedState;		// 13C
	UInt8								unk13D[3];			// 13D
	UInt32								unk140[8];			// 140
	MagicItem*							magicItem160;		// 160
	UInt32								unk164[3];			// 164
	Float32								actorAlpha;			// 170
	UInt32								unk174;				// 174
	BSFaceGenAnimationData*				unk178;				// 178
	UInt8								byte17C;			// 17C
	UInt8								byte17D;			// 17D
	UInt8								byte17E;			// 17E
	UInt8								byte17F;			// 17F
	UInt8								byte180;			// 180
	UInt8								byte181;			// 181
	UInt8								byte182;			// 182
	UInt8								byte183;			// 183
	UInt32								unk184;				// 184
	UInt8								hasCaughtPCPickpocketting;	// 188
	UInt8								byte189;			// 189
	UInt8								byte18A;			// 18A
	UInt8								byte18B;			// 18B
	UInt8								byte18C;			// 18C
	UInt8								forceNextUpdate;	// 18D
	UInt8								pad18E[2];			// 18E
	UInt32								unk190[10];			// 190
	void*								unk1B8;				// 1B8
	MagicTarget*						magicTarget1BC;		// 1BC
	AnimData*							animData;			// 1C0
	BSAnimGroupSequence*				animSequence[3];	// 1C4
	Float32								angle1D0;			// 1D0
	Float32								unk1D4;				// 1D4
	UInt8								byte1D8;			// 1D8
	UInt8								usingAutoWeapon;	// 1D9
	UInt8								isAttacked;			// 1DA
	UInt8								byte1DB;			// 1DB
	NiNode*								limbNodes[15];		// 1DC
	NiNode*								unk218;				// 218
	NiNode*								weaponNode;			// 21C
	void*								ptr220;				// 220
	BSBound*							boundingBox;		// 224
	UInt8								isAiming;			// 228
	UInt8								byte229;			// 229
	UInt16								byte22A;			// 22A
	UInt32								unk22C;				// 22C
	BSSimpleList<ItemChange*>			queuedEquipItems;	// 230
	Float32								rads238;			// 238
	Float32								waterRadsSec;		// 23C
	ActorHitData*						lastHitData;		// 240 - last damage taken by actor
	UInt32								unk244;				// 244
	BSFaceGenNiNode*					unk248;				// 248
	BSFaceGenNiNode*					unk24C;				// 24C
	NiTriShape*							unk250;				// 250
	ActorHitData*						lastTargetHitData;	// 254 - last damage dealt by actor
	UInt32								unk258;				// 258
};
static_assert(sizeof(MiddleHighProcess) == 0x25C);