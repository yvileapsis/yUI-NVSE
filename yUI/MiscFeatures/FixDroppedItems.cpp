#include <main.h>
#include <GameExtraData.h>
#include <SafeWrite.h>

#include <SimpleINILibrary.h>

namespace Fix::DroppedItems
{
	inline int bEnable = 1;

	TList<TESObjectREFR>::Node* iterDroppedItem;


	void HandleINIs()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		const auto iniPath = GetCurPath() + yUI_INI;
		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		bEnable = ini.GetOrCreate("General", "bFixDroppedItems", 1, "; fix the issue where Container Menu would display only a single dropped item at a time");

		ini.SaveFile(iniPath.c_str(), false);
	}

	TESObjectREFR* __fastcall GetDroppedWeaponHead(ExtraDataList* extradatalist)
	{
		const auto xDropped = reinterpret_cast<ExtraDroppedItemList*>(extradatalist->GetByType(kExtraData_DroppedItemList));
		if (!xDropped) return nullptr;
		iterDroppedItem = xDropped->itemRefs.Head();
		if (!iterDroppedItem) return nullptr;
		return iterDroppedItem->data;
	}

	TESObjectREFR* __fastcall GetDroppedWeaponNext()
	{
		iterDroppedItem = iterDroppedItem->next;
		if (!iterDroppedItem) return nullptr;
		return iterDroppedItem->data;
	}

	template <UInt32 retn> __declspec(naked) void HookGetDroppedWeaponNext()
	{
		static const UInt32 retnAddr = retn;
		static const UInt32 ProcessWeapon = 0x419970;
		static const auto GetNextWeapon = reinterpret_cast<UInt32>(GetDroppedWeaponNext);
		__asm
		{
			mov eax, [ecx] // dereference
			mov ecx, [eax] // this
			call ProcessWeapon
			call GetNextWeapon
			jmp retnAddr
		}
	}

	void Patch(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelCall(0x75C793, GetDroppedWeaponHead);
			WriteRelJump(0x75C9AB, HookGetDroppedWeaponNext<0x75C798>);
		}
		else
		{
			UndoSafeWrite(0x75C793);
			UndoSafeWrite(0x75C9AB);
		}
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		Patch(bEnable);
	}
}