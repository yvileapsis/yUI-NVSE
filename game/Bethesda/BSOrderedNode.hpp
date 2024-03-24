#pragma once

#include "BSNiNode.hpp"
#include "NiBound.hpp"

class BSCullingProcess;

NiSmartPointer(BSOrderedNode);

class BSOrderedNode : public BSNiNode {
public:
	BSOrderedNode();
	~BSOrderedNode();

	NiBound kStaticBound;
	bool bHasStaticBound;

	CREATE_OBJECT(BSOrderedNode, 0xBC2CB0);

	static void __fastcall OnVisibleEx(BSOrderedNode* apThis, void*, BSCullingProcess* apCuller);
};

ASSERT_SIZE(BSOrderedNode, 0xC0);