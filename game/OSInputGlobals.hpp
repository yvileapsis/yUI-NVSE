#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define DIRECTSOUND_VERSION 0x0800
#include <dinput.h>

// 1C04
struct OSInputGlobals
{
	enum
	{
		kFlag_HasJoysticks		= 1 << 0,
		kFlag_HasMouse			= 1 << 1,
		kFlag_HasKeyboard		= 1 << 2,
		kFlag_BackgroundMouse	= 1 << 3,
	};


	enum ControlType
	{
		kControlType_Keyboard,
		kControlType_Mouse,
		kControlType_Joystick
	};


	enum KeyState
	{
		isHeld = 0x0,
		isPressed = 0x1,
		isDepressed = 0x2,
		isChanged = 0x3,
	};

	enum ControllerState
	{
		kControllerState_NotHeld = -1,
		kControllerState_IsHeld = 0,
		kControllerState_IsDepressed = 1,
		kControllerState_IsPressed = 2,
	};

	enum ControlCode
	{
		Forward = 0x0,
		Backward = 0x1,
		Left = 0x2,
		Right = 0x3,
		Attack = 0x4,
		Activate = 0x5,
		Aim = 0x6,
		ReadyItem = 0x7,
		Crouch = 0x8,
		Run = 0x9,
		AlwaysRun = 0xA,
		AutoMove = 0xB,
		Jump = 0xC,
		TogglePOV = 0xD,
		MenuMode = 0xE,
		Rest = 0xF,
		VATS_ = 0x10,
		Hotkey1 = 0x11,
		AmmoSwap = 0x12,
		Hotkey3 = 0x13,
		Hotkey4 = 0x14,
		Hotkey5 = 0x15,
		Hotkey6 = 0x16,
		Hotkey7 = 0x17,
		Hotkey8 = 0x18,
		QuickSave = 0x19,
		QuickLoad = 0x1A,
		Grab = 0x1B,
		Escape_ = 0x1C,
		Console = 0x1D,
		Screenshot = 0x1E,
		kControlCode_NONE = 255,
	};

	enum Scancodes
	{
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
		LeftMouse = 0x100,
		RightMouse = 0x101,
		MiddleMouse = 0x102,
		MouseButton3 = 0x103,
		MouseButton4 = 0x104,
		MouseButton5 = 0x105,
		MouseButton6 = 0x106,
		MouseButton7 = 0x107,
		MouseWheelUp = 0x108,
		MouseWheelDown = 0x109,
	};

	enum
	{
		kMaxControlBinds = 0x1C,
	};

	enum XboxControlCode
	{
		kXboxCtrl_DPAD_UP = 0x1,
		kXboxCtrl_DPAD_DOWN = 0x2,
		kXboxCtrl_DPAD_LEFT = 0x3,
		kXboxCtrl_DPAD_RIGHT = 0x4,
		kXboxCtrl_START = 0x5,
		kXboxCtrl_BACK = 0x6,
		kXboxCtrl_LS_BUTTON = 0x7,
		kXboxCtrl_RS_BUTTON = 0x8,
		kXboxCtrl_BUTTON_A = 0x9,
		kXboxCtrl_BUTTON_B = 0xA,
		kXboxCtrl_BUTTON_X = 0xB,
		kXboxCtrl_BUTTON_Y = 0xC,
		kXboxCtrl_BUTTON_LT = 0xD,
		kXboxCtrl_BUTTON_RT = 0xE,
		kXboxCtrl_LB = 0xF,
		kXboxCtrl_RB = 0x10,
		kXboxCtrl_L3 = 0x11,
		kXboxCtrl_R3 = 0x12,
	};

	enum XboxButtonMask
	{
		kXboxMask_DPAD_UP = 1,
		kXboxMask_DPAD_DOWN = 2,
		kXboxMask_DPAD_LEFT = 4,
		kXboxMask_DPAD_RIGHT = 8,
		kXboxMask_START = 0x10,
		kXboxMask_BACK = 0x20,
		kXboxMask_LS_BUTTON = 0x40,
		kXboxMask_RS_BUTTON = 0x80,
		kXboxMask_LB = 0x100,
		kXboxMask_RB = 0x200,
		kXboxMask_GUIDE = 0x400,
		kXboxMask_BUTTON_A = 0x1000,
		kXboxMask_BUTTON_B = 0x2000,
		kXboxMask_BUTTON_X = 0x4000,
		kXboxMask_BUTTON_Y = 0x8000,
	};

	enum InputGlobalsController
	{
		kThumbLX = 0x7,
		kThumbLY = 0x8,
		kThumbRX = 0x9,
		kThumbRY = 0xA,
	};

	enum MouseButtonNames
	{
		kLeftClick = 0,
		kRightClick
	};

	struct Bind
	{
		UInt8 controller;
		UInt8 mouse;
		UInt8 key;
		UInt8 pad03;
	};
	static_assert(sizeof(Bind) == 0x4);

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

	void ResetMouseButtons();
	void ResetPressedControls() { ThisCall(0xA253D0, this); };
	void ResetControlState(UInt8 code);

	bool GetControlState(ControlCode code, KeyState state) { return ThisCall<bool>(0xA24660, this, code, state); };
	void SetControlHeld(ControlCode code) { ThisCall(0xA24280, this, code); };
	bool GetMouseState(MouseButtonNames buttonID, KeyState state) { return ThisCall<bool>(0xA23A50, this, buttonID, state); };
	bool GetKeyState(int key, KeyState state) { return ThisCall<bool>(0xA24180, this, key, state); };
	signed int GetControllerValue(int a1, int key) { return ThisCall<signed int>(0xA23390, this, a1, key); };
	void SetDebounceMenuMode(bool isSet = true)
	{
		if (isSet)
		{
			overrideFlags |= 8;
		}
		else
		{
			overrideFlags &= ~8;
		}
	}
	bool IsDebounceMenuMode()
	{
		return overrideFlags & 8;
	}

	void SetBind(ControlCode code, Bind bind)
	{
		keyBinds[code] = bind.key;
		mouseBinds[code] = bind.mouse;
		controllerBinds[code] = bind.controller;
	}

	void Unbind(ControlCode code)
	{
		Bind unbound;
		unbound.key = 0xFF;
		unbound.mouse = 0xFF;
		unbound.controller = 0xFF;
		unbound.pad03 = 0xFF;
		SetBind(code, unbound);
	}

	enum ERumblePattern
	{
		eRumblePattern_Constant = 0x0,
		eRumblePattern_Square = 0x1,
		eRumblePattern_Triangle = 0x2,
		eRumblePattern_Sawtooth = 0x3,
	};

	static ControllerState GetControllerState(XboxControlCode button, bool includeThumbsticks = 1);
	void ResetRumble();
	void Rumble(float left, float right, int durationMS, int pulseDuration, ERumblePattern pattern, bool a7, bool isForce);

};
static_assert(sizeof(OSInputGlobals) == 0x1C04);
static_assert(offsetof(OSInputGlobals, mouseBinds) == 0x1BB0);