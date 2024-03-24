#pragma once

namespace NiViewerStrings {
	const char* GetViewerString(const char* apcTitle);

	const char* GetViewerString(const char* apText, void* apValue);
	const char* GetViewerString(const char* apText, UInt32 auiValue);
	const char* GetViewerString(const char* apText, UInt16 ausValue);
	const char* GetViewerString(const char* apText, UInt8 aucValue);
	const char* GetViewerString(const char* apText, char acValue);
	const char* GetViewerString(const char* apText, bool abValue);
	const char* GetViewerString(const char* apText, float afValue);
	const char* GetViewerString(const char* apText, double adValue);
	const char* GetViewerString(const char* apText, const char* apcValue);
}