#pragma once

#include "NiTriShape.hpp"

NiSmartPointer(BSResizableTriShape);

class BSResizableTriShape : public NiTriShape {
public:
	virtual void SetBounds(NiBound akBound);
	virtual NiBound* GetLocalBound();

	UInt32	uiActiveTriCount;
	NiBound kLocalBound;

	static BSResizableTriShape* Create(NiGeometryData* apData);

	void SetActiveTriCount(UInt32 auiCount);
	UInt32 GetActiveTriCount() const;
};
