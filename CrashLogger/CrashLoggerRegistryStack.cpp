#include <CrashLogger.hpp>

namespace CrashLogger::Registry
{
	extern void Get(EXCEPTION_POINTERS* info)
	try
	{
		Log() << "Registry:" << std::endl
			<< ("REG |    VALUE   | DEREFERENCE INFO");

		const std::map<std::string, UInt32> registers{
			{ "eax", info->ContextRecord->Eax },
			{ "ebx", info->ContextRecord->Ebx },
			{ "ecx", info->ContextRecord->Ecx },
			{ "edx", info->ContextRecord->Edx },
			{ "edi", info->ContextRecord->Edi },
			{ "esi", info->ContextRecord->Esi },
			{ "ebp", info->ContextRecord->Ebp },
			{ "esp", info->ContextRecord->Esp },
			{ "eip", info->ContextRecord->Eip },
		};

		for (const auto& [name, value] : registers)
		{
			Log log{};
			log << std::format("{} | 0x{:08X} | ", name, value);
			const auto buffer = Stack::GetLineForObject((void**)value, 5);
			if (!buffer.empty()) log << buffer;
		}

		Log();
	}
	catch (...) { Log() << "Failed to log registry." << std::endl; }
}

namespace CrashLogger::Stack
{
	UInt32 VFTableLowerLimit()
	{
		if (g_currentGame == kFalloutNewVegas)
			return 0xFDF000;
		else if (g_currentGame == kFallout3) 	// thanks Stewie
			return 0xD9B000;
		else if (g_currentGame == kOblivion)
			return 0x0;
	}

	bool GetStringForRTTIorPDB(void** object, std::string& buffer)
	try {
		if (*(UInt32*)object > VFTableLowerLimit() && *(UInt32*)object < 0x1200000)
		{
			if (const auto& name = PDB::GetClassNameFromRTTIorPDB((void*)object); !name.empty())
			{
				buffer += std::format("0x{:08X} ==> Unhandled: ", *(UInt32*)object) + name;
				return true;
			}
		}
		return false;
	}
	catch (...) {}

	// taken out of CrashLoggerSSE
	bool GetAsString(void** object, std::string& buffer)
	{
		try {
			const auto printable = [](char a_ch) noexcept {
				if (' ' <= a_ch && a_ch <= '~') return true;

				switch (a_ch) {
				case '\t':
				case '\n':
					return true;
				default:
					return false;
				}
				};

			const auto str = reinterpret_cast<const char*>(object);
			constexpr std::size_t max = MAX_PATH;

			std::size_t len = 0;
			for (; len < max && str[len] != '\0'; ++len) if (!printable(str[len])) return false;
		
			if (len == 0 || len >= max || len < 3) return false;

			buffer += std::format(R"(0x{:08X} ==> String: "{}")", *(UInt32*)object, std::string(str));
			return true;
		} catch (...) {
			return false;
		}
	}

	bool GetStringForLabel(void** object, std::string& buffer)
	try {
		if (Labels::GetStringForLabel(object, buffer)) return true;

		if (GetStringForRTTIorPDB(object, buffer)) return true;

		if (GetAsString(object, buffer)) return true;

		return false;
	}
	catch (...) {
		return false;
	}

	std::string GetLineForObject(void** object, UInt32 depth)
	{
		if (!object) return "";
		std::string buffer;
		UInt32 deref = 0;
		do
		{
			if (GetStringForLabel(object, buffer)) return buffer;
			deref = Dereference<UInt32>(object);
			buffer += std::format("0x{:08X} ==> ", deref);
			object = (void**)deref;
			depth--;
		} while (object && depth);

		return "";
	}

	UInt32 GetESPi(UInt32* esp, UInt32 i) try { return esp[i]; } catch (...) { return 0; }

	extern void Get(EXCEPTION_POINTERS* info)
	try {
		Log() << "Stack:" << std::endl << " # |    VALUE   | DEREFERENCE INFO";

		const auto esp = reinterpret_cast<UInt32*>(info->ContextRecord->Esp);

		for (unsigned int i : std::views::iota(0x0, 0x100)) {
			const auto espi = GetESPi(esp, i);

			const auto str = Stack::GetLineForObject((void**)espi, 5);

			if (i <= 0x8 || !str.empty())
			{
				Log line{};
				line << std::format("{:2X} | 0x{:08X} | ", i, espi);
				if (!str.empty()) line << str;
			}
		}
		Log();
	}
	catch (...) { Log() << "Failed to log stack." << std::endl; }
}
