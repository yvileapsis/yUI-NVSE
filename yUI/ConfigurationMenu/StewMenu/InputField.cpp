#include "InputField.h"

#include "Menus.h"
#include "Stootils.h"
#include "Tiles.h"

void InputField::Init()
{
	this->tile = nullptr;
	input.m_data = nullptr;
	input.Init(0);
	isActive = false;
	isCaretShown = false;
	caretIndex = 0;
	lastCaretUpdateTime = 0;
	inputType = kInputType_String;
}

void InputField::Set(const char* str)
{
	input.Set(str);
	if (!str || !*str)
	{
		tile->SetString(kTileValue_string, "");
		caretIndex = 0;
	}
	else
	{
		auto len = strlen(str);
		caretIndex = len;
	}
}

const char* InputField::GetText()
{
	return input.CStr();
}

UInt16 InputField::GetLen()
{
	return input.m_dataLen;
}

void InputField::UpdateCaretDisplay()
{
	static const UInt32 _CaretIndex = Tile::TraitNameToID("_CaretIndex");

	String inputString;
	inputString.m_data = nullptr;
	inputString.Init(0);
	inputString.Set(GetText());
	if (isActive)
	{
		inputString.InsertChar(isCaretShown ? '|' : ' ', caretIndex);
		tile->SetFloat(_CaretIndex, caretIndex);
	}
	tile->SetString(kTileValue_string, inputString.CStr());

	inputString.Set("");
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
			return caretIndex == 0 && *GetText() != '-';
		}

		if (inputType == kInputType_Float)
		{
			// only allow one . character
			if (key == '.') return strchr(GetText(), '.') == nullptr;
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
				caretIndex = input.EraseWord(caretIndex);
			}
			else
			{
				input.EraseAt(--caretIndex);
				caretIndex = max(0, caretIndex);
			}
		}
		else
		{
			if (IsControlHeld())
			{
				input.EraseNextWord(caretIndex);
			}
			else
			{
				input.EraseAt(caretIndex);
				caretIndex = min(input.m_dataLen, caretIndex);
			}
		}

		if (input.m_dataLen)
		{
			tile->SetString(kTileValue_string, this->GetText());
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
			caretIndex -= GetCharsSinceSpace(input.m_data, caretIndex);
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
			caretIndex += GetCharsTillSpace(input.m_data, caretIndex) + 1;
		}
		else
		{
			++caretIndex;
		}
		caretIndex = min(input.m_dataLen, caretIndex);
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
		caretIndex = input.m_dataLen;
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
			char c = clipboardText[i];
			if (IsKeyValid(c))
			{
				input.InsertChar(c, caretIndex++);
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
			input.InsertChar(key, caretIndex++);
		}

		tile->SetString(kTileValue_string, this->GetText());
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
	input.Set(nullptr);
}