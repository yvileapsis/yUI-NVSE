#pragma once

#include "Menu.hpp"

class StartMenu : public Menu {
public:
	StartMenu();
	virtual ~StartMenu();

	class Option : public BSMemObject {
	public:
		Option(const char* apName, void(__cdecl* apCallback)(), UInt32 auiFlags);
		virtual ~Option() {};

		const char*		sName;
		void(__cdecl*	pFollowupOption)();
		UInt32			uiFlags;
	};

	class UserOption : public Option {
	public:
		enum OPTION_TYPE : UInt32 {
			TOGGLE_CYCLES = 0x0,
			TOGGLE_NO_CYCLE = 0x1,
			SLIDER = 0x2,
		};

		UserOption(const char* apName, void(__cdecl* apCallback)(StartMenu::UserOption*), UInt32 auiFlags, OPTION_TYPE aeType, UInt32 auiOptionCount, UInt32 auiWidth, ...);
		virtual ~UserOption();

		const char*		sTemplateName;
		UInt32			uiCurrValue;
		UInt32			unk18;
		OPTION_TYPE		eType;
		UInt32			uiNumOptions;
		UInt32			uiButtonWidth;
		const char**	ppCurrValueName;
		void(__cdecl*	pOnSelection)(StartMenu::UserOption*);
	};

	static BSSimpleList<UserOption*>* GetSettingsArray();
};
ASSERT_SIZE(StartMenu::Option, 0x10);
ASSERT_SIZE(StartMenu::UserOption, 0x30);