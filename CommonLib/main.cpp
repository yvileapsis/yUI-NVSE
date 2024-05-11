#include <filesystem>
#include <span>
#include "classes.hpp"

#define DYNAMIC_CAST(obj, from, to) ( ## to *) Fallout_DynamicCast((void*)(obj), 0, RTTI_ ## from, RTTI_ ## to, 0)

UInt8* menuVisibility = reinterpret_cast<UInt8*>(0x011F308F);
NiTPrimitiveArray<TileMenu*>* tileMenuArray = reinterpret_cast<NiTPrimitiveArray<TileMenu*>*>(0x011F3508);
// IF requires change of skeleton - and back to false when model updated

bool Menu::IsVisible(const UInt32 menuType) { return menuType >= kType_Min && menuType <= kType_Max ? menuVisibility[menuType] : false; }
bool Menu::IsVisible() const { return IsVisible(eID); }

TileMenu* Menu::GetTileMenu(const UInt32 menuType) { return menuType >= kType_Min && menuType <= kType_Max ? tileMenuArray->GetAt(menuType - kType_Min) : nullptr; }
TileMenu* Menu::GetTileMenu() const { return tileMenuArray->GetAt(eID - kType_Min); }

Menu* Menu::GetMenu(const UInt32 menuType)
{
	const auto tileMenu = GetTileMenu(menuType);
	return tileMenu ? tileMenu->pkParentMenu : nullptr;
}

UInt32 Setting::GetType() const
{
	if (!pKey) return kSetting_Other;

	switch (pKey[0])
	{
	case 'b':
	case 'B': return kSetting_Bool;
	case 'c':
	case 'C': return kSetting_Char;
	case 'i':
	case 'I': return kSetting_Integer;
	case 'u':
	case 'U': return kSetting_Unsigned;
	case 'f':
	case 'F': return kSetting_Float;
	case 's':
	case 'S': return kSetting_String;
	case 'r':
	case 'R': return kSetting_r;
	case 'a':
	case 'A': return kSetting_a;
	default: return kSetting_Other;
	}
}

Float32 Setting::GetAsFloat() const
{
	switch (GetType())
	{
	case kSetting_Bool:
	case kSetting_Integer: return uValue.i;
	case kSetting_Unsigned: return uValue.u;
	case kSetting_Float: return uValue.f;
	default:
		return false;
	}
}

std::string Setting::GetAsString() const
{
	if (GetType() == kSetting_String) return uValue.str;
	return "";
}

bool Setting::Set(double newVal)
{
	switch (GetType())
	{
	case kSetting_Bool:
		uValue.i = newVal ? 1 : 0;
		return true;
	case kSetting_Integer:
		uValue.i = newVal;
		return true;
	case kSetting_Unsigned:
		uValue.u = newVal;
		return true;
	case kSetting_Float:
		uValue.f = newVal;
		return true;
	default:
		return false;
	}
}

bool Setting::Set(const char* str)
{
	if (GetType() == kSetting_String)
	{
		char* newVal = static_cast<char*>(GameHeapAlloc(strlen(str) + 1));
		if (!strcpy_s(newVal, strlen(str) + 1, str)) {
			uValue.str = newVal;
			return true;
		}
	}
	return false;
}

TESForm* TESForm::TryGetREFRParent() const
{
	auto result = this;
	if (const auto refr = DYNAMIC_CAST(this, TESForm, TESObjectREFR); refr && refr->pkObjectReference)
		result = refr->pkObjectReference;
	return const_cast<TESForm*>(result);
}

std::string	Tile::GetFullPath() const 
{
	auto tile = this;

	std::string fullPath = tile->kName.StdStr();

	while ((tile->GetType() != Tile::kValue_tilemenu) && (tile = tile->pkParent)) {
		fullPath = tile->kName.StdStr() + "/" + fullPath;
	}

	return fullPath;
}
