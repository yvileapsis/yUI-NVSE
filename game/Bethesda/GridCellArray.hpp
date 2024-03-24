#pragma once

#include "NiPoint3.hpp"
#include "GridCell.hpp"
#include "GridArray.hpp"
#include "NiSmartPointer.hpp"

class BSRenderedTexture;

class GridCellArray : public GridArray {
public:
	SInt32							iDimension;
	GridCell*						pGridCells;
	NiPoint3						kWorldCenter;
	bool							bLandAttached;
	NiPointer<BSRenderedTexture>	spShadowMask;

	GridCell* GetCell(SInt32 aX, SInt32 aY);
};