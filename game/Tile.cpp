#include <Tile.h>
#include <Menu.h>

#include "Safewrite.hpp"

NiTMapBase <const char*, int>* g_traitNameMap = reinterpret_cast<NiTMapBase<const char*, int>*>(0x011F32F4);

Tile::Value* Tile::GetValue(const TileValueIDs id) const
{
	const auto data = values.data;
	auto iterLeft = 0;
	auto iterRight = values.size;
	while (iterLeft != iterRight)
	{
		const auto addr = (iterLeft + iterRight) >> 1;

		Value* result = data[addr];
		if (result->id == id) return result;

		 result->id < id ? iterLeft = addr + 1 : iterRight = addr;
	}

	return nullptr;
}

Tile* Tile::GetNthChild(const UInt32 index)
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

Tile* Tile::InjectUIXML(const std::filesystem::path& xmlPath, const bool ignoreUIO)
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

std::string Tile::GetFullPath() const
{
	auto tileMenu = this;

	std::string fullPath = std::string(tileMenu->name.CStr());

	while (NOT_TYPE(tileMenu, TileMenu) && (tileMenu = tileMenu->parent)) {
		fullPath = std::string(tileMenu->name.CStr()) + "/" + fullPath;
	} 
	
	return fullPath;
}

// not a one-way mapping, so we just return the first
// also this is slow and sucks
const char* Tile::TraitIDToName(const TileValueIDs id)
{
	for (const auto bucket : *g_traitNameMap)
		if (bucket->data == id)
			return bucket->key;
	return nullptr;
}

TileMenu* Tile::GetTileMenu()
{
	auto tileMenu = this;
	do if IS_TYPE(tileMenu, TileMenu) break;
	while ((tileMenu = tileMenu->parent));
	return reinterpret_cast<TileMenu*>(tileMenu);
}

void Tile::PokeValue(const TileValueIDs id)
{
	const auto value = GetValue(id);
	if (!value) return;
	const auto oldValue = value->num;
	value->SetFloat(static_cast<Float32>(0x3F800000), 0);
	value->SetFloat(oldValue, 0);
}