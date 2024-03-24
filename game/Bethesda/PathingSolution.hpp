#pragma once

#include "NiRefObject.hpp"
#include "BSSimpleArray.hpp"

NiSmartPointer(PathingSolution);

class PathingSolution : public NiRefObject {
public:
	BSSimpleArray<void*>	kVirtualPathingNodes;
	SInt32					iFirstLoadedVirtualNodeIndex;
	SInt32					iLastLoadedVirtualNodeIndex;
	BSSimpleArray<void*>	kCurrentPathingNodes;
	BSSimpleArray<void*>	kPreviousPathingNodes;
	BYTE					byte40;
};

ASSERT_SIZE(PathingSolution, 0x44);