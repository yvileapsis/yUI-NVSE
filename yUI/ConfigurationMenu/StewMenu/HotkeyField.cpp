#include "HotkeyField.h"


void HotkeyField::Init()
{
	tile = nullptr;
	isActive = false;
	frameDebounce = true;
	value = 0;
}

int HotkeyField::GetPressedKey()
{
	if (frameDebounce) return 0;

	auto input = OSInputGlobals::GetSingleton();
	for (int i = Scancodes::_Escape; i < 265; ++i)
	{
		if (input->GetKeyState(i, OSInputGlobals::isPressed))
		{
			return i;
		}
	}

	return 0;
}

int HotkeyField::Update()
{
	auto key = GetPressedKey();
	if (key > 0)
	{
		if (key == _Escape)
		{
			value = 0;
		}
		else
		{
			value = key;
		}
	}

	frameDebounce = false;
	return key;
}

void HotkeyField::SetActive(bool active)
{
	frameDebounce = active;
	isActive = active;
}

void HotkeyField::Free()
{

}