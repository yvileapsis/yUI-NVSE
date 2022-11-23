#include <Logging.h>

#include <fstream>
#include <vector>

#include <ConsoleManager.h>
#include <GameBSExtraData.h>
#include <GameExtraData.h>
#include <InterfaceManager.h>
#include <Tiles.h>
#include <Utilities.h>

typedef void(*LoggingFunction)(std::fstream& file, const std::string& str);
typedef LoggingFunction(*UpdateFunction)();

class DebugLog
{
	LoggingFunction		logger = nullptr;
	UpdateFunction		update = nullptr;

	std::fstream		file;
	std::string			modString;

	UInt32				indent = 0;
	std::vector<std::string> queuedMessages;

public:
	DebugLog() = default;
	~DebugLog() { if (file) file.close(); }

	void Create(LoggingFunction rhslogger, const std::filesystem::path& rhspath, const std::string& rhsmodString = "")
	{
		logger = (rhslogger);
		file = std::fstream(rhspath, std::fstream::out | std::fstream::trunc);
		modString = rhsmodString;

	};
	void Create(UpdateFunction rhsupdate, const std::string& rhsmodString = "")
	{
		update = rhsupdate;
		modString = rhsmodString;
	};

	void Message(const std::string& str, SInt32 deltaIndent = 0)
	{
		indent += deltaIndent;

		std::string indentstr;
		for (UInt32 i = 0; i < indent; i++) indentstr += '\t';

		queuedMessages.emplace_back(indentstr + str);

		if (!logger && update) logger = update();
		if (logger)
		{
			for (const auto& iter : queuedMessages) logger(file, (modString.empty() ? "" : modString + ":") + iter);
			queuedMessages.clear();
		}
	}

	void Indent() { indent++; }
	void Outdent() { indent--; }
};

inline DebugLog console;
inline DebugLog file;

void ConsoleLoggerPrint(std::fstream& file, const std::string& str) { ConsoleManager::GetSingleton()->Print(str.c_str(), nullptr); }
LoggingFunction ConsoleLoggerUpdate() { return ConsoleManager::GetSingleton() ? ConsoleLoggerPrint : nullptr; }

void DumpClass(void* theClassPtr, UInt32 nIntsToDump)
{
	file.Message("DumpClass:");
	UInt32* basePtr = (UInt32*)theClassPtr;

	file.Indent();

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
			otherFloat = *(float*)(curPtr);

			if (otherPtr) {
				otherPtrName = GetObjectClassName((void*)otherPtr);
			}
		}

		file.Message(FormatString("%3d +%03X ptr: 0x%08X: %32s *ptr: 0x%08x | %f: %32s", ix, ix * 4, curPtr, curPtrName, otherPtr, otherFloat, otherPtrName));
	}

	file.Outdent();
}

void Dump(Tile* tile)
{
	file.Message(tile->name.m_data);
	file.Indent();

	file.Message("values:");

	file.Indent();

	for (UInt32 i = 0; i < tile->values.size; i++)
	{
		const TileValue* val = tile->values[i];
		const char* traitName = tile->TraitIDToName(val->id);
		char traitNameIDBuf[16];

		if (!traitName)
		{
			sprintf_s(traitNameIDBuf, "%08X", val->id);
			traitName = traitNameIDBuf;
		}

		if (val->str)			file.Message(FormatString("%s: %s", traitName, val->str));
		else if (val->action)	file.Message(FormatString("%s: action %08X", traitName, val->action));
		else					file.Message(FormatString("%s: %f", traitName, val->num));
	}

	file.Outdent();

	for (const auto node : tile->children) Dump(node);

	file.Outdent();
}

void Dump(BaseExtraList* extra)
{
	file.Message("BaseExtraList Dump:");
	file.Indent();

	if (extra->m_data)
	{
		for (auto traverse = extra->m_data; traverse; traverse = traverse->next) {
			file.Message(GetObjectClassName(traverse));
			//_MESSAGE("Extra types %4x (%s) %s", traverse->type, GetExtraDataName(traverse->type), GetExtraDataValue(traverse));
		}
	}
	else
		file.Message("No data in list");

	file.Outdent();
}

void Dump(ExtraContainerChanges* extra)
{
	file.Message("Dumping ExtraContainerChanges");
	file.Indent();

	if (extra->data && extra->data->objList)
	{
		for (const auto entry : *extra->data->objList)
		{
			file.Message(FormatString("Type: %s CountDelta: %d [%08X]", entry->form->GetFullName()->name.CStr(), entry->countDelta, entry));
			file.Indent();
			if (!entry || !entry->extendData)
				file.Message("* No extend data *");
			else for (const auto extendData : *entry->extendData)
			{
				file.Message(FormatString("Extend Data: [%08X]", extendData));
				file.Indent();
				if (extendData) {
					Dump(extendData);
					if (const auto xCount = (ExtraCount*)extendData->GetByType(kExtraData_Count))
						file.Message("ExtraCount value : %d", xCount->count);
				}
				else file.Message("NULL");
				file.Outdent();
			}
			file.Outdent();
		}
	}
	file.Outdent();
}

void DumpFontNames(void)
{
	FontInfo** fonts = FontManager::GetSingleton()->fontInfos;

	for (UInt32 i = 0; i < FontArraySize; i++)
		file.Message(FormatString("Font %d is named %s", i + 1, fonts[i]->path));
}


void LogInit(const std::filesystem::path& path, const std::string& modName)
{
	file.Create(ConsoleLoggerPrint, path);
	console.Create(ConsoleLoggerUpdate, modName);
}

void Log(const std::string& msg, UInt32 loglevel)
{
	if (loglevel & kToLog) file.Message(msg);
	if (loglevel & kToConsole) console.Message(msg);
}