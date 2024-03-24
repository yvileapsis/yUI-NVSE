#pragma once

#include "BSSimpleList.hpp"
#include "DList.hpp"
#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "BSMemObject.hpp"

class TileMenu;
class Tile;
class TileExtra;

enum MenuSpecialKeyboardInputCode
{
	kMenu_Enter = -2,
	kMenu_UpArrow = 0x1,
	kMenu_DownArrow = 0x2,
	kMenu_RightArrow = 0x3,
	kMenu_LeftArrow = 0x4,
	kMenu_Space = 0x9,
	kMenu_Tab = 0xA,
	kMenu_ShiftEnter = 0xB,
	kMenu_AltEnter = 0xC,
	kMenu_ShiftLeft = 0xD,
	kMenu_ShiftRight = 0xE,
	kMenu_PageUp = 0xF,
	kMenu_PageDown = 0x10,
};

class Menu : public BSMemObject {
public:
	virtual			~Menu();
	virtual void	SetTile(UInt32 auiTileID, Tile* apValue); // can be used to determine how many Tile*s are in a Menu class
	virtual void	HandleLeftClickPress(UInt32 auiTileID, Tile* apCctiveTile); // called when the mouse has moved and left click is pressed
	virtual void	HandleClick(SInt32 auiTileID, Tile* apClickedTile);
	virtual void	HandleMouseover(UInt32 auiTileID, Tile* apActiveTile);	//	Called on mouseover, activeTile is moused-over Tile
	virtual void	HandleUnmouseover(UInt32 auiTileID, Tile* apTile);
	virtual void	PostDragTileChange(UInt32 auiTileID, Tile* apNewTile, Tile* apActiveTile); // unused in vanilla
	virtual void	PreDragTileChange(UInt32 auiTileID, Tile* apOldTile, Tile* apActiveTile); // unused in vanilla
	virtual void	HandleActiveMenuClickHeld(UInt32 auiTileID, Tile* apActiveTile); // StartMenu, RaceSexMenu, VATSMenu, BookMenu
	virtual void	OnClickHeld(UInt32 auiTileID, Tile* apActiveTile); // unused by vanilla menus
	virtual void	HandleMousewheel(UInt32 auiTileID, Tile* apTile);
	virtual void	Update();	// Called every frame while the menu is active
	virtual bool	HandleKeyboardInput(UInt32 auiInputChar);	// Return false for handling keyboard shortcuts
	virtual UInt32	GetID();
	virtual bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode aeCode, float afKeyState);
	virtual bool	HandleControllerInput(int aiInput, Tile* apTile);
	virtual void    OnUpdateUserTrait(int aitileVal) {}; // unimplemented by any vanilla menus, called at 0xA1F28E - called when a tilevalue > 10000 is used (any field starting with an underscore like <_XXX></_XXX> is a user trait)
	virtual void	HandleControllerConnectOrDisconnect(bool abIsControllerConnected);

	struct TemplateInstance {
		UInt32		unk00;
		float		flt04;
		BSStringT	strTileName;
		Tile*		pTile;
	};

	// 14
	struct TemplateData {
		const char*				pTemplateName;
		TileExtra*				pTileExtra;
		DList<TemplateInstance>	instances;
	};

	TileMenu*							pRootTile;
	BSSimpleList<Menu::TemplateData*>	kMenuTemplates;
	UInt32								unk10;
	UInt32								unk14;
	UInt32								unk18;
	UInt8								byte1C;
	UInt8								byte1D;
	UInt8								byte1E;
	UInt8								byte1F;
	UInt32								uiID;
	UInt32								uiVisibilityState;
};