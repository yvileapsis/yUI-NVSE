#include "main.h"

#include "SafeWrite.h"
#include "ConfigurationMenu.h"

namespace ConfigurationMenu
{

	void* __fastcall MCMInit(UInt32 id)
	{
		if (id == MENU_ID)
			return ModConfigurationMenu::Create();
		return nullptr;
	}

	UInt32 medicalQuestionaireCaseAddr;
	__declspec(naked) void MedicalQuestionaireCreateHook()
	{
		_asm
		{
			mov ecx, [ebp + 8]
			cmp ecx, 1047
			jnz createConfigurationMenu
			jmp [medicalQuestionaireCaseAddr]

			createConfigurationMenu:
			push 0x71F142
			jmp MCMInit
		}
	}

	inline void ConfigurationMenuButtonCallback()
	{
		ModConfigurationMenu::ReloadMenu()->ShowMenuFirstTime();
	}

	inline void __fastcall AddConfigurationMenuButton(BSSimpleArray<StartMenu::Option*>* startMenuOptions, void* edx, StartMenu::Option** settingsMenuItem)
	{
		startMenuOptions->Append(settingsMenuItem);

		auto option = new StartMenu::Option("Mods", ConfigurationMenuButtonCallback, StartMenu::Option::kMainMenu | StartMenu::Option::kPauseMenu);
		startMenuOptions->Append(&option);
	}

	void DeferredInit()
	{
		WriteMCMHooks();

	}

	void Init()
	{

		SafeWrite8(0x71F1EC + (MENU_ID - 1001), 16); // use switch case for CreditsMenu
		medicalQuestionaireCaseAddr = DetourVtable(0x71F154 + 4 * 16, UInt32(MedicalQuestionaireCreateHook));

		// reload the menu when alt-tabbing
	//	WriteRelCall(0x86A1AB, UInt32(OnAltTabReloadStewMenu));

		WriteRelCall(0x7CCA3E, AddConfigurationMenuButton);
		
		deferredInit.emplace_back(DeferredInit);
		mainLoop.emplace_back(MainLoop);
	}
}
