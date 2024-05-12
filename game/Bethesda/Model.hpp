#pragma once

#include "NiNode.hpp"

class BSStream;

NiSmartPointer(Model);

class Model {
public:
	Model();
	Model(const char* apPath, BSStream& arStream, bool abAssignShaders, bool abKeepUV);
	~Model();

	const char* pcPath;
	UInt32		uiRefCounter1;
	UInt32		uiRefCounter2;
	NiNodePtr	spNode;

	void Initialize(const char* apPath, BSStream &arStream, bool abAssignShaders, bool abKeepUV);
};

ASSERT_SIZE(Model, 0x10)