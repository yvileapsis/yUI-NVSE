#pragma once

#include "NiColor.hpp"
#include "Sun.hpp"
#include "Clouds.hpp"
#include "Moon.hpp"
#include "BSSimpleList.hpp"
#include "BSMultiBoundNode.hpp"
#include "Atmosphere.hpp"
#include "Stars.hpp"
#include "Precipitation.hpp"

class TESClimate;
class TESWeather;
class ImageSpaceModifierInstanceForm;

NiSmartPointer(Sky);

class Sky {
public:
	Sky();
	virtual ~Sky();

	enum SkyObjectType {
		SOT_SUNGLARE = 0x1,
		SOT_CLOUDS = 0x3,
		SOT_STARS = 0x5,
		SOT_MOON = 0x7,
	};

	enum SkyColors {
		SC_SKY_UPPER	= 0,
		SC_FOG			= 1,
		SC_CLOUDS_LOWER = 2,
		SC_AMBIENT		= 3,
		SC_SUNLIGHT		= 4,
		SC_SUN			= 5,
		SC_STARS		= 6,
		SC_SKY_LOWER	= 7,
		SC_HORIZON		= 8,
		SC_CLOUDS_UPPER = 9,
		SC_COUNT,
	};

	struct SkySound {
		UInt32		unk00;		// 00
		UInt32		unk04;		// 04
		UInt32		unk08;		// 08
		TESWeather* pWeather;	// 0C
		UInt32		uiType;		// 10
		UInt32		uiSoundID;	// 14
	};

	enum SKY_MODE {
		SM_NONE = 0x0,
		SM_INTERIOR = 0x1,
		SM_SKYDOME_ONLY = 0x2,
		SM_FULL = 0x3,
	};

	struct COLOR_BLEND {
		UInt32	uiRGBVal[4];
		float	fBlend[4];
	};

	BSMultiBoundNodePtr				spRoot;
	NiNodePtr						spMoonsRoot;
	TESClimate*						pCurrentClimate;
	TESWeather*						pCurrentWeather;
	TESWeather*						pLastWeather;
	TESWeather*						pDefaultWeather;
	TESWeather*						pOverrideWeather;
	Atmosphere*						pAtmosphere;
	Stars*							pStars;
	Sun*							pSun;
	Clouds*							pClouds;
	Moon*							pMasser;
	Moon*							pSecunda;
	Precipitation*					pPrecipitation;
	NiColor							kColors[SC_COUNT];
	NiColor							kColorUnkown0B4;
	NiColor							kColorSunFog;
	float							fWindSpeed;
	float							fWindAngle;
	float							fFogNearPlane;
	float							fFogFarPlane;
	UInt32							unk0DC;
	UInt32							unk0E0;
	UInt32							unk0E4;
	float							fFogPower;
	float							fCurrentGameHour;
	float							fLastWeatherUpdate;
	float							fCurrentWeatherPct;
	SKY_MODE						eMode;
	BSSimpleList<SkySound>*			pSkySoundList;
	float							fFlash;
	UInt32							uiFlashTime;
	UInt32							uiLastMoonPhaseUpdate;
	float							fWindowReflectionTimer;
	float							fAccelBeginPct;
	UInt32							unk114;
	UInt32							uiFlags;
	ImageSpaceModifierInstanceForm* pFadeInIMODCurrent;
	ImageSpaceModifierInstanceForm* pFadeOutIMODCurrent;
	ImageSpaceModifierInstanceForm* pFadeInIMODLast;
	ImageSpaceModifierInstanceForm* pFadeOutIMODLast;
	float							f12_0;
	float							f23_99;
	float							f0_0;

	__forceinline static Sky* GetSingleton() { return *(Sky**)0x11DEA20; }

	__forceinline NiColor* GetSunLightColor()	{ return &kColors[SC_SUNLIGHT]; }
	__forceinline NiColor* GetAmbientColor()	{ return &kColors[SC_AMBIENT]; }

	void RefreshMoon();

	bool GetIsRaining();

	static NiColorA* GetBlendColor(UInt32 auID);

	static void SetIsUnderwater(bool abIsUnderwater) { *(bool*)0x11FF8C4 = abIsUnderwater; };
	static bool IsUnderwater() { return *(bool*)0x11FF8C4; };
};

ASSERT_SIZE(Sky, 0x138);