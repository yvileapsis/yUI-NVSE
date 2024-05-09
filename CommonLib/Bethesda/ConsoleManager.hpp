#pragma once

#include "NiTList.hpp"
#include "BSString.hpp"

static const UInt32 s_Console__Print = 0x0071D0A0;

class ConsoleManager {
public:
	ConsoleManager();
	~ConsoleManager();

	struct RecordedCommand {
		char buf[100];
	};


	void*				pScriptContext;
	NiTList<BSStringT<char>> lPrintedLines;
	NiTList<BSStringT<char>> lInputHistory;
	UInt32				uiHistoryIndex;
	UInt32				uiUnk020;
	UInt32				uiPrintedCount;
	UInt32				uiUnk028;
	UInt32				uiLineHeight;
	SInt32				iTextXPos;
	SInt32				iTextYPos;
	UInt8				ucConsoleState;
	bool				bUnk39;
	bool				bIsBatchRecording;
	bool				bUnk3B;
	UInt32				uiNumRecordedCommands;
	RecordedCommand		recordedCommands[20];
	char				COFileName[260];

	__forceinline static ConsoleManager* GetSingleton(bool canCreateNew = true) { return CdeclCall<ConsoleManager*>(0x0071B160, canCreateNew); }

	static char* GetConsoleOutputFilename();
	static bool HasConsoleOutputFilename();

	__forceinline void Print(const char* fmt, va_list args) { ThisCall(0x0071D0A0, this, fmt, args); }
	bool IsConsoleActive();
	bool ToggleConsole();
};

ASSERT_SIZE(ConsoleManager, 0x914u);