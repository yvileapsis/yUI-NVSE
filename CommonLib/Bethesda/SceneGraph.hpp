#pragma once

#include "BSSceneGraph.hpp"

NiSmartPointer(SceneGraph);

class SceneGraph : public BSSceneGraph {
public:
	static double __fastcall CalculateNoLODFarDistEx(SceneGraph* apThis);

	static SceneGraph* Create(const char* apName, bool abMenuSceneGraph = false, NiCamera* apCamera = nullptr);

	__forceinline void SetCameraFOV(float afFOV, bool a3, NiCamera* apCamera, bool bSetLODAdjust) {
		ThisStdCall(0xC52020, this, afFOV, a3, apCamera, bSetLODAdjust);
	}
	static __forceinline void UpdateParticleShaderFoVData(float afFOV) {
#if 1
		CdeclCall(0xB54000, afFOV);
#else
		double f1 = afFOV * 0.5f * 0.0175f;
		ParticleShader::SetFOVAdjust((std::sin(f1) / std::cos(f1)) / (std::sin(0.65) / std::cos(0.65)));
#endif
	}

	NiNode* GetFlyCamNode() const {
		return static_cast<NiNode*>(GetAtSafely(0));
	}
};