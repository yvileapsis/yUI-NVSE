#include <Utilities.h>
#include <SafeWrite.h>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <vector>

#include <GameData.h>
#include <PluginAPI.h>
#include <PluginManager.h>

#include <GameAPI.h>
#include <GameForms.h>
#include <GameScript.h>
#include <settings.h>

//#include <codecvt>
//#include <locale>
#include <stringapiset.h>

std::vector<std::string> queuedConsoleMessages;

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

bool __fastcall FileExists(const char* path)
{
	const auto attr = GetFileAttributes(path);
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}


bool FileStream::Open(const char* filePath)
{
	theFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (theFile == INVALID_HANDLE_VALUE)
		return false;
	streamLength = GetFileSize(theFile, nullptr);
	return true;
}

bool FileStream::OpenAt(const char* filePath, UInt32 inOffset)
{
	theFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (theFile == INVALID_HANDLE_VALUE)
		return false;
	streamLength = GetFileSize(theFile, nullptr);
	streamOffset = inOffset;
	if (streamOffset >= streamLength)
	{
		Close();
		return false;
	}
	if (streamOffset)
		SetFilePointer(theFile, streamOffset, nullptr, FILE_BEGIN);
	return true;
}

bool FileStream::OpenWrite(const char* filePath)
{
	theFile = CreateFile(filePath, GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (theFile == INVALID_HANDLE_VALUE)
		return false;
	streamOffset = streamLength = GetFileSize(theFile, nullptr);
	SetFilePointer(theFile, streamLength, nullptr, FILE_BEGIN);
	return true;
}

bool FileStream::Create(const char* filePath)
{
	theFile = CreateFile(filePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	return theFile != INVALID_HANDLE_VALUE;
}

bool FileStream::OpenWriteEx(char* filePath, bool append)
{
	MakeAllDirs(filePath);
	if (append)
	{
		if (!OpenWrite(filePath))
			return false;
		if (streamLength)
		{
			char newLn = '\n';
			WriteBuf(&newLn, 1);
		}
	}
	else if (!Create(filePath))
		return false;
	return true;
}

void FileStream::SetOffset(UInt32 inOffset)
{
	if (inOffset > streamLength)
		streamOffset = streamLength;
	else streamOffset = inOffset;
	SetFilePointer(theFile, streamOffset, nullptr, FILE_BEGIN);
}

void FileStream::ReadBuf(void* outData, UInt32 inLength)
{
	UInt32 bytesRead;
	if (!ReadFile(theFile, outData, inLength, &bytesRead, nullptr)) Log("Filestream Failure!");
	streamOffset += bytesRead;
}

void FileStream::WriteBuf(const void* inData, UInt32 inLength)
{
	if (streamOffset > streamLength)
		SetEndOfFile(theFile);
	UInt32 bytesWritten;
	WriteFile(theFile, inData, inLength, &bytesWritten, nullptr);
	streamOffset += bytesWritten;
	if (streamLength < streamOffset)
		streamLength = streamOffset;
}

void FileStream::MakeAllDirs(char* fullPath)
{
	char* traverse = fullPath, curr;
	while (curr = *traverse)
	{
		if ((curr == '\\') || (curr == '/'))
		{
			*traverse = 0;
			CreateDirectory(fullPath, nullptr);
			*traverse = '\\';
		}
		traverse++;
	}
}
bool DebugLog::Create(const char* filePath)
{
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile != nullptr;
}

const char kIndentLevelStr[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

void DebugLog::Message(const char* msgStr)
{
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	fputs(msgStr, theFile);
	fputc('\n', theFile);
	fflush(theFile);
}

void DebugLog::FmtMessage(const char* fmt, va_list args)
{
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	vfprintf(theFile, fmt, args);
	fputc('\n', theFile);
	fflush(theFile);
}

void PrintLog(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	gLog.FmtMessage(fmt, args);
	va_end(args);
}

void PrintAndClearQueuedConsoleMessages()
{
	for (const auto iter : queuedConsoleMessages)
		Console_Print("yUI: %s", iter.c_str());
	queuedConsoleMessages.clear();
}

extern DataHandler* g_dataHandler;

void ConsoleQueueOrPrint(const std::string& msg)
{
	if (*reinterpret_cast<ConsoleManager**>(0x11D8CE8) || g_dataHandler) // g_dataHandler will be non-null if Deferred init has been called
		Console_Print("yUI: %s", msg.c_str());
	else
		queuedConsoleMessages.push_back(msg);
}

void Log(const std::string& msg)
{
	PrintLog("%s", msg.c_str());
	if (g_logLevel == 2)
		ConsoleQueueOrPrint(msg);
}

void DumpClass(void* theClassPtr, UInt32 nIntsToDump)
{
	PrintLog("DumpClass:");
	UInt32* basePtr = (UInt32*)theClassPtr;

	gLog.Indent();

	if (!theClassPtr) return;
	for (UInt32 ix = 0; ix < nIntsToDump; ix++) {
		UInt32* curPtr = basePtr + ix;
		const char* curPtrName = nullptr;
		UInt32 otherPtr = 0;
		float otherFloat = 0.0;
		const char* otherPtrName = nullptr;
		if (curPtr) {
			curPtrName = GetObjectClassName(curPtr);

			__try
			{
				otherPtr = *curPtr;
				otherFloat = *(float*)(curPtr);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				//
			}

			if (otherPtr) {
				otherPtrName = GetObjectClassName((void*)otherPtr);
			}
		}

		PrintLog("%3d +%03X ptr: 0x%08X: %32s *ptr: 0x%08x | %f: %32s", ix, ix * 4, curPtr, curPtrName, otherPtr, otherFloat, otherPtrName);
	}

	gLog.Outdent();
}

#pragma warning (push)
#pragma warning (disable : 4200)
struct RTTIType
{
	void	* typeInfo;
	UInt32	pad;
	char	name[0];
};

struct RTTILocator
{
	UInt32		sig, offset, cdOffset;
	RTTIType	* type;
};
#pragma warning (pop)

// use the RTTI information to return an object's class name
const char * GetObjectClassName(void * objBase)
{
	const char	* result = "<no rtti>";

	__try
	{
		void		** obj = (void **)objBase;
		RTTILocator	** vtbl = (RTTILocator **)obj[0];
		RTTILocator	* rtti = vtbl[-1];
		RTTIType	* type = rtti->type;

		// starts with ,?
		if((type->name[0] == '.') && (type->name[1] == '?'))
		{
			// is at most 100 chars long
			for(UInt32 i = 0; i < 100; i++)
			{
				if(type->name[i] == 0)
				{
					// remove the .?AV
					result = type->name + 4;
					break;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// return the default
	}

	return result;
}

const std::string & GetFalloutDirectory(void)
{
	static std::string s_falloutDirectory;

	if(s_falloutDirectory.empty())
	{
		// can't determine how many bytes we'll need, hope it's not more than MAX_PATH
		char	falloutPathBuf[MAX_PATH];
		const UInt32	falloutPathLength = GetModuleFileName(GetModuleHandle(nullptr), falloutPathBuf, sizeof(falloutPathBuf));

		if(falloutPathLength && (falloutPathLength < sizeof(falloutPathBuf)))
		{
			std::string	falloutPath(falloutPathBuf, falloutPathLength);

			// truncate at last slash
			const std::string::size_type	lastSlash = falloutPath.rfind('\\');
			if(lastSlash != std::string::npos)	// if we don't find a slash something is VERY WRONG
			{
				s_falloutDirectory = falloutPath.substr(0, lastSlash + 1);

				PrintLog("fallout root = %s", s_falloutDirectory.c_str());
			}
			else
			{
				PrintLog("no slash in fallout path? (%s)", falloutPath.c_str());
			}
		}
		else
		{
			PrintLog("couldn't find fallout path (len = %d, err = %08X)", falloutPathLength, GetLastError());
		}
	}

	return s_falloutDirectory;
}

static const std::string & GetNVSEConfigPath(void)
{
	static std::string s_configPath;

	if(s_configPath.empty())
	{
		std::string	falloutPath = GetFalloutDirectory();
		if(!falloutPath.empty())
		{
			s_configPath = falloutPath + "Data\\NVSE\\nvse_config.ini";

			PrintLog("config path = %s", s_configPath.c_str());
		}
	}

	return s_configPath;
}

std::string GetNVSEConfigOption(const char * section, const char * key)
{
	std::string	result;

	const std::string & configPath = GetNVSEConfigPath();
	if(!configPath.empty())
	{
		char	resultBuf[256];
		resultBuf[0] = 0;

		UInt32	resultLen = GetPrivateProfileString(section, key, nullptr, resultBuf, 255, configPath.c_str());

		result = resultBuf;
	}

	return result;
}

bool GetNVSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut)
{
	std::string	data = GetNVSEConfigOption(section, key);
	if(data.empty())
		return false;

	return (sscanf_s(data.c_str(), "%lu", dataOut) == 1);
}

namespace MersenneTwister
{

/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

#undef N
#undef M
#undef MATRIX_A
#undef UPPER_MASK
#undef LOWER_MASK

};

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

const char GetSeparatorChar(Script * script)
{
	if(IsConsoleMode())
	{
		if(script && script->GetModIndex() != 0xFF)
			return '|';
		else
			return '@';
	}
	else
		return '|';
}

const char * GetSeparatorChars(Script * script)
{
	if(IsConsoleMode())
	{
		if(script && script->GetModIndex() != 0xFF)
			return "|";
		else
			return "@";
	}
	else
		return "|";
}

void Console_Print_Long(const std::string& str)
{
	UInt32 numLines = str.length() / 500;
	for (UInt32 i = 0; i < numLines; i++)
		Console_Print("%s ...", str.substr(i*500, 500).c_str());

	Console_Print("%s", str.substr(numLines*500, str.length() - numLines*500).c_str());
}

#endif

struct ControlName
{
	UInt32		unk0;
	const char	* name;
	UInt32		unkC;
};

ControlName ** g_keyNames = (ControlName **)0x011D52F0;
ControlName ** g_mouseButtonNames = (ControlName **)0x011D5240;
ControlName ** g_joystickNames = (ControlName **)0x011D51B0;

const char * GetDXDescription(UInt32 keycode)
{
	const char * keyName = "<no key>";

	if(keycode <= 220)
	{
		if(g_keyNames[keycode])
			keyName = g_keyNames[keycode]->name;
	}
	else if(255 <= keycode && keycode <= 263)
	{
		if(keycode == 255)
			keycode = 256;
		if(g_mouseButtonNames[keycode - 256])
			keyName = g_mouseButtonNames[keycode - 256]->name;
	}
	else if (keycode == 264)
		keyName = "WheelUp";
	else if (keycode == 265)
		keyName = "WheelDown";

	return keyName;
}

bool ci_equal(char ch1, char ch2)
{
	return tolower((unsigned char)ch1) == tolower((unsigned char)ch2);
}

bool ci_less(const char* lh, const char* rh)
{
	ASSERT(lh && rh);
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
	char* result = (char*)FormHeap_Allocate(size+1);
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

const char* GetModName(Script* script)
{
	const char* modName = "In-game console";
	if (script->GetModIndex() != 0xFF)
	{
		modName = DataHandler::GetSingleton()->GetNthModName(script->GetModIndex());
		if (!modName || !modName[0])
			modName = "Unknown";
	}
	return modName;
}

void ShowRuntimeError(Script* script, const char* fmt, ...)
{
	if (!script) return;
	
	va_list args;
	va_start(args, fmt);

	char errorMsg[0x800];
	vsprintf_s(errorMsg, sizeof(errorMsg), fmt, args);
	
	char errorHeader[0x900];
	const auto* modName = GetModName(script);
	
	const auto* scriptName = script ? script->GetName() : nullptr; // JohnnyGuitarNVSE allows this
	if (scriptName && strlen(scriptName) != 0)
	{
		sprintf_s(errorHeader, sizeof(errorHeader), "Error in script %08X (%s) in mod %s\n%s", script ? script->refID : 0, scriptName, modName, errorMsg);
	}
	else
	{
		sprintf_s(errorHeader, sizeof(errorHeader), "Error in script %08X in mod %s\n%s", script ? script->refID : 0, modName, errorMsg);
	}

	g_warnedScripts.insert(script->refID);
	
	if (g_warnedScripts.find(script->refID) != g_warnedScripts.end())
	{
		char message[512];
		snprintf(message, sizeof(message), "%s: kNVSE error (see console print)", GetModName(script));
		if (!IsConsoleMode())
			QueueUIMessage(message, 0, reinterpret_cast<const char*>(0x1049638), nullptr, 2.5F, false);
	}

	Console_Print(errorHeader);
	PrintLog(errorHeader);

	va_end(args);
}

std::string FormatString(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char msg[0x400];
	vsprintf_s(msg, 0x400, fmt, args);
	return msg;
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
		PrintLog("Failed to find GECK Extender Message Log window");
		return;
	}
	
	char buffer[0x400];
	vsprintf_s(buffer, 0x400, fmt, args);
	strcat_s(buffer, "\n");

	SendMessage(window, 0x8004, 0, reinterpret_cast<LPARAM>(buffer));
}

bool Cmd_Default_Execute(COMMAND_ARGS)
{
	return true;
}

bool Cmd_Default_Eval(COMMAND_ARGS_EVAL)
{
	return true;
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
		call    _malloc_base
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

std::string GetCurPath()
{
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
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

#include <cstdlib>

__declspec(noreturn) static void IErrors_Halt(void)
{
	// crash
	*((int*)nullptr) = 0xDEADBEEF;
}

/**
 *	Report a failed assertion and exit the program
 *
 *	@param file the file where the error occured
 *	@param line the line number where the error occured
 *	@param desc an error message
 */
void _AssertionFailed(const char* file, unsigned long line, const char* desc)
{
	PrintLog("Assertion failed in %s (%d): %s", file, line, desc);

	IErrors_Halt();
}

/**
 *	Report a failed assertion and exit the program
 *
 *	@param file the file where the error occured
 *	@param line the line number where the error occured
 *	@param desc an error message
 *	@param code the error code
 */
void _AssertionFailed_ErrCode(const char* file, unsigned long line, const char* desc, unsigned long long code)
{
	if (code & 0xFFFFFFFF00000000)
		PrintLog("Assertion failed in %s (%d): %s (code = %16I64X (%I64d))", file, line, desc, code, code);
	else
	{
		UInt32	code32 = code;
		PrintLog("Assertion failed in %s (%d): %s (code = %08X (%d))", file, line, desc, code32, code32);
	}

	IErrors_Halt();
}

/**
 *	Report a failed assertion and exit the program
 *
 *	@param file the file where the error occured
 *	@param line the line number where the error occured
 *	@param desc an error message
 *	@param code the error code
 */
void _AssertionFailed_ErrCode(const char* file, unsigned long line, const char* desc, const char* code)
{
	PrintLog("Assertion failed in %s (%d): %s (code = %s)", file, line, desc, code);

	IErrors_Halt();
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
		mov		al, byte ptr ds : [0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, fs : [0x2C]
		mov		edx, ds : [0x126FD98]
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		al, [eax + 0x268]
	done :
		 retn
	}
}