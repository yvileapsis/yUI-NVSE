#pragma once
#include <Containers.h>
#include <Utilities.h>

void PrintConsole(const char* fmt, ...);
bool IsConsoleMode();
bool GetConsoleEcho();
void SetConsoleEcho(bool doEcho);

class ConsoleManager
{
public:
	ConsoleManager();
	~ConsoleManager();

	struct TextNode
	{
		TextNode* next;
		TextNode* prev;
		String				text;
	};

	struct TextList
	{
		TextNode* first;
		TextNode* last;
		UInt32				count;

		TextList* Append(TextNode*);
	};

	struct RecordedCommand
	{
		char				buf[100];
	};

	void* scriptContext;
	TextList				printedLines;
	TextList				inputHistory;
	unsigned int			historyIndex;
	unsigned int			unk020;
	unsigned int			printedCount;
	unsigned int			unk028;
	unsigned int			lineHeight;
	int						textXPos;
	int						textYPos;
	UInt8					isConsoleOpen;
	UInt8					unk39;
	UInt8					isBatchRecording;
	UInt8					unk3B;
	unsigned int			numRecordedCommands;
	RecordedCommand			recordedCommands[20];
	char					scofPath[260];

	__forceinline static ConsoleManager*	GetSingleton(bool canCreateNew = true) { return CdeclCall<ConsoleManager*>(0x0071B160, canCreateNew); }
	void									AppendToSentHistory(const char*);
	__forceinline void						Print(const char* fmt, va_list args) { ThisCall(0x0071D0A0, this, fmt, args); }
	static char*							GetConsoleOutputFilename();
	static bool								HasConsoleOutputFilename();
};
