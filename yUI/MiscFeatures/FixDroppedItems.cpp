#include <main.h>
#include <Safewrite.hpp>

#include <SimpleINILibrary.h>
#include <BSExtraData.h>

namespace Fix::DroppedItems
{
	inline int enable = 1;

	TList<TESObjectREFR>::Node* iterDroppedItem;

	TESObjectREFR* __fastcall GetHead(ExtraDataList* extradatalist)
	{
		const auto xDropped = reinterpret_cast<ExtraDroppedItemList*>(extradatalist->GetByType(kExtraData_DroppedItemList));
		if (!xDropped) return nullptr;
		iterDroppedItem = xDropped->itemRefs.Head();
		if (!iterDroppedItem) return nullptr;
		return iterDroppedItem->data;
	}

	TESObjectREFR* __fastcall GetNext()
	{
		iterDroppedItem = iterDroppedItem->next;
		if (!iterDroppedItem) return nullptr;
		return iterDroppedItem->data;
	}

	template <UInt32 retn> __declspec(naked) void HookGetNext()
	{
		static const UInt32 retnAddr = retn;
		static const UInt32 ProcessWeapon = 0x419970;
		static const auto GetNextWeapon = reinterpret_cast<UInt32>(GetNext);
		__asm
		{
			mov eax, [ecx] // dereference
			mov ecx, [eax] // this
			call ProcessWeapon
			call GetNextWeapon
			jmp retnAddr
		}
	}

	void Patch(const bool enable)
	{
		if (enable)
		{
			WriteRelCall(0x75C793, GetHead);
			WriteRelJump(0x75C9AB, HookGetNext<0x75C798>);
		}
		else
		{
			UndoSafeWrite(0x75C793);
			UndoSafeWrite(0x75C9AB);
		}
	}

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() / yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bFixDroppedItems", 1, "; fix the issue where Container Menu would display only a single dropped item at a time");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		Patch(enable);
	}
}
