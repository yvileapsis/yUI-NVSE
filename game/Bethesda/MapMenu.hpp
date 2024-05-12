#pragma once
#include "Menu.hpp"

// 230
class MapMenu : public Menu				// 1023
{
public:
	MapMenu();
	~MapMenu();

	enum MapMenuTabs
	{
		kLocalMap = 0x20,
		kWorldMap = 0x21,
		kQuests = 0x22,
		kMisc = 0x23,
		kRadio = 0x24,
	};
	union
	{
		Tile*	pkTiles[21];
		struct
		{
			TileText*	pkTile028;		// 028	MM_MainRect\MM_HeadlineRect\MM_Headline_LocationInfo
			TileText*	pkTile02C;		// 02C	MM_MainRect\MM_HeadlineRect\MM_Headline_TimeDateInfo
			TileImage*	pkTile030;		// 030	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMap_ParentImage
			TileImage*	pkTileLocalMapCursor;	// 034	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMapCursor
			TileImage*	pkTile038;		// 038	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMap_ParentImage
			TileImage*	pkTileWorldMapCursor;	// 03C	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMapCursor
			TileRect* mapSelectionRectangle;	// 040	MM_MainRect\MM_Highlight_ClipWindow\MM_MapHighlightBox
			TileImage*	pkTile044;		// 044	MM_MainRect\MM_QuestsList
			TileImage*	pkTile048;		// 048	MM_MainRect\MM_NotesList
			TileImage*	pkTile04C;		// 04C	MM_MainRect\MM_NotesList\MM_Notes_SelectedNoteHighlight
			TileImage*	pkTile050;		// 050	MM_MainRect\MM_RadioStationList
			TileImage*	pkTile054;		// 054	MM_MainRect\MM_ButtonRect\MM_ButtonA
			TileImage*	pkTile058;		// 058	MM_MainRect\MM_ButtonRect\MM_ButtonX
			TileRect*	pkTile05C;		// 05C	MM_MainRect\MM_DataRect
			TileImage*	pkTile060;		// 060	MM_MainRect\MM_DataRect\MM_DataTextRect
			TileImage*	pkTile064;		// 064	MM_MainRect\MM_DataRect\MM_QuestObjectivesList
			TileImage*	pkTile068;		// 068	MM_MainRect\MM_WaveformRect\MM_Waveform
			TileRect*	pkTile06C;		// 06C	MM_Tabline
			TileImage*	pkTile070;		// 070	MM_MainRect\MM_ButtonRect\MM_ButtonY
			TileImage*	pkTile074;		// 074	MM_MainRect\MM_ChallengeList
			TileImage*	pkTile078;		// 078	MM_MainRect\MM_ChallengeList\MM_Challenge_SelectedHighlight
			TileText*	pkTile07C;		// 07C	MM_MainRect\MM_Headline_ChallengeType
		};
	};

	UInt8							currentTab;		// 080
	UInt8							pad081[3];		// 081
	TileImage*	pkTileWorldMap; // 084
	UInt32 unk088;
	UInt8 isShowAllNotes;
	UInt8 byte08D;
	UInt8 byte08E;
	UInt8 byte08F;
	BGSNote* currentNote;
	UInt32 timeNoteViewed;
	UInt32 someSoundsList[4];
	BSSimpleArray<String>			arr0A8;			// 0A8
	Sound* maybeHolotapeSound;
	UInt8 isHolotapeVoicePlaying;
	UInt8 pad0BD;
	UInt8 pad0BE;
	UInt8 pad0BF;
	Float32 holotapeTotalTime;
	UInt32 time0C4;
	Float32 radioVolume;
	TESQuest* selectedQuest;
	BGSQuestObjective* currentObjective;
	TileImage* mapMarker;		// 0D4
	TList<TESObjectREFR>			mapMarkerList;	// 0D8
	TList<TESObjectREFR>			doorList;		// 0E0
	Float32							flt0E8;			// 0E8
	Float32							flt0EC;			// 0EC
	Float32							flt0F0;			// 0F0
	Float32							flt0F4;			// 0F4
	TESForm* markerForm;		// 0F8
	NiPoint3						markerPos;		// 0FC
	TESObjectCELL* cell108;		// 108
	TESWorldSpace* wspc10C;		// 10C
	UInt32							unk110;			// 110
	TESObjectREFR* lastExtDoor;	// 114
	TESObjectREFR* selectedMarker;// 118
	TESObjectCELL* cell11C;		// 11C
	NiPoint2						clickStartPos;	// 120
	UInt32							unk128;			// 128
	bool							fogOfWar;		// 12C
	UInt8							pad12D[3];		// 12D
	ListBox<TESQuest>				questList;		// 130
	ListBox<BGSNote>				noteList;		// 160
	ListBox<TESObjectREFR>			radioRefList;	// 190
	ListBox<BGSQuestObjective>		objectiveList;	// 1C0
	ListBox<TESChallenge>			challengeList;	// 1F0
	BSSimpleArray<Tile>				arr220;			// 220

	static MapMenu* GetSingleton() { return *reinterpret_cast<MapMenu**>(0x11DA368); };

	void Zoom(bool direction, Float32 amount);

	void Move(Float32 deltaX, Float32 deltaY);

	void MoveDirection(MenuSpecialKeyboardInputCode direction);

	void RefreshWorldMap();
	void RefreshLocalMap();

	// only works for local map doors due to the hook in MapMenu::SetupLocalMap which sets the _MarkerIndex trait!
	TESObjectREFR* GetLocalMapHoveredDoorRef();
	TESObjectREFR* GetHoveredMarkerRef();

	void SetupLinesToMarker(TESObjectREFR* lastExtDoor, TESObjectREFR* ref, QuestTarget* questTarget, signed int tileID);

	bool ScrollNotes(MenuSpecialKeyboardInputCode direction, bool isShiftHeld);
};
static_assert(sizeof(MapMenu) == 0x230);