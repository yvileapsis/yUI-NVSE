#include "TESDataHandler.hpp"

TESDataHandler* TESDataHandler::GetSingleton() {
	return *(TESDataHandler**)0x11C3F2C;
}

// GAME - 0x45DFC0
BSSimpleList<TESFile*>* TESDataHandler::GetFileList() {
	return &kFiles;
}

uint32_t TESDataHandler::GetCompiledFileCount() const {
	return kCompiledFiles.GetFileCount();
}

TESFile* TESDataHandler::GetCompiledFile(uint32_t auiIndex) const {
	return ThisCall<TESFile*>(0x465010, this, auiIndex);
}

TESFile* TESDataHandler::GetListFile(uint32_t auiIndex) {
	BSSimpleList<TESFile*>* pIter = GetFileList();

	uint32_t i = 0;
	if (auiIndex) {
		while (true) {
			pIter = pIter->GetNext();
			if (!pIter)
				break;

			if (pIter->GetItem() && ++i < auiIndex)
				continue;

			return pIter->GetItem();
		}
	}
	else {
		if (pIter)
			return pIter->GetItem();
	}

	return nullptr;
}

// GAME - 0x462F40
TESFile* TESDataHandler::GetListFile(const char* apFileName) {
	return ThisCall<TESFile*>(0x462F40, this, apFileName);
}

bool TESDataHandler::HasExtendedPlugins() const {
	return ucFlags.GetBit(0x80);
}

bool TESDataHandler::ExtendedPlugins() {
	return GetSingleton()->HasExtendedPlugins();
}

uint32_t TESDataHandler::GetSmallCompiledFileCount() const {
	return kCompiledFiles.GetSmallFileCount();
}

TESFile* TESDataHandler::GetSmallFile(uint32_t auiIndex) const {
	return kCompiledFiles.GetSmallFile(auiIndex);
}

uint32_t TESDataHandler::GetMediumCompiledFileCount() const {
	return kCompiledFiles.GetMediumFileCount();
}

TESFile* TESDataHandler::GetMediumFile(uint32_t auiIndex) const {
	return kCompiledFiles.GetMediumFile(auiIndex);
}

uint32_t TESDataHandler::GetOverlayFileCount() const {
	return kCompiledFiles.GetOverlayFileCount();
}

TESFile* TESDataHandler::GetOverlayFile(uint32_t auiIndex) const {
	return kCompiledFiles.GetOverlayFile(auiIndex);
}

uint32_t CompiledFiles::GetFileCount() const {
	if (TESDataHandler::ExtendedPlugins())
		return kNormalFiles.GetSize();

	return uiCompiledFileCount;
}

TESFile* CompiledFiles::GetFile(uint32_t auiIndex) const {
	if (auiIndex >= GetFileCount())
		return nullptr;

	if (TESDataHandler::ExtendedPlugins())
		return kNormalFiles.GetAt(auiIndex);

	return pFileArray[auiIndex];
}

uint32_t CompiledFiles::GetSmallFileCount() const {
	if (TESDataHandler::ExtendedPlugins())
		return kSmallFiles.GetSize();
	return 0;
}

TESFile* CompiledFiles::GetSmallFile(uint32_t auiIndex) const {
	if (auiIndex >= GetSmallFileCount())
		return nullptr;

	if (TESDataHandler::ExtendedPlugins())
		return kSmallFiles.GetAt(auiIndex);
	return nullptr;
}

uint32_t CompiledFiles::GetMediumFileCount() const {
	if (TESDataHandler::ExtendedPlugins())
		return kMediumFiles.GetSize();
	return 0;
}

TESFile* CompiledFiles::GetMediumFile(uint32_t auiIndex) const {
	if (auiIndex >= GetMediumFileCount())
		return nullptr;

	if (TESDataHandler::ExtendedPlugins())
		return kMediumFiles.GetAt(auiIndex);
	return nullptr;
}

uint32_t CompiledFiles::GetOverlayFileCount() const {
	if (TESDataHandler::ExtendedPlugins())
		return kOverlayFiles.GetSize();
	return 0;
}

TESFile* CompiledFiles::GetOverlayFile(uint32_t auiIndex) const {
	if (auiIndex >= GetOverlayFileCount())
		return nullptr;

	if (TESDataHandler::ExtendedPlugins())
		return kOverlayFiles.GetAt(auiIndex);
	return nullptr;
}