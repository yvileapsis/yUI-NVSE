#pragma once
#include "Menu.hpp"

class RaceSexMenu : public Menu
{
public:
	~RaceSexMenu();
	RaceSexMenu();
	enum
	{
		kFlagHasRandomizePopupBeenShown = 1 << 7,
	};

	enum TemplateIDs
	{
		kRSM_Sex = 0x0,
		kRSM_Race = 0x1,
		kRSM_Face = 0x2,
		kRSM_Hair = 0x3,
		kRSM_Customize = 0x4,
		kRSM_HairStyle = 0x5,
		kRSM_HairColor = 0x6,
		kRSM_FacialHair = 0x7,
		kRSM_EyeColor = 0x8,
		kRSM_Shape = 0x9,
		kRSM_General = 0xA,
		kRSM_Forehead = 0xB,
		kRSM_Brow = 0xC,
		kRSM_Eyes = 0xD,
		kRSM_Nose = 0xE,
		kRSM_Mouth = 0xF,
		kRSM_Cheeks = 0x10,
		kRSM_Jaw = 0x11,
		kRSM_Chin = 0x12,
		kRSM_Tone = 0x13,
		kRSM_14 = 0x14,
		kRSM_Male = 0x15,
		kRSM_Female = 0x16,
		kRSM_Race2 = 0x17,
		kRSM_Preset = 0x18,
		kRSM_Randomize = 0x19,
		kRSM_Age = 0x1A,
		kRSM_HairType = 0x1B,
		kRSM_HeadPart = 0x1C,
		kRSM_HairColor2 = 0x1D,
		kRSM_HairColorR = 0x1E,
		kRSM_HairColorG = 0x1F,
		kRSM_HairColorB = 0x20,
		kRSM_EyeType = 0x21,
		kRSM_22 = 0x22,
		kRSM_23 = 0x23,
	};

	struct Entry
	{
		Tile*	pkTile;
		UInt32 id;
		void* data; // e.g. TESRace for races, an integer for face sliders
	};

	// 24
	struct List
	{
		DList<Entry> items; // 0
		UInt32 parentID; // or previous menu template ID // C
		UInt32 childID; // next menu e.g. sex->race->face->hair // 10
		Tile* listItemTemplate; // 14
		Float32 height; // 18
		bool(__cdecl* IsItemSelected)(Entry* item); // called whenever something is selected to add a white square beside active options // 1C
		Entry* selectedEntry; // 20

		Entry* GetEntryWithStringCI(const char* name)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->data && !_stricmp((char*)iter->data->data, name))
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		void HandleCallbacks() { ThisCall(0x7B40D0, this); };

		Entry* GetEntryWithData(void* data)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->data == data)
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		Entry* GetEntryWithID(UInt32 _id)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->id == _id)
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		Entry* GetEntryByTile(Tile*	pkTile)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->tile == tile)
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		Entry* AddItem(const char* displayString, UInt32 _id, bool removeSound = true)
		{
			auto entry = ThisCall<Entry*>(0x7B3B80, this, displayString, _id);
			if (removeSound)
			{
				entry->tile->Set(kTileValue_clicksound, "");
			}
			return entry;
		}

		// adds an item without looping over all items in the list to set their positions...
		Entry* AddItemAlt(const char* str, UInt32 _id)
		{
			auto entry = (Entry*)GameHeapAlloc(sizeof(Entry));
			entry->data = nullptr;
			entry->id = _id;

			auto rsm = RaceSexMenu::GetSingleton();
			auto tile = rsm->AddTileFromTemplate(rsm->rsmBackground, "RSM_list_item_template", 0);
			tile->Set(kTileValue_string, str);
			tile->Set(kTileValue_user0, listItemTemplate->Get(kTileValue_user0));
			tile->Set(kTileValue_target, true);
			tile->Set(kTileValue_clicksound, "");
			entry->tile = tile;

			this->items.Append(entry);

			return entry;
		}

		void RemoveEntry(Entry* entry)
		{
			if (!entry)
			{
				selectedEntry = nullptr;
				return;
			}

			entry->tile->~Tile();
			auto nextInList = items.Remove(entry);
			if (selectedEntry == entry) selectedEntry = nextInList ? nextInList->data : nullptr;
		}

		void AddSubMenu(const char* displayString, List* subMenu)
		{
			ThisCall(0x7B3BB0, this, displayString, subMenu);
		}

		void UpdateListPositions()
		{
			// used to update the Y position of the entries in a list (e.g. after sorting the items list) of the menu
			auto iter = items.Head();
			if (!iter) return;

			auto topTrait = Tile::TraitNameToID("_top_bound");
			auto tile = iter->data->tile;
			Float32 yPos = tile->parent->Get(topTrait);

			tile->Set(kTileValue_y, yPos);
			ThisCall(0xA04640, tile, false);

			while (iter = iter->next)
			{
				auto tileAbove = iter->prev->data->tile;
				yPos = tileAbove->Get(kTileValue_y);
				yPos += tileAbove->Get(kTileValue_height);

				tile = iter->data->tile;
				tile->Set(kTileValue_y, yPos);
				ThisCall(0xA04640, tile, false);
			}

			this->height = max(this->height, items.Tail()->data->tile->Get(kTileValue_height));
		}

		void UpdateSelectedSquares()
		{
			ThisCall(0x7B40D0, this);
		}

		void SetActive()
		{
			ThisCall(0x7B4410, this);
		}

		static List* Create(const char* displayString, UInt32 id)
		{
			auto item = (List*)GameHeapAlloc(sizeof(List));
			ThisCall(0x7B39E0, item, id, displayString);
			return item;
		}

		void Destroy(bool doFree = true) { ThisCall(0x7AC700, this, doFree); };
	};
	static_assert(sizeof(List) == 0x24);

	union
	{
		List* menus[20];
		struct
		{
			List* sex;
			List* race;
			List* face;
			List* hair;
			List* customize;
			List* hairStyle;
			List* hairColor;
			List* facialHair;
			List* eyeColor;
			List* shape;
			List* general;
			List* forehead;
			List* brow;
			List* eyes;
			List* nose;
			List* mouth;
			List* cheeks;
			List* jaw;
			List* chin;
			List* tone;
		};
	};

	TileImage* rsmBackground;
	TileImage* rsmFaceGrab;
	TileImage* rsmScrollUpTarget;
	TileImage* rsmScrollDownTarget;
	TileImage* rsmBackButton;
	TileImage* rsmNextButton;
	String nifPath;
	String confirmDone;
	NiPoint3 point0A0;
	NiPoint3 point0AC;
	Float32 lastClickX;
	UInt32 currentSliderValue;
	Tile* activeClickedSliderTile;
	Entry* currentSlider;
	Float32 currentZoom;
	Float32 somethingZoom;
	Float32 remainingZoom;
	NiNode* node0D4;
	TESNPC* npc;
	NiTArray<TESNPC*> presetNPCs;
	UInt32 flags;
	Sound OBJComputerTerminal01LP;
	Sound QSTBirthRoomLP;
	Sound UIFaceGenZoomLP;
	Sound UIFaceGenRotateLP;
	UInt32 finalPageIndex;
	FaceGenData faceData[2][2];

	static RaceSexMenu* GetSingleton() { return *reinterpret_cast<RaceSexMenu**>(0x11DA634); };

	void RefreshSliders();
	void UpdateScrollButtonVisibilities() { ThisCall(0x7AF6B0, this); };
	UInt32 GetCurrentMenuID() { return tile->Get(kTileValue_user0); };
	void SetMenuActive(UInt32 _id) { ThisCall(0x7AF180, this, _id); };
	void UpdatePlayerHead() { ThisStdCall(0x007B25A0, this); }
};
static_assert(sizeof(RaceSexMenu) == 0x1A4);
