#pragma once

#include "BSFaceGenMorphData.hpp"

class BSFaceGenMorphDifferential;

class BSFaceGenMorphDataHead : public BSFaceGenMorphData {
public:
	BSFaceGenMorphDataHead();
	virtual ~BSFaceGenMorphDataHead();

	BSFaceGenMorphDifferential** ppExpressions; // 15
	BSFaceGenMorphDifferential** ppAnimations;	// 17
	BSFaceGenMorphDifferential** ppPhonemes;	// 16
	BSFaceGenMorphDifferential** ppCustom;		// 1 - Only vampire morph is present
};

ASSERT_SIZE(BSFaceGenMorphDataHead, 0x18);