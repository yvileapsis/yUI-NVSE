#include <main.hpp>

#include <Safewrite.hpp>
#include <DbgHelpWrapper.hpp>
#include <set>
#include <map>

namespace CrashLogger::Playtime		{ inline void Init(); inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Exception	{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Thread		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Calltrace	{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Registry		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Stack		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Modules		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Install		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Memory		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Mods			{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::Device		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }
namespace CrashLogger::GameData		{ inline void __fastcall Process(EXCEPTION_POINTERS* info); }

namespace CrashLogger::Stack
{
	inline bool __fastcall GetLineForObject(const char* prefix, void** object, UInt32 depth);
}

namespace CrashLogger::PDB
{
	inline bool __fastcall GetModule(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize);
	inline UInt32 __fastcall GetModuleBase(UInt32 eip, HANDLE process);
	inline bool __fastcall GetSymbol(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize);
	inline bool __fastcall GetLine(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize);
	inline bool __fastcall GetClassNameFromRTTIorPDB(void* object, char* buffer, size_t bufferSize);
}

namespace CrashLogger
{
	namespace {
		static bool __fastcall IsValidAddress(void* pointer, size_t size) {
			MEMORY_BASIC_INFORMATION mbi;
			if (::VirtualQuery(pointer, &mbi, sizeof(mbi))) {
				if (mbi.State != MEM_COMMIT) return false;

				DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
				if ((mbi.Protect & mask) == 0) return false;

				if (mbi.Protect & PAGE_GUARD) return false;
				if (mbi.Protect & PAGE_NOACCESS) return false;

				if (size_t(mbi.RegionSize) < size) return false;

				return true;
			}
			return false;
		}

		static bool __fastcall AttemptDereference(void* pointer)
			try {
				// Attempt to read the address as a UInt32
				volatile UInt32 temp = *reinterpret_cast<const volatile UInt32*>(pointer);
				return true;
			}
		catch (...) {
			return false;
		}

		static bool __fastcall IsVtableValid(void* pointer) {
			UInt32 vtable = *reinterpret_cast<UInt32*>(pointer);

			if (vtable > 0xFE0000 && vtable < 0x1200000)
				return true;

			return false;
		}

		static bool __fastcall IsValidPointer(void* pointer, size_t size)
			try {
				if (!IsValidAddress(pointer, size)) return false;

				if (!AttemptDereference(pointer)) return false;

				if (!IsVtableValid(pointer)) return false;

				return true;
			}
		catch (...) {
			return false;
		}
	}

	template<typename T>
	class Dereference {
		intptr_t pointer;
		std::size_t size;

	public:
		Dereference(intptr_t pointer, std::size_t size) : pointer(pointer), size(size) {}

		Dereference(intptr_t pointer) : pointer(pointer), size(sizeof(T)) {}
		Dereference(const void* pointer) : pointer((intptr_t) pointer), size(sizeof(T)) {}

		operator bool()
		{
			return IsValidPointer(reinterpret_cast<void*>(pointer), size);
		}

		operator T* () {
			if (IsValidPointer(reinterpret_cast<void*>(pointer), size)) {
				return reinterpret_cast<T*>(pointer);
			}
			return nullptr;
		}

		T* operator->() {
			if (IsValidPointer(reinterpret_cast<void*>(pointer), size)) {
				return reinterpret_cast<T*>(pointer);
			}
			return nullptr;
		}
	};
}

namespace CrashLogger::Labels
{
	inline void __fastcall AsUInt32(void* ptr) {
		_MESSAGE("%#08X", **static_cast<UInt32**>(ptr));
	}

	template<typename T> void __fastcall As(void* ptr)
	try {
		if (auto sanitized = Dereference<T>((UInt32)ptr)) {
			LogClassLineByLine(*sanitized);
		}
		else{
			_MESSAGE("Unable to dereference");
		}
	}
	catch (...) {
		_MESSAGE("Failed to format");
	}


	class Label
	{
		static inline std::vector<std::unique_ptr<Label>> labels;
		typedef void (__fastcall *FormattingHandler)(void* ptr);
		static inline FormattingHandler lastHandler = nullptr;

	public:

		UInt32 address;
		UInt32 size;
		FormattingHandler function = lastHandler;
		mutable std::string labelName;
		mutable std::string name;
		mutable std::string description;

		static auto& GetAll() { return labels; }

		Label() : address(0), size(0), function(nullptr) {}
		virtual ~Label() = default;
		
		Label(UInt32 aAddress, FormattingHandler aFunction = lastHandler, const char* aName = "", UInt32 aSize = 4)
			: Label()
		{
			address = aAddress;
			size = aSize;
			function = aFunction;
			name = aName;

			lastHandler = aFunction;
		};
		
		bool __fastcall Satisfies(void* ptr) const
		try {
			return *static_cast<UInt32*>(ptr) >= address && *static_cast<UInt32*>(ptr) <= address + size; 
		}
		catch (...)
		{
			return false;
		}

		static bool __fastcall GetTypeName(void* ptr, char* buffer, size_t bufferSize)
		{
			return PDB::GetClassNameFromRTTIorPDB(ptr, buffer, bufferSize);
		}
		
		virtual const char* __fastcall GetLabelName() const { return nullptr; }

		virtual const char* __fastcall GetName(void* object) const { return name.c_str(); }
		
		virtual void __fastcall GetDescription(void* object) const
		{
			if (function)
				function(object);
			else if (!description.empty())
				_MESSAGE(description.c_str());
		}
	};

	class LabelClass : public Label
	{
	public:
		using Label::Label;
		
		const char* __fastcall GetLabelName() const override { return "Class"; }

		const char* __fastcall GetName(void* object) const override {
			if (name.empty()) {
				char buffer[MAX_PATH];
				if (GetTypeName(object, buffer, sizeof(buffer)))
					name = buffer;
			}
			
			return name.c_str();
		}
	};

	class LabelGlobal : public Label
	{
	public:
		using Label::Label;
		
		const char* __fastcall GetLabelName() const override { return "Global"; }
	};

	class LabelEmpty : public Label
	{
	public:
		using Label::Label;
	};
	
	template <class LabelType = LabelClass, class... _Types> void Push(_Types... args) {
		Label::GetAll().push_back(std::make_unique<LabelType>(std::forward<_Types>(args)...));
	}

	void FillNVSELabels();
	
	inline void FillLabels()
	{
		FillNVSELabels();
	}
}