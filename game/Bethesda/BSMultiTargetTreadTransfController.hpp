#pragma once

#include "NiInterpController.hpp"
#include "BSTreadTransfInterpolator.hpp"

class BSBlendTreadTransfInterpolator;

class BSMultiTargetTreadTransfController : public NiInterpController {
public:
	BSMultiTargetTreadTransfController();
	virtual ~BSMultiTargetTreadTransfController();

	struct LinkAVObj {
		UInt16									usBlendId;
		NiAVObject*								pTarget;
		BSTreadTransfInterpolator::LinkInfo*	pLink;
	};

	BSBlendTreadTransfInterpolator* pBlendInterpolators;
	UInt16							usNumInterps;
	LinkAVObj**						ppLinks;
	UInt16							usNumLinks;
};

ASSERT_SIZE(BSMultiTargetTreadTransfController, 0x44);