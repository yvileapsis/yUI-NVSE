#pragma once

#include "BSMemObject.hpp"
#include "NiSmartPointer.hpp"

class BSFile;

NiSmartPointer(LoadedFile);

// NiRefObject in FO3 GECK
class LoadedFile : public BSMemObject {
public:
	LoadedFile(char* apPath, BSFile* apFile);
	~LoadedFile();

	UInt32	uiRefCount;
	char*	pPath;
	BSFile* pFile;
	bool	bAddedToMap;
	bool	bUsed;

	void IncRefCount();
	void DecRefCount();
};