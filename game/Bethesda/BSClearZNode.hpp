#pragma once

#include "NiNode.hpp"

class BSCullingProcess;

class BSClearZNode : public NiNode {
public:
	bool  bChangeFarClip;
	bool  bChangeNearClip;
	float fFarClip;
	float fNearClip;

	static BSClearZNode* CreateObject();
	NIRTTI_ADDRESS(0x1201F74);

	static void __fastcall OnVisibleEx(BSClearZNode* apThis, void*, BSCullingProcess* apCuller);
};

ASSERT_SIZE(BSClearZNode, 0xB8);