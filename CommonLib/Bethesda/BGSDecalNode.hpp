#pragma once
#include "NiNode.hpp"
#include "BSSimpleList.hpp"
#include "BSTempEffectGeometryDecal.hpp"

// 0xB4
class BGSDecalNode : public NiNode
{
public:
	BGSDecalNode();
	virtual ~BGSDecalNode();
	virtual bool IsDecalNode();
	virtual void AttachDecal(BSTempEffectGeometryDecal* apDecal, bool abFistAvail);

	BSSimpleList<NiPointer<BSTempEffectGeometryDecal>>* pkDecals;
	bool												bSkinnedNode;
	static void __fastcall OnVisibleEx(BGSDecalNode* apThis, void*, NiCullingProcess* apCuller);
};
static_assert(sizeof(BGSDecalNode) == 0xB4);