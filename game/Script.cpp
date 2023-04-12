#include <Script.h>

#include <TESForm.h>
#include <TESObjectREFR.h>
#include <RTTI.h>
#include <CommandTable.h>

UInt32 GetDeclaredVariableType(const char* varName, const char* scriptText)
{
	Tokenizer scriptLines(scriptText, "\n\r");
	std::string curLine;
	while (scriptLines.NextToken(curLine) != -1)
	{
		Tokenizer tokens(curLine.c_str(), " \t\n\r;");
		std::string curToken;

		if (tokens.NextToken(curToken) != -1)
		{
			UInt32 varType = -1;

			// variable declaration?
			if (!StrCompare(curToken.c_str(), "string_var"))
				varType = Script::eVarType_String;
			else if (!StrCompare(curToken.c_str(), "array_var"))
				varType = Script::eVarType_Array;
			else if (!StrCompare(curToken.c_str(), "float"))
				varType = Script::eVarType_Float;
			else if (!StrCompare(curToken.c_str(), "long") || !StrCompare(curToken.c_str(), "int") || !StrCompare(curToken.c_str(), "short"))
				varType = Script::eVarType_Integer;
			else if (!StrCompare(curToken.c_str(), "ref") || !StrCompare(curToken.c_str(), "reference"))
				varType = Script::eVarType_Ref;

			if (varType != -1 && tokens.NextToken(curToken) != -1 && !StrCompare(curToken.c_str(), varName))
			{
				return varType;
			}
		}
	}

	return Script::eVarType_Invalid;
}

Script* GetScriptFromForm(TESForm* form)
{
	if (const auto refr =  DYNAMIC_CAST(form, TESForm, TESObjectREFR))
		form = refr->baseForm;
	const auto scriptable = DYNAMIC_CAST(form, TESForm, TESScriptableForm);
	return scriptable ? scriptable->script : nullptr;
}

enum class ScriptOperatorCode
{
	kOp_LeftBracket = 0x0,
	kOp_RightBracket = 0x1,
	kOp_LogicalAnd = 0x2,
	kOp_LogicalOr = 0x3,
	kOp_LessThanOrEqual = 0x4,
	kOp_LessThan = 0x5,
	kOp_GreaterThanOrEqual = 0x6,
	kOp_GreaterThan = 0x7,
	kOp_Equals = 0x8,
	kOp_NotEquals = 0x9,
	kOp_Minus = 0xA,
	kOp_Plus = 0xB,
	kOp_Multiply = 0xC,
	kOp_Divide = 0xD,
	kOp_Modulo = 0xE,
	kOp_Tilde = 0xF,
	kOp_MAX = 0x10,
};

struct ScriptOperator
{
	ScriptOperatorCode code;
	UInt8 precedence;
	char operatorString[3];
};
//std::span<ActorValueInfo*> g_actorValues = { reinterpret_cast<ActorValueInfo**>(0x11D61C8), kAVCode_Max };

std::span g_eventBlockCommandInfos = { reinterpret_cast<CommandInfo*>(0x118E2F0), 38 };
std::span g_scriptStatementCommandInfos = { reinterpret_cast<CommandInfo*>(0x118CB50), 16 };
std::span g_gameScriptOperators = { reinterpret_cast<ScriptOperator*>(0x118CAD0), 16 };

CommandInfo* GetEventCommandInfo(UInt16 opcode)
{
	if (opcode > 37) return nullptr;
	return &g_eventBlockCommandInfos[opcode];
}

UInt32 Script::GetVariableType(VariableInfo* varInfo)
{
	if (text) return GetDeclaredVariableType(varInfo->name.m_data, text);
	// if it's a ref var a matching varIdx will appear in RefList
	for (RefListEntry* refEntry = &refList; refEntry; refEntry = refEntry->next)
		if (refEntry->var->varIdx == varInfo->idx) return eVarType_Ref;
	return varInfo->type;
}

bool Script::IsUserDefinedFunction() const
{
	auto* scriptData = static_cast<UInt8*>(data);
	return *(scriptData + 8) == 0x0D;
}

UInt32 Script::GetVarCount() const
{
	// info->varCount include index
	auto count = 0U;
	auto* node = &this->varList;
	while (node)
	{
		if (node->data)
			++count;
		node = node->Next();
	}
	return count;
}

UInt32 Script::GetRefCount() const
{
	auto count = 0U;
	auto* node = &this->refList;
	while (node)
	{
		if (node->var)
			++count;
		node = node->Next();
	}
	return count;
}

TList<VariableInfo>* Script::GetVars()
{
	return reinterpret_cast<TList<VariableInfo>*>(&this->varList);
}

TList<Script::RefVariable>* Script::GetRefList()
{
	return reinterpret_cast<TList<RefVariable>*>(&this->refList);
}

#if RUNTIME

Script* Script::CreateScript()
{
	auto* buf = GameHeapAlloc(sizeof Script);
	return ThisStdCall<Script*>(0x5AA0F0, buf);
}

void Script::DeleteScript() const
{
	ThisStdCall(0x5AA170, this, false);
}

ScriptEventList* Script::CreateEventList()
{
	ScriptEventList* result = nullptr;
//	EnterCriticalSection(&csGameScript);
	try
	{
		result = ThisCall<ScriptEventList*>(0x5ABF60, this);	// 4th sub above Script::Execute (was 1st above in Oblivion) Execute is the second to last call in Run
	} catch(...) {}

//	LeaveCriticalSection(&csGameScript);
	return result;
}

#endif

UInt32 ScriptBuffer::GetRefIdx(Script::RefVariable* ref)
{
	UInt32 idx = 0;
	for (Script::RefListEntry* curEntry = &refVars; curEntry && curEntry->var; curEntry = curEntry->next)
	{
		idx++;
		if (ref == curEntry->var) break;
	}
	return idx;
}

UInt32 ScriptBuffer::GetVariableType(VariableInfo* varInfo, Script::RefVariable* refVar)
{
	const char* scrText = scriptText;
	if (refVar)
	{
		if (refVar->form)
		{
			TESScriptableForm* scriptable = NULL;
			switch (refVar->form->typeID)
			{
			case kFormType_TESObjectREFR:
				{
					TESObjectREFR* refr = DYNAMIC_CAST(refVar->form, TESForm, TESObjectREFR);
					scriptable = DYNAMIC_CAST(refr->baseForm, TESForm, TESScriptableForm);
					break;
				}
			case kFormType_TESQuest:
				scriptable = DYNAMIC_CAST(refVar->form, TESForm, TESScriptableForm);
			}

			if (scriptable && scriptable->script)
			{
				if (scriptable->script->text)
					scrText = scriptable->script->text;
				else
					return scriptable->script->GetVariableType(varInfo);
			}
		}
		else			// this is a ref variable, not a literal form - can't look up script vars
			return Script::eVarType_Invalid;
	}

	return GetDeclaredVariableType(varInfo->name.m_data, scrText);
}

/******************************
 Script
******************************/

class ScriptVarFinder
{
public:
	const char* m_varName;
	ScriptVarFinder(const char* varName) : m_varName(varName)
		{	}
	bool Accept(VariableInfo* varInfo)
	{
		//_MESSAGE("  cur var: %s to match: %s", varInfo->name.m_data, m_varName);
		if (!StrCompare(m_varName, varInfo->name.m_data))
			return true;
		return false;
	}
};

VariableInfo* Script::GetVariableByName(const char* varName)
{
	VarListVisitor visitor(&varList);
	const VarInfoEntry* varEntry = visitor.Find(ScriptVarFinder(varName));
	if (varEntry) return varEntry->data;
	return nullptr;
}

Script::RefVariable	* Script::GetRefFromRefList(UInt32 refIdx)
{
	UInt32	idx = 1;	// yes, really starts at 1
	if (refIdx)	for(RefListEntry * entry = &refList; entry; entry = entry->next)
	{
		if(idx == refIdx) return entry->var;
		idx++;
	}
	return nullptr;
}

VariableInfo* Script::GetVariableInfo(UInt32 idx)
{
	for (Script::VarInfoEntry* entry = &varList; entry; entry = entry->next)
		if (entry->data && entry->data->idx == idx)
			return entry->data;

	return NULL;
}

UInt32 Script::AddVariable(TESForm * form)
{
	UInt32		resultIdx = 1;

	RefVariable	* var = (RefVariable*)GameHeapAlloc(sizeof(RefVariable));

	var->name.Set("");
	var->form = form;
	var->varIdx = 0;

	if(!refList.var)
	{
		// adding the first object
		refList.var = var;
		refList.next = NULL;
	}
	else
	{
		resultIdx++;

		// find the last RefListEntry
		RefListEntry	* entry;
		for(entry = &refList; entry->next; entry = entry->next, resultIdx++) ;

		RefListEntry	* newEntry = (RefListEntry *)GameHeapAlloc(sizeof(RefListEntry));

		newEntry->var = var;
		newEntry->next = NULL;

		entry->next = newEntry;
	}

	info.numRefs = resultIdx + 1;

	return resultIdx;
}

void Script::CleanupVariables()
{
	delete refList.var;

	RefListEntry	* entry = refList.next;
	while(entry)
	{
		RefListEntry	* next = entry->next;

		delete entry->var;
		delete entry;

		entry = next;
	}
}

VariableInfo* Script::VarInfoEntry::GetVariableByName(const char* varName)
{
	for (Script::VarInfoEntry* entry = this; entry; entry = entry->next)
	{
		if (entry->data && !StrCompare(entry->data->name.m_data, varName))
			return entry->data;
	}

	return NULL;
}

UInt32 Script::RefListEntry::GetIndex(Script::RefVariable* refVar)
{
	UInt32 idx = 0;
	for (RefListEntry* cur = this; cur; cur = cur->next)
	{
		idx++;
		if (cur->var == refVar)
			return idx;
	}

	return 0;
}

/***********************************
 ScriptLineBuffer
***********************************/

//const char	* ScriptLineBuffer::kDelims_Whitespace = " \t\n\r";
//const char  * ScriptLineBuffer::kDelims_WhitespaceAndBrackets = " \t\n\r[]";

bool ScriptLineBuffer::Write(const void* buf, UInt32 bufsize)
{
	if (dataOffset + bufsize >= kBufferSize)
		return false;

	memcpy(dataBuf + dataOffset, buf, bufsize);
	dataOffset += bufsize;
	return true;
}

bool ScriptLineBuffer::Write32(UInt32 buf)
{
	return Write(&buf, sizeof(UInt32));
}

bool ScriptLineBuffer::WriteString(const char* buf)
{
	UInt32 len = StrLen(buf);
	if ((len < 0x10000) && Write16(len) && len)
		return Write(buf, len);

	return false;
}

bool ScriptLineBuffer::Write16(UInt16 buf)
{
	return Write(&buf, sizeof(UInt16));
}

bool ScriptLineBuffer::WriteByte(UInt8 buf)
{
	return Write(&buf, sizeof(UInt8));
}

ScriptBuffer::ScriptBuffer()
{
	ThisStdCall(0x5AE490, this);
}

ScriptBuffer::~ScriptBuffer()
{
	ThisStdCall(0x5AE5C0, this);
}

bool ScriptLineBuffer::WriteFloat(double buf)
{
	return Write(&buf, sizeof(double));
}


void ScriptEventList::Destructor()
{
	if (m_eventList)
		m_eventList->RemoveAll();
	while (m_vars) {
		if (m_vars->var) GameHeapFree(m_vars->var);
		VarEntry* next = m_vars->next;
		GameHeapFree(m_vars);
		m_vars = next;
	}
}

TList<ScriptEventList::Var>* ScriptEventList::GetVars() const
{
	return reinterpret_cast<TList<Var>*>(m_vars);
}

void ScriptEventList::EventCreate(UInt32 eventCode, TESObjectREFR* container)
{
	// create Event struct
	const auto pEvent = (ScriptEventList::Event*)GameHeapAlloc(sizeof(ScriptEventList::Event));
	if (pEvent) {
		pEvent->eventMask = eventCode;
		pEvent->object = container;
	}

	if (!m_eventList) {
		m_eventList = (ScriptEventList::EventList*)GameHeapAlloc(sizeof(ScriptEventList::EventList));
		m_eventList->Init();
	}
	if (m_eventList && pEvent)
		m_eventList->AddAt(pEvent, 0);
}

UInt32 ScriptEventList::ResetAllVariables()
{
	UInt32 numVars = 0;
	for (VarEntry* entry = m_vars; entry; entry = entry->next)
		if (entry->var)
		{
			entry->var->data = 0.0;
			numVars++;
		}
	return numVars;
}

ScriptEventList::Var* ScriptEventList::GetVariable(UInt32 id)
{
	for (VarEntry* entry = m_vars; entry; entry = entry->next)
		if (entry->var && entry->var->id == id) return entry->var;
	return nullptr;
}

ScriptEventList* EventListFromForm(TESForm* form)
{
	if (const auto refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR))
		return refr->GetEventList();
	if (const auto quest = DYNAMIC_CAST(form, TESForm, TESQuest))
		return quest->scriptEventList;
	return nullptr;
}

UInt32 ScriptEventList::Var::GetFormId()
{
	return *(UInt32*)(&data);
}

void ShowCompilerError(ScriptLineBuffer* lineBuf, const char* fmt, ...)
{

	char errorHeader[0x400];
	UInt32 offset = sprintf_s(errorHeader, 0x400, "Error on line %d\n\n", lineBuf->lineNumber);

	va_list	args;
	va_start(args, fmt);

	char	errorMsg[0x200];
	vsprintf_s(errorMsg, 0x200, fmt, args);

	strcat_s(errorHeader, 0x400, errorMsg);
	Log() << errorHeader;

	va_end(args);
}

UInt8* GetScriptDataPosition(Script* script, void* scriptDataIn, const UInt32* opcodeOffsetPtrIn)
{
	if (scriptDataIn != script->data) // set ... to or if ..., script data is stored on stack and not heap
	{
		auto* scriptData = *(static_cast<UInt8**>(scriptDataIn) + 0x1D5);
		return scriptData + *opcodeOffsetPtrIn + 1;
	}
	return static_cast<UInt8*>(scriptDataIn) + *opcodeOffsetPtrIn;
}


const _MarkBaseExtraListScriptEvent MarkBaseExtraListScriptEvent = reinterpret_cast<_MarkBaseExtraListScriptEvent>(0x005AC750);
const _DoCheckScriptRunnerAndRun DoCheckScriptRunnerAndRun = reinterpret_cast<_DoCheckScriptRunnerAndRun>(0x005AC190);
