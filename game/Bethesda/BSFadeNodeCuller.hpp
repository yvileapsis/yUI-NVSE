#pragma once

#include "NiCullingProcess.hpp"

class BSFadeNodeCuller : public NiCullingProcess {
public:
	BSFadeNodeCuller() {};
	~BSFadeNodeCuller() {};

	NIRTTI_ADDRESS(0x1201F7C)

	const NiRTTI* GetRTTI() const override { return BSFadeNodeCuller::ms_RTTI; };
	void Process(NiAVObject* apObject) override;
	void ProcessAlt(const NiCamera* apCamera, NiAVObject* apScene, NiVisibleArray* apVisibleSet) override;
	void AppendVirtual(NiGeometry* apGeom) override {};
};