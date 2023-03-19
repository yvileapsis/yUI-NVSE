#include "InputField.h"

#include "functions.h"
#include "Menus.h"
#include "Tiles.h"

void InputField::Init()
{
	this->tile = nullptr;
	input.clear();
	isActive = false;
	isCaretShown = false;
	caretIndex = 0;
	lastCaretUpdateTime = 0;
	inputType = kInputType_String;
}

void InputField::Set(std::string str)
{
	input = str;
	if (str.empty())
	{
		tile->SetString(kTileValue_string, "");
		caretIndex = 0;
	}
	else
	{
		caretIndex = str.length();
	}
}

std::string InputField::GetText()
{
	return input;
}

UInt16 InputField::GetLen()
{
	return input.length();
}

void InputField::UpdateCaretDisplay()
{
	static const UInt32 _CaretIndex = Tile::TraitNameToID("_CaretIndex");

	std::string inputString;
	inputString = GetText();
	if (isActive)
	{
		inputString.insert(caretIndex, isCaretShown ? "|" : " ");
		tile->SetFloat(_CaretIndex, caretIndex);
	}
	tile->SetString(kTileValue_string, inputString.c_str());
}

void InputField::SetActive(bool active)
{
	isActive = active;
	lastCaretUpdateTime = 0;
	isCaretShown = false;

	if (tile)
	{
		static auto textAlphaTrait = Tile::TraitNameToID("_IsActive");
		tile->SetFloat(textAlphaTrait, active);
		if (!active)
		{
			UpdateCaretDisplay();
		}
	}
}

bool InputField::IsKeyValid(UInt32 key)
{
	if (key >= kInputCode_Backspace) return false;

	if (inputType != kInputType_String)
	{
		if (key == '-')
		{
			return caretIndex == 0 && GetText()[0] != '-';
		}

		if (inputType == kInputType_Float)
		{
			// only allow one . character
			if (key == '.') return strchr(GetText().c_str(), '.') == nullptr;
		}
		return key >= '0' && key <= '9';
	}

	return key != '\t' && key != '`' && key != '\r' && key != '\n';
}



bool InputField::HandleSpecialInputKey(UInt32 key)
{
	switch (key)
	{
	case kInputCode_Backspace:
	case kInputCode_Delete:
	{
		if (key == kInputCode_Backspace)
		{
			if (IsControlHeld())
			{
//				caretIndex = (input.c_str()).EraseWord(caretIndex);
			}
			else
			{
				input.erase(--caretIndex);
				caretIndex = max(0, caretIndex);
			}
		}
		else
		{
			if (IsControlHeld())
			{
//				input.EraseNextWord(caretIndex);
			}
			else
			{
				input.erase(caretIndex);
				caretIndex = min(input.length(), caretIndex);
			}
		}

		if (input.length())
		{
			tile->SetString(kTileValue_string, this->GetText().c_str());
		}
		else
		{
			tile->SetString(kTileValue_string, "");
		}

		return true;
	}
	case kInputCode_ArrowLeft:
	{
		if (IsControlHeld())
		{
			caretIndex -= GetCharsSinceSpace(input.c_str(), caretIndex);
		}
		else
		{
			--caretIndex;
		}
		caretIndex = max(0, caretIndex);
		return true;
	}
	case kInputCode_ArrowRight:
	{
		if (IsControlHeld())
		{
			caretIndex += GetCharsTillSpace(input.c_str(), caretIndex) + 1;
		}
		else
		{
			++caretIndex;
		}
		caretIndex = min(input.length(), caretIndex);
		return true;
	}
	case kInputCode_Enter:
	{
		SetActive(false);
		return true;
	}
	case kInputCode_Home:
	{
		caretIndex = 0;
		return true;
	}
	case kInputCode_End:
	{
		caretIndex = input.length();
		return true;
	}
	}

	return false;
}

void InputField::HandleKeyboardShortcuts(UInt32 key)
{
	if ((key | 0x20) == 'v')
	{
		// handle pasting from clipboard
		auto clipboardText = GetClipboardText();
		auto len = min(200, clipboardText.length()); // cap paste to 200 characters
		for (int i = 0; i < len; ++i)
		{
			if (char c = clipboardText[i]; IsKeyValid(c))
			{
				input.insert(caretIndex++, 1, c);
			}
		}
	}
}

bool InputField::HandleKey(UInt32 key)
{
	lastCaretUpdateTime = 0;
	isCaretShown = false;

	if (IsKeyValid(key))
	{
		if (IsControlHeld())
		{
			HandleKeyboardShortcuts(key);
		}
		else
		{
			input.insert(caretIndex++, 1, key);
		}

		tile->SetString(kTileValue_string, this->GetText().c_str());
		return true;
	}

	return HandleSpecialInputKey(key);
}

void InputField::Update()
{
	if (isActive)
	{
		auto currentTime = StdCall<UInt32>(0x457FE0);
		if (currentTime - lastCaretUpdateTime > 530)
		{
			isCaretShown = !isCaretShown;
			lastCaretUpdateTime = currentTime;
			UpdateCaretDisplay();
		}
	}
}

void InputField::Free()
{
}