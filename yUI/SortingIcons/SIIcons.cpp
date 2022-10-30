#include "SI.h"
#include <Tiles.h>
#include <functions.h>

#include <InterfaceManager.h>

namespace SortingIcons::Icons
{
	Float32 compassRoseX = 0, compassRoseY = 0;
	
	void InjectTemplates()
	{
		for (const auto& iter : g_XMLPaths)
		{
			HUDMainMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
			RepairMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
			InventoryMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
		}
	}

	bool InjectTemplatesToMenu(TileMenu* tilemenu, const std::string& templateName) {
		if (const auto menu = tilemenu->menu; !menu->GetTemplateExists(templateName.c_str()))
		{
			if (menu->id != kMenuType_Barter && menu->id != kMenuType_Container && menu->id != kMenuType_RepairServices) return false;
			for (auto& iter : g_XMLPaths) tilemenu->InjectUIXML(iter.generic_string().c_str());
			if (!menu->GetTemplateExists(templateName.c_str())) return false;
		}
		return true;
	}

	void InjectIconTile(const CategoryPtr& category_, MenuItemEntryList* list, Tile* tile, InventoryChanges* entry)
	{
		//	if (g_Items.find(entry->type) == g_Items.end()) return;
		const auto& category = *category_;

		if (category.filename.empty()) return;

		TileMenu* menu = tile->GetTileMenu();

		if (!menu || !menu->menu) return;

		const auto text = tile->children.Tail()->data;

		if (!text || !std::string("ListItemText")._Equal(text->name.CStr())) return;

		Tile* icon = tile->GetChild(category.xmltemplate.c_str());

		if (!icon) {
			if (!InjectTemplatesToMenu(menu, category.xmltemplate)) return;
			const auto last = tile->children.Head();
			icon = tile->AddTileFromTemplate(category.xmltemplate.c_str());
			tile->children.ExchangeNodeData(tile->children.Head(), last);
			if (!icon) return;
		}

		if (!category.filename.empty()) icon->SetString(kTileValue_filename, category.filename.c_str(), false);
		if (!category.texatlas.empty()) icon->SetString(kTileValue_texatlas, category.texatlas.c_str(), false);
		if (!category.systemcolor)
			icon->SetFloat(kTileValue_systemcolor, menu->GetFloat(kTileValue_systemcolor));
		else
			icon->SetFloat(kTileValue_systemcolor, category.systemcolor, false);
		//	icon->SetFloat(kTileValue_alpha, 255, propagate);

		Float32 x = text->GetFloat(kTileValue_x);

		if (icon->GetValue(kTileValue_user0)) x += icon->GetFloat(kTileValue_user0);

		icon->SetFloat(kTileValue_x, x, true);

		x += icon->GetFloat(kTileValue_width);

		if (icon->GetValue(kTileValue_user1)) x += icon->GetFloat(kTileValue_user1);

		text->SetFloat(kTileValue_x, x, true);
		text->SetFloat(kTileValue_wrapwidth, text->GetFloat(kTileValue_wrapwidth) - x, true);
	}

	void __fastcall SetTileStringInjectTile(Tile* tile, InventoryChanges* entry, MenuItemEntryList* list, const eTileValue tilevalue, const char* tileText, bool propagate)
	{
		tile->SetString(tilevalue, tileText, propagate);
		const auto item = Categories::ItemGetCategory(entry);
		if (entry && entry->form && TryGetTypeOfForm(entry->form)) InjectIconTile(item, list, tile, entry);
	}

	void __fastcall SetStringValueTagImage(Tile* tile, InventoryChanges* entry, eTileValue tilevalue, char* src, char propagate)
	{
		if (!tile) return;

		if (Tile* icon = tile->GetChild("HK_Icon"); icon) {
			icon->SetFloat(kTileValue_width, tile->GetFloat(kTileValue_width) - 16, propagate);
			icon->SetFloat(kTileValue_height, tile->GetFloat(kTileValue_height) - 16, propagate);
			icon->SetFloat(kTileValue_x, 8, propagate);
			icon->SetFloat(kTileValue_y, 8, propagate);
		}

		const auto icon = Categories::ItemGetCategory(entry)->filename;
		tile->SetString(tilevalue, icon.empty() ? src : icon.c_str(), propagate);
	}

	void __fastcall SetStringValueTagRose(Tile* tile, InventoryChanges* entry, eTileValue tilevalue, char* src, char propagate)
	{
		if (!tile) return;

		if (compassRoseX == 0) compassRoseX = tile->GetFloat(kTileValue_x);
		if (compassRoseY == 0) compassRoseY = tile->GetFloat(kTileValue_y);

		tile->SetFloat(kTileValue_width, 44, propagate);
		tile->SetFloat(kTileValue_height, 44, propagate);
		tile->SetFloat(kTileValue_x, compassRoseX + 3, propagate);
		tile->SetFloat(kTileValue_y, compassRoseY + 3, propagate);

		const auto icon = Categories::ItemGetCategory(entry)->filename;
		tile->SetString(tilevalue, icon.empty() ? src : icon.c_str(), propagate);
	}


	void __fastcall Tile__PropagateIntValue(Tile* tile, void* dummyedx, UInt32 a2, signed int a3)
	{
		auto icon = tile->GetChild("ySIImage");
		if (!icon) icon = tile->AddTileFromTemplate("ySIDefault");
		const auto stringdimensions = FontManager::GetSingleton()->GetStringDimensions(tile->GetValue(kTileValue_string)->str, 7, tile->GetFloat(kTileValue_wrapwidth));
		icon->SetFloat(kTileValue_x, -stringdimensions->x / 2 - icon->GetFloat(kTileValue_width) - icon->GetFloat(kTileValue_user1));
		icon->SetFloat(kTileValue_y, icon->GetFloat(kTileValue_user2) - icon->GetFloat(kTileValue_height) / 4);
		icon->SetFloat(kTileValue_visible, true, true);
		tile->SetFloat(a2, a3, true);
	}
}