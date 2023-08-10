//https://codereview.stackexchange.com/questions/238234/a-simple-multithreaded-filelogger-in-c
//https://codereview.stackexchange.com/questions/256137/simple-multithreaded-filelogger-in-c

#include <Logging.h>

#include <fstream>
#include <vector>
#include <format>

#include <ConsoleManager.h>
#include <BSExtraData.h>
#include <InventoryChanges.h>
#include <InterfaceManager.h>
#include <Tile.h>
#include <Utilities.h>

#include "TESForm.h"

typedef void(*LoggingFunction)(std::fstream& file, const std::string& str);
typedef LoggingFunction(*UpdateFunction)();

class DebugLog
{
	LoggingFunction		logger = nullptr;
	UpdateFunction		update = nullptr;

	std::fstream		file;
	std::filesystem::path path;

	std::string			modString;

	UInt32				indent = 0;
	std::vector<std::string> queuedMessages;

public:
	DebugLog() = default;
	~DebugLog() { if (file) file.close(); }

	void Create(LoggingFunction rhslogger, const std::filesystem::path& rhspath, const std::string& rhsmodString = "")
	{
		logger = rhslogger;
		if (!exists(rhspath.parent_path())) std::filesystem::create_directory(rhspath.parent_path());
		file = std::fstream(rhspath, std::fstream::out | std::fstream::trunc);
		path = rhspath;
		modString = rhsmodString;
	}

	void Create(UpdateFunction rhsupdate, const std::string& rhsmodString = "")
	{
		update = rhsupdate;
		modString = rhsmodString;
	}

	void Queue(const std::string& str, SInt32 deltaIndent = 0)
	{
		indent += deltaIndent;

		std::string indentstr;
		for (UInt32 i = 0; i < indent; i++) indentstr += '\t';

		queuedMessages.emplace_back(indentstr + str);
	}

	void Flush()
	{
		if (!logger && update) logger = update();
		if (logger)
		{
			for (const auto& iter : queuedMessages) logger(file, (modString.empty() ? "" : modString + ": ") + iter);
			queuedMessages.clear();
		}
	}

	DebugLog& operator()()
	{
		Flush();
		return *this;
	}

	DebugLog& operator<<(const std::string& str)
	{
		Queue(str);
		Flush();
		return *this;
	}

	DebugLog& operator++() { indent++; return *this; }
	DebugLog& operator--() { indent--; return *this; }

	void Copy(const std::filesystem::path& path);
};

inline DebugLog console;
inline DebugLog file;

void ConsoleLoggerPrint(std::fstream& file, const std::string& str) { ConsoleManager::GetSingleton()->Print(str.c_str(), nullptr); }
LoggingFunction ConsoleLoggerUpdate() { return ConsoleManager::GetSingleton() ? ConsoleLoggerPrint : nullptr; }

void FileLoggerPrint(std::fstream& file, const std::string& str)
{
	file << str << "\n";
	file.flush();
}

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


void Log::Init(const std::filesystem::path& path, const std::string& modName)
{
	file.Create(FileLoggerPrint, path);
	console.Create(ConsoleLoggerUpdate, modName);
}

void Log::Copy(const std::filesystem::path& path) 
{
	file.Copy(path);
}

Log& Log::operator<<(const std::string& str)
{
	if (logLevel)
	{
		if (logDest & kLog) file << str;
		if (logDest & kConsole) console << str;
	}
	return *this;
}

Log& Log::operator>>(const std::filesystem::path& path)
{
	Copy(path);
	return *this;
}

Log& Log::operator()()
{
	if (logLevel)
	{
		if (logDest & kLog) file();
		if (logDest & kConsole) console();
	}
	return *this;
}

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

void DebugLog::Copy(const std::filesystem::path& logFolder)
{
	const auto lastmod = std::format(".{0:%F}-{0:%H}-{0:%M}-{0:%S}", floor<std::chrono::seconds>(std::chrono::time_point(std::chrono::system_clock::now())));
	std::filesystem::path newPath = path.parent_path().string() + "\\";
	newPath += logFolder.string() + "\\";
	if (!exists(newPath)) std::filesystem::create_directory(newPath);
	newPath += path.stem();
	newPath += lastmod;
	newPath += path.extension();

	try {
		std::filesystem::copy_file(path, newPath);
	}
	catch (std::filesystem::filesystem_error& e) {
		Log() << "Could not copy sandbox/abc: " << e.what() << "\n";
	}
}