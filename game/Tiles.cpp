#include <Tiles.h>
#include <Menus.h>

#include "SafeWrite.h"

NiTMapBase <const char*, int>* g_traitNameMap = reinterpret_cast<NiTMapBase<const char*, int>*>(0x011F32F4);

__declspec(naked) TileValue* __fastcall Tile::GetValue(UInt32 typeID)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, [ecx+0x14]
		xor		esi, esi
		mov		edi, [ecx+0x18]
		ALIGN 16
	iterHead:
		cmp		esi, edi
		jz		iterEnd
		lea		ecx, [esi+edi]
		shr		ecx, 1
		mov		eax, [ebx+ecx*4]
		cmp		[eax], edx
		jz		done
		jb		isLT
		mov		edi, ecx
		jmp		iterHead
		ALIGN 16
	isLT:
		lea		esi, [ecx+1]
		jmp		iterHead
	iterEnd:
		xor		eax, eax
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

Tile* Tile::GetNthChild(UInt32 index)
{
	if (!children.Empty()) if (const auto node = children.GetNthChild(index); node) return node->data;
	return nullptr;
}

Tile* Tile::GetChild(const std::string& childName) const
{
	bool any = false;
	UInt32 index = 0;
	auto tileName = childName;

	if (childName[0] == '*') any = true;
	if (const auto separator = childName.find_last_of(':')) {
		index = atoi(childName.substr(separator + 1).c_str());
		tileName = childName.substr(0, separator - 1);
	}

	for (const auto tile : children) if ((any || tileName == tile->name.CStr()) && !index--) return tile;
	return nullptr;
}

Tile* Tile::GetComponent(const std::string& componentPath)
{
	auto child = this;
	for (const auto word : std::views::split(componentPath, '/'))
	{
		child = child->GetChild(word.data());
		if (!child) return nullptr;
	}
	return child;
}

Tile* Tile::InjectUIXML(const std::filesystem::path& xmlPath, bool ignoreUIO)
{
	if (ignoreUIO) 	// allow hot-reloading the menu even if UIO is installed
	{
		const UInt32 previousResolveXMLFile = DetourRelCall(0xA01B87, 0xA01E20);
		const auto tile = InjectUIXML(xmlPath);
		WriteRelCall(0xA01B87, previousResolveXMLFile);
		return tile;
	}
	return InjectUIXML(xmlPath);
}

Tile* Tile::LookUpRectByName(const char* name)
{
	return ThisCall<Tile*>(0xA03DA0, this, name);
}

Tile* Tile::AddTileFromTemplate(const char* templateName, const char* altName)
{
	const auto tile = GetParentMenu()->AddTileFromTemplate(this, templateName, 0);
	if (altName) tile->name.Set(altName);
	return tile;
}


void Debug_DumpTraits(void)
{
	for (UInt32 i = 0; i < g_traitNameMap->numBuckets; i++)
	{
		for (auto bucket = g_traitNameMap->buckets[i]; bucket; bucket = bucket->next)
		{
			Log() << FormatString("%s,%08X,%d", bucket->key, bucket->data, bucket->data);
		}
	}
}

// not a one-way mapping, so we just return the first
// also this is slow and sucks
const char * Tile::TraitIDToName(int id)
{
	for (UInt32 i = 0; i < g_traitNameMap->numBuckets; i++)
		for (auto bucket = g_traitNameMap->buckets[i]; bucket; bucket = bucket->next)
			if (bucket->data == id)
				return bucket->key;
	return nullptr;
}

void Debug_DumpTileImages(void) {};

TileMenu* Tile::GetTileMenu()
{
	auto tileMenu = this;
	do if IS_TYPE(tileMenu, TileMenu) break;
	while ((tileMenu = tileMenu->parent));
	return reinterpret_cast<TileMenu*>(tileMenu);
}

void Tile::PokeValue(UInt32 valueID)
{
	const auto value = GetValue(valueID);
	if (!value) return;
	const auto oldValue = value->num;
	value->SetFloat(static_cast<Float32>(0x3F800000), 0);
	value->SetFloat(oldValue, 0);
}