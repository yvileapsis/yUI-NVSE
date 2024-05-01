#pragma once

#include "PathingRequest.hpp"

class PathingRequestCoveredMove : public PathingRequest {
public:
	float					floatB0;
	float					floatB4;
	float					floatB8;
	float					floatBC;
	float					floatC0;
	DWORD					dwordC4;
	NiPoint3				pointC8;
	NiPoint3				pointD4;
	BYTE					byteE0;
	BYTE					byteE1;
	BSSimpleArray<UInt32>	bssimplearray_voidE4;
	BSSimpleArray<NiPoint3> bssimplearray_nipoint3F4;
	DWORD					dword104;
	DWORD					dword108;
};