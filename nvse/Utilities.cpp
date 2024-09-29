#include <Utilities.hpp>

#include <cstdlib>
#include <utility>
#include <oleauto.h>

/*
ScopedLock::ScopedLock(CriticalSection& critSection) : m_critSection(critSection)
{
	m_critSection.Enter();
}

ScopedLock::~ScopedLock()
{
	m_critSection.Leave();
}

void SpinLock::Enter()
{
	UInt32 threadID = GetCurrentThreadId();
	if (owningThread == threadID)
	{
		enterCount++;
		return;
	}
	while (InterlockedCompareExchange(&owningThread, threadID, 0));
	enterCount = 1;
}

#define FAST_SLEEP_COUNT 10000UL

void SpinLock::EnterSleep()
{
	UInt32 threadID = GetCurrentThreadId();
	if (owningThread == threadID)
	{
		enterCount++;
		return;
	}
	UInt32 fastIdx = FAST_SLEEP_COUNT;
	while (InterlockedCompareExchange(&owningThread, threadID, 0))
	{
		if (fastIdx)
		{
			fastIdx--;
			Sleep(0);
		}
		else Sleep(1);
	}
	enterCount = 1;
}

void SpinLock::Leave()
{
	if (owningThread && !--enterCount)
		owningThread = 0;
}
*/

static std::filesystem::path s_falloutDirectory;

const std::filesystem::path& GetFalloutDirectory()
{
	if (!s_falloutDirectory.empty()) return s_falloutDirectory;

	// can't determine how many bytes we'll need, hope it's not more than MAX_PATH
	char	falloutPathBuf[MAX_PATH];
	const UInt32	falloutPathLength = GetModuleFileName(GetModuleHandle(nullptr), falloutPathBuf, sizeof(falloutPathBuf));

	if (!falloutPathLength || falloutPathLength >= sizeof falloutPathBuf)
	{
		Log() << (std::format("couldn't find fallout path (len = {:d}, err = {:08X})", falloutPathLength, GetLastError()));
		return s_falloutDirectory;
	}

	const std::string	falloutPath(falloutPathBuf, falloutPathLength);

	// truncate at last slash
	const std::string::size_type lastSlash = falloutPath.rfind('\\');

	if (lastSlash != std::string::npos)	// if we don't find a slash something is VERY WRONG
	{
		s_falloutDirectory = falloutPath.substr(0, lastSlash + 1);
#if _DEBUG
		Log() << ("fallout root = " + s_falloutDirectory.generic_string());
#endif
	}
	else
	{
#if _DEBUG
		Log() << ("no slash in fallout path? (" + falloutPath + ")");
#endif
	}

	return s_falloutDirectory;
}

static std::filesystem::path s_configPath;

static const std::filesystem::path& GetNVSEConfigPath()
{

	if (!s_configPath.empty()) return s_configPath;

	if (const auto& falloutPath = GetFalloutDirectory(); !falloutPath.empty())
	{
		s_configPath = falloutPath.generic_string() + "Data\\NVSE\\nvse_config.ini";
		Log() << ("config path =" + s_configPath.generic_string());
	}

	return s_configPath;
}

std::string GetNVSEConfigOption(const char * section, const char * key)
{
	std::string	result;

	if (const auto& configPath = GetNVSEConfigPath(); !configPath.empty())
	{
		char	resultBuf[256];
		resultBuf[0] = 0;

		GetPrivateProfileString(section, key, nullptr, resultBuf, 255, configPath.generic_string().c_str());

		result = resultBuf;
	}

	return result;
}

bool GetNVSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut)
{
	const auto data = GetNVSEConfigOption(section, key);
	if (data.empty()) return false;
	return sscanf_s(data.c_str(), "%lu", dataOut) == 1;
}

Tokenizer::Tokenizer(const char* src, const char* delims)
: m_offset(0), m_delims(delims), m_data(src)
{
	//
}

Tokenizer::~Tokenizer()
{
	//
}

UInt32 Tokenizer::NextToken(std::string& outStr)
{
	if (m_offset == m_data.length())
		return -1;

	size_t start = m_data.find_first_not_of(m_delims, m_offset);
	if (start != -1)
	{
		size_t end = m_data.find_first_of(m_delims, start);
		if (end == -1)
			end = m_data.length();

		m_offset = end;
		outStr = m_data.substr(start, end - start);
		return start;
	}

	return -1;
}

UInt32 Tokenizer::PrevToken(std::string& outStr)
{
	if (m_offset == 0)
		return -1;

	size_t searchStart = m_data.find_last_of(m_delims, m_offset - 1);
	if (searchStart == -1)
		return -1;

	size_t end = m_data.find_last_not_of(m_delims, searchStart);
	if (end == -1)
		return -1;

	size_t start = m_data.find_last_of(m_delims, end);	// okay if start == -1 here

	m_offset = end + 1;
	outStr = m_data.substr(start + 1, end - start);
	return start + 1;
}

#if RUNTIME

// TODO: remove jazz code
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

const char GetSeparatorChar(Script* script)
{
	if(IsConsoleMode())
	{
		if(script && script->ucModIndex != 0xFF)
			return '|';
		else
			return '@';
	}
	else
		return '|';
}

std::string GetSeparatorChars(Script * script)
{
	return IsConsoleMode() ? script && script->ucModIndex != 0xFF ? "|" : "@" : "|";
}

#endif

bool ci_equal(char ch1, char ch2)
{
	return tolower((unsigned char)ch1) == tolower((unsigned char)ch2);
}

bool ci_less(const char* lh, const char* rh)
{
	assert(lh && rh);
	while (*lh && *rh) {
		char l = toupper(*lh);
		char r = toupper(*rh);
		if (l < r) {
			return true;
		}
		else if (l > r) {
			return false;
		}
		lh++;
		rh++;
	}

	return toupper(*lh) < toupper(*rh);
}

void MakeUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

void MakeLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

char* CopyCString(const char* src)
{
	UInt32 size = src ? strlen(src) : 0;
	char* result = (char*)GameHeapAlloc(size+1);
	result[size] = 0;
	if (size) {
		strcpy_s(result, size+1, src);
	}

	return result;
}

#pragma warning(push)
#pragma warning(disable: 4996)	// warning about std::transform()

void MakeUpper(char* str)
{
	if (str) {
		UInt32 len = strlen(str);
		std::transform(str, str + len, str, toupper);
	}
}

void MakeLower(char* str)
{
	if (str) {
		UInt32 len = strlen(str);
		std::transform(str, str + len, str, tolower);
	}
}

#pragma warning(pop)

// ErrOutput
ErrOutput::ErrOutput(_ShowError errorFunc, _ShowWarning warningFunc)
{
	ShowWarning = warningFunc;
	ShowError = errorFunc;
}

void ErrOutput::vShow(ErrOutput::Message& msg, va_list args)
{
	char msgText[0x400];
	vsprintf_s(msgText, sizeof(msgText), msg.fmt, args);
	if (msg.bCanDisable)
	{
		if (!msg.bDisabled)
			if (ShowWarning(msgText))
				msg.bDisabled = true;
	}
	else
		ShowError(msgText);
}

void ErrOutput::Show(ErrOutput::Message msg, ...)
{
	va_list args;
	va_start(args, msg);

	vShow(msg, args);
}

void ErrOutput::Show(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	vShow(msg, args);
}

void ErrOutput::vShow(const char* msg, va_list args)
{
	Message tempMsg;
	tempMsg.fmt = msg;
	tempMsg.bCanDisable = false;
	tempMsg.bDisabled = false;

	vShow(tempMsg, args);
}

void ShowErrorMessageBox(const char* message)
{
	int msgboxID = MessageBox(
		nullptr,
		message,
		"Error",
		MB_ICONWARNING | MB_OK
	);
}

std::unordered_set<UInt32> g_warnedScripts;

const char* GetModName(const TESForm* script)
{
	const char* modName = "In-game console";
	if (script->ucModIndex != 0xFF)
	{
		modName = TESDataHandler::GetSingleton()->GetNthModName(script->ucModIndex);
		if (!modName || !modName[0])
			modName = "Unknown";
	}
	return modName;
}

std::vector<void*> GetCallStack(int i)
{
	std::vector<void*> vecTrace(i, nullptr);
	CaptureStackBackTrace(0, i, vecTrace.data(), nullptr);
	return vecTrace;
}

void GeckExtenderMessageLog(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	auto* window = FindWindow("RTEDITLOG", nullptr);
	if (!window)
	{
		Log() << ("Failed to find GECK Extender Message Log window");
		return;
	}
	
	char buffer[0x400];
	vsprintf_s(buffer, 0x400, fmt, args);
	strcat_s(buffer, "\n");

	SendMessage(window, 0x8004, 0, reinterpret_cast<LPARAM>(buffer));
}

memcpy_t _memcpy = memcpy, _memmove = memmove;

__declspec(naked) int __vectorcall ifloor(float value)
{
	__asm
	{
		movd	eax, xmm0
		test	eax, eax
		jns		isPos
		push	0x3FA0
		ldmxcsr[esp]
		cvtss2si	eax, xmm0
		mov		dword ptr[esp], 0x1FA0
		ldmxcsr[esp]
		pop		ecx
		retn
		isPos :
		cvttss2si	eax, xmm0
			retn
	}
}

__declspec(naked) int __vectorcall iceil(float value)
{
	__asm
	{
		movd	eax, xmm0
		test	eax, eax
		js		isNeg
		push	0x5FA0
		ldmxcsr[esp]
		cvtss2si	eax, xmm0
		mov		dword ptr[esp], 0x1FA0
		ldmxcsr[esp]
		pop		ecx
		retn
		isNeg :
		cvttss2si	eax, xmm0
			retn
	}
}

__declspec(naked) UInt32 __fastcall StrLen(const char* str)
{
	__asm
	{
		test	ecx, ecx
		jnz		proceed
		xor eax, eax
		retn
		proceed :
		push	ecx
			test	ecx, 3
			jz		iter4
			iter1 :
		mov		al, [ecx]
			inc		ecx
			test	al, al
			jz		done1
			test	ecx, 3
			jnz		iter1
			ALIGN 16
			iter4:
		mov		eax, [ecx]
			mov		edx, 0x7EFEFEFF
			add		edx, eax
			not eax
			xor eax, edx
			add		ecx, 4
			test	eax, 0x81010100
			jz		iter4
			mov		eax, [ecx - 4]
			test	al, al
			jz		done4
			test	ah, ah
			jz		done3
			test	eax, 0xFF0000
			jz		done2
			test	eax, 0xFF000000
			jnz		iter4
			done1 :
		lea		eax, [ecx - 1]
			pop		ecx
			sub		eax, ecx
			retn
			done2 :
		lea		eax, [ecx - 2]
			pop		ecx
			sub		eax, ecx
			retn
			done3 :
		lea		eax, [ecx - 3]
			pop		ecx
			sub		eax, ecx
			retn
			done4 :
		lea		eax, [ecx - 4]
			pop		ecx
			sub		eax, ecx
			retn
	}
}

__declspec(naked) void __fastcall MemZero(void* dest, UInt32 bsize)
{
	__asm
	{
		push	edi
		test	ecx, ecx
		jz		done
		mov		edi, ecx
		xor eax, eax
		mov		ecx, edx
		shr		ecx, 2
		jz		write1
		rep stosd
		write1 :
		and edx, 3
			jz		done
			mov		ecx, edx
			rep stosb
			done :
		pop		edi
			retn
	}
}

__declspec(naked) char* __fastcall StrCopy(char* dest, const char* src)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jnz		proceed
		mov[eax], 0
		done:
		retn
	proceed :
		push	ecx
		mov		ecx, edx
		call	StrLen
		pop		edx
		push	eax
		inc		eax
		push	eax
		push	ecx
		push	edx
		call	_memmove
		add		esp, 0xC
		pop		ecx
		add		eax, ecx
		retn
	}
}

__declspec(naked) char* __fastcall StrNCopy(char* dest, const char* src, UInt32 length)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		cmp		dword ptr[esp + 4], 0
		jz		nullTerm
		push	esi
		mov		esi, ecx
		mov		ecx, edx
		call	StrLen
		mov		edx, [esp + 8]
		cmp		edx, eax
		cmova	edx, eax
		push	edx
		push	ecx
		push	esi
		add		esi, edx
		call	_memmove
		add		esp, 0xC
		mov		eax, esi
		pop		esi
	nullTerm :
		mov		[eax], 0
	done :
		retn	4
	}
}

__declspec(naked) char* __fastcall StrCat(char* dest, const char* src)
{
	__asm
	{
		test	ecx, ecx
		jnz		proceed
		mov		eax, ecx
		retn
	proceed :
		push	edx
		call	StrLen
		pop		edx
		add		ecx, eax
		jmp		StrCopy
	}
}

__declspec(align(16)) const UInt8 kCaseConverter[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

char __fastcall StrCompare(const char* lstr, const char* rstr)
{
	if (!lstr) return rstr ? -1 : 0;
	if (!rstr) return 1;
	UInt8 lchr, rchr;
	while (*lstr)
	{
		lchr = kCaseConverter[*(UInt8*)lstr];
		rchr = kCaseConverter[*(UInt8*)rstr];
		if (lchr == rchr)
		{
			lstr++;
			rstr++;
			continue;
		}
		return (lchr < rchr) ? -1 : 1;
	}
	return *rstr ? -1 : 0;
}

void __fastcall StrToLower(char* str)
{
	if (!str) return;
	UInt8 curr;
	while (curr = *str)
	{
		*str = kCaseConverter[curr];
		str++;
	}
}

char* __fastcall SubStrCI(const char* srcStr, const char* subStr)
{
	int srcLen = StrLen(srcStr);
	if (!srcLen) return nullptr;
	int subLen = StrLen(subStr);
	if (!subLen) return nullptr;
	srcLen -= subLen;
	if (srcLen < 0) return nullptr;
	int index;
	do
	{
		index = 0;
		while (true)
		{
			if (kCaseConverter[*(UInt8*)(srcStr + index)] != kCaseConverter[*(UInt8*)(subStr + index)])
				break;
			if (++index == subLen)
				return const_cast<char*>(srcStr);
		}
		srcStr++;
	} while (--srcLen >= 0);
	return nullptr;
}

char* __fastcall SlashPos(const char* str)
{
	if (!str) return nullptr;
	char curr;
	while (curr = *str)
	{
		if ((curr == '/') || (curr == '\\'))
			return const_cast<char*>(str);
		str++;
	}
	return nullptr;
}

__declspec(naked) char* __fastcall CopyString(const char* key)
{
	__asm
	{
		call	StrLen
		inc		eax
		push	eax
		push	ecx
		push	eax
#if !_DEBUG
		call	_malloc_base
#else
		call	malloc
#endif
		pop		ecx
		push	eax
		call	_memcpy
		add		esp, 0xC
		retn
	}
}

__declspec(naked) char* __fastcall IntToStr(char* str, int num)
{
	__asm
	{
		push	esi
		push	edi
		test	edx, edx
		jns		skipNeg
		neg		edx
		mov[ecx], '-'
		inc		ecx
	skipNeg :
		mov		esi, ecx
		mov		edi, ecx
		mov		eax, edx
		mov		ecx, 0xA
		workIter :
		xor edx, edx
		div		ecx
		add		dl, '0'
		mov[esi], dl
		inc		esi
		test	eax, eax
		jnz		workIter
		mov[esi], 0
		mov		eax, esi
	swapIter :
		dec		esi
		cmp		esi, edi
		jbe		done
		mov		dl, [esi]
		mov		cl, [edi]
		mov[esi], cl
		mov[edi], dl
		inc		edi
		jmp		swapIter
	done :
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrHashCS(const char* inKey)
{
	__asm
	{
		push	esi
		mov		eax, 0x1505
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor ecx, ecx
		ALIGN 16
	iterHead:
		mov		cl, [esi]
		test	cl, cl
		jz		done
		mov		edx, eax
		add		eax, ecx
		shl		edx, 5
		add		eax, edx
		inc		esi
		jmp		iterHead
	done :
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrHashCI(const char* inKey)
{
	__asm
	{
		push	esi
		mov		eax, 0x1505
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor ecx, ecx
		ALIGN 16
	iterHead:
		mov		cl, [esi]
		test	cl, cl
		jz		done
		mov		edx, eax
		shl		edx, 5
		add		eax, edx
		movzx	edx, kCaseConverter[ecx]
		add		eax, edx
		inc		esi
		jmp		iterHead
	done :
		pop		esi
		retn
	}
}

std::filesystem::path GetCurPath()
{
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	const std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

/// Try to find in the Haystack the Needle - ignore case

std::string& ToLower(std::string&& data)
{
	ra::transform(data, data.begin(), [](const unsigned char c) { return std::tolower(c); });
	return data;
}

std::string& StripSpace(std::string&& data)
{
	std::erase_if(data, isspace);
	return data;
}

bool StartsWith(const char* string, const char* prefix)
{
	size_t count = 0;
	while (*prefix)
	{
		if (tolower(*prefix++) != tolower(*string++))
			return false;
		++count;
	}
	if (!count)
		return false;
	return true;
}

char* stristr(const char* str1, const char* str2)
{
	const char* p1 = str1;
	const char* p2 = str2;
	const char* r = *p2 == 0 ? str1 : nullptr;

	while (*p1 != 0 && *p2 != 0)
	{
		if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
		{
			if (r == nullptr)
			{
				r = p1;
			}

			p2++;
		}
		else
		{
			p2 = str2;
			if (r != nullptr)
			{
				p1 = r + 1;
			}

			if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
			{
				r = p1;
				p2++;
			}
			else
			{
				r = nullptr;
			}
		}

		p1++;
	}

	return *p2 == 0 ? (char*)r : nullptr;
}

std::string GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		return "";
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{
		CloseClipboard();
		return "";
	}

	// Lock the handle to get the actual text pointer
	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		CloseClipboard();
		return "";
	}

	// Save text in a string class instance
	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

std::string UTF8toANSI(const std::string& str)
{	
	const char* pszCode = str.c_str();

	auto nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, nullptr, NULL);
	const auto bstrWide = SysAllocStringLen(nullptr, nLength);

	if (bstrWide == nullptr) return "ERROR!! SOMETHING WENT WRONG.";
	
	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, nullptr, 0, nullptr, nullptr);
	const auto pszAnsi = new char[nLength];

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, nullptr, nullptr);
	SysFreeString(bstrWide);

	std::string rstr(pszAnsi);
	delete[] pszAnsi;
	return rstr;
}

// jazz code
__declspec(naked) bool IsConsoleOpen()
{
	__asm
	{
		mov		al, byte ptr ds : [0x11DEA2E] // console open
		test	al, al
		jz		done
		mov		eax, fs : [0x2C]
		mov		edx, ds : [0x126FD98] // some index
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		al, [eax + 0x268]
	done :
		 retn
	}
}

int HexStringToInt(const std::string& str)
{
	char* p;
	const auto id = strtoul(str.c_str(), &p, 16);
	return *p == 0 ? id : -1;
}

char* ConvertLiteralPercents(char* srcPtr)
{
	char* endPtr = srcPtr + StrLen(srcPtr);
	while (srcPtr = strchr(srcPtr, '%'))
	{
		srcPtr++;
		memmove(srcPtr + 1, srcPtr, endPtr - srcPtr);
		*srcPtr++ = '%';
		endPtr++;
	}
	*endPtr = 0;
	return endPtr;
}

#include <PluginAPI.hpp>

std::string DecompileScriptToFolder(const std::string& scriptName, Script* script, const std::string& fileExtension, const std::string_view& modName)
{
	char buffer[0x1000];
	if (!DecompileScript(script, 0, buffer, sizeof(buffer)))
		return std::format("Script {} is not compiled", scriptName);

	std::filesystem::path dirName = "DecompiledScripts";
	if (!std::filesystem::exists(dirName))
		std::filesystem::create_directory(dirName);

	dirName /= modName;

	if (!std::filesystem::exists(dirName))
		std::filesystem::create_directory(dirName);

	dirName /= scriptName + '.' + fileExtension;

	std::ofstream os(dirName);
	os << buffer;
	return std::format("Decompiled script to '{}'", dirName.string());
}

std::string& SanitizeStringBySize(std::string& str)
{
	for (UInt32 i = 0; i < MAX_PATH; i++) if (str[i] == 0) 
		return str;
	str = "";
	return str;
}

std::string& SanitizeStringFromBadData(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), [](char c) { return !(c >= 0 && c <= 0x128); }), str.end());

	std::replace_if(str.begin(), str.end(), [](char c) { return c == '\n' || c == '\r' || c == '\0' || c == '\v'; }, ' ');

	return str;
}

std::string pcName;
std::string userName;

std::string& SanitizeStringFromUserInfo(std::string& str)
{
	[[unlikely]]
	if (pcName.empty())
	{
		TCHAR infoBuf[MAX_PATH];
		DWORD bufCharCount = MAX_PATH;
		if (GetComputerName(infoBuf, &bufCharCount)) pcName = infoBuf;
	}

	[[unlikely]]
	if (userName.empty())
	{
		TCHAR infoBuf[MAX_PATH];
		DWORD bufCharCount = MAX_PATH;
		if (GetUserName(infoBuf, &bufCharCount)) userName = infoBuf;
	}

	if (pcName.size() > 1)
		while (str.find(pcName) != -1) str.replace(str.find(pcName), pcName.size(), pcName.size(), '*');

	if (userName.size() > 1)
		while (str.find(userName) != -1) str.replace(str.find(userName), userName.size(), userName.size(), '*');

	return str;
}

const std::string& SanitizeString(std::string&& str) 
{
	SanitizeStringBySize(str);
	SanitizeStringFromBadData(str);
	SanitizeStringFromUserInfo(str);

	return str;
}

float ConvertToKiB(const UInt64 size) {
	return (float)size / 1024.0f;
}

float ConvertToMiB(const UInt64 size) {
	return (float)size / 1024.0f / 1024.0f;
}

float ConvertToGiB(const UInt64 size) {
	return (float)size / 1024.0f / 1024.0f / 1024.0f;
}

std::string FormatSize(const UInt64 size) {
	std::string result;
	if (size < 1024) {
		result = std::format("{:>6d} B", size);
	}
	else if (size < 1024ull * 1024ull) {
		result = std::format("{:>6.2f} KiB", ConvertToKiB(size));
	}
	else if (size < 1024ull * 1024ull * 1024ull) {
		result = std::format("{:>6.2f} MiB", ConvertToMiB(size));
	}
	else {
		result = std::format("{:>6.2f} GiB", ConvertToGiB(size));
	}
	return result;
}

std::string GetMemoryUsageString(const UInt64 used, const UInt64 total) {
	float usedPercent = (float)used / total * 100.0f;
	return std::format("{:10} / {:10} ({:.2f}%)", FormatSize(used), FormatSize(total), usedPercent);
}

//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetErrorAsString(UInt32 errorMessageID)
{
	if (errorMessageID == 0) return ""; //No error message has been recorded

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message;
}

std::string GetExceptionAsString(UInt32 exceptionMessageID)
{
	switch (exceptionMessageID) {
	case EXCEPTION_ACCESS_VIOLATION:			return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_BREAKPOINT:					return "EXCEPTION_BREAKPOINT >>";
	case EXCEPTION_DATATYPE_MISALIGNMENT:		return "EXCEPTION_DATATYPE_MISALIGNMENT >>";
	case EXCEPTION_FLT_DENORMAL_OPERAND:		return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:			return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:			return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:		return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:				return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:				return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:				return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_ILLEGAL_INSTRUCTION:			return "EXCEPTION_ILLEGAL_INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR:				return "EXCEPTION_IN_PAGE_ERROR";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:			return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:				return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION:			return "EXCEPTION_INVALID_DISPOSITION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:	return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_PRIV_INSTRUCTION:			return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_SINGLE_STEP:					return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_STACK_OVERFLOW:				return "EXCEPTION_STACK_OVERFLOW";
	default: return "UKNOWN_EXCEPTION";
	}
}

void PrintSeparator(const UInt32 length) {
	std::string separator(length, '-');
	Log() << separator << '\n';
}