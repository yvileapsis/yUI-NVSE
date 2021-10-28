#pragma once

#include <Hooks_DirectInput8Create.h>

#include "GameAPI.h"
#include "GameData.h"
#include "GameEffects.h"
#include "GameObjects.h"
#include "GameOSDepend.h"
#include "GameProcess.h"
#include "GameRTTI.h"
#include "GameScript.h"
#include "GameTasks.h"
#include "GameUI.h"

#include "SafeWrite.h"

class BSGameSound;
class BSWin32AudioListener;
class BSAudioManagerThread;

// 254
class BSSoundInfo
{
public:
	BSSoundInfo();
	~BSSoundInfo();

	UInt32			unk000[72];		// 000
	const char* filePath;		// 120
	UInt32			unk124[76];		// 124
};

// 188
class BSAudioManager
{
public:
	BSAudioManager();
	~BSAudioManager();

	enum Volumes
	{
		kMaster = 0,
		kFoot,
		kVoice,
		kEffects,
		kMusic,
		kRadio,
		kVATSCamera,
	};

	virtual void				Destroy(bool doFree);

	struct AudioManagerStruct
	{
		UInt32 type;
		UInt32 unk04;
		UInt32 unk08;
		AudioManagerStruct* ptr0C;
		UInt32 unk10;
		NiPoint3 pos;
	};

	struct UnkC
	{
		UInt32 ptr00;
		UInt32 unk04;
		UInt32 count08;
	};

	struct Unk24
	{
		UInt32 count00;
		UInt8 isInitialized;
		UInt8 gap04[3];
		AudioManagerStruct* ptr08;
		UInt32 time0C;
	};

	enum AudioFlags
	{
		kAudioFlags_2D = 0x1,
		kAudioFlags_3D = 0x2,
		kAudioFlags_UseMissingVoiceAssetIfMissing = 0x4,
		kAudioFlags8 = 0x8,
		kAudioFlags_Loop = 0x10,
		kAudioFlags20 = 0x20,
		kAudioFlags_RandomFrequencyShift = 0x40,
		kAudioFlags80 = 0x80,
		kAudioFlags100 = 0x100,
		kAudioFlags800 = 0x800,
		kAudioFlagsMaybeUnderwater = 0x2000,
		kAudioFlags4000 = 0x4000,
		kAudioFlags10000 = 0x10000,
		kAudioFlags20000 = 0x20000,
		kAudioFlags40000 = 0x40000,
		kAudioFlags_Radio100000 = 0x100000,
		kAudioFlags200000 = 0x200000,
		kAudioFlags_Radio400000 = 0x400000,
		kAudioFlags_Modulated = 0x80000,
		kAudioFlags1000000 = 0x1000000,
		kAudioFlags_EnvelopeFast = 0x2000000,
		kAudioFlags_EnvelopeSlow = 0x4000000,
		kAudioFlags_2DRadius = 0x8000000,
		kAudioFlags20000000 = 0x20000000,
		kAudioFlags40000000 = 0x40000000,
	};

	UInt32 unk004;
	UInt32 unk008;
	BSAudioManager::UnkC unk00C;
	BSAudioManager::UnkC unk018;
	BSAudioManager::Unk24 unk024;
	BSAudioManager::Unk24 unk034;
	BSAudioManager::Unk24 unk044;
	NiTPointerMap<BSGameSound>	playingSounds;		// 054
	NiTPointerMap<BSSoundInfo>	playingSoundInfos1;	// 064
	NiTPointerMap<BSSoundInfo>	playingSoundInfos2;	// 074
	NiTPointerMap<NiAVObject>	soundPlayingObjects;// 084
	DList<BSGameSound>			gameSounds;			// 094
	UInt32						unk0A0;				// 0A0
	UInt32						unk0A4;				// 0A4
	float						flt0A8;				// 0A8
	_RTL_CRITICAL_SECTION criticalSectionAC;
	_RTL_CRITICAL_SECTION criticalSectionC4;
	_RTL_CRITICAL_SECTION criticalSectionDC;
	_RTL_CRITICAL_SECTION criticalSectionF4;
	_RTL_CRITICAL_SECTION criticalSection10C;
	UInt32 unk124;
	UInt32 unk128;
	UInt32 unk12C;
	UInt32 time130;
	UInt8 byte134;
	UInt8 byte135;
	UInt8 gap136[2];
	UInt32 threadID;
	BSAudioManagerThread* audioMgrThread;	// 13C
	float						volumes[12];		// 140
	//	0	Master
	//	1	Foot
	//	2	Voice
	//	3	Effects
	//	4	Music
	//	5	Radio
	UInt32						unk170[4];			// 170
	UInt32						nextMapKey;			// 180
	UInt8 ignoreTimescale;
	UInt8 byte185;
	UInt8 byte186;
	UInt8 byte187;

	static BSAudioManager* GetSingleton() { return (BSAudioManager*)0x11F6EF0; };
};
STATIC_ASSERT(sizeof(BSAudioManager) == 0x188);


class BSWin32Audio
{
public:
	BSWin32Audio();
	~BSWin32Audio();

	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual BSGameSound* CreateGameSound(const char* filePath);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);

	UInt32					unk004[3];		// 004
	BSWin32AudioListener* listener;		// 010
	UInt32					unk014[3];		// 014
	bool					(*sub_82D150)(UInt32*, UInt32*, UInt32*, UInt32*);	// 020
	bool					(*sub_82D280)(UInt32*, UInt32*, UInt32*, UInt32*);	// 024
	bool					(*sub_5E3630)(UInt32*);	// 028
	UInt32(*sub_82D400)(UInt32*, TESSound*, UInt32*);	// 02C
	void					(*sub_832C40)(void);	// 030
	void					(*sub_832C80)(void);	// 034

	static BSWin32Audio* GetSingleton() { return *(BSWin32Audio**)0x11F6D98; };
};