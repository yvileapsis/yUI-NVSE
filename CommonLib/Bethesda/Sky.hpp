#pragma once
#include "NiColor.hpp"
#include "BSSimpleList.hpp"

class TESClimate;
class TESWeather;

class Sun;
class Moon;
class Stars;
class Clouds;
class Atmosphere;
class Precipitation;

class ImageSpaceModifierInstanceForm;

NiSmartPointer(Sky);

class Sky 
{
public:
	Sky();
	virtual ~Sky();

	enum EnumSkyObjectType {
		SOT_SUNGLARE	= 0x1,
		SOT_CLOUDS		= 0x3,
		SOT_STARS		= 0x5,
		SOT_MOON		= 0x7,
	};

	enum EnumSkyColors {
		SC_SKY_UPPER	= 0x0,
		SC_FOG			= 0x1,
		SC_UNK			= 0x2,
		SC_AMBIENT		= 0x3,
		SC_SUNLIGHT		= 0x4,
		SC_SUN			= 0x5,
		SC_STARS		= 0x6,
		SC_SKY_LOWER	= 0x7,
		SC_HORIZON		= 0x8,
		SC_MAX			= 0xA,
	};

	struct SkySound {
		UInt32		unk00;		// 00
		UInt32		unk04;		// 04
		UInt32		unk08;		// 08
		TESWeather* pkWeather;	// 0C
		UInt32		uiType;		// 10
		UInt32		uiSoundID;	// 14
	};

	enum EnumSkyMode {
		SM_NONE			= 0x0,
		SM_INTERIOR		= 0x1,
		SM_SKYDOME_ONLY	= 0x2,
		SM_FULL			= 0x3,
	};

	struct COLOR_BLEND
	{
		UInt32	uiRGBVal[4];
		Float32	fBlend[4];
	};

	NiPointer<BSMultiBoundNode> spRoot;
	NiNodePtr		spMoonsRoot;
	TESClimate*		pkCurrentClimate;
	TESWeather*		pkCurrentWeather;
	TESWeather*		pkLastWeather;
	TESWeather*		pkDefaultWeather;
	TESWeather*		pkOverrideWeather;
	Atmosphere*		pkAtmosphere;
	Stars*			pkStars;
	Sun*			pkSun;
	Clouds*			pkClouds;
	Moon*			pkMasser;
	Moon*			pkSecunda;
	Precipitation*	pkPrecipitation;
	NiColor			kColors[SC_MAX];
	NiColor			kColorUnkown0B4;
	NiColor			kColorSunFog;
	Float32			fWindSpeed;
	Float32			fWindAngle;
	Float32			fFogNearPlane;
	Float32			fFogFarPlane;
	UInt32			unk0DC;
	UInt32			unk0E0;
	UInt32			unk0E4;
	Float32			fFogPower;
	Float32			fCurrentGameHour;
	Float32			fLastWeatherUpdate;
	Float32			fCurrentWeatherPct;
	EnumSkyMode		eMode;
	BSSimpleList<SkySound>* pSkySoundList;
	Float32			fFlash;
	UInt32			uiFlashTime;
	UInt32			uiLastMoonPhaseUpdate;
	Float32			fWindowReflectionTimer;
	Float32			fAccelBeginPct;
	UInt32			unk114;
	UInt32			uiFlags;
	ImageSpaceModifierInstanceForm* fadeInIMODCurrent;
	ImageSpaceModifierInstanceForm* fadeOutIMODCurrent;
	ImageSpaceModifierInstanceForm* fadeInIMODLast;
	ImageSpaceModifierInstanceForm* fadeOutIMODLast;
	Float32 f12_0;
	Float32 f23_99;
	Float32 f0_0;

	__forceinline static Sky* GetInstance() { return *(Sky**)0x11DEA20; }

	__forceinline NiColor* GetSunColor() { return &kColors[SC_SUNLIGHT]; }
	__forceinline NiColor* GetAmbientColor() { return &kColors[SC_AMBIENT]; }

	void RefreshMoon();

	bool GetIsRaining();

	static NiColorA* GetBlendColor(UInt32 auID);

	static void SetIsUnderwater(bool abIsUnderwater) { *(bool*)0x11FF8C4 = abIsUnderwater; };
	static bool IsUnderwater() { return *(bool*)0x11FF8C4; };
};
static_assert(sizeof(Sky) == 0x138);