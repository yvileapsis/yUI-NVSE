#include "main.h"
#include "ConfigurationMenu.h"

#include "SafeWrite.h"
#include "ConfigurationMenuStew.h"

namespace ConfigurationMenu
{

	UInt32 medicalQuestionaireCaseAddr;
	__declspec(naked) void MedicalQuestionaireCreateHook()
	{
		_asm
		{
			mov ecx, [ebp + 8]
			cmp ecx, MENU_ID
			je createStewMenu
			jmp[medicalQuestionaireCaseAddr]

		createStewMenu:
			push 0x71F142
			jmp ModConfigurationMenu::Create
		}
	}

	inline void TweaksButtonCallback()
	{
		ModConfigurationMenu::ReloadMenu()->ShowTweaksMenu();
		// return the StartMenu to the settings menu, to prevent a visual bug since we don't open a sub-menu
		//CdeclCall(0x7D0700); // MenuButton:Settings
	}

	// wraps a call that adds the settings menu to the start menu 
	inline void __fastcall addTweaksButton(BSSimpleArray<StartMenuOption*>* startMenuOptions, void* edx, StartMenuOption** settingsMenuItem)
	{
		startMenuOptions->Append(settingsMenuItem);

		StartMenuOption* tweaksButton = StartMenuOption::Create("Mods", TweaksButtonCallback, StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu);
		startMenuOptions->Append(&tweaksButton);
	}


	bool AtMainMenuOrStewMenuOpen()
	{
		return CdeclCall<bool>(0x70EDF0) || ModConfigurationMenu::GetSingleton();
	}

	void Init()
	{
		/*
_SelectedTrait = Tile::TraitNameToID("_selected");
_TextAlphaTrait = Tile::TraitNameToID("_TextAlpha");
_IsSearchActiveTrait = Tile::TraitNameToID("_IsSearchActive");
_IsCategoriesActiveTrait = Tile::TraitNameToID("_IsCategoriesActive");
_FilterModeTrait = Tile::TraitNameToID("_FilterMode");
_CursorXTrait = Tile::TraitNameToID("_CursorX");
_CursorYTrait = Tile::TraitNameToID("_CursorY");
_MinTrait = Tile::TraitNameToID("_min");
_MaxTrait = kTileValue_user0;
_SetVTrait = kTileValue_user2;
_ValueTrait = Tile::TraitNameToID("_Value");
_ValueStringTrait = Tile::TraitNameToID("_ValueString");
_BackgroundVisibleTrait = Tile::TraitNameToID("_BackgroundVisible");
*/

		SafeWrite8(0x71F1EC + (MENU_ID - 1001), 16); // use switch case for CreditsMenu
		medicalQuestionaireCaseAddr = DetourVtable(0x71F154 + 4 * 16, UInt32(MedicalQuestionaireCreateHook));

		// reload the menu when alt-tabbing
	//	WriteRelCall(0x86A1AB, UInt32(OnAltTabReloadStewMenu));

		// prevent Escape closing the whole start menu if ModConfigurationMenu is open
		WriteRelCall(0x70E686, UInt32(AtMainMenuOrStewMenuOpen));

		WriteRelCall(0x7CCA3E, addTweaksButton);
		
//		deferredInit.emplace_back(DeferredInit);
	}
}
