#pragma once
#include "bhkSerializable.hpp"
#include "NiColor.hpp"

class bhkShapeCollection;

enum EnumMaterialType {
	iMaterialType_Stone = 0x0,
	iMaterialType_Cloth = 0x1,
	iMaterialType_Dirt = 0x2,
	iMaterialType_Glass = 0x3,
	iMaterialType_Grass = 0x4,
	iMaterialType_Metal = 0x5,
	iMaterialType_Organic = 0x6,
	iMaterialType_Skin = 0x7,
	iMaterialType_Water = 0x8,
	iMaterialType_Wood = 0x9,
	iMaterialType_HeavyStone = 0xA,
	iMaterialType_HeavyMetal = 0xB,
	iMaterialType_HeavyWood = 0xC,
	iMaterialType_Chain = 0xD,
	iMaterialType_Snow = 0xE,
	iMaterialType_Elevator = 0xF,
	iMaterialType_HollowMetal = 0x10,
	iMaterialType_SheetMetal = 0x11,
	iMaterialType_Sand = 0x12,
	iMaterialType_BrokenConcrete = 0x13,
	iMaterialType_VehicleBody = 0x14,
	iMaterialType_VehiclePartSolid = 0x15,
	iMaterialType_VehiclePartHollow = 0x16,
	iMaterialType_Barrel = 0x17,
	iMaterialType_Bottle = 0x18,
	iMaterialType_SodaCan = 0x19,
	iMaterialType_Pistol = 0x1A,
	iMaterialType_Rifle = 0x1B,
	iMaterialType_ShoppingCart = 0x1C,
	iMaterialType_Lunchbox = 0x1D,
	iMaterialType_BabyRattle = 0x1E,
	iMaterialType_RubberBall = 0x1F,
	iMaterialType_ChainLink = 0x20,
	iMaterialType_Tile = 0x21,
	iMaterialType_Carpet = 0x22,
	iMaterialType_Tumbleweed = 0x23,
	iMaterialType_MAX = 0x24,
};


class bhkShape : public bhkSerializable {
public:
	virtual void CopyMembers(bhkSerializable* apDestination, NiCloningProcess* apCloneProc);							// 49
	virtual bool CalcMass(void* apMassProperty);																		// 50
	virtual bhkShapeCollection* FindShapeCollection(); 																	// 51
	virtual bool CanShare(NiCloningProcess* apCloneProc);																// 52
	virtual void BuildDisplayGeometry(NiNode* apNode, NiColor* apColor);												// 53
	virtual void DestroyDisplayGeometry();																				// 54
	virtual void BuildDisplayFromhkGeometry(NiNode* apNode, void* apGeometry, NiColor* apColor, const char* apName);	// 55

	EnumMaterialType eMaterialType;
};

