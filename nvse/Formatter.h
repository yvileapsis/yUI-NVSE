#if 0


// Now, the LoggerStream class can be used with the updated LoggerManager class, and you can create as many LoggerStream instances as needed, specifying different log levels for each. Each LoggerStream sends log messages to the LoggerManager, which processes them according to the defined behavior.
// can you show me an example of std format
// Sure, std::format is a C++20 feature that provides type-safe and extensible string formatting. Here's an example of how to use std::format:

#include <iostream>
#include <format>

int main() {
	int int_value = 42;
	double double_value = 3.14159;
	std::string str_value = "Hello, world!";

	// Format a string with variables
	std::string formatted_string = std::format("An int value: {}\nA double value: {:.2f}\nA string value: {}\n",
		int_value, double_value, str_value);

	std::cout << formatted_string;

	return 0;
}
//In this example, we use the std::format function to format a string using placeholders {}. The variables are passed after the formatting string, and their values will be used to replace the placeholders. You can also add various formatting options, like specifying the number of decimal places for a floating-point value with :.2f.

// This approach provides better safety compared to using printf, as the type of the arguments is checked at compile-time, significantly reducing the chances of runtime errors due to type mismatches.

// To use std::format in a project, you need a C++20-compatible compiler and project settings that enable C++20 features.
// how would I go about implementing formatting for a custom class I have so I could print it to LoggerStream in one line
// To implement formatting for a custom class, you can customize the std::formatter class for your custom class. This allows you to use std::format to print it with LoggerStream in a more fluent way.

// Assume you have a class CustomClass:

class CustomClass {
public:
	CustomClass(const std::string &name, int value)
		: name(name), value(value) {}

private:
	std::string name;
	int value;

	// Define a custom formatter for this class
	friend std::ostream &operator<<(std::ostream &os, const CustomClass &obj) {
		os << "CustomClass{name: " << obj.name << ", value: " << obj.value << "}";
		return os;
	}
};
//Now, define the format function for CustomClass:

#include <format>

template <>
struct std::formatter<CustomClass> {
	constexpr auto parse(format_parse_context &ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const CustomClass &custom_obj, FormatContext &ctx) {
		std::stringstream ss;
		ss << custom_obj;
		return format_to(ctx.out(), "{}", ss.str());
	}
};
//This provides a custom implementation of std::formatter for CustomClass using its operator<< function.

//Finally, you can use std::format with your CustomClass and LoggerStream:

int main() {
	LoggerManager loggerManager;
	// ... (Add destinations and set up loggerManager)

	LoggerStream infoStream(loggerManager, LogLevel::Info);

	CustomClass myObj("Object1", 42);
	infoStream << std::format("The state of my custom object: {}\n", myObj);

	// Give worker thread some time to finish processing
	std::this_thread::sleep_for(std::chrono::seconds(1));

	return 0;
}
// Now your CustomClass object can be formatted in a single line using std::format and sent to the LoggerStream.


void DumpClass(void* theClassPtr, UInt32 nIntsToDump)
{
	file << "DumpClass:";
	UInt32* basePtr = (UInt32*)theClassPtr;

	++file;

	if (!theClassPtr) return;
	for (UInt32 ix = 0; ix < nIntsToDump; ix++) {
		UInt32* curPtr = basePtr + ix;
		const char* curPtrName = nullptr;
		UInt32 otherPtr = 0;
		float otherFloat = 0.0;
		const char* otherPtrName = nullptr;
		if (curPtr) {
			curPtrName = GetObjectClassName(curPtr);

			otherPtr = *curPtr;
			otherFloat = *(float*)curPtr;

			if (otherPtr) {
				otherPtrName = GetObjectClassName((void*)otherPtr);
			}
		}

		file << FormatString("%3d +%03X ptr: 0x%08X: %32s *ptr: 0x%08x | %f: %32s", ix, ix * 4, curPtr, curPtrName, otherPtr, otherFloat, otherPtrName);
	}

	--file;
}


void Dump(Tile* tile)
{
	file << tile->name.m_data;

	++file;

	file << "values:";

	++file;

	for (UInt32 i = 0; i < tile->values.size; i++)
	{
		const Tile::Value* val = tile->values[i];
		const char* traitName = tile->TraitIDToName(val->id);
		char traitNameIDBuf[16];

		if (!traitName)
		{
			sprintf_s(traitNameIDBuf, "%08X", val->id);
			traitName = traitNameIDBuf;
		}

		if (val->str)			file << FormatString("%s: %s", traitName, val->str);
		else if (val->action)	file << FormatString("%s: action %08X", traitName, val->action);
		else					file << FormatString("%s: %f", traitName, val->num);
	}

	--file;

	for (const auto node : tile->children) Dump(node);

	--file;
}

void Dump(BaseExtraList* extra)
{
	file << "BaseExtraList Dump:";
	++file;

	if (extra->m_data)
	{
		for (auto traverse = extra->m_data; traverse; traverse = traverse->next) {
			file << GetObjectClassName(traverse);
			//_MESSAGE("Extra types %4x (%s) %s", traverse->type, GetExtraDataName(traverse->type), GetExtraDataValue(traverse));
		}
	}
	else
		file << "No data in list";

	--file;
}

void Dump(ExtraContainerChanges* extra)
{
	file << "Dumping ExtraContainerChanges";
	++file;

	if (extra->data && extra->data->objList)
	{
		for (const auto entry : *extra->data->objList)
		{
			file << FormatString("Type: %s CountDelta: %d [%08X]", entry->form->GetFullName()->name.CStr(), entry->countDelta, entry);
			++file;
			if (!entry || !entry->extendData)
				file << "* No extend data *";
			else for (const auto extendData : *entry->extendData)
			{
				file << FormatString("Extend Data: [%08X]", extendData);
				++file;
				if (extendData) {
					Dump(extendData);
					if (const auto xCount = (ExtraCount*)extendData->GetByType(kExtraData_Count))
						file << FormatString("ExtraCount value : %d", xCount->count);
				}
				else file << "NULL";
				--file;
			}
			--file;
		}
	}
	--file;
}

void DumpFontNames()
{
	FontInfo** fonts = FontManager::GetSingleton()->fontInfos;

	for (UInt32 i = 0; i < FontArraySize; i++)
		file << (FormatString("Font %d is named %s", i + 1, fonts[i]->filePath));
}


/*

void Log::Init(const std::filesystem::path& path, const std::string& modName)
{
file.Create(FileLoggerPrint, path);
console.Create(ConsoleLoggerUpdate, modName);
}

void Log::Copy(const std::filesystem::path& path) 
{
file.Copy(path);
}

Log& Log::operator>>(const std::filesystem::path& path)
{
Copy(path);
return *this;
}

std::vector<InternalFunction> queue;

Log& Log::operator()()
{
if (logLevel)
{
for (const auto& iter : queue) 
{
const auto str = iter(); 
if (logDest & kLog) file << str;
if (logDest & kConsole) console << str;
}
queue.clear();
if (logDest & kLog) file();
if (logDest & kConsole) console();
}
return *this;
}

Log& Log::operator<<(const Record& rec)
{
if (logLevel) queue.push_back(rec);
return *this;
}
*/

/*
Log& operator<<(Log& log, Record rec)
{
if (log.logLevel)
{
const auto& str = rec();
if (log.logDest & Log::kLog) file << str;
if (log.logDest & Log::kConsole) console << str;
}
return log;
}
*/

/*
auto make_B(auto&&... args) // since C++20
{
std::function<void(decltype(args))>

return B(std::forward<decltype(args)>(args)...);
}
*/
/*
extern NiTMapBase<const char*, int>* g_traitNameMap;

void Debug_DumpTraits()
{
for (const auto bucket : *g_traitNameMap)
Log() << FormatString("%s, %08X ,%d", bucket->key, bucket->data, bucket->data);
}
*/
/*void ScriptEventList::Dump()
{
const auto nEvents = m_eventList->Count();
for (UInt16 n = 0; n < nEvents; ++n)
if (const Event* pEvent = m_eventList->GetNthItem(n))
Log(FormatString("%08X (%s) %08X", pEvent->object, GetObjectClassName(pEvent->object), pEvent->eventMask));
}
*/

#endif