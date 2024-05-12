#pragma once
// rework this file, add tilelist and remove my doublestacked stuff
#include "BSSimpleList.hpp"
#include "InventoryChanges.hpp"

enum KeyboardMenuInputCode;

// 04 not a template
class ListBoxBase
{
public:

	virtual bool	SetSelectedTile(Tile* apkTile) { return false; };
	virtual Tile*	GetSelectedTile() { return nullptr;};
	virtual Tile*	HandleKeyboardInput(KeyboardMenuInputCode aeCode) { return nullptr; };
	virtual bool	IsMenuEqual(Menu* apkMenu) { return false; };
	virtual void	ScrollToHighlight() {};
	virtual Tile*	GetTileByIndex(int aiIndex, char abIsNotTileListIndex) { return nullptr; };

	ListBoxBase() { ThisCall(0x7240C0, this); };
};
static_assert(sizeof(ListBoxBase) == 0x4);

// 0xC
template <typename Item> struct ListBoxItem
{
	Tile*	pkTile		= nullptr;
	Item	item		= 0;
	UInt8	byte08		= 0;
	UInt8	pad09[3];

	ListBoxItem(Item* aitem, Tile*	apkTile = nullptr) : pkTile(apkTile), item(aitem), pad09{} {}
};
static_assert(sizeof(ListBoxItem<void*>) == 0xC);

// 30
template <typename Item> 
class ListBox : public ListBoxBase, BSSimpleList<ListBoxItem<Item*>*>
{
public:

	ListBox() { ThisCall(0x723750, this); }
	virtual	~ListBox() { ThisCall(0x723820, this); };

	virtual void	RemoveAll() {};
	virtual void	Sort(SInt32(__cdecl*)(ListBoxItem<Item>*, ListBoxItem<Item>*)) {};

	enum Flags : UInt16
	{
		kInitiated = 1 << 0,
		kRecalculateHeightsOnInsert = 1 << 1,
		kFreeContChangeOnListItemDestruction = 1 << 2, // assumes the object is a InventoryChanges - do not set this if the object isn't one...
	};

	Tile*			pkTile;				// 0C
	Tile*			pkSelected;			// 10
	Tile*			pkScrollBar;		// 14
	const char*		pcTemplateName;		// 18
	UInt16			usItemCount;		// 1C
	UInt16			pad1E;				// 1E
	Float32			fSumY;				// 20 some sorta y
	Float32			fStoredListIndex;	// 24
	Float32			fStoredScrollbarPos;// 28
	Flags			eFlags;				// 2C
	UInt16			pad2E;				// 2E


};
static_assert(sizeof(ListBox<void*>) == 0x30);
// 011D8B54 BSSimpleList<ListBox<void>>