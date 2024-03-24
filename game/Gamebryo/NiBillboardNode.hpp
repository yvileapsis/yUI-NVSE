#pragma once

#include "NiNode.hpp"

NiSmartPointer(NiBillboardNode);

class NiBillboardNode : public NiNode {
public:
	NiBillboardNode();
	virtual ~NiBillboardNode();

	enum FaceMode {
		ALWAYS_FACE_CAMERA,
		ROTATE_ABOUT_UP,
		RIGID_FACE_CAMERA,
		ALWAYS_FACE_CENTER,
		RIGID_FACE_CENTER,
		BSROTATE_ABOUT_UP
	};

	enum {
		FACE_MODE_MASK			= 0x0007,
		FACE_MODE_POS			= 0,
		UPDATECONTROLLERS_MASK	= 0x0008,
	};

	Bitfield16	m_usFlags;
	float		m_fSavedTime;

	CREATE_OBJECT(NiBillboardNode, 0xA7E770);
	NIRTTI_ADDRESS(0x11F4AA0);

	void SetMode(FaceMode eMode);
	FaceMode GetMode() const;

	void RotateToCamera(NiCamera* apCamera);
	static void __fastcall OnVisibleEx(NiBillboardNode* apThis, void*, NiCullingProcess* apCuller);
};

ASSERT_SIZE(NiBillboardNode, 0xB4);