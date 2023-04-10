#include "main.h"

#include "SafeWrite.h"
#include "ConfigurationMenu.h"

namespace ConfigurationMenu
{

	void* MCMInit()
	{
		return ModConfigurationMenu::Create();
	}

	UInt32 medicalQuestionaireCaseAddr;
	__declspec(naked) void MedicalQuestionaireCreateHook()
	{
		_asm
		{
			mov ecx, [ebp + 8]
			cmp ecx, 1047
			jnz createStewMenu
			jmp [medicalQuestionaireCaseAddr]

		createStewMenu:
			push 0x71F142
			jmp MCMInit
		}
	}

	inline void TweaksButtonCallback()
	{
		const auto menu = ModConfigurationMenu::ReloadMenu();
		menu->ShowTweaksMenu();
		// return the StartMenu to the settings menu, to prevent a visual bug since we don't open a sub-menu
		//CdeclCall(0x7D0700); // MenuButton:Settings
	}

	// wraps a call that adds the settings menu to the start menu 
	inline void __fastcall addTweaksButton(BSSimpleArray<StartMenu::Option*>* startMenuOptions, void* edx, StartMenu::Option** settingsMenuItem)
	{
		startMenuOptions->Append(settingsMenuItem);

		StartMenu::Option* tweaksButton = new StartMenu::Option("Mods", TweaksButtonCallback, StartMenu::Option::kMainMenu + StartMenu::Option::kPauseMenu);
		startMenuOptions->Append(&tweaksButton);
	}

	void Init()
	{

		SafeWrite8(0x71F1EC + (MENU_ID - 1001), 16); // use switch case for CreditsMenu
		medicalQuestionaireCaseAddr = DetourVtable(0x71F154 + 4 * 16, UInt32(MedicalQuestionaireCreateHook));

		// reload the menu when alt-tabbing
	//	WriteRelCall(0x86A1AB, UInt32(OnAltTabReloadStewMenu));

		WriteRelCall(0x7CCA3E, addTweaksButton);
		
//		deferredInit.emplace_back(DeferredInit);
	}
}
