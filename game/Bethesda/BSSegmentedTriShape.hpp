#pragma once

#include "NiTriShape.hpp"

NiSmartPointer(BSSegmentedTriShape);

class BSSegmentedTriShape : public NiTriShape {
public:
	BSSegmentedTriShape();
	~BSSegmentedTriShape();

	struct Segment {
		UInt32	uiStartIndex;
		UInt32	uiNumPrimitives;
		bool	bIsEnabled;
		UInt32	uiTriCount;
		bool	ucDisabledCount;
	};

	Segment*	pSegments;
	UInt32		uiNumSegments;
	bool		bSegmentsChanged;
	bool		bIgnoreSegments;

	CREATE_OBJECT(BSSegmentedTriShape, 0xA87140);

	static void __fastcall RenderImmediateEx(BSSegmentedTriShape* apThis, void*, NiDX9Renderer* apRenderer);
	static void __fastcall RenderImmediateAltEx(BSSegmentedTriShape* apThis, void*, NiDX9Renderer* apRenderer);

	void EnableSegment(UInt32 uiLevel);
	void DisableSegment(UInt32 uiLevel);
	bool IsSegmentEmpty(UInt32 uiLevel);
	UInt32 GetNumSegments();
	void EnableAllSegments();
	void UpdateDrawData();
};

ASSERT_SIZE(BSSegmentedTriShape, 0xD0);