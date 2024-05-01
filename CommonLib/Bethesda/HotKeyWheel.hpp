#pragma once

class TileRect;

struct HotKeyWheel
{
	TileRect* pParent;
	TileRect* pHotkeys[8];
	UInt8 byte24;
	UInt8 gap25[3];
	UInt32 uiSelectedHotkey;
	UInt32 uiSelectedHotkeyTrait;
	UInt32 uiSelectedTextTrait;

	void SetVisible(bool isVisible) { ThisStdCall(0x701760, this, isVisible); };
};