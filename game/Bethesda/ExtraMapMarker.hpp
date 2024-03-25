#pragma once

#include "BSExtraData.hpp"
#include "TESFullName.hpp"

class TESForm;

class ExtraMapMarker : BSExtraData {
public:
	ExtraMapMarker();
	~ExtraMapMarker();

	enum
	{
		kFlag_Visible = 1 << 0,		// shown on the world map
		kFlag_CanTravel = 1 << 1,		// visited, can fast-travel to it
		kFlag_Hidden = 1 << 2,		// does not appear with Explorer perk
	};
	enum
	{
		kType_None = 0,				// this determines the icon on the world map
		kType_City,
		kType_Settlement,
		kType_Encampment,
		kType_NaturalLandmark,
		kType_Cave,
		kType_Factory,
		kType_Memorial,
		kType_Military,
		kType_Office,
		kType_TownRuins,
		kType_UrbanRuins,
		kType_SewerRuins,
		kType_Metro,
		kType_Vault,
	};

	struct Data
	{
		TESFullName fullName;			// not all markers have this
		UInt16 flags;
		UInt16 type;
		TESForm* reputation;			// not all markers have this
	};
	Data* pkData;

	// flag member functions
	bool IsVisible() { return (data->flags & kFlag_Visible) == kFlag_Visible; }
	bool CanTravel() { return (data->flags & kFlag_CanTravel) == kFlag_CanTravel; }
	bool IsHidden() { return (data->flags & kFlag_Hidden) == kFlag_Hidden; }
	void SetVisible(bool visible) { data->flags = (visible) ? (data->flags | kFlag_Visible) : (data->flags & ~kFlag_Visible); }
	void SetCanTravel(bool travel) { data->flags = (travel) ? (data->flags | kFlag_CanTravel) : (data->flags & ~kFlag_CanTravel); }
	void SetHidden(bool hidden) { data->flags = (hidden) ? (data->flags | kFlag_Hidden) : (data->flags & ~kFlag_Hidden); }
};
static_assert(sizeof(ExtraMapMarker) == 0x10);