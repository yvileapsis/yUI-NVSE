#pragma once

#include "BSFadeNode.hpp"
#include "BSTreeModel.hpp"

class NiBillboardNode;

NiSmartPointer(BSTreeNode);

class BSTreeNode : public BSFadeNode {
public:
	BSTreeNode();
	virtual ~BSTreeNode();
	virtual BSTreeModel*		GetTreeModel();
	virtual NiNode*				GetNode0();
	virtual NiNode*				GetNode1();
	virtual NiBillboardNode*	GetBillboardNode();
	virtual NiTriStrips			GetBranch(UInt16 ausID);
	virtual NiTriStrips*		GetLeaf(UInt16 ausID);
	virtual NiNode*				Unk_70();
	virtual bool				SetBranch(UInt16 ausID, NiTriStrips*);
	virtual bool				SetLeaf(UInt16 ausID, NiTriStrips*);
	virtual void				Unk_73(NiAVObject*);
	virtual bool				ToggleNode0(bool abState);
	virtual bool				ToggleNode1(bool abState);
	virtual bool				ToggleBillboardNode(bool abState);
	virtual bool				SetBranchAlpha(UInt16 ausID, UInt8 aucAlphaTestRef);
	virtual bool				SetLeafAlpha(UInt16 ausID, UInt8 aucAlphaTestRef);
	virtual bool				Unk_79(UInt8);
	virtual bool				Unk_80(NiProperty*);
	virtual void				Unk_81(float);

	BSTreeModelPtr			spTreeModel;
	NiPointer<NiTriStrips>* spBranches;
	NiPointer<NiTriStrips>* spLeaves;
	NiRefObjectPtr			spUnkE8;
	float					fDistanceBetweenLODTrees;
};

ASSERT_SIZE(BSTreeNode, 0xF8);