#pragma once

#include "BSMemObject.hpp"
#include "BSWin32AudioListener.hpp"

class TESSound;
class BSWin32GameSound;

struct AudioTick {
	UInt32 dword0;
	UInt32 uiLastTick;
	UInt32 uiFlags;
};


class BSAudio : public BSMemObject {
public:
	BSAudio();
	virtual						~BSAudio();
	virtual void				Init(HWND* apWindow);
	virtual void				Shutdown();
	virtual void				Unk_03(int);	// Stub
	virtual UInt32				Unk_04();		// Stub
	virtual BSWin32GameSound*	GetGameSound(const char* apPath);
	virtual void				PrependDataSoundPath(char* apFilename);
	virtual void 				Unk_07();		// Stub

	bool					bReady;
	bool					bIsInitialized;
	bool					bIsMultithreaded;
	DWORD					dword8;
	BYTE					byteC;
	BSWin32AudioListener*	pListener;
	AudioTick				kTimePassed;
	bool(__cdecl*			GetSoundDataFromRefID)(UInt32 refID, char* outFilePath, UInt32* outFlags, TESSound** outSound);
	bool(__cdecl*			GetSoundDataFromEDID)(const char* EDIDstr, char* outFilePath, UInt32* outFlags, TESSound** outSound);
	bool(__cdecl*			PickSoundFileFromFolder)(char* outFilePath);
	UInt32(__cdecl*			FillGameSoundProps)(UInt32* mapKey, TESSound* soundForm, UInt32* outFlags0C);
	void (*					sub_832C40)(void);
	void (*					sub_832C80)(void);
};

ASSERT_SIZE(BSAudio, 0x38);