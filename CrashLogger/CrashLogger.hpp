#include <main.hpp>

#include <Safewrite.hpp>
#include <DbgHelpWrapper.hpp>
#include <set>
#include <map>

namespace CrashLogger::Playtime		{ inline void Init(); inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Exception	{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Thread		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Calltrace	{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Registry		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Stack		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Modules		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Install		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Memory		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Mods			{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::Device		{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }
namespace CrashLogger::AssetTracker	{ inline void Process(EXCEPTION_POINTERS* info); inline std::stringstream& Get(); }

namespace CrashLogger::Stack
{
	inline std::string GetLineForObject(void** object, UInt32 depth);
}

namespace CrashLogger::PDB
{
	inline std::string GetModule(UInt32 eip, HANDLE process);
	inline UInt32 GetModuleBase(UInt32 eip, HANDLE process);
	inline std::string GetSymbol(UInt32 eip, HANDLE process);
	inline std::string GetLine(UInt32 eip, HANDLE process);
	inline std::string GetClassNameFromRTTIorPDB(void* object);
}

namespace CrashLogger
{
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
			return IsValidPointer();
		}

		operator T* () {
			if (IsValidPointer()) {
				return reinterpret_cast<T*>(pointer);
			}

			throw std::runtime_error("Bad dereference");
		}

		T* operator->() {
			if (IsValidPointer()) {
				return reinterpret_cast<T*>(pointer);
			}

			throw std::runtime_error("Bad dereference");
		}

	private:
		bool IsValidAddress() const
		{
			MEMORY_BASIC_INFORMATION mbi;
			if (::VirtualQuery((void*)pointer, &mbi, sizeof(mbi)))
			{
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

		bool AttemptDereference() const 
			try {
			// Attempt to read the address as a UInt32
			volatile UInt32 temp = *reinterpret_cast<const volatile UInt32*>(pointer);
			return true;
		}
		catch (...) {
			return false;
		}

		bool IsVtableValid() const
		{
			//			if (vtables_.find(vtable) == vtables_.end()) return false;

			UInt32 vtable = *reinterpret_cast<UInt32*>(pointer);

			if (vtable > 0xFE0000 && vtable < 0x1200000)
				return true;

			return false;
		}

		bool IsValidPointer() const 
			try {
			if (!IsValidAddress()) return false;

			if (!AttemptDereference()) return false;

			if (!IsVtableValid()) return false;

			return true;
		}
		catch (...)
		{
			return false;
		}
	};
}

namespace CrashLogger::Labels
{
	inline std::string AsUInt32(void* ptr) { return std::format("{:#08X}", **static_cast<UInt32**>(ptr)); }

	template<typename T> std::string As(void* ptr)
	try {
		if (auto sanitized = Dereference<T>((UInt32)ptr))
			return LogClassLineByLine(*sanitized);
		return "Unable to dereference";
	}
	catch (...) {
		return "Failed to format";
	}


	class Label
	{
		static inline std::vector<std::unique_ptr<Label>> labels;
		typedef std::string (*FormattingHandler)(void* ptr);
		static inline FormattingHandler lastHandler = nullptr;

	public:

		UInt32 address;
		UInt32 size;
		FormattingHandler function = lastHandler;
		std::string name;

		static auto& GetAll() { return labels; }

		Label() : address(0), size(0), function(nullptr) {}
		virtual ~Label() = default;
		
		Label(UInt32 aAddress, FormattingHandler aFunction = lastHandler, std::string aName = "", UInt32 aSize = 4)
			: Label()
		{
			address = aAddress;
			size = aSize;
			function = aFunction;
			name = std::move(aName);

			lastHandler = aFunction;
		};
		
		bool Satisfies(void* ptr) const
		try {
			return *static_cast<UInt32*>(ptr) >= address && *static_cast<UInt32*>(ptr) <= address + size; 
		}
		catch (...)
		{
			return false;
		}

		static std::string GetTypeName(void* ptr)
		{
			return PDB::GetClassNameFromRTTIorPDB(ptr);
		}
		
		virtual std::string GetLabelName() const { return "None"; }

		virtual std::string GetName(void* object) const { return name; }
		
		virtual std::string GetDescription(void* object) const
		{
			if (function) return function(object);
			return "";
		}
	};

	class LabelClass : public Label
	{
	public:
		using Label::Label;
		
		std::string GetLabelName() const override { return "Class"; }

		std::string GetName(void* object) const override { return name.empty() ? GetTypeName(object) : name; }
	};

	class LabelGlobal : public Label
	{
	public:
		using Label::Label;
		
		std::string GetLabelName() const override { return "Global"; }
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
	
	inline void FillFOSELabels() {}

	inline void FillOBSELabels() {}

	inline void FillLabels()
	{
		if (g_currentGame == kFalloutNewVegas)
			FillNVSELabels();
		else if (g_currentGame == kFallout3) 	// thanks Stewie
			FillFOSELabels();
		else if (g_currentGame == kOblivion)
			FillOBSELabels();
	}
}