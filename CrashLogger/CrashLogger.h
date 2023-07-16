#pragma once

namespace CrashLogger
{
	void ApplyNVSE();
	void ApplyFOSE();
	void ApplyOBSE();
	namespace VirtualTables
	{
		void FillNVSELabels();
		void FillFOSELabels();
		void FillOBSELabels();
	}
}

struct TypeDescriptor
{
	void* pVFTable;
	void* spare;
	char name[];
};

struct _RTTICompleteObjectLocator
{
	int signature;
	int offset;
	int cdOffset;
	TypeDescriptor* typeDescriptor;
	void* classDescriptor;
};

template <typename R> R	ReadData(size_t ptr) { return *((const R*)ptr); }

template <typename R, typename T> R	ReadData(const T* ptr) { return ReadData<R>((size_t)ptr); }

template <typename T> inline _RTTICompleteObjectLocator* GetCompleteObjectLocator(const T* ptr) // not have vbtable
{
	ptrdiff_t offset = 0;
	if (ReadData<ptrdiff_t>(ReadData<size_t>(ptr)) == 0)
		offset = ReadData<ptrdiff_t>(ReadData<size_t>(ptr) + 4);
	return (_RTTICompleteObjectLocator*)(ReadData<size_t>(ReadData<size_t>((size_t)ptr + offset) - 4));
}

template <typename T> inline TypeDescriptor* __RTtypeid(T* ptr)
{
	if (!ptr) return nullptr;

	const _RTTICompleteObjectLocator* pCompleteLocator = GetCompleteObjectLocator(ptr);

	if (!pCompleteLocator) return nullptr;

	TypeDescriptor* pTypeDescriptor = pCompleteLocator->typeDescriptor;

	return pTypeDescriptor;
}

inline TypeDescriptor* WrappedRTTypeID(void* object)
{
	__try {
		return __RTtypeid<void>(object);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return nullptr;
	}
}

inline std::string printTypeName(void* object) {
	try {
		const auto typeInfo = WrappedRTTypeID(object);

		if (!typeInfo) return "";

		const std::string name = typeInfo->name;
		return name.substr(4, name.size() - 6);
	}
	catch (const std::exception& e) {
		return "";
	}
}
