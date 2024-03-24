#pragma once

#include "NiMatrix3.hpp"
#include "BSShaderManager.hpp"

namespace NiD3DUtility {
	// 0xB71A40
	inline void GetD3DFromNi(D3DMATRIX& kD3D, const NiMatrix3& kNiRot, const NiPoint3& kNiTrans, float fNiScale) {
		kD3D = (kNiRot * fNiScale).ToD3D();
		kD3D._41 = kNiTrans.x - BSShaderManager::kCameraPos->x;
		kD3D._42 = kNiTrans.y - BSShaderManager::kCameraPos->y;
		kD3D._43 = kNiTrans.z - BSShaderManager::kCameraPos->z;
	}

	// 0xB799A0
	inline void GetD3DFromNi(D3DMATRIX& kD3D, const NiTransform& kNiTransform) {
		GetD3DFromNi(kD3D, kNiTransform.m_Rotate, kNiTransform.m_Translate, kNiTransform.m_fScale);
	}
}