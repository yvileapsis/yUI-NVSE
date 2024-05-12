#pragma once

#include "NiNode.hpp"
#include "NiCamera.hpp"

class ScreenSplatter {
public:

	static __forceinline NiNode* GetRootNode() { return *(NiNode**)0x11C786C; };

	static __forceinline NiCamera* GetCamera() { return *(NiCamera**)0x11C7840; };

	static __forceinline bool HasNodes() {
		if (!GetRootNode()) return false;
		return GetRootNode()->m_kChildren.m_usESize != 0;
	}
};