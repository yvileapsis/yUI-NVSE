#include <ConsoleManager.h>

__declspec(naked) bool IsConsoleMode()
{
	__asm
	{
		mov		al, byte ptr ds : [0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, dword ptr ds : [0x126FD98]
		mov		edx, fs : [0x2C]
		mov		eax, [edx + eax * 4]
		test	eax, eax
		jz		done
		mov		al, [eax + 0x268]
		done :
		retn
	}
}

ConsoleManager::TextList* ConsoleManager::TextList::Append(TextNode* append)
{
	append->prev = 0;
	append->next = this->first;
	if (this->first) this->first->prev = append;
	else this->last = append;
	this->first = append;
	++this->count;
	return this;
}

char* ConsoleManager::GetConsoleOutputFilename()
{
	return GetSingleton()->scofPath;
};

bool ConsoleManager::HasConsoleOutputFilename() {
	return 0 != GetSingleton()->scofPath[0];
}

void ConsoleManager::AppendToSentHistory(const char* str)
{
	// create text node
	auto* textNode = static_cast<TextNode*>(GameHeapAlloc(sizeof(TextNode)));
	textNode->next = 0;
	textNode->prev = 0;

	textNode->text.m_data = NULL;
	textNode->text.Init(0);
	textNode->text.Set(str);

	// append it to the input history
	this->inputHistory.Append(textNode);
}

void PrintConsole(const char* fmt, ...)
{
	if (const auto mgr = ConsoleManager::GetSingleton())
	{
		va_list	args;
		va_start(args, fmt);
		mgr->Print(fmt, args);
		va_end(args);
	}
}

bool* bEchoConsole = reinterpret_cast<bool*>(0x011F158C);

bool GetConsoleEcho()
{
	return *bEchoConsole != 0;
}

void SetConsoleEcho(const bool doEcho)
{
	*bEchoConsole = doEcho ? true : false;
}