#pragma once
#include "Menu.hpp"

class DialogMenu : public Menu			// 1009
{
	struct DialogMenu070_00
	{
		char* str;
		UInt32 unk04[9];
		TESTopic* topic;
	};

	struct __declspec(align(4)) DialogMenu070
	{
		DialogMenu070_00* unk00;
		TList<TList<void>> listOfLists04;
		Actor* actor;
		UInt8 isChallengeSpeechFailure;
		UInt8 pad11[3];
		UInt32 unk14;
	};

public:
	DialogMenu();
	~DialogMenu();

	int stage;
	int field_2C;
	TileImage*	pkTile030;		// 030
	TileText*	pkTile034;		// 034
	TileText*	pkTile038;		// 038
	TileImage*	pkTile03C;		// 03C
	ListBox<int>		topicList;		// 040
	DialogMenu070* clickedMenuItem;// 070
	Sound	sound74;	// 074
	TESObjectREFR* partnerRef;	// 080
	UInt8 byte084;				// 84
	UInt8 gap085[3];			// 85
	Float32 unk088;				// 88
	void* unk08C;				// 8C
	void* unk090;				// 90
	TESTopicInfo* infos094[16];	// 94
	void* unk0D4;				// D4
	UInt32 unk0D8[3];			// D8
	TESTopicInfo* infos0E4[8];	// E4
	UInt8 byte104;				// 104
	UInt8 gap105[3];			// 104
	int field_108;				// 108
	int lastMenuClickTime;		// 10C
	TList<TESTopicInfo> infos110;// 110
	TList<TESTopicInfo> infos118;// 118
	TList<TESTopicInfo> infos120;// 120
	Float32 unk128;				// 128
	ImageSpaceModifierInstance* depthOfFieldIMOD; // 12C
	char bWasPlayerThirdPerson;	// 130
	char byte131;				// 131
	char byte132;				// 132
	char byte133;				// 133
	UInt32 unk134;				// 134
	char isBarterMenuShown;		// 138
	char bShouldShowHelpMessage;// 139
	char byte13A;				// 13A
	char byte13B;				// 13B

	bool IsNPCTalking() const;
	static DialogMenu* GetSingleton() { return *reinterpret_cast<DialogMenu**>(0x11D9510); };
};
static_assert(sizeof(DialogMenu) == 0x13C);
