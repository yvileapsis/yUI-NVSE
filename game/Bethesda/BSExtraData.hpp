#pragma once

enum ExtraDataType : UInt8 {
	ED_HAVOK					= 1,
	ED_CELL3D					= 2,
	ED_CELLWATERTYPE			= 3,
	ED_REGIONLIST				= 4,
	ED_SEENDATA					= 5,
	ED_UNK_6					= 6,
	ED_CELLMUSICTYPE			= 7,
	ED_CELLCLIMATE				= 8,
	ED_PROCESSMIDDLELOW			= 9,
	ED_CELLCANOPYSHADOWMASK		= 10,
	ED_DETACHTIME				= 11,
	ED_PERSISTENTCELL			= 12,
	ED_SCRIPT					= 13,
	ED_ACTION					= 14,
	ED_STARTINGPOSITION			= 15,
	ED_ANIM						= 16,
	ED_USEDMARKERS				= 18,
	ED_DISTANTDATA				= 19,
	ED_RAGDOLLDATA				= 20,
	ED_CONTAINERCHANGES			= 21,
	ED_WORN						= 22,
	ED_WORNLEFT					= 23,
	ED_PACKAGESTARTLOCATION		= 24,
	ED_PACKAGE					= 25,
	ED_TRESPASSPACKAGE			= 26,
	ED_RUNONCEPACKS				= 27,
	ED_REFERENCEPOINTER			= 28,
	ED_FOLLOWER					= 29,
	ED_LEVCREAMODIFIER			= 30,
	ED_GHOST					= 31,
	ED_ORIGINALREFERENCE		= 32,
	ED_OWNERSHIP				= 33,
	ED_GLOBAL					= 34,
	ED_RANK						= 35,
	ED_COUNT					= 36,
	ED_HEALTH					= 37,
	ED_USES						= 38,
	ED_TIMELEFT					= 39,
	ED_CHARGE					= 40,
	ED_LIGHT					= 41,
	ED_LOCK						= 42,
	ED_TELEPORT					= 43,
	ED_MAPMARKER				= 44,
	ED_UNK_45					= 45,
	ED_LEVELEDCREATURE			= 46,
	ED_LEVELEDITEM				= 47,
	ED_SCALE					= 48,
	ED_SEED						= 49,
	ED_NONACTORMAGICCASTER		= 50,
	ED_NONACTORMAGICTARGET		= 51,
	ED_UNK_52					= 52,
	ED_PLAYERCRIMELIST			= 53,
	ED_UNK_54					= 54,
	ED_ENABLESTATEPARENT		= 55,
	ED_ENABLESTATECHILDREN		= 56,
	ED_ITEMDROPPER				= 57,
	ED_DROPPEDITEMLIST			= 58,
	ED_RANDOMTELEPORTMARKER		= 59,
	ED_MERCHANTCONTAINER		= 60,
	ED_SAVEDHAVOKDATA			= 61,
	ED_CANNOTWEAR				= 62,
	ED_POISON					= 63,
	ED_UNK_0x40					= 64,	// REFERENCED DURING LOADFORMINMODULE (IN OPOSITION TO LIGHT)
	ED_LASTFINISHEDSEQUENCE		= 65,
	ED_SAVEDANIMATION			= 66,
	ED_NORTHROTATION			= 67,
	ED_XTARGET					= 68,
	ED_FRIENDHITS				= 69,
	ED_HEADINGTARGET			= 70,
	ED_UNK_71					= 71,
	ED_REFRACTIONPROPERTY		= 72,
	ED_STARTINGWORLDORCELL		= 73,
	ED_HOTKEY					= 74,
	ED_UNK_75					= 75,
	ED_EDITORREFMOVEDDATA		= 76,
	ED_INFOGENERALTOPIC			= 77,
	ED_HASNORUMORS				= 78,
	ED_SOUND					= 79,
	ED_TERMINALSTATE			= 80,
	ED_LINKEDREF				= 81,
	ED_LINKEDREFCHILDREN		= 82,
	ED_ACTIVATEREF				= 83,
	ED_ACTIVATEREFCHILDREN		= 84,
	ED_TALKINGACTOR				= 85,
	ED_OBJECTHEALTH				= 86,
	ED_DECALREFS				= 87,
	ED_CELLIMAGESPACE			= 89,
	ED_NAVMESHPORTAL			= 90,
	ED_MODELSWAP				= 91,
	ED_RADIUS					= 92,
	ED_RADIATION				= 93,
	ED_FACTIONCHANGES			= 94,
	ED_DISMEMBEREDLIMBS			= 95,
	ED_MULTIBOUND				= 97,
	ED_MULTIBOUNDDATA			= 98,
	ED_MULTIBOUNDREF			= 99,
	ED_UNK_100					= 100,
	ED_REFLECTEDREFS			= 101,
	ED_REFLECTORREFS			= 102,
	ED_EMITTANCESOURCE			= 103,
	ED_RADIODATA				= 104,
	ED_COMBATSTYLE				= 105,
	ED_PRIMITIVE				= 107,
	ED_OPENCLOSEACTIVATEREF		= 108,
	ED_ANIMNOTERECIEVER			= 109,
	ED_AMMO						= 110,
	ED_PATROLREFDATA			= 111,
	ED_PACKAGEDATA				= 112,
	ED_OCCLUSIONPLANE			= 113,
	ED_COLLISIONDATA			= 114,
	ED_SAYTOPICINFOONCEADAY		= 115,
	ED_ENCOUNTERZONE			= 116,
	ED_SAYTOTOPICINFO			= 117,
	ED_OCCLUSIONPLANEREFDATA	= 118,
	ED_PORTALREFDATA			= 119,
	ED_PORTAL					= 120,
	ED_ROOM						= 121,
	ED_HEALTHPERC				= 122,
	ED_ROOMREFDATA				= 123,
	ED_GUARDEDREFDATA			= 124,
	ED_CREATUREAWAKESOUND		= 125,
	ED_WATERZONEMAP				= 126,
	ED_UNK_127					= 127,
	ED_IGNOREDBYSANDBOX			= 128,
	ED_CELLACOUSTICSPACE		= 129,
	ED_RESERVEDMARKERS			= 130,
	ED_WEAPONIDLESOUND			= 131,
	ED_WATERLIGHTREFS			= 132,
	ED_LITWATERREFS				= 133,
	ED_WEAPONATTACKSOUND		= 134,
	ED_ACTIVATELOOPSOUND		= 135,
	ED_PATROLREFINUSEDATA		= 136,
	ED_ASHPILEREF				= 137,
	ED_CREATUREMOVEMENTSOUND	= 138,
	ED_FOLLOWERSWIMBREADCRUMBS	= 139,
	ED_UNK_140					= 140,
	ED_WEAPONMODFLAGS			= 141,
	ED_UNK_142					= 142,
	ED_UNK_143					= 143,
	ED_UNK_0x90					= 144,
	ED_UNK_0x91					= 145,
	ED_SPECIALRENDERFLAGS		= 146
};

#define EXTRADATATYPE(x) static constexpr UInt8 ExtraDataType = ED_##x;

class BSExtraData {
public:
	BSExtraData();
	virtual			~BSExtraData();
	virtual bool	Differs(BSExtraData* apExtra);

	static BSExtraData* Create(UInt8 ucType, UInt32 uiSize, UInt32 vtbl);

	UInt8			cType;
	BSExtraData*	pNext;
};

class BaseExtraList {
public:
	virtual ~BaseExtraList();

	BSExtraData*	pHead;
	UInt8			cPresenceBitfield[0x15];	// 008 - if a bit is set, then the extralist should contain that extradata
	// bits are numbered starting from the lsb

	bool HasType(UInt32 type) const;

	__forceinline BSExtraData* GetExtraData(UInt8 acType) const {
		return ThisStdCall<BSExtraData*>(0x410220, this, acType);
	}

	template <class T_Data>
	__forceinline T_Data* GetExtraData() const {
		return (T_Data*)GetExtraData(T_Data::ExtraDataType);
	}

	void MarkType(UInt32 type, bool bCleared);

	__forceinline void RemoveExtra(BSExtraData* apExtra, bool abDelete) {
		ThisStdCall(0x410020, this, apExtra, abDelete);
	}

	__forceinline void RemoveExtra(UInt32 acType) {
		ThisStdCall(0x410140, this, acType);
	}

	__forceinline BSExtraData* AddExtra(BSExtraData* apExtra) {
		return ThisStdCall<BSExtraData*>(0x40FF60, this, apExtra);
	}

	__forceinline void RemoveAll(bool abDelete) {
		ThisStdCall(0x40FAE0, this, abDelete);
	}

	bool MarkScriptEvent(UInt32 eventMask, TESForm* eventTarget);

	__forceinline void CopyList(BaseExtraList* apExtra) {
		ThisStdCall(0x411EC0, this, apExtra);
	}

	void DebugDump() const;

	bool IsWorn() const;
};


ASSERT_SIZE(BaseExtraList, 0x20);