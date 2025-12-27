#include <CrashLogger.hpp>

namespace CrashLogger
{

	bool fillLables = false;

	bool __fastcall GetClassLabelData(const char* prefix, void* object, Labels::Label& label) {
		const char* labelName = label.GetLabelName();
		if (!labelName) {
			if (!prefix)
				_MESSAGE("0x%08X", *reinterpret_cast<UInt32*>(object));
			else
				_MESSAGE("%s 0x%08X", prefix, *reinterpret_cast<UInt32*>(object));
			return true;
		}

		if (!prefix)
			_MESSAGE("0x%08X => %s: %s", *reinterpret_cast<UInt32*>(object), labelName, label.GetName(object));
		else	
			_MESSAGE("%s 0x%08X => %s: %s", prefix, *reinterpret_cast<UInt32*>(object), labelName, label.GetName(object));
		const uint32_t orgIndent = IDebugLog::GetIndent();
		IDebugLog::SetIndent(19);
		__try {
			label.GetDescription(object);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			_MESSAGE("Failed to retrieve description!");
		}
		IDebugLog::SetIndent(orgIndent);
		return true;
	}

	bool __fastcall GetStringForClassLabel(const char* prefix, void* object) {
		if (!fillLables) {
			Labels::FillLabels();
			fillLables = true;
		}

		for (const auto& iter : Labels::Label::GetAll()) {
			if (iter->Satisfies(object)) {
				return GetClassLabelData(prefix, object, *iter);
			}
		}

		return false;
	}

	bool __fastcall GetAsString(const char* prefix, const void* object) {
		try {
			const auto printable = [](const char a_ch) noexcept {
				if (' ' <= a_ch && a_ch <= '~') 
					return true;

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
			for (; len < max && cstr[len] != '\0'; ++len) {
				if (!printable(cstr[len]))
					return false;
			}

			if (len == 0 || len >= max || len < 3) 
				return false;

			char sanitizedBuffer[MAX_PATH] = {};
			const char* str = SanitizeString(cstr, sanitizedBuffer, sizeof(sanitizedBuffer));

			// Check if string is equal to a predefined prefix and print out the file name if true
			const char* pos = strstr(str, "D:\\_Fallout3\\");
			if (!pos) {
				if (!prefix)
					_MESSAGE("0x%08X => %s: %s", object, "String", str);
				else
					_MESSAGE("%s 0x%08X => %s: %s", prefix, object, "String", str);
			}
			else {
				char fileName[MAX_PATH] = {};
				_splitpath_s(str, nullptr, 0, nullptr, 0, fileName, sizeof(fileName), nullptr, 0);
				if (!prefix)
					_MESSAGE("0x%08X => %s: %s", object, "Source", fileName);
				else
					_MESSAGE("%s 0x%08X => %s: %s", prefix, object, "Source", fileName);
			}

			return true;
		}
		catch (...) {
			return false;
		}
	}

}

namespace CrashLogger::Registry
{
	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		_MESSAGE("\nRegistry:\n REG | %*s%*s | DEREFERENCE INFO", CENTERED_TEXT(10, "Value"));

		const std::map<const char*, UInt32> registers{
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


		for (const auto& [name, value] : registers) {
			char prefix[64];
			sprintf_s(prefix, " %s | 0x%08X |", name, value);
			try {
				if (!Stack::GetLineForObject(prefix, (void**)value, 5))
					_MESSAGE(prefix);
			}
			catch (...) {
				_MESSAGE(prefix);
			}
		}

	}
}

namespace CrashLogger::Stack
{
	std::map<UInt32, UInt8> memoize;

	// can be done smartly by checking bounds of modules
	UInt32 VFTableLowerLimit()
	{
		return 0xFDF000;
	}

	bool __fastcall GetStringForRTTIorPDB(const char* prefix, void** object) {
		bool result = false;
		try {
			char buffer[MAX_PATH];
			if (PDB::GetClassNameFromRTTIorPDB((void*)object, buffer, sizeof(buffer))) {
				if (!prefix)
					_MESSAGE("0x%08X => RTTI: %s", *reinterpret_cast<UInt32*>(object), buffer);
				else
					_MESSAGE("%s 0x%08X => RTTI: %s", prefix, *reinterpret_cast<UInt32*>(object), buffer);
				result = true;
			}
		}
		catch (...) {
			result = false;
		}
		return result;
	}


	bool __fastcall GetStringForLabel(const char* prefix, void** object) {
		try {
			UInt32 objPtr = *(UInt32*)object;
			if (objPtr == 0) {
				return true;
			}

			if (GetStringForClassLabel(prefix, object)) {
				return true;
			}

			if (GetStringForRTTIorPDB(prefix, object))
				return true;

#if 0
			if (GetAsString(prefix, object)) {
				return true;
			}
#endif
			return false;
		}
		catch (...) {
			return false;
		}
	}

	bool __fastcall GetLineForObject(const char* prefix, void** object, UInt32 depth) {
		if (!object)
			return false;

		try {
			UInt32 deref = 0;
			do {
				if (GetStringForLabel(prefix, object))
					return true;

				deref = Dereference<UInt32>(object);
				if (deref == 0)
					return false;

				if (!prefix)
					_MESSAGE("0x%08X", deref);
				else
					_MESSAGE("%s 0x%08X", prefix, deref);

				object = (void**)deref;
				depth--;
			} while (object && depth);

			return false;
		}
		catch (...) {
			return false;
		}
	}

	UInt32 __fastcall GetESPi(UInt32* esp, UInt32 i) try { return esp[i]; } catch (...) { return 0; }

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		
		_MESSAGE("\nStack:\n  # | %*s%*s | DEREFERENCE INFO", CENTERED_TEXT(10, "Value"));

		const auto esp = reinterpret_cast<UInt32*>(info->ContextRecord->Esp);
		try {
			for (UInt32 i : std::views::iota(0x0, 0x100)) {
				const auto espi = GetESPi(esp, i);
				if (espi == 0)
					continue;

				//_MESSAGE(" %02X | 0x%08X", i, espi);
				if (!memoize.contains(espi)) {
					char prefix[64];
					sprintf_s(prefix, " %02X | 0x%08X |", i, espi);
					if (!Stack::GetLineForObject(prefix, (void**)espi, 5) && i < 0xF)
						_MESSAGE(prefix);
					memoize.emplace(espi, i);
				}
			}
		}
		catch (...) {
			_MESSAGE("Failed to log stack."); 
		}
	}
}