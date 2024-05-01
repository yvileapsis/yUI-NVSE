#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// Real name unknown
class BSInputManager {
private:
	LPCDIDATAFORMAT GetMouseDataFormat();
	LPCDIDATAFORMAT GetKeyboardDataFormat();

public:
	enum Scancodes {
		_Escape = 0x1,
		One = 0x2,
		Two = 0x3,
		Three = 0x4,
		Four = 0x5,
		Five = 0x6,
		Six = 0x7,
		Seven = 0x8,
		Eight = 0x9,
		Nine = 0xA,
		Zero = 0xB,
		Minus = 0xC,
		Equals = 0xD,
		Backspace = 0xE,
		Tab = 0xF,
		Q = 0x10,
		W = 0x11,
		E = 0x12,
		R = 0x13,
		T = 0x14,
		Y = 0x15,
		U = 0x16,
		I = 0x17,
		O = 0x18,
		P = 0x19,
		LeftBracket = 0x1A,
		RightBracket = 0x1B,
		Enter = 0x1C,
		LeftControl = 0x1D,
		A = 0x1E,
		S = 0x1F,
		D = 0x20,
		F = 0x21,
		G = 0x22,
		H = 0x23,
		J = 0x24,
		K = 0x25,
		L = 0x26,
		SemiColon = 0x27,
		Apostrophe = 0x28,
		Tilde = 0x29,
		LeftShift = 0x2A,
		BackSlash = 0x2B,
		Z = 0x2C,
		X = 0x2D,
		C = 0x2E,
		V = 0x2F,
		B = 0x30,
		N = 0x31,
		M = 0x32,
		Comma = 0x33,
		Period = 0x34,
		ForwardSlash = 0x35,
		RightShift = 0x36,
		NumStar = 0x37,
		LeftAlt = 0x38,
		SpaceBar = 0x39,
		CapsLock = 0x3A,
		F1 = 0x3B,
		F2 = 0x3C,
		F3 = 0x3D,
		F4 = 0x3E,
		F5 = 0x3F,
		F6 = 0x40,
		F7 = 0x41,
		F8 = 0x42,
		F9 = 0x43,
		F10 = 0x44,
		NumLock = 0x45,
		ScrollLock = 0x46,
		Num7 = 0x47,
		Num8 = 0x48,
		Num9 = 0x49,
		NumMinus = 0x4A,
		Num4 = 0x4B,
		Num5 = 0x4C,
		Num6 = 0x4D,
		NumPlus = 0x4E,
		Num1 = 0x4F,
		Num2 = 0x50,
		Num3 = 0x51,
		Num0 = 0x52,
		NumPeriod = 0x53,
		F11 = 0x57,
		F12 = 0x58,
		NumEnter = 0x9C,
		RightControl = 0x9D,
		NumSlash = 0xB5,
		PrintScrn = 0xB7,
		RightAlt = 0xB8,
		Home = 0xC7,
		UpArrow = 0xC8,
		PageUp = 0xC9,
		LeftArrow = 0xCB,
		RightArrow = 0xCD,
		End_ = 0xCF,
		DownArrow = 0xD0,
		PageDown = 0xD1,
		Delete_ = 0xD3,
	};

	enum MouseScanCodes {
		LeftMouse = 0x0,
		RightMouse = 0x1,
		MiddleMouse = 0x2,
		MouseButton3 = 0x3,
		MouseButton4 = 0x4,
		MouseButton5 = 0x5,
		MouseButton6 = 0x6,
		MouseButton7 = 0x7,
		MouseWheelUp = 0x8,
		MouseWheelDown = 0x9,
		Mouse_MAX = 0xA,
	};

	enum {
		Keyboard = 0,
		Mouse = 1,
		Controller = 3,
	};

	enum KeyState {
		None = -1,
		Held = 0x0,
		Pressed = 0x1,
		Depressed = 0x2,
		Changed = 0x3,
	};


	enum Flags {
		kFlag_HasJoysticks = 1 << 0,
		kFlag_HasMouse = 1 << 1,
		kFlag_HasKeyboard = 1 << 2,
		kFlag_BackgroundMouse = 1 << 3,
	};

	enum {
		kMaxControlBinds = 0x1C,
	};


	enum {
		kMaxDevices = 8,
	};

	struct JoystickObjectsInfo {
		enum {
			kHasXAxis = 1 << 0,
			kHasYAxis = 1 << 1,
			kHasZAxis = 1 << 2,
			kHasXRotAxis = 1 << 3,
			kHasYRotAxis = 1 << 4,
			kHasZRotAxis = 1 << 5
		};

		UInt32	axis;
		UInt32	buttons;
	};

	struct VibrationStates {
		UInt32	uiMotorStrength;
		UInt32	uiPattern;
		UInt32	uiDurationMS;
		UInt32	uiPulseDuration;
		UInt8	byte10;
		UInt32	unk14;
		UInt16	wrd18[2];
		UInt32	unk1C;
		UInt16	wrd20[2];
	};


	struct ControllerVibration {
		struct MotorParams {
			UInt32 duration;
			UInt32 unk04;
			UInt32 unk08;
			UInt32 uiTimePassed;
			UInt32 unk10;
		};


		UInt32				uiControllerIndex;
		VibrationStates*	pLeftStates;
		VibrationStates*	pRightStates;
		MotorParams			LeftParams;
		MotorParams			RightParams;
		UInt32				unk34;
		UInt32				uiTickCount;
		UInt32				unk3C;
	};

	struct MouseData {
		SInt32 xDelta;
		SInt32 yDelta;
		SInt32 iMouseWheelScroll;
		bool   bCurrButtonStates[8];
	};

	bool bIsControllerDisabled;
	UInt8 byte0001;
	UInt8 byte0002;
	UInt8 byte0003;
	Bitfield32 uiFlags;
	LPDIRECTINPUT8A pDirectInput;
	LPDIRECTINPUTDEVICE8A unk000C[8];
	LPDIRECTINPUTDEVICE8A pKeyboard;
	LPDIRECTINPUTDEVICE8A pMouse;
	UInt32 unk0034[8][40];
	UInt32 unk534[1264];
	UInt32 unk18F4;
	UInt8 currKeyStates[256];
	UInt8 lastKeyStates[256];
	DIDEVCAPS mouseCaps;
	MouseData kCurrentMouseData;
	MouseData kLastMouseData;
	BOOL bSwapLeftRightMouseButtons;
	UInt8 cMouseSensitivity;
	UInt8 byte1B51;
	UInt8 byte1B52;
	UInt8 byte1B53;
	UInt32 uiDoubleClickTime;
	UInt8 buttonStates1B58[8];
	UInt32 unk1B60[8];
	ControllerVibration* pControllerVibration;
	VibrationStates* pVibrationProperties;
	UInt8 isControllerEnabled;
	UInt8 byte1B89;
	UInt8 byte1B8A;
	UInt8 byte1B8B;
	UInt32 unk1B8C;
	UInt8 byte1B90;
	UInt8 byte1B91;
	UInt16 usOverrideFlags;
	UInt8 ucKeyBinds[28];
	UInt8 ucMouseBinds[28];
	UInt8 ucJoystickBinds[28];
	UInt8 ucControllerBinds[28];

	static BSInputManager* GetSingleton();

	static LPDIRECTINPUT8A GetDirectInput();
	static LPDIRECTINPUTDEVICE8A GetKeyboard();
	static LPDIRECTINPUTDEVICE8A GetMouse();

	void ToggleMouse(bool abDisable);
	void ToggleKeyboard(bool abDisable);
	void ChangeMouseCooperativeLevel(bool abBackground);
	bool GetButtonState(UInt32 uiInputDevice, UInt8 aucKey, UInt32 auiState);
	SInt32 GetControlState(UInt8 aucKey, UInt32 auiState);
};

ASSERT_SIZE(BSInputManager, 0x1C04);