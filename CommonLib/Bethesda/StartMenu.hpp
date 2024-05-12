#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"

class NiSourceTexture;
class Tile3D;

class StartMenu : public Menu
{
public:
	~StartMenu();
	StartMenu();

	// 10
	class Option : public BSMemObject
	{
	public:
		// which menus the option should be shown in
		enum EnumWhichMenus
		{
			kMainMenu = 0x1,
			kPauseMenu = 0x2,
			kSettingsMenu = 0x4,
			kGameplayMenu = 0x8,
			kDisplayMenu = 0x10,
			kAudioMenu = 0x20,
			kControlsMenu = 0x40,
			kNewGameMenu = 0x80,
			kContinueGameMenu = 0x100,
			kQuitGameMenu = 0x2000,
		};


		Option(const char* str, void (*callback)(), UInt32 flags) { ThisCall<Option*>(0x07CE620, this, str, callback, flags); }

		virtual ~Option() {};
		
		const char*	pcDisplayString;
		void		(*pCallback)();	// 08
		UInt32		uiData;
	};

	// 30
	class UserOption : Option
	{
	public:
		enum EnumType : UInt32
		{
			// kToggleCycles will show a right arrow at max value which will cycle it back to the lowest value
			kToggleCycles = 0,
			kToggleNoCycle = 1,
			kSlider = 2
		};

		const char*		pcTemplateName;				// 10
		UInt32			uiCurrentValue;				// 14
		UInt32			uiDefaultValue;				// 18
		EnumType		eType;						// 1C
		UInt32			uiNumOptions;				// 20
		UInt32			uiButtonWidth;				// 24
		const char**	ppcCurrValueName;			// 28
		void			(__cdecl* pOnSelection)(UserOption*);	// 2C

		UserOption(char* str, void (*_callback)(), EnumWhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
			: Option(str, _callback, flags)
		{
			ThisCall<UserOption*>(0x7D6350, this, str, _callback, flags, type, numOptions, buttonWidth, optionNames);;
		}

		~UserOption() override {};

		void UpdateVisibility() { ThisCall(0x7D4CE0, GetSingleton(), this, NULL); };
	};

	enum EnumStartMenuFlags : UInt32
	{
		kInStartMenu			= 1 << 0,
		kHasChangedSettings		= 1 << 1,
		kLoad					= 1 << 2,
		kIsSaveMenuNotLoad		= 1 << 3,
		kIsMainMenuShown		= 1 << 4,
		kAreOptionsInitialised	= 1 << 5,
		kShowDLCPopup			= 1 << 6,

		kIsActionMapping		= 1 << 12,
		kDeleteSave				= 1 << 13,
		kShowCredits			= 1 << 16,
		kControllerDisconnected	= 1 << 17,
		kShowMustRestartToSaveSettings = 1 << 22,
		ksomething_credits		= 1 << 25,
		kSomethingSave			= 1 << 30,
	};

	TileImage*						pkTileMainContainer;
	TileImage*						pkTileSettingsContainer;
	TileImage*						pkTileOptionsContainer;
	TileImage*						pkTileMainTitle;
	TileImage*						pkTileMainDefaultsButton;
	TileImage*						pkTileMainBackButton;
	TileImage*						pkTileSaveLoadContainer;
	TileImage*						pkTileConfirmContainer;
	TileText*						pkTileConfirmQuestion;
	TileText*						pkTileMainPressStart;
	TileText*						pkTileMainVersion;
	TileImage*						pkTileSaveLoadScreenshot;
	TileText*						pkTileSaveLoadPlayerName;
	Tile3D*							pkTilePauseBackground;
	TileImage*						pkTileDownloadsContainer;
	TileImage*						pkTileMainDeviceButton;
	TileText*						pkTileActionMappingDeviceName; // controls header
	TileImage*						pkTileOptionsBrightnessGradient;
	TileText*						pkTileSaveLoadPlayerLevel;
	TileText*						pkTileSaveLoadPlaytime;
	TileText*						pkTileConfirmWarning;
	TileImage*						pkTileMainRedeemCodeButton;
	TileText*						pkTileSaveLoadVersion;
	ListBox<Option>					kOptionsMain;
	ListBox<Option>					kOptionsSettings;
	ListBox<Option>					kOptionsNewGame;
	ListBox<Option>					kOptionsActionMapping;
	ListBox<int>					kDownloadsContainer;
	ListBox<BGSSaveLoadFileEntry>	kSavesList;
	UInt32							unk1A4;
	UInt32							uiFlags;
	Float32							fScrolledLoadedSavesHeight;
	UInt32							unk1B0;
	Option*							pkBrightnessSettingCallback;
	NiSourceTexture*				pkTexture1B8;
	UInt32							unk1BC;
	TileImage*						tile1C0;
	TileImage*						tile1C4;
	UInt32							uiCurrentControlsDevice;
	UInt32							unk1CC;
	UInt32							unk1D0;

	static StartMenu* GetSingleton() { return *reinterpret_cast<StartMenu**>(0x11DAAC0); };

	static bool IsActive() { return GetSingleton() && GetSingleton()->uiFlags & kInStartMenu; }
};
static_assert(sizeof(StartMenu) == 0x1D4);
