#pragma once

#include "NiMemObject.hpp"
#include "NiPoint2.hpp"
#include "NiMatrix3.hpp"

class NiTextureTransform : public NiMemObject {
public:
	enum TransformMethod {
		MAYA_DEPRECATED_TRANSFORM = 0x0,
		MAX_TRANSFORM = 0x1,
		MAYA_TRANSFORM = 0x2,
	};


	NiPoint2			m_kTranslate;
	float				m_fRotate;
	NiPoint2			m_kScale;
	NiPoint2			m_kCenter;
	bool				m_bMatrixDirty;
	NiMatrix3			m_kMatrix;
	TransformMethod		m_eMethod;

	void UpdateMatrix();
};
