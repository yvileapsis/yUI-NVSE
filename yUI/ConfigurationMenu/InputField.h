#pragma once
#include "Containers.h"

struct InputField
{
	// used for filtering which characters are valid
	enum InputType
	{
		kInputType_String = 0,
		kInputType_Integer,
		kInputType_Float
	};

	Tile* tile;
	std::string input;
	bool isActive;
	bool isCaretShown;
	SInt16 caretIndex;
	UInt32 lastCaretUpdateTime;
	UInt8 inputType;

	void Init();
	void Set(std::string str);

	std::string GetText();
	UInt16 GetLen();
	void UpdateCaretDisplay();
	void SetActive(bool active);
	bool IsKeyValid(UInt32 key);
	bool HandleSpecialInputKey(UInt32 key);
	void HandleKeyboardShortcuts(UInt32 key);
	bool HandleKey(UInt32 key);
	void Update();
	void Free();
};
