#pragma once

#include "NiNode.hpp"
#include "BSSimpleList.hpp"
#include "BSTempEffectGeometryDecal.hpp"

class BGSDecalNode : public NiNode {
public:
	BGSDecalNode();
	virtual ~BGSDecalNode();

	virtual bool IsDecalNode();
	virtual void AttachDecal(BSTempEffectGeometryDecal* apDecal, bool abFistAvail);

	BSSimpleList<BSTempEffectGeometryDecalPtr>* pDecals;
	bool										bSkinnedNode;

	static void __fastcall OnVisibleEx(BGSDecalNode* apThis, void*, NiCullingProcess* apCuller);
};

ASSERT_SIZE(BGSDecalNode, 0xB4);