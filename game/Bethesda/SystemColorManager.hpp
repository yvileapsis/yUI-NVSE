#pragma once

#include "NiTPointerList.hpp"
#include "BSString.hpp"
#include "NiColor.hpp"
#include "Setting.hpp"

class SystemColor;

class SystemColorManager : public NiTPointerList<SystemColor*> {
public:

	enum SystemColorID {
		SC_HUDMain	= 0x1,
		SC_HUDAlt	= 0x2,
		SC_Terminal = 0x3,
		SC_PipBoy	= 0x4,
	};


	class SystemColor {
	public:
		SystemColor();
		virtual ~SystemColor();
		virtual UInt32 GetColor();
		virtual void SetColor(UInt32 auiColor);
		virtual bool IsHard();
		virtual bool IsSoft();

		BSStringT sTraitName;

		void GetColor(NiColorA& akColor);
		void SetColor(NiColorA& akColor);
	};

	class HardSystemColor : public SystemColor {
	public:
		UInt32 uiColor;
	};
	
	class SoftSystemColor : public SystemColor {
	public:
		Setting* pSetting;
	};

	SInt32	iLastID;

	static SystemColorManager* GetSingleton();

	void RegisterSoftColor(const char* apName, UInt32 auiRed, UInt32 auiGreen, UInt32 auiBlue, Setting* apSetting, SInt32 aiID);
	void RegisterHardColor(const char* apName, UInt32 auiRed, UInt32 auiGreen, UInt32 auiBlue, SInt32 aiID);

	SystemColorManager::SystemColor* LookupByID(SInt32 aiID);
	UInt32 GetColor(SInt32 aiID);
	bool GetColor(SInt32 aiID, NiColorA& akColor);
	bool SetColor(SInt32 aiID, UInt32 auiColor);
	bool SetColor(SInt32 aiID, NiColorA& akColor);
};

ASSERT_SIZE(SystemColorManager, 0x10);