#pragma once

#include "BSSimpleArray.hpp"
#include "BSResizableTriShape.hpp"
#include "BSDistantTreeShaderProperty.hpp"
#include "BSMap.hpp"

struct BGSTerrainNode;

struct BGSDistantTreeBlock {
	struct InstanceData {
		InstanceData();
		~InstanceData();

		NiPoint3	kPos;
		float		fRotZ;
		float		fScale;
		UInt32		uiFormID;
		UInt32		uiTreeGroupIndex;
		bool		bHidden;
	};

	struct TreeType
	{
		UInt32				uiType;
		NiPoint2			kSize;
		NiPoint2			kPosMin;
		NiPoint2			kPosMax;
		NiTriShapeDataPtr	spTriShapeData;
	};


	class TreeGroup : public BSMemObject {
	public:
		TreeGroup();
		~TreeGroup();

		TreeType*						pTreeType;// ucTreeType?
		BSResizableTriShapePtr			spGeometry;
		BSSimpleArray<InstanceData>		kInstances;
		UInt32							uiNum;
		bool							bShaderPropertyUpToDate;
	};

	void*							pLoadTask;
	BSSimpleArray<TreeGroup*>		kTreeGroups;
	BSMap<UInt32, InstanceData*>	kInstanceMap;
	BSMap<UInt32, TreeGroup*>		kNextGroup;
	BSSimpleArray<UInt32>			kUIntArray;
	BGSTerrainNode*					pNode;
	bool							bDoneLoading;
	bool							bAttached;
	bool							bAllVisible;

	void UpdateShaderPropertyRecurse();
	void UpdateShaderProperty(BSDistantTreeShaderProperty* apShaderProp, TreeGroup* apGroup);
	void Unk_6F9300();
	void HideLOD(UInt32 aID, bool unk3);
	void AddTree(UInt32 auiType, InstanceData& akInstanceData);
	void Attach();
	NiBound* CalculateBound(NiBound& akBound, TreeGroup* apGroup);

	static constexpr UInt32 uiMaxInstanceCount = 114;

	static UInt32 GetMaxInstanceCount();

	static NiAlphaPropertyPtr* const spSharedAlpha;
	static NiStencilPropertyPtr* const spSharedStencil;
	static BSSimpleArray<TreeType>* const pTreeTypes;
};

ASSERT_SIZE(BGSDistantTreeBlock, 0x4C);
ASSERT_SIZE(BGSDistantTreeBlock::InstanceData, 0x20);