#include "SortingIcons.h"
#include <Tile.h>
#include <functions.h>

#include <InterfaceManager.h>

#include <Safewrite.hpp>

namespace SortingIcons::Icons
{
	Float32 compassRoseX = 0, compassRoseY = 0;
	
	void InjectTemplates()
	{
		for (const auto& iter : g_XMLPaths)
		{
			HUDMainMenu::GetSingleton()->tile->InjectUIXML(iter);
			RepairMenu::GetSingleton()->tile->InjectUIXML(iter);
			InventoryMenu::GetSingleton()->tile->InjectUIXML(iter);
		}
	}

	bool InjectTemplatesToMenu(TileMenu* tilemenu, const std::string& templateName) {
		if (const auto menu = tilemenu->menu; !menu->GetTemplateExists(templateName))
		{
			if (menu->id != kMenuType_Barter && menu->id != kMenuType_Container && menu->id != kMenuType_RepairServices) return false;
			for (auto& iter : g_XMLPaths) tilemenu->InjectUIXML(iter);
			if (!menu->GetTemplateExists(templateName)) return false;
		}
		return true;
	}

	void InjectIconTile(const Icon* category, Tile* tile)
	{
		if (!category->IsValid()) return;

		const auto& tag = category->tag;
		const auto priority = category->priority;
		const auto& xmltemplate = category->xmltemplate;
		const auto& filename = category->filename;
		const auto& texatlas = category->texatlas;
		const auto font = category->font;
		const auto systemcolor = category->systemcolor;

		if (filename.empty()) return;

		TileMenu* menu = tile->GetTileMenu();

		if (!menu || !menu->menu) return;

		const auto text = tile->children.Tail()->data;

		if (!text || std::string(text->name.CStr()) != "ListItemText") return;

		Tile* icon = tile->GetChild(xmltemplate);

		if (!icon) {
			if (!InjectTemplatesToMenu(menu, xmltemplate)) return;
			const auto last = tile->children.Head();
			icon = tile->AddTileFromTemplate(xmltemplate.c_str());
			tile->children.ExchangeNodeData(tile->children.Head(), last);
			if (!icon) return;
		}

		if (!filename.empty()) icon->Set(kTileValue_filename, filename, false);
		if (!texatlas.empty()) icon->Set(kTileValue_texatlas, texatlas, false);
		if (font.has_value()) icon->Set(kTileValue_texatlas, font.value(), false);
		if (systemcolor.has_value())
			icon->Set(kTileValue_systemcolor, systemcolor.value(), false);
		else
			icon->Set(kTileValue_systemcolor, menu->Get(kTileValue_systemcolor));

		const Float32 x = text->Get(kTileValue_x);
		Float32 width = 0;

		if (icon->GetValue(kTileValue_user0)) width += icon->Get(kTileValue_user0);

		icon->Set(kTileValue_x, x + width, true);

		width += icon->Get(kTileValue_width);

		if (icon->GetValue(kTileValue_user1)) width += icon->Get(kTileValue_user1);

		text->Set(kTileValue_x, x + width, true);
		text->Set(kTileValue_wrapwidth, text->Get(kTileValue_wrapwidth) - width, false);
	}

	void __fastcall InjectTileSetTileString(Tile* tile, const InventoryChanges* entry, MenuItemEntryList* list, const TileValueIDs tilevalue, const char* tileText, bool propagate)
	{
		tile->Set(tilevalue, tileText, propagate);
		if (entry && entry->form && TryGetTypeOfForm(entry->form)) InjectIconTile(Icon::Get(Item::Get(entry->form)), tile);
	}

	void __fastcall TagImageSetStringValue(Tile* tile, InventoryChanges* entry, TileValueIDs tilevalue, char* src, char propagate)
	{
		if (!tile) return;
//		if (g_Categories.empty()) return;

		if (Tile* icon = tile->GetChild("HK_Icon"); icon) {
			icon->Set(kTileValue_width, (Float32) tile->Get(kTileValue_width) - 16, propagate);
			icon->Set(kTileValue_height, (Float32) tile->Get(kTileValue_height) - 16, propagate);
			icon->Set(kTileValue_x, 8, propagate);
			icon->Set(kTileValue_y, 8, propagate);
		}

		const auto category = Icon::Get(Item::Get(entry->form));
		tile->Set(tilevalue, category->IsValid() && !category->filename.empty() ? category->filename.c_str() : src, propagate);
	}

	void __fastcall TagRoseSetStringValue(Tile* tile, InventoryChanges* entry, TileValueIDs tilevalue, char* src, char propagate)
	{
		if (!tile) return;
//		if (g_Categories.empty()) return;

		if (compassRoseX == 0) compassRoseX = tile->Get(kTileValue_x);
		if (compassRoseY == 0) compassRoseY = tile->Get(kTileValue_y);

		tile->Set(kTileValue_width, 44, propagate);
		tile->Set(kTileValue_height, 44, propagate);
		tile->Set(kTileValue_x, compassRoseX + 3, propagate);
		tile->Set(kTileValue_y, compassRoseY + 3, propagate);

		const auto category = Icon::Get(Item::Get(entry->form));
		tile->Set(tilevalue, category->IsValid() && !category->filename.empty() ? category->filename.c_str() : src, propagate);
	}


	void __fastcall TagPromptPropagateIntValue(Tile* tile, void* dummyedx, TileValueIDs id, signed int a3)
	{
		tile->Set(id, a3, true);

		const auto ref = HUDMainMenu::GetSingleton()->crosshairRef;
		const auto item = !ref ? nullptr : ref->baseForm;
		const auto category = Icon::Get(Item::Get(item));

		auto icon = tile->GetChild("ySIImage");
		if (!icon) icon = tile->AddTileFromTemplate("ySIDefault");
		if (!item || !item->IsInventoryObjectAlt() || !category->IsValid())
		{
			icon->Set(kTileValue_visible, false, true);
			return;
		}
		const auto fontManager = FontManager::GetSingleton();
		const auto string = tile->GetValue(kTileValue_string)->str;
		const auto wrapwidth = tile->Get(kTileValue_wrapwidth);
		const auto stringDimensions = fontManager->GetStringDimensions(string, 7, wrapwidth);

		icon->Set(kTileValue_x, -stringDimensions->x / 2 - icon->Get(kTileValue_width) - icon->Get(kTileValue_user1));
		icon->Set(kTileValue_y, - icon->Get(kTileValue_user2));
		icon->Set(kTileValue_visible, true, true);
		icon->Set(kTileValue_systemcolor, tile->Get(kTileValue_systemcolor));
		icon->Set(kTileValue_brightness, tile->Get(kTileValue_brightness));
		icon->Set(kTileValue_alpha, tile->Get(kTileValue_alpha));

		icon->Set(kTileValue_filename, category->filename);
	}
}

namespace SortingIcons::Icons::Hook
{
	template <UInt32 retn> __declspec(naked) void TileSetStringValueIconInject() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(InjectTileSetTileString);
		static const UInt32 retnAddr = retn;
		__asm
		{
//			mov		edx, [ebp - 0x10]	//tile
//			push	edx
			mov		edx, [ebp - 0x2C]	//menu item entry list
			push	edx
			mov		edx, [ebp + 0x8]	//entry
			call	SetStringValue
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void TileSetStringValueIconHotkeyHUD() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(TagImageSetStringValue);
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		edx, [ebp - 0x24]
			call	SetStringValue
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void TileSetStringValueIconHotkeyPipBoy() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(TagRoseSetStringValue);
		static const UInt32 retnAddr = retn;
		static const UInt32 g_inventoryMenuSelection = 0x011D9EA8;
		__asm
		{
			mov		edx, dword ptr ds : [0x011D9EA8]
			call	SetStringValue
			jmp		retnAddr
		}
	}
}

namespace SortingIcons::Patch
{
	void AddIcons(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x71A3D5, Icons::Hook::TileSetStringValueIconInject<0x71A3DA>);
		}
		else
		{
			UndoSafeWrite(0x71A3D5);
		}
	}

	void AddPromptIcon(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelCall(0x7786CF, Icons::TagPromptPropagateIntValue);
		}
		else
		{
			UndoSafeWrite(0x7786CF);
		}
	}

	void ReplaceHotkeyIcons(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x70189E, Icons::Hook::TileSetStringValueIconHotkeyHUD<0x7018A3>);
			WriteRelJump(0x7814FA, Icons::Hook::TileSetStringValueIconHotkeyPipBoy<0x7814FF>);
		}
		else
		{
			UndoSafeWrite(0x70189E);
			UndoSafeWrite(0x7814FA);
		}
	}
}