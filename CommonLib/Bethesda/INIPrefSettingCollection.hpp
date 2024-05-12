#pragma once

#include "INISettingCollection.hpp"

class INIPrefSettingCollection : public INISettingCollection {
public:
	static INIPrefSettingCollection* GetSingleton();

	struct Audio {
		static float* const fDefaultEffectsVolume;
		static float* const fDefaultFootVolume;
		static float* const fDefaultMasterVolume;
		static float* const fDefaultMusicVolume;
		static float* const fDefaultRadioVolume;
		static float* const fDefaultVoiceVolume;
	};

	struct Controls {
		static bool* const bGamePadRumble;
		static bool* const bInvertYValues;

		static float* const fMouseSensitivity;
		static float* const fXenonHorizLookSpeed;
		static float* const fXenonVertLookSpeed;
	};

	struct Display {
		static SInt32* const iScreenShotIndex;

		static float* const fGamma;
	};

	struct GamePlay {
		static bool* const bCrossHair;
		static bool* const bDialogueSubtitles;
		static bool* const bDisableDynamicCrosshair;
		static bool* const bGeneralSubtitles;
		static bool* const bSaveOnInteriorExteriorSwitch;
		static bool* const bSaveOnRest;
		static bool* const bSaveOnTravel;
		static bool* const bSaveOnWait;
		static bool* const bShowQuestMarkers;
		static bool* const bTrueIronSights;

		static SInt32* const iDifficulty;
		static SInt32* const iKillCamera;

		static UInt32* const uiRedeemedContent;
	};

	struct Interface {
		static bool* const bDisable360Controller;
		static bool* const bHelpEnabled;

		static UInt32* const uHUDColor;
		static UInt32* const uPipboyColor;

		static float* const fHudOpacity;
	};
};