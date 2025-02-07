#include <CrashLogger.hpp>

namespace CrashLogger
{

	inline bool GetStringForClassLabel(void* object, std::string& labelName, std::string& objectName, std::string& description)
	try
	{
		static bool fillLables = false;

		if (!fillLables)
		{
			Labels::FillLabels();
			fillLables = true;
		}

		for (const auto& iter : Labels::Label::GetAll()) if (iter->Satisfies(object))
		{
			labelName = iter->GetLabelName();
			objectName = iter->GetName(object);
			description = iter->GetDescription(object);
			return true;
		}

		return false;
	}
	catch (...) {
		return false;
	}

	bool GetAsString(const void* object, std::string& labelName, std::string& string)
	try {
		const auto printable = [](const char a_ch) noexcept {
			if (' ' <= a_ch && a_ch <= '~') return true;

			switch (a_ch) {
			case '\t':
			case '\n':
				return true;
			default:
				return false;
			}
		};

		const auto cstr = static_cast<const char*>(object);
		constexpr std::size_t max = MAX_PATH;

		std::size_t len = 0;
		for (; len < max && cstr[len] != '\0'; ++len) if (!printable(cstr[len])) return false;

		if (len == 0 || len >= max || len < 3) return false;

		const auto str = SanitizeString(cstr);

		// Check if string is equal to a predefined prefix and print out the file name if true
		if (const auto pos = str.find("D:\\_Fallout3\\"); pos == std::string::npos) {
			labelName = "String";
			string = str;
		} else {
			const std::filesystem::path path = str.substr(pos);
			labelName = "Source";
			string = path.filename().string();
		}

		return true;
	} catch (...) {
		return false;
	}

}

namespace CrashLogger::Registry
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try
	{
		char textBuffer[128];
		sprintf_s(textBuffer, "Registry:\n REG | %*s%*s | DEREFERENCE INFO\n", CENTERED_TEXT(10, "Value"));
		output << textBuffer;

		const std::map<std::string_view, UInt32> registers{
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
			std::stringstream str;
			sprintf_s(textBuffer, "%s | 0x%08X | ", name.data(), value);
			str << textBuffer;
			const auto buffer = Stack::GetLineForObject((void**)value, 5);
			if (!buffer.empty()) str << buffer;

			output << str.str() << '\n';
		}
	}
	catch (...) { output << "Failed to log registry." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}

namespace CrashLogger::Stack
{
	std::map<UInt32, UInt8> memoize;
	
	std::stringstream output;

	// can be done smartly by checking bounds of modules
	UInt32 VFTableLowerLimit()
	{
		if (g_currentGame == kFalloutNewVegas)
			return 0xFDF000;
		else if (g_currentGame == kFallout3) 	// thanks Stewie
			return 0xD9B000;
		else if (g_currentGame == kOblivion)
			return 0x0;
		return 0;
	}

	bool GetStringForRTTIorPDB(void** object, std::string& buffer)
	try {
//		if (*(UInt32*)object > VFTableLowerLimit() && *(UInt32*)object < 0x1200000)
		{
			char textBuffer[256];
			if (const auto& name = PDB::GetClassNameFromRTTIorPDB((void*)object); !name.empty())
			{
				sprintf_s(textBuffer, "0x%08X ==> RTTI: %s", *(UInt32*)object, name.c_str());
				buffer = textBuffer;
				return true;
			}
		}
		return false;
	}
	catch (...) { return false; }


	bool GetStringForLabel(void** object, std::string& buffer)
	try {

		std::string labelName, objectName, description;

		if (GetStringForClassLabel(object, labelName, objectName, description))
		{
			char textBuffer[256];
			if (description.empty()) {
				sprintf_s(textBuffer, "0x%08X ==> %s: %s", *(UInt32*)object, labelName.c_str(), objectName.c_str());
			}
			else {
				sprintf_s(textBuffer, "0x%08X ==> %s: %s:\n%s", *(UInt32*)object, labelName.c_str(), objectName.c_str(), description.c_str());
			}
			buffer += textBuffer;
			return true;
		}

		if (GetStringForRTTIorPDB(object, buffer)) return true;

		if (GetAsString(object, labelName, description))
		{
			char textBuffer[256];
			sprintf_s(textBuffer, "0x%08X ==> %s: %s", *(UInt32*)object, labelName.c_str(), description.c_str());
			return true;
		}

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
			char textBuffer[32];
			sprintf_s(textBuffer, "0x%08X ==> ", deref);
			buffer += textBuffer;
			object = (void**)deref;
			depth--;
		} while (object && depth);

		return "";
	}

	UInt32 GetESPi(UInt32* esp, UInt32 i) try { return esp[i]; } catch (...) { return 0; }

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		char textBuffer[128];
		sprintf_s(textBuffer, "Stack:\n  # | %*s%*s | DEREFERENCE INFO\n", CENTERED_TEXT(10, "Value"));
		output << textBuffer;

		const auto esp = reinterpret_cast<UInt32*>(info->ContextRecord->Esp);

		for (unsigned int i : std::views::iota(0x0, 0x100)) {
			const auto espi = GetESPi(esp, i);

			const auto str = Stack::GetLineForObject((void**)espi, 5);

			if (i <= 0x8 || (!str.empty() && !memoize.contains(espi)))
			{
				std::stringstream line;
				sprintf_s(textBuffer, " %02X | 0x%08X | ", i, espi);
				line << textBuffer;
				if (!memoize.contains(espi))
				{
					if (!str.empty()) line << str;				
					memoize.emplace(espi, i);
				}
#if 0
				else
				{
					sprintf_s(textBuffer, "Identical to %02X", memoize[espi]);
					line << textBuffer;
				}
#endif
				output << line.str() << '\n';

			}
		}
	}
	catch (...) { output << "Failed to log stack." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}