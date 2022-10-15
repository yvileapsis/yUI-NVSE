#include <functions.h>
#include <Utilities.h>
#include <GameProcess.h>
#include <GameAPI.h>
#include <GameData.h>
#include <GameRTTI.h>
#include <SafeWrite.h>
#include <algorithm>

#include "GameObjects.h"
#include <InterfaceMenus.h>

extern DataHandler* g_dataHandler;

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

void FillCraftingComponents()
{
	for (const auto mIter : g_dataHandler->recipeList)
	{
		for (const auto node : mIter->inputs)
			if (node && node->item) g_CraftingComponents.emplace(node->item);
		for (const auto node : mIter->outputs)
			if (node && node->item) g_CraftingProducts.emplace(node->item);
	}
}

bool IsCraftingComponent(TESForm* form)
{
	return g_CraftingComponents.contains(form);
}

bool IsCraftingProduct(TESForm* form)
{
	return g_CraftingProducts.contains(form);
}

TESForm* GetRefFromString(char* mod, char* id)
{
	const auto itemID = (g_dataHandler->GetModIndex(mod) << 24) | strtoul(id, nullptr, 0);
	return LookupFormByRefID(itemID);
}

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

void PatchPause(UInt32 ptr)
{
	SafeWriteBuf(ptr, "\xEB\xFE", 2);
}

void SetUIStringFull(const char* tochange, const char* changeto, UInt32 tileValue)
{
	HUDMainMenu::GetSingleton()->tile->SetStringRecursive(tileValue, changeto, tochange);
}

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


void Tile::SetStringRecursive(const UInt32 tileValue, const char* changeto, const char* tochange) {
	if (GetValue(tileValue))
		SetString(tileValue, changeto, true);
	for (const auto iter : this->children)
		iter->SetStringRecursive(tileValue, changeto, tochange);
}


char* __fastcall StrFromINI(DWORD *address)
{
	return address ? reinterpret_cast<char*>(address[1]) : nullptr;
}

void (*RegTraitID)(const char*, UInt32) = (void (*)(const char*, UInt32))0x9FF8A0;
void RegisterTraitID(const char* var1, UInt32 var2) { RegTraitID(var1, var2);  }

void purefun()
{
	PrintConsoleOrQueue("AAAAA");
}

__declspec(naked) void funpatch()
{
	static const UInt32 retnAddr = 0xA095D8;
	static const auto purefun2 = reinterpret_cast<UInt32>(purefun);
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

void PatchAddyCMToSettingsMenu()
{

	WriteRelCall(0x7CC9D4, UInt32(AddyCMToSettingsMenu));
	WriteRelJump(0x7CCA43, 0x7CCAAD);

	//	SafeWriteBuf(0x7CBF77

	//	SafeWriteBuf(0x7CBF8C, "\x66\x0F\x1F\x44\x00\x00", 6);//
	//	SafeWriteBuf(0x7CB674, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB686, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB698, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6AA, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6BC, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6CE, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6E0, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CBE50, "\x81\x7A\x08\x50\x05\x7D\x00", 7);

	/*	SafeWriteBuf(0x7CC044, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC392, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC23E, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC0C3, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC044, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC416, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC582, "\x0F\x1F\x44\x00\x00", 5);*/
		//	SafeWriteBuf(0x7CBF59, "\x0F\x1F\x44\x00\x00", 5);
		//	SafeWriteBuf(0x7CC01F, "\x0F\x1F\x44\x00\x00", 5);

}
#endif