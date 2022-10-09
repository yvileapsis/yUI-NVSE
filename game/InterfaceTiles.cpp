#include <InterfaceTiles.h>
#include <InterfaceMenus.h>
#include <Utilities.h>

typedef NiTMapBase <const char *, int>	TraitNameMap;
TraitNameMap	* g_traitNameMap = (TraitNameMap *)0x011F32F4;

__declspec(naked) Tile::Value* __fastcall Tile::GetValue(UInt32 typeID)
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

Tile * Tile::GetChild(const char * childName)
{
	int childIndex = 0;
	char *colon = strchr(const_cast<char*>(childName), ':');
	if (colon)
	{
		if (colon == childName) return NULL;
		*colon = 0;
		childIndex = atoi(colon + 1);
	}
	Tile *result = NULL;
	for(DListNode<Tile>* node = children.Head(); node; node = node->next)
	{
		if (node && node->data && ((*childName == '*') || !StrCompare(node->data->name.m_data, childName)) && !childIndex--)
		{
			result = node->data;
			break;
		}
	}
	if (colon) *colon = ':';
	return result;
}

// Find a tile or tile value by component path.
// Returns NULL if component path not found.
// Returns Tile* and clears "trait" if component was a tile.
// Returns Tile* and sets "trait" if component was a tile value.
Tile * Tile::GetComponent(const char * componentPath, const char **trait)
{
	Tile *parentTile = this;
	char *slashPos;
	while (slashPos = SlashPos(componentPath))
	{
		*slashPos = 0;
		parentTile = parentTile->GetChild(componentPath);
		if (!parentTile) return NULL;
		componentPath = slashPos + 1;
	}
	if (*componentPath)
	{
		Tile *result = parentTile->GetChild(componentPath);
		if (result) return result;
		*trait = componentPath;
	}
	return parentTile;
}

Tile::Value * Tile::GetComponentValue(const char * componentPath)
{
	const char *trait = NULL;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && trait) ? tile->GetValue(trait) : NULL;
}

Tile * Tile::GetComponentTile(const char * componentPath)
{
	const char *trait = NULL;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && !trait) ? tile : NULL;
}

char *Tile::GetComponentFullName(char *resStr)
{
	if (*(UInt32*)this == 0x106ED44)
		return (char*)memcpy(resStr, name.m_data, name.m_dataLen) + name.m_dataLen;
	char *fullName = parent->GetComponentFullName(resStr);
	*fullName++ = '/';
	fullName = (char*)memcpy(fullName, name.m_data, name.m_dataLen) + name.m_dataLen;
	DListNode<Tile> *node = static_cast<DList<Tile>*>(&parent->children)->Tail();
	while (node->data != this)
		node = node->prev;
	int index = 0;
	while ((node = node->prev) && !strcmp(name.m_data, node->data->name.m_data))
		index++;
	if (index)
	{
		*fullName++ = ':';
		fullName = IntToStr(fullName, index);
	}
	return fullName;
}

void Tile::Dump(void)
{
	PrintLog("%s", name.m_data);
	gLog.Indent();

	PrintLog("values:");

	gLog.Indent();
	
	for(UInt32 i = 0; i < values.size; i++)
	{
		const Value* val = values[i];
		const char* traitName = TraitIDToName(val->id);
		char traitNameIDBuf[16];

		if (!traitName)
		{
			sprintf_s(traitNameIDBuf, "%08X", val->id);
			traitName = traitNameIDBuf;
		}

		if (val->str)			PrintLog("%s: %s", traitName, val->str);
		else if(val->action)	PrintLog("%s: action %08X", traitName, val->action);
		else					PrintLog("%s: %f", traitName, val->num);
	}

	gLog.Outdent();

	for(DListNode<Tile>* node = children.Head(); node; node = node->next)
		if (node && node->data)	node->data->Dump();

	gLog.Outdent();
}

Tile* Tile::LookUpRectByName(const char* name)
{
	return ThisCall<Tile*>(0xA03DA0, this, name);
}

Tile* Tile::AddTileFromTemplate(const char* templateName, const char* altName)
{
	const auto tile = this->GetParentMenu()->AddTileFromTemplate(this, templateName, 0);
	if (altName) tile->name.Set(altName);
	return tile;
}


void Debug_DumpTraits(void)
{
	for(UInt32 i = 0; i < g_traitNameMap->numBuckets; i++)
	{
		for(TraitNameMap::Entry * bucket = g_traitNameMap->buckets[i]; bucket; bucket = bucket->next)
		{
			PrintLog("%s,%08X,%d", bucket->key, bucket->data, bucket->data);
		}
	}
}

// not a one-way mapping, so we just return the first
// also this is slow and sucks
const char * Tile::TraitIDToName(int id)
{
	for(UInt32 i = 0; i < g_traitNameMap->numBuckets; i++)
		for(TraitNameMap::Entry * bucket = g_traitNameMap->buckets[i]; bucket; bucket = bucket->next)
			if(bucket->data == id)
				return bucket->key;

	return nullptr;
}

void Debug_DumpTileImages(void) {};

TileMenu* Tile::GetTileMenu()
{
	auto tilemenu = this;
	do if IS_TYPE(tilemenu, TileMenu) break;
	while ((tilemenu = tilemenu->parent));
	return reinterpret_cast<TileMenu*>(tilemenu);
}

__forceinline Tile::Value* TileGetValue(Tile* tile, UInt32 typeID)
{
	return tile->GetValue(typeID);
}

__declspec(naked) void __fastcall Tile::PokeValue(UInt32 valueID)
{
	__asm
	{
		call	TileGetValue
		test	eax, eax
		jz		done
		push	eax
		push	0
		push	0x3F800000
		mov		ecx, eax
		mov		eax, 0xA0A270
		call	eax
		pop		ecx
		push	0
		push	0
		mov		eax, 0xA0A270
		call	eax
	done :
		retn
	}
}
