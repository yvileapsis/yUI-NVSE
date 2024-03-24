#pragma once

#include "BSWin32TaskletData.hpp"
#include "NiAVObject.hpp"

class UpdateMasterPsysTaskletData : public BSWin32TaskletData {
public:
	virtual ~UpdateMasterPsysTaskletData();

	NiAVObject*		pParticle;
	NiUpdateData*	pUpdateData;
};