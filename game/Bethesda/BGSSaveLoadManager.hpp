#pragma once
#include "BSSimpleList.hpp"
#include "BSString.hpp"

class BGSSaveLoadFile;

// 0x38
class BGSSaveLoadManager {
public:
	BGSSaveLoadManager();
	~BGSSaveLoadManager();

	struct SaveGameData {
		const char* pName;
		UInt32		uiUnk04;
		UInt32		uiSaveNumber;
		const char* pPlayerName;
		const char* pPlayerTitle;
		const char* pLocation;
		const char* pTime;
	};

	BSSimpleList<SaveGameData*>*	pSaveList;
	UInt32					uiNumSaves;
	UInt32					uiCurrentSaveGameNumber;
	bool					bSimulatedMode;
	bool					bAutosaveDisabledForDiskspace;
	UInt8					unk0E;
	UInt8					cAutoSaveTimer;
	UInt8					cForceSaveTimer;
	UInt8					cSystemSaveTimer;
	UInt8					unk12;
	UInt8					unk13;
	const char*				pCurrentLoadedSaveName;
	SInt32					iMostRecentSaveGameDeviceID;
	UInt8					unk1C;
	UInt8					unk1D;
	UInt8					pad1E[2];
	BGSSaveLoadFile*		pSaveLoadFile;
	UInt8					unk24;
	UInt8					unk25;
	bool					bIsLoadingGame;
	UInt8					unk27;
	void*					pSysUtilSaveCompleteCallback;
	void*					pFunc2C;
	BSStringT<char>			kSaveName;


	static BGSSaveLoadManager* GetSingleton();


	void GetFullFileName(const char* apFilename, char* apFullPath);
	void GetFullPath(const char* apFilename, char* apFullPath, bool abTemp);
	void GetSaveDirectoryPath(char* apPath);
};
static_assert(sizeof(BGSSaveLoadManager) == 0x38);