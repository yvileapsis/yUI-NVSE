#pragma once

#include "Menu.hpp"
#include "BSSimpleArray.hpp"
#include "NiPoint3.hpp"
#include "BSSoundHandle.hpp"
#include "BSTimer.hpp"
#include "HotKeyWheel.hpp"

class TileImage;
class TileText;
class TileRect;
class Tile3D;
class TESObjectREFR;
class NiControllerSequence;
class Actor;

class HUDMainMenu : public Menu
{
public:
	HUDMainMenu();
	virtual ~HUDMainMenu();

	struct QueuedMessage
	{
		char	msgText[0x204];			// 000
		char	iconPate[MAX_PATH];		// 204
		char	soundPath[MAX_PATH];	// 308
		float	displayTime;			// 40C
	};

	struct QueuedQuestText
	{
		char msg[0x104];
		bool byte104;
		bool byte105;
	};

	struct SubtitleData
	{
		char text[MAX_PATH];
		UInt32 startTime;
		float displayTime;
		NiPoint3 pos;
		UInt32 priority;
	};

	enum VisibilityFlags
	{
		kActionPoints = 0x1,
		kHitPoints = 0x2,
		kRadiationMeter = 0x4,
		kEnemyHealth = 0x8,
		kQuestReminder = 0x10,
		kRegionLocation = 0x20,
		kReticleCenter = 0x40,
		kSneakMeter = 0x80,
		kMessages = 0x100,
		kInfo = 0x200,
		kSubtitles = 0x400,
		kHotkeys = 0x800,
		kXpMeter = 0x1000,
		kBreathMeter = 0x2000,
		kExplosivePositioning = 0x4000,
		kCrippledLimbIndicator = 0x8000,
		kHardcoreMode = 0x10000,
	};

	enum HUDStates
	{
		kRECALCULATE = 0x1,
		kNormal,
		kPipBoy,
		kPause,
		kLoading,
		kDialog,
		kVATSMenu,
		kVATSPlayback,
		kContainer,
		kBeginSit,
		kSleepWait,
		kPlayerDisabledControls,
		kUnusedD,
		kLevelUpMenu,
		kHacking,
		kComputers,
		kMessage,
		kSpecialBook,
		kLoveTester,
		kVanityCam,
		kUnused15,
		kAiming,
		kAimingScope,
		kIntroMovie,
		kGambling,
	};

	enum MeterAlign
	{
		kHUDHealthAlign_Left = 0x0,
		kHUDHealthAlign_Right = 0x1,
		kHUDHealthAlign_Centered = 0x2,
	};

	UInt32							unk028;
	union
	{
		Tile* tiles[0x56];
		struct
		{
			TileImage* hitPointsMeter;// 02C	HitPoints\meter
			TileText* tile030;		// 030	HitPoints\justify_right_text
			TileRect* tile034;		// 034	HitPoints\compass_window\compass_icon_group
			TileRect* tile038;		// 038	HitPoints\compass_window\compass_icon_group
			TileRect* tile03C;		// 03C	HitPoints\compass_window\compass_icon_group
			TileImage* compassWindow;// 040	HitPoints\compass_window
			TileImage* actionPointsMeter;		// 044	ActionPoints\meter
			TileText* tile048;		// 048	ActionPoints\justify_right_text
			TileText* cndLabel;		// 04C	ActionPoints\justify_right_text
			TileImage* cndMeter;		// 050	ActionPoints\meter
			TileImage* cndMeterBackground;		// 054	ActionPoints\MeterBackground
			TileText* ammoCount;		// 058	ActionPoints\justify_right_text
			TileRect* tile05C;		// 05C	QuestReminder\QuestStages
			TileRect* tile060;		// 060	QuestReminder\QuestAdded
			TileText* regionLocationText;	// 064	Region_Location\justify_left_text
			TileText* tile068;		// 068	Region_Location\justify_left_text
			TileImage* tile06C;		// 06C	RadiationMeter\radiation_bracket
			TileImage* tile070;		// 070	RadiationMeter\radiation_pointer
			TileText* tile074;		// 074	RadiationMeter\radiation_text_value
			TileText* tile078;		// 078	RadiationMeter\radiation_text
			TileImage* tile07C;		// 07C	EnemyHealth\enemy_health_bracket
			TileImage* tile080;		// 080	EnemyHealth\meter
			TileText* tile084;		// 084	EnemyHealth\justify_center_text
			TileText* sneakLabel;	// 088	SneakMeter\sneak_nif
			TileImage* cornerMessageIcon;// 08C	Messages\message_icon
			TileText* cornerMessageText;// 090	Messages\justify_left_text
			TileImage* cornerMessageBracket;// 094	Messages\message_bracket
			TileText* tile098;		// 098	Subtitles\justify_center_text
			TileRect* tileTakePrompt;		// 09C	Info\justify_center_hotrect
			TileText* tile0A0;		// 0A0	Info\justify_center_hotrect\PCShortcutLabel
			TileImage* tile0A4;		// 0A4	Info\justify_center_hotrect\xbox_button
			TileText* tile0A8;		// 0A8	Info\justify_center_text
			TileText* tileLockLevelStr;	// 0AC	Info\justify_center_text
			TileText* tile0B0;		// 0B0	Info\justify_center_text
			TileText* tilePickupWeightStr;	// 0B4	Info\justify_right_text
			TileText* tilePickupWG;			// 0B8	Info\justify_left_text
			TileText* tilePickupValueStr;	// 0BC	Info\justify_right_text
			TileText* tilePickupVAL;		// 0C0	Info\justify_left_text
			TileImage* tile0C4;		// 0C4	Info\info_seperator
			TileRect* tile0C8;		// 0C8	Hokeys\hotkey_selector
			TileText* tile0CC;		// 0CC	Hokeys\justify_center_text
			TileImage* tile0D0;		// 0D0	HitPoints\left_bracket
			TileImage* tile0D4;		// 0D4	ActionPoints\right_bracket
			TileImage* tile0D8;		// 0D8	XPMeter\XPBracket
			TileText* tile0DC;		// 0DC	XPMeter\XPAmount
			TileText* tile0E0;		// 0E0	XPMeter\XPLabel
			TileImage* tile0E4;		// 0E4	XPMeter\XPPointer
			TileText* tile0E8;		// 0E8	XPMeter\XPLastLevel
			TileText* tile0EC;		// 0EC	XPMeter\XPNextLevel
			TileText* tile0F0;		// 0F0	XPMeter\XPLevelUp
			TileImage* tile0F4;		// 0F4	ReticleCenter\reticle_center
			TileImage* tile0F8;		// 0F8	crippled_limb_indicator\Face
			TileImage* tile0FC;		// 0FC	crippled_limb_indicator\Head
			TileImage* tile100;		// 100	crippled_limb_indicator\Torso
			TileImage* tile104;		// 104	crippled_limb_indicator\Left_Arm
			TileImage* tile108;		// 108	crippled_limb_indicator\Right_Arm
			TileImage* tile10C;		// 10C	crippled_limb_indicator\Left_Leg
			TileImage* tile110;		// 110	crippled_limb_indicator\Right_Leg
			TileRect* actionPoints;		// 114	ActionPoints
			TileRect* hitPoints;		// 118	HitPoints
			TileRect* radiationMeter;		// 11C	RadiationMeter
			TileRect* enemyHealth;		// 120	EnemyHealth
			TileRect* questReminder;		// 124	QuestReminder
			TileRect* regionLocation;		// 128	Region_Location
			TileRect* reticleCenter;		// 12C	ReticleCenter
			TileRect* sneakMeter;		// 130	SneakMeter
			TileRect* messages;		// 134	Messages
			TileRect* info;		// 138	Info
			TileRect* subtitles;		// 13C	Subtitles
			TileRect* hotkeys;		// 140	Hokeys
			TileRect* xpMeter;		// 144	XPMeter
			Tile3D* breathMeter;			// 148	BreathMeter
			TileRect* explosivesPositioningRect;	// 14C	Explosive_positioning_rect
			TileRect* crippledLimbIndicator;		// 150	crippled_limb_indicator
			TileImage* tile154;		// 154	DDTIcon
			TileImage* tile158;		// 158	DDTIconEnemy
			TileText* tile15C;		// 15C	AmmoTypeLabel
			TileRect* tile160;		// 160	HardcoreMode
			TileText* tile164;		// 164	HardcoreMode\Dehydration
			TileText* tile168;		// 168	HardcoreMode\Sleep
			TileText* tile16C;		// 16C	HardcoreMode\Hunger
			TileImage* tile170;		// 170	DDTIcon
			TileImage* tile174;		// 174	DDTIconEnemyAP
			TileText* tile178;		// 178	HardcoreMode\Rads
			TileText* tile17C;		// 17C	HardcoreMode\LMBs
			TileImage* cndArrows;	// 180	CNDArrows
		};
	};

	UInt32							unk184;				// 184
	float							flt188;				// 188
	BSSimpleList<QueuedMessage*>	queuedMessages;		// 18C
	UInt32							uiCurrMsgStartTime;	// 194
	BSSimpleArray<SubtitleData>		subtitlesArr;		// 198
	SubtitleData*					pCurrentSubtitle;	// 1A8
	BSSoundHandle					radiationSound;		// 1AC
	TESObjectREFR*					pCrosshairRef;		// 1B8
	UInt32							uiVisibilityFlags;	// 1BC
	Bitfield32						uiVisibilityOverrides;// 1C0
	UInt32							uiStage;				// 1C4
	HotKeyWheel						hotkeyWheel;		// 1C8	Hokeys\hotkey_selector
	bool							bIsForceFirstPerson;			// 1FC
	UInt8							byte1FD;				// 1FD
	UInt8							byte1FE;				// 1FE
	UInt8							byte1FF;				// 1FF
	NiControllerSequence*			pNiContSeq;				// 200
	bool							bIsLevelUpQueued;		// 204
	bool							bAutoDisplayObjectives;	// 205
	UInt8							pad206[2];				// 206
	UInt32							unk208;					// 208
	UInt32							uiQuestTargets;			// 20C
	UInt32							unk210;					// 210
	UInt32							uiCompassWidth;			// 214
	UInt32							uiMaxCompassAngle;		// 218
	Actor*							pHealthTarget;			// 21C
	UInt32							unk220;					// 220
	BSTimer							timer224;				// 224
	float							fEnemyHealthBarTime;	// 244
	float							fLastQuestUpdateReminderTime; // 248
	UInt32							timeArmorIcon;			// 24C
	UInt32							timeDTIcon;				// 250
	bool							bDoClearQuestUpdates;	// 254
	UInt8							byte255;				// 255
	UInt8							byte256;				// 256
	UInt8							byte257;				// 257
	BSSimpleList<QueuedQuestText*>	queuedQuestTextList;	// 258
	bool							bIsQuestAddedTextVisible;//260
	bool							bIsQuestStagesTextVisible;//261
	UInt8							pad262[2];				// 262
	BSSimpleList<UInt32*>			xpMessages;				// 264
	BSSimpleList<UInt32*>			list26C;				// 26C
	float							fHUDShake;				// 274

	static HUDMainMenu* GetSingleton() { return *(HUDMainMenu**)(0x11D96C0); }
	static void __cdecl SetQuestUpdateText(char* src, bool a2, bool a3) { CdeclCall(0x77A5B0, src, a2, a3); }
	static float GetOpacity() { return *(float*)0x11D979C; };
	static bool	 IsUsingScope() { return GetSingleton() && GetSingleton()->bIsForceFirstPerson; };

	void RemoveQueuedQuestAndLocationUpdates();
	void RemoveCrippledLimbIndicator();
};

ASSERT_SIZE(HUDMainMenu, 0x278);