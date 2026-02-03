#include <CrashLogger.hpp>

#define SYMOPT_EX_WINE_NATIVE_MODULES 1000

constexpr UInt32 ce_printStackCount = 256;

namespace CrashLogger::PDB 
{
	extern bool __fastcall GetModule(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize)
	{
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!Safe_SymGetModuleInfo(process, eip, &module)) 
			return false;

		strcpy_s(buffer, bufferSize, module.ModuleName);
		return true;
	}

	extern UInt32 __fastcall GetModuleBase(UInt32 eip, HANDLE process)
	{
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!Safe_SymGetModuleInfo(process, eip, &module)) return 0;

		return module.BaseOfImage;
	}

	extern bool __fastcall GetSymbol(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize)
	{
		char symbolBuffer[sizeof(SYMBOL_INFO) + 255];
		const auto symbol = (SYMBOL_INFO*)symbolBuffer;

		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = 254;
		DWORD64 offset = 0;

		if (!Safe_SymFromAddr(process, eip, &offset, symbol)) 
			return false;

		sprintf_s(buffer, bufferSize, "%s+0x%I64X", symbol->Name, offset);
		return true;
	}

	extern bool __fastcall GetLine(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize)
	{
		char lineBuffer[sizeof(IMAGEHLP_LINE) + 255];
		const auto line = (IMAGEHLP_LINE*)lineBuffer;
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD offset = 0;

		if (!Safe_SymGetLineFromAddr(process, eip, &offset, line)) 
			return false;

		sprintf_s(buffer, bufferSize, "%s:%d", line->FileName, line->LineNumber);
		return true;
	}

	bool __fastcall GetClassNameGetSymbol(void* object, char* buffer, size_t bufferSize)
	{
		return GetSymbol(*((UInt32*)object), GetCurrentProcess(), buffer, bufferSize);
	}

	bool __fastcall GetClassNameFromPDBSEH(void* object, char* buffer, size_t bufferSize)
	try { 
		return GetClassNameGetSymbol(object, buffer, bufferSize);
	}
	catch (...) { 
		return false; 
	}


	bool __fastcall GetClassNameFromPDB(void* object, char* buffer, size_t bufferSize)
	{
		char tempBuffer[MAX_PATH];
		if (GetClassNameFromPDBSEH(object, tempBuffer, sizeof(tempBuffer))) {
			std::string_view name = buffer;
			strcpy_s(buffer, bufferSize, name.substr(0, name.find("::`vftable'")).data());
			return !name.empty();
		}
		return false;

	}

	struct RTTIType
	{
		void*	typeInfo;
		UInt32	pad;
		char	name[0];
	};

	struct RTTILocator
	{
		UInt32		sig, offset, cdOffset;
		RTTIType	* type;
	};

	// use the RTTI information to return an object's class name
	const char* __fastcall GetObjectClassNameInternal2(void * objBase)
	try
	{
		const char* result = "";
		void		** obj = (void **)objBase;
		RTTILocator	** vtbl = (RTTILocator **)obj[0];
		RTTILocator	* rtti = vtbl[-1];
		RTTIType	* type = rtti->type;

		// starts with .?AV
		if((type->name[0] == '.') && (type->name[1] == '?'))
		{
			// is at most MAX_PATH chars long
			for (UInt32 i = 0; i < MAX_PATH; i++) if (type->name[i] == 0)
			{
				result = type->name;
				break;
			}
		}
		return result;
	}
	catch (...)
	{
		return "";
	}

	bool __fastcall GetClassNameFromRTTI(void* object, char* buffer, size_t bufferSize)
	{
		std::string_view name = GetObjectClassNameInternal2(object);

		// Starts with .?AV, ends with @@
//		return name.substr(4, name.size() - 6);

		char tempBuffer[MAX_PATH];
		Safe_UnDecorateSymbolName(name.substr(1, name.size() - 1).data(), tempBuffer, MAX_PATH, UNDNAME_NO_ARGUMENTS);
		std::string_view undecoratedName = tempBuffer;
		strcpy_s(buffer, bufferSize, undecoratedName.substr(6, undecoratedName.size() - 6).data());
		return !undecoratedName.empty();
	}

	extern bool __fastcall GetClassNameFromRTTIorPDB(void* object, char* buffer, size_t bufferSize)
	{
		if (!GetClassNameFromRTTI(object, buffer, bufferSize)) {
			return GetClassNameFromPDB(object, buffer, bufferSize);
		}
	}
};

namespace CrashLogger
{

	void __fastcall Get(EXCEPTION_POINTERS* info)
	{
		__try {
			const auto begin = std::chrono::system_clock::now();

			Exception::Process(info);
			Thread::Process(info);
			Calltrace::Process(info);
			Registry::Process(info);
			Stack::Process(info);
			PrintSeparator();
			Device::Process(info);
			PrintSeparator();
			Memory::Process(info);
			PrintSeparator();
			Playtime::Process(info);
			GameData::Process(info);
			PrintSeparator();
			Mods::Process(info);
			PrintSeparator();
			Modules::Process(info);
			PrintSeparator();
			Install::Process(info);
			PrintSeparator();

			const auto processing = std::chrono::system_clock::now();

			const auto printing = std::chrono::system_clock::now();

			const auto timeProcessing = std::chrono::duration_cast<std::chrono::milliseconds>(processing - begin);

			_MESSAGE("\nProcessed in %lld ms", timeProcessing.count());

			Safe_SymCleanup(GetCurrentProcess());
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			_MESSAGE("Fatal error in exception handler.");
		}

		Logger::Copy();
	};

	static LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

	LONG WINAPI Filter(EXCEPTION_POINTERS* info) {
		static bool caught = false;
		bool ignored = false;
		//
		if (caught) ignored = true;
		else
		{
			caught = true;
			try { Get(info); }
			catch (...) {};
		}
		if (s_originalFilter) s_originalFilter(info); // don't return
		return !ignored ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	};

	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI FakeSetUnhandledExceptionFilter(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) {
		s_originalFilter = lpTopLevelExceptionFilter;
		return nullptr;
	}

	extern void Init()
	{
		Playtime::Init();

		s_originalFilter = SetUnhandledExceptionFilter(&Filter);

		HMODULE hKernel = GetModuleHandleA("kernel32.dll");
		if (hKernel) {
			auto pAddr = GetProcAddress(hKernel, "SetUnhandledExceptionFilter");
			if (pAddr) {
				WriteRelJump((UInt32)pAddr, (UInt32)&FakeSetUnhandledExceptionFilter);
			}
		}

#if 0
		SafeWrite32(0x00FDF180, (UInt32)&FakeSetUnhandledExceptionFilter);
#endif
	}
}