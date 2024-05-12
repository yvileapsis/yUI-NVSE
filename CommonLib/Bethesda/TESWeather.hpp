#pragma once
#include "TESForm.hpp"
#include "TESImageSpaceModifiableCountForm.hpp"
#include "TESTexture1024.hpp"
#include "TESModel.hpp"

class TESWeather :
	public TESForm,
	public TESImageSpaceModifiableCountForm
{
public:
	TESWeather();
	~TESWeather() override;

	struct WeatherSound
	{
		UInt32 soundID;
		UInt32 type;
	};

	enum EnumFogData {
		FD_DAY_NEAR			= 0,
		FD_DAY_FAR			= 1,
		FD_NIGHT_NEAR		= 2,
		FD_NIGHT_FAR		= 3,
		FD_DAY_POWER		= 4,
		FD_NIGHT_POWER		= 5,
		FD_COUNT
	};

	enum EnumWeatherData {
		WD_WIND_SPEED		= 0x0,
		WD_CLOUD_SPEEDY		= 0x1,
		WD_CLOUD_SPEEDX		= 0x2,
		WD_TRANS_DELTA		= 0x3,
		WD_SUN_GLARE		= 0x4,
		WD_SUN_DAMAGE		= 0x5,
		WD_BEGIN_PRECIP		= 0x6,
		WD_END_PRECIP		= 0x7,
		WD_BEGIN_THUNDER	= 0x8,
		WD_END_THUNDER		= 0x9,
		WD_THUNDER_FREQ		= 0xA,
		WD_FLAGS			= 0xB,
		WD_LIGHTNING_R		= 0xC,
		WD_LIGHTNING_G		= 0xD,
		WD_LIGHTNING_B		= 0xE,
		WD_COUNT,
	};

	enum EnumColorType : UInt32 {
		COLOR_SKY_UPPER = 0,
		COLOR_FOG = 1,
		COLOR_CLOUDS_LOWER = 2,
		COLOR_AMBIENT = 3,
		COLOR_SUNLIGHT = 4,
		COLOR_SUN = 5,
		COLOR_STARS = 6,
		COLOR_SKY_LOWER = 7,
		COLOR_HORIZON = 8,
		COLOR_CLOUDS_UPPER = 9,
		COLOR_COUNT,
	};

	enum EnumColorTime : UInt32 {
		TIME_SUNRISE = 0,
		TIME_DAY = 1,
		TIME_SUNSET = 2,
		TIME_NIGHT = 3,
		TIME_HIGH_NOON = 4,
		TIME_MIDNIGHT = 5,
		TIME_COUNT,
	};

	TESTexture1024				kCloudTexture[4];
	UInt8						ucCloudSpeed[4];
	UInt32						uiCloudColorData[4][TIME_COUNT];
	TESModel					kModel;
	UInt8						ucWeatherData[WD_COUNT];
	float						fFogData[FD_COUNT];
	UInt32						uiColorData[COLOR_COUNT][TIME_COUNT];
	BSSimpleList<WeatherSound*> kSoundList;
	float						fHDRInfo[14];
	UInt32						unk238[76]; // Unused
	UInt32						uiNumCloudLayers;

	bool IsRainy() const;
	double GetWeatherData(EnumWeatherData aeData, float afMax, float afMin) const;
	UInt32 GetColor(EnumColorType aeColorType, EnumColorTime aeColorTime) const;
};
static_assert(sizeof(TESWeather) == 0x36C);