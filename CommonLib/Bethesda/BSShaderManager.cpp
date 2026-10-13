#include "BSShaderManager.hpp"

// GAME - 0x450B80
ShadowSceneNode* BSShaderManager::GetShadowSceneNode(UInt32 aeType) {
	return reinterpret_cast<ShadowSceneNode**>(0x11F91C8)[aeType];
}