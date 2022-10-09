#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define DIRECTSOUND_VERSION 0x0800
#include <dinput.h>
//#include <dsound.h>

// 1C00 (1.0) / 1C04 (1.1)
struct OSInputGlobals
{
	enum
	{
		kFlag_HasJoysticks		= 1 << 0,
		kFlag_HasMouse			= 1 << 1,
		kFlag_HasKeyboard		= 1 << 2,
		kFlag_BackgroundMouse	= 1 << 3,
	};

	enum
	{
		kMaxControlBinds = 0x1C,
	};
	UInt8 isControllerDisabled;
	UInt8 byte0001;
	UInt8 byte0002;
	UInt8 byte0003;
	UInt32 flags;
	void* directInput;
	UInt32 unk000C;
	UInt32 unk0010;
	UInt32 unk0014;
	UInt32 unk0018;
	UInt32 unk001C;
	UInt32 unk0020;
	UInt32 unk0024;
	UInt32 unk0028;
	void* unk002C;
	void* unk0030;
	UInt32 unk0034[320];
	UInt32 unk534[1264];
	UInt32 unk18F4;
	UInt8 currKeyStates[256];
	UInt8 lastKeyStates[256];
	UInt32 unk1AF8;
	UInt32 unk1AFC;
	UInt32 unk1B00;
	UInt32 unk1B04;
	UInt32 numMouseButtons;
	UInt32 unk1B0C;
	UInt32 unk1B10;
	UInt32 unk1B14;
	UInt32 unk1B18;
	UInt32 unk1B1C;
	UInt32 unk1B20;
	int xDelta;
	int yDelta;
	int mouseWheelScroll;
	UInt8 currButtonStates[8];
	int lastxDelta;
	int lastyDelta;
	int lastMouseWheelScroll;
	UInt8 lastButtonStates[8];
	UInt32 swapLeftRightMouseButtons;
	UInt8 mouseSensitivity;
	UInt8 byte1B51;
	UInt8 byte1B52;
	UInt8 byte1B53;
	UInt32 doubleClickTime;
	UInt8 buttonStates1B58[8];
	UInt32 unk1B60[8];
	UInt32* controllerVibration;
	UInt32 unk1B84;
	UInt8 isControllerEnabled;
	UInt8 byte1B89;
	UInt8 byte1B8A;
	UInt8 byte1B8B;
	UInt32 unk1B8C;
	UInt8 byte1B90;
	UInt8 byte1B91;
	UInt16 overrideFlags;
	UInt8 keyBinds[28];
	UInt8 mouseBinds[28];
	UInt8 joystickBinds[28];
	UInt8 controllerBinds[28];

	static OSInputGlobals* GetSingleton() { return *reinterpret_cast<OSInputGlobals**>(0x011F35CC); }
};
static_assert(sizeof(OSInputGlobals) == 0x1C04);
static_assert(offsetof(OSInputGlobals, mouseBinds) == 0x1BB0);

#if 0
#include "GameTypes.h"

class TESGameSound;
class NiAVObject;

// 58
class TESGameSound
{
public:
	TESGameSound();
	~TESGameSound();

	UInt32			unk00[3];	// 00
	UInt32			hashKey;	// 0C
	UInt32			unk10[4];	// 10
	float			x;			// 20
	float			y;			// 24
	float			z;			// 28
	UInt32			unk2C[4];	// 2C
	float			unk3C;		// 3C
	UInt32			unk40[3];	// 40
	const char *	name;		// 4C
	UInt32			unk50;		// 50
	UInt32			unk54;		// 54
};

// 328
class OSSoundGlobals
{
public:
	OSSoundGlobals();
	~OSSoundGlobals();

	enum
	{
		kFlags_HasDSound =		1 << 0,
		kFlags_HasHardware3D =	1 << 2,
	};
	
	typedef NiTPointerMap <TESGameSound>	TESGameSoundMap;
	typedef NiTPointerMap <NiAVObject>		NiAVObjectMap;

	UInt32					unk000;						// 000
	UInt32					unk004;						// 004
	IDirectSound8			* dsoundInterface;			// 008
	IDirectSoundBuffer8		* primaryBufferInterface;	// 00C
	DSCAPS					soundCaps;					// 010
	UInt32					unk070;						// 070
	UInt32					unk074;						// 074
	IDirectSound3DListener	* listenerInterface;		// 078
	UInt32					unk07C[(0x0A4-0x07C) >> 2];	// 07C
	UInt8					unk0A4;						// 0A4
	UInt8					unk0A5;						// 0A5
	UInt8					unk0A6;						// 0A6
	UInt8					pad0A7;						// 0A7
	UInt32					unk0A8;						// 0A8
	UInt32					flags;						// 0AC - flags?
	UInt32					unk0B0;						// 0B0
	float					unk0B4;						// 0B4
	float					masterVolume;				// 0B8
	float					footVolume;					// 0BC
	float					voiceVolume;				// 0C0
	float					effectsVolume;				// 0C4
	UInt32					unk0C8;						// 0C8 - time
	UInt32					unk0CC;						// 0CC - time
	UInt32					unk0D0;						// 0D0 - time
	UInt32					unk0D4[(0x0DC-0x0D4) >> 2];	// 0D4
	UInt32					unk0DC;						// 0DC
	UInt32					unk0E0[(0x2F0-0x0E0) >> 2];	// 0E0
	float					musicVolume;				// 2F0
	UInt32					unk2F4;						// 2F4
	float					musicVolume2;				// 2F8
	UInt32					unk2FC;						// 2FC
	TESGameSoundMap			* gameSoundMap;				// 300
	NiAVObjectMap			* niObjectMap;				// 304
	NiTPointerList <void>	* soundMessageMap;			// 308 - AudioManager::SoundMessage *
	UInt32					unk30C[(0x320-0x30C) >> 2];	// 30C
	void					* soundMessageList;			// 320
	UInt32					unk324;						// 324
};

static_assert(sizeof(OSSoundGlobals) == 0x328);
#endif

class OSSoundGlobals {
};

// A4
class OSGlobals
{
public:
	OSGlobals();
	~OSGlobals();

	UInt8			oneMore;			// 00
	UInt8			quitGame;			// 01	// The seven are initialized to 0, this one is set by QQQ
	UInt8			exitToMainMenu;		// 02
	UInt8			unk03;				// 03
	UInt8			unk04;				// 04
	UInt8			unk05;				// 05
	UInt8			unk06;				// 06	// This looks promising as TFC bool byte
	UInt8			unk07;				// 07
	HWND			window;				// 08
	HINSTANCE		procInstance;		// 0C
	UInt32			mainThreadID;		// 10
	HANDLE			mainThreadHandle;	// 14
	UInt32*			unk18;				// 18 ScrapHeapManager::Buffer*
	UInt32			unk1C;				// 1C
	OSInputGlobals	* input;			// 20
	OSSoundGlobals	* sound;			// 24
	UInt32			unk28;				// 28 relates to unk18
	//...
	UInt32*			unk50;				// 50, same object as unk18
	//..
	UInt32			unk60;				// 60 relates to unk50

	OSGlobals* GetSingleton() { return *reinterpret_cast<OSGlobals**>(0x011DEA0C); }
};

//static_assert(sizeof(OSGlobals) == 0x0A4);	// found in oldWinMain 0x0086AF4B

extern OSGlobals ** g_osGlobals;
