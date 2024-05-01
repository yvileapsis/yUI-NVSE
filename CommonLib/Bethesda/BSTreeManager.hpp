#pragma once

#include "NiRefObject.hpp"
#include "NiMaterialProperty.hpp"
#include "NiAlphaProperty.hpp"

class BSXFlags;
class NiCamera;

class BSTreeManager {
public:
	virtual void Destructor();

	NiRefObjectPtr spUnk04;
	NiMaterialPropertyPtr spMaterialProperty;
	NiAlphaPropertyPtr spAlphaProperty;
	BSXFlags* spExtraFlags;
	float fLeafRustleAmountSway;
	bool bTreesEnabled;
	bool byte19;
	bool byte1A;
	bool bForceFullLOD;
	unsigned int refTreeNodeMap;

	static BSTreeManager* GetSingleton(bool abCreate = true);

	static void SetIsForceFullLOD(bool abEnable);
	static bool GetIsForceFullLOD();

	static void Update(NiCamera* apCamera, bool abOnlySetCamera = false);

	static bool GetTreesEnabled();
	static void SetTreesEnabled(bool abEnable);
};
