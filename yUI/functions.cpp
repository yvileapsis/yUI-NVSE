#include <functions.h>
#include <main.h>

#include <unordered_set>

#include "dinput8.h"
#include "GameData.h"
#include "Menus.h"


namespace CraftingComponents
{
	std::unordered_set<TESForm*> g_Components;
	std::unordered_set<TESForm*> g_Products;

	void Fill()
	{
		for (const auto mIter : g_TESDataHandler->recipeList)
		{
			for (const auto node : mIter->inputs)
				if (node->item) g_Components.emplace(node->item);
			for (const auto node : mIter->outputs)
				if (node->item) g_Products.emplace(node->item);
		}
	}

	bool IsComponent(TESForm* form) { return g_Components.contains(form); }

	bool IsProduct(TESForm* form) { return g_Products.contains(form); }
}

namespace HideInfoPrompt
{
	UInt32 shouldHide = 0;

	void Hide()
	{
		shouldHide++;
	}

	void Show()
	{
		if (shouldHide > 0) shouldHide--;
	}

	void Update()
	{
		g_HUDMainMenu->tileInfo->SetFloat(kTileValue_visible, !shouldHide);
	}
}

/*
__declspec(naked) void UIWidth()
{
	static const UInt32 retnAddr = 0x715D8D;
	__asm
	{
		fld[ebp - 0x4]
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight()
{
	static const UInt32 retnAddr = 0x715DED;
	__asm
	{
		fld[ebp - 0x4]
		jmp		retnAddr
	}
}

double f1920 = 1920.0;
double f1080 = 1080.0;

__declspec(naked) void UIWidth2()
{
	static const UInt32 retnAddr = 0x71303D;
	__asm
	{
		fld		ds : f1920
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight2()
{
	static const UInt32 retnAddr = 0x713046;
	__asm
	{
		fld		ds : f1080
		jmp		retnAddr
	}
}

__declspec(naked) void UIWidth3()
{
	static const UInt32 retnAddr = 0x7FBBE2;
	__asm
	{
		fld		ds : f1920
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight3()
{
	static const UInt32 retnAddr = 0x7FBBEB;
	__asm
	{
		fld		ds : f1080
		jmp		retnAddr
	}
}
*/

bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle)
{
	const auto it = ra::search(strHaystack, strNeedle, [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin();
	return it != strHaystack.end();
}

bool IsPlayersOtherAnimData(AnimData* animData)
{
	if (PlayerCharacter::GetSingleton()->IsThirdPerson() && animData == PlayerCharacter::GetSingleton()->firstPersonAnimData)
		return true;
	if (!PlayerCharacter::GetSingleton()->IsThirdPerson() && animData == PlayerCharacter::GetSingleton()->baseProcess->GetAnimData())
		return true;
	return false;
}

AnimData* GetThirdPersonAnimData(AnimData* animData)
{
	if (animData == PlayerCharacter::GetSingleton()->firstPersonAnimData)
		return PlayerCharacter::GetSingleton()->baseProcess->GetAnimData();
	return animData;
}

/*
void PatchPause(UInt32 ptr)
{
	SafeWriteBuf(ptr, "\xEB\xFE", 2);
}

void SetUIStringFull(const char* tochange, const char* changeto, UInt32 tileValue)
{
	HUDMainMenu::GetSingleton()->tile->SetStringRecursive(tileValue, changeto, tochange);
}
*/

bool TryGetTypeOfForm(TESForm* form)
{
	__try {
		const auto whaa = form->typeID && form->refID;
		return whaa;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

/*
void Tile::SetStringRecursive(const UInt32 tileValue, const char* changeto, const char* tochange) {
	if (GetValue(tileValue))
		SetString(tileValue, changeto, true);
	for (const auto iter : this->children)
		iter->SetStringRecursive(tileValue, changeto, tochange);
}
*/

char* __fastcall StrFromINI(DWORD *address)
{
	return address ? reinterpret_cast<char*>(address[1]) : nullptr;
}

void (*RegTraitID)(const char*, UInt32) = (void (*)(const char*, UInt32))0x9FF8A0;
void RegisterTraitID(const char* var1, UInt32 var2) { RegTraitID(var1, var2);  }


__declspec(naked) void funpatch()
{
	static const UInt32 retnAddr = 0xA095D8;
	static const auto purefun2 = 0;//reinterpret_cast<UInt32>(purefun);
	__asm
	{
		cmp [ebp - 0x30], 2032
		jne welp
		call purefun2
	welp :
		cmp [ebp - 0x30], 0x7E8
		jmp retnAddr
	}
}

#if 0

#include <patches.h>
#include <SafeWrite.h>
#include <functions.h>


float __cdecl WeaponSpread(float a1, float a2)
{
	return a2;
};

float __fastcall MyFunc2(Actor* actor, void* edx, int i)
{
	return 0;
}

/*
	RegisterTraitID("&runsnig;", 2032);
	RegisterTraitID("runsnig", 2032);
	WriteRelJump(0xA095D1, reinterpret_cast<UInt32>(funpatch));
*/

void Patch1080pUI()
{
	//	WriteRelJump(0x715D5F, 0x715D8D);
	//	WriteRelJump(0x715DAE, 0x715DED);

	//	WriteRelJump(0x715DE7, testetest);

	//	WriteRelCall(0x76C0F1, ret1080);
	//	WriteRelCall(0x76C0FE, ret1920);
		//	WriteRelCall(0x70B9CE, UInt32(Tile_SetStringValueCursor));
	//	WriteRelCall(0x70B320, UInt32(Tile_SetStringValueCursor));
	//	WriteRelCall(0x70CC1F, UInt32(Tile_SetStringValueCursor));

		/*
		WriteRelJump(0x715D54, UInt32(UIWidth));
		WriteRelCall(0x715DA6, 0x4DC200);
		WriteRelJump(0x715DB4, UInt32(UIHeight));


		WriteRelJump(0x713037, UInt32(UIWidth2));
		WriteRelJump(0x713040, UInt32(UIHeight2));
		WriteRelJump(0x7FBBDC, UInt32(UIWidth3));
		WriteRelJump(0x7FBBE5, UInt32(UIHeight3));
		*/

		/*
		SafeWriteBuf(0x106EC38, "\x00\x00\xF0\x44", 4); //1280
		SafeWriteBuf(0x106F2DC, "\x00\x00\x87\x44", 4); //960
		SafeWriteBuf(0x106E960, "\x00\x00\x00\x00\x00\x00\x9E\x40", 8); //1280
		SafeWriteBuf(0x106E7F8, "\x00\x00\x00\x00\x00\xe0\x90\x40", 8); //960
		SafeWriteBuf(0x1021790, "\x1D\xC7\x71\x1C\xC7\x71\xFC\x3F", 8); //1.3333
		SafeWriteBuf(0x1078128, "\x1D\xC7\x71\x1C\xC7\x71\xFC\x3F", 8); //1.3333
		SafeWriteBuf(0x713040, "\xDD\x05\xF8\xE7\x06\x01", 6);

		*/

		//	`DC 0D 60 E9 06 01`
		//	`D9 05 DC F2 06 01`
		//	SafeWriteBuf(0x11A0190, "\x60\x05\x00\x00", 4); //960


		//	SafeWriteBuf(0x106F070, "\x00\x00\x00\x00\x00\x60\x78\x40", 8);
			//	SafeWriteBuf(0x1021790, "\x00\x00\x00\x00\x00\x00\xF8\x3F", 8);

			//01189488 0118948C 01189490 01189494 01189498

		//	SafeWriteBuf(0x71302B, "\x66\x0F\x1F\x44\x00\x00", 6);
		//	SafeWriteBuf(0x713031, "\x66\x0F\x1F\x44\x00\x00", 6);
		//	SafeWriteBuf(0x713037, "\xD9\x05\x38\xEC\x06\x01", 6);
			// sub_E68A80 Tile_A04640
}

#endif

typedef bool (*_JG_WorldToScreen)(NiPoint3* posXYZ, NiPoint3& posOut, float offscreenHandling);
_JG_WorldToScreen JG_WorldToScreen;

bool WorldToScreen(NiPoint3* posXYZ, NiPoint3& posOut, float offscreenHandling)
{
	if (!JG_WorldToScreen)
	{
		HMODULE jg = GetModuleHandle("johnnyguitar");
		if (jg) JG_WorldToScreen = (_JG_WorldToScreen)GetProcAddress(jg, "JG_WorldToScreen");
		if (!JG_WorldToScreen) return false;
	}
	return JG_WorldToScreen(posXYZ, posOut, offscreenHandling);
}

bool IsKeyPressed(UInt32 key, UInt32 flags)
{
	return g_DIHook->IsKeyPressed(key, flags);
}

void DisableKey(UInt32 key, bool disable, UInt32 mask)
{
	g_DIHook->SetKeyDisableState(key, disable, mask);
}

Script* LNIsButtonPressed;
bool IsButtonPressed(UInt32 button)
{
	if (!LNIsButtonPressed) LNIsButtonPressed = CompileScript(R"(Begin Function { int button }
		SetFunctionValue (IsButtonPressed button)
	end)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(LNIsButtonPressed, nullptr, nullptr, &element, 1, button);
	return element.num;
}

UInt32 GetControl(UInt32 whichControl, UInt32 type)
{
	const auto globs = OSInputGlobals::GetSingleton();

	if (whichControl >= globs->kMaxControlBinds)
		return 0xFF;

	UInt32	result;

	switch (type)
	{
	case OSInputGlobals::kControlType_Keyboard:
		result = globs->keyBinds[whichControl];
		break;

	case OSInputGlobals::kControlType_Mouse:
		result = globs->mouseBinds[whichControl];

		if (result != 0xFF) result += 0x100;
		break;

	case OSInputGlobals::kControlType_Joystick:
		result = globs->joystickBinds[whichControl];
		break;

	default:
		result = 0xFF;
		break;
	}

	return result;
}

NVSEArrayElement nativeHandlerFunctionValue{};
void SetNativeHandlerFunctionBool(bool trueorfalse)
{
	nativeHandlerFunctionValue.dataType = NVSEArrayVarInterface::kType_Numeric;
	nativeHandlerFunctionValue.num = trueorfalse;
	SetHandlerFunctionValue(nativeHandlerFunctionValue);
}

Script* JIPToggleVanityWheel;
UInt32 levelOfToggle;
bool ToggleVanityWheel(bool toggle)
{
	if (toggle && levelOfToggle > 0) levelOfToggle--;
	if (!toggle) levelOfToggle++;
	if (!JIPToggleVanityWheel) JIPToggleVanityWheel = CompileScript(R"(Begin Function { int toggle }
		ToggleVanityWheel toggle
	end)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPToggleVanityWheel, nullptr, nullptr, &element, 1, !levelOfToggle);
	return element.num;
}

/*
	bool s_vanityEnabled = true;
	s_vanityEnabled = !s_vanityEnabled;
	SafeWrite8(0x945A29, toggle ? 0x8B : 0x89);
*/

Script* JIPGetAuxVarOrDefault;
Float32 GetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 def)
{
	if (!JIPGetAuxVarOrDefault) JIPGetAuxVarOrDefault = CompileScript(R"(Begin Function { string_var auxvar, int index, float def }
		if PlayerRef.AuxiliaryVariableGetType (auxvar) index
			SetFunctionValue (PlayerRef.AuxiliaryVariableGetFloat (auxvar) index)
		else
			SetFunctionValue def
		endif
	End)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPGetAuxVarOrDefault, nullptr, nullptr, &element, 3, auxvar, index, *(UInt32*)&def);
	return element.num;
}

Script* JIPSetAuxVarOrDefault;
Float32 SetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 value)
{
	if (!JIPSetAuxVarOrDefault) JIPSetAuxVarOrDefault = CompileScript(R"(Begin Function { string_var auxvar, int index, float value }
		PlayerRef.AuxiliaryVariableSetFloat (auxvar) value index
	End)");
	NVSEArrayElement element;
	g_scriptInterface->CallFunction(JIPSetAuxVarOrDefault, nullptr, nullptr, &element, 3, auxvar, index, *(UInt32*)&value);
	return element.num;
}

TList<void*> activationPromptTList;
bool GetCannibalPrompt(TESObjectREFR* ref)
{
	activationPromptTList.RemoveAll();
	activationPromptTList.Init();
	ApplyPerkModifiers(0x1B, g_player, ref, &activationPromptTList, ref);
	return !activationPromptTList.Empty();
}


std::string GetStringForButton(UInt32 button)
{
	switch (button)
	{
	case 1 << 0: return "Up)";
	case 1 << 1: return "Down)";
	case 1 << 2: return "Left)";
	case 1 << 3: return "Right)";
	case 1 << 4: return "Start)";
	case 1 << 5: case 1 << 6: case 1 << 7: case 1 << 8: case 1 << 9:
	case 1 << 11: case 1 << 12: case 1 << 13: case 1 << 14:	return "";
	case 1 << 10: return "Guide)";
	default: return std::to_string(button);
	}
}