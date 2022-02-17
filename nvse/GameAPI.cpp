#include "GameAPI.h"
#include "GameRTTI.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameTypes.h"
#include "CommandTable.h"
#include "GameScript.h"
#include "StringVar.h"
#include "printf.h"

static NVSEStringVarInterface* s_StringVarInterface = NULL;
bool alternateUpdate3D = false;

// arg1 = 1, ignored if canCreateNew is false, passed to 'init' function if a new object is created
typedef void * (* _GetSingleton)(bool canCreateNew);

char s_tempStrArgBuffer[0x4000];

#if RUNTIME
TESForm* __stdcall LookupFormByID(UInt32 refID)
{
	NiTPointerMap<TESForm> *formsMap = *(NiTPointerMap<TESForm>**)0x11C54C0;
	return formsMap->Lookup(refID);
}

const _ExtractArgs ExtractArgs = (_ExtractArgs)0x005ACCB0;

const _FormHeap_Allocate FormHeap_Allocate = (_FormHeap_Allocate)0x00401000;
const _FormHeap_Free FormHeap_Free = (_FormHeap_Free)0x00401030;

const _CreateFormInstance CreateFormInstance = (_CreateFormInstance)0x00465110;

const _GetSingleton ConsoleManager_GetSingleton = (_GetSingleton)0x0071B160;
bool * bEchoConsole = (bool*)0x011F158C;

const _QueueUIMessage QueueUIMessage = (_QueueUIMessage)0x007052F0;	// Called from Cmd_AddSpell_Execute

const _ShowMessageBox ShowMessageBox = (_ShowMessageBox)0x00703E80;
const _ShowMessageBox_Callback ShowMessageBox_Callback = (_ShowMessageBox_Callback)0x005B4A70;
const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID = (_ShowMessageBox_pScriptRefID)0x011CAC64;
const _ShowMessageBox_button ShowMessageBox_button = (_ShowMessageBox_button)0x0118C684;

const _GetActorValueName GetActorValueName = (_GetActorValueName)0x00066EAC0;	// See Cmd_GetActorValue_Eval
const UInt32 * g_TlsIndexPtr = (UInt32 *)0x0126FD98;
const _MarkBaseExtraListScriptEvent MarkBaseExtraListScriptEvent = (_MarkBaseExtraListScriptEvent)0x005AC750;
const _DoCheckScriptRunnerAndRun DoCheckScriptRunnerAndRun = (_DoCheckScriptRunnerAndRun)0x005AC190;

SaveGameManager ** g_saveGameManager = (SaveGameManager**)0x011DE134;

// Johnny Guitar supports this
const _GetFormByID GetFormByID = (_GetFormByID)(0x483A00);


#elif EDITOR

//	FormMap* g_FormMap = (FormMap *)0x009EE18C;		// currently unused
//	DataHandler ** g_dataHandler = (DataHandler **)0x00A0E064;
//	TES** g_TES = (TES**)0x00A0ABB0;
	const _LookupFormByID LookupFormByID = (_LookupFormByID)0x004F9620;	// Call between third reference to RTTI_TESWorldspace and RuntimeDynamicCast
	const _GetFormByID GetFormByID = (_GetFormByID)(0x004F9650); // Search for aNonPersistentR and aPlayer (third call below aPlayer, second is LookupFomrByID)
	const _FormHeap_Allocate FormHeap_Allocate = (_FormHeap_Allocate)0x00401000;
	const _FormHeap_Free FormHeap_Free = (_FormHeap_Free)0x0000401180;
	const _ShowCompilerError ShowCompilerError = (_ShowCompilerError)0x005C5730;	// Called with aNonPersistentR (still same sub as the other one)

// 0x5C64C0 <- start of huge editor function that IDA can't disassemble.

#endif

struct TLSData
{
	// thread local storage

	UInt32	pad000[(0x260 - 0x000) >> 2];	// 000
	NiNode			* lastNiNode;			// 260	248 in FOSE
	TESObjectREFR	* lastNiNodeREFR;		// 264	24C in FOSE
	UInt8			consoleMode;			// 268
	UInt8			pad269[3];				// 269
	// 25C is used as do not head track the player , 
	// 2B8 is used to init QueudFile::unk0018, 
	// 28C might count the recursive calls to Activate, limited to 5.
};


static TLSData * GetTLSData()
{
	UInt32 TlsIndex = *g_TlsIndexPtr;
	TLSData * data = NULL;

	__asm {
		mov		ecx,	[TlsIndex]
		mov		edx,	fs:[2Ch]	// linear address of thread local storage array
		mov		eax,	[edx+ecx*4]
		mov		[data], eax
	}

	return data;
}

__declspec(naked) bool IsConsoleMode()
{
	__asm
	{
		mov		al, byte ptr ds:[0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, dword ptr ds:[0x126FD98]
		mov		edx, fs:[0x2C]
		mov		eax, [edx+eax*4]
		test	eax, eax
		jz		done
		mov		al, [eax+0x268]
	done:
		retn
	}
}

bool GetConsoleEcho()
{
	return *bEchoConsole != 0;
}

void SetConsoleEcho(bool doEcho)
{
	*bEchoConsole = doEcho ? 1 : 0;
}

const char * GetFullName(TESForm * baseForm)
{
	if(baseForm)
	{
		TESFullName* fullName = baseForm->GetFullName();
		if(fullName && fullName->name.m_data)
		{
			if (fullName->name.m_dataLen)
				return fullName->name.m_data;
		}
	}

	return "<no name>";
}

ConsoleManager * ConsoleManager::GetSingleton(void)
{
	return (ConsoleManager *)ConsoleManager_GetSingleton(true);
}

char * ConsoleManager::GetConsoleOutputFilename(void)
{
	return GetSingleton()->scofPath;
};

bool ConsoleManager::HasConsoleOutputFilename(void) {
	return 0 != GetSingleton()->scofPath[0];
}

bool s_InsideOnActorEquipHook = false;
UInt32 s_CheckInsideOnActorEquipHook = 1;

void PrintConsole(const char * fmt, ...)
{
	if (ConsoleManager * mgr = ConsoleManager::GetSingleton())
	{
		va_list	args;

		va_start(args, fmt);

		CALL_MEMBER_FN(mgr, Print)(fmt, args);

		va_end(args);
	}
}

TESSaveLoadGame * TESSaveLoadGame::Get()
{
	return (TESSaveLoadGame *)0x011DE45C;
}

SaveGameManager* SaveGameManager::GetSingleton()
{
	return *g_saveGameManager;
}

std::string GetSavegamePath()
{
	char path[0x104];
	CALL_MEMBER_FN(SaveGameManager::GetSingleton(), ConstructSavegamePath)(path);
	return path;
}

void ScriptEventList::Destructor()
{
// OBLIVION	ThisStdCall(0x004FB4E0, this);
	if (m_eventList)
		m_eventList->RemoveAll();
	while (m_vars) {
		if (m_vars->var) {
			FormHeap_Free(m_vars->var);
		}
		VarEntry* next = m_vars->next;
		FormHeap_Free(m_vars);
		m_vars = next;
	}
}

tList<ScriptEventList::Var>* ScriptEventList::GetVars() const
{
	return reinterpret_cast<tList<Var>*>(m_vars);
}

static const char* StringFromStringVar(UInt32 strID)
{

	if (s_StringVarInterface)
		return s_StringVarInterface->GetString(strID);
	else
		return "";
}

void ScriptEventList::Dump(void)
{
	UInt32 nEvents = m_eventList->Count();

	for(SInt32 n = 0; n < nEvents; ++n)
	{
		Event* pEvent = m_eventList->GetNthItem(n);
		if(pEvent)
		{
			PrintConsole("%08X (%s) %08X", pEvent->object, GetObjectClassName(pEvent->object), pEvent->eventMask);
		}
	}
}

UInt32 ScriptEventList::ResetAllVariables()
{
	UInt32 numVars = 0;
	for (VarEntry * entry = m_vars; entry; entry = entry->next)
		if (entry->var)
		{
			entry->var->data = 0.0;
			numVars++;
		}
	return numVars;
}

ScriptEventList::Var * ScriptEventList:: GetVariable(UInt32 id)
{
	for(VarEntry * entry = m_vars; entry; entry = entry->next)
		if(entry->var && entry->var->id == id)
			return entry->var;

	return NULL;
}

ScriptEventList* EventListFromForm(TESForm* form)
{
	ScriptEventList* eventList = NULL;
	TESObjectREFR* refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR);
	if (refr)
		eventList = refr->GetEventList();
	else
	{
		TESQuest* quest = DYNAMIC_CAST(form, TESForm, TESQuest);
		if (quest)
			eventList = quest->scriptEventList;
	}

	return eventList;
}

char* ConvertLiteralPercents(char *srcPtr)
{
	char *endPtr = srcPtr + StrLen(srcPtr);
	while (srcPtr = strchr(srcPtr, '%'))
	{
		srcPtr++;
		memmove(srcPtr + 1, srcPtr, endPtr - srcPtr);
		*srcPtr++ = '%';
		endPtr++;
	}
	*endPtr = 0;
	return endPtr;
}

UInt32 ScriptEventList::Var::GetFormId()
{
	return *reinterpret_cast<UInt32*>(&data);
}

// g_baseActorValueNames is only filled in after oblivion's global initializers run
const char* GetActorValueString(UInt32 actorValue)
{
	const char* name = 0;
	if (actorValue <= eActorVal_FalloutMax)
		name = GetActorValueName(actorValue);
	if (!name)
		name = "unknown";

	return name;
}

UInt32 GetActorValueForScript(const char* avStr) 
{
	for (UInt32 i = 0; i <= eActorVal_FalloutMax; i++) {
		char* name = GetActorValueName(i);
		if (_stricmp(avStr, name) == 0)
			return i;
	}

	return eActorVal_NoActorValue;
}

UInt32 GetActorValueForString(const char* strActorVal, bool bForScript)
{
	if (bForScript)
		return GetActorValueForScript(strActorVal);

	for (UInt32 n = 0; n <= eActorVal_FalloutMax; n++) {
		char* name = GetActorValueName(n);
		if (_stricmp(strActorVal, name) == 0)
			return n;
	}
	return eActorVal_NoActorValue;
}

//Log error if expression evaluates to false
bool SCRIPT_ASSERT(bool expr, Script* script, const char * errorMsg, ...)
{
	//	static bool bAlerted = false;			//only alert user on first error
	//	static std::set<UInt32> naughtyScripts;	//one error per script to avoid thrashing
	//
	//	if (!expr && naughtyScripts.find(script->refID) == naughtyScripts.end())
	//	{
	//		const ModEntry ** activeMods = (*g_dataHandler)->GetActiveModList();
	//		UInt8 modIndex = script->GetModIndex();
	//		const ModEntry * modEntry = activeMods[modIndex];
	//
	//		const char * modName;
	//		if (modIndex != 0xFF && modEntry && modEntry->data && modEntry->data->name)
	//			modName = modEntry->data->name;
	//		else
	//			modName = "Unknown";
	//
	////		sprintf_s(errorHeader, sizeof(errorHeader) - 1, "** Error: Script %08X in file \"%s\" **", script->refID, modName);
	////		_MESSAGE("%s", errorHeader);
	//		_MESSAGE("** Script Error: Script %08x in file \"%s\" **", script->refID, modName);
	//
	//		va_list args;
	//		va_start(args, errorMsg);
	//
	//		char errorBuf[512];
	//		vsprintf_s(errorBuf, sizeof(errorBuf) - 1, errorMsg, args);
	//		va_end(args);
	//
	//		gLog.Indent();
	//		_MESSAGE("%s", errorBuf);
	//		gLog.Outdent();
	//
	//		if (!bAlerted)
	//		{
	//			MessageBoxAlert("NVSE has detected a script error. \n\nPlease check nvse.log for details.");
	//			bAlerted = true;
	//		}
	//
	//		naughtyScripts.insert(script->refID);
	//	}
	return expr;
}

void ShowCompilerError(ScriptLineBuffer* lineBuf, const char * fmt, ...)
{

	char errorHeader[0x400];
	UInt32 offset = sprintf_s(errorHeader, 0x400, "Error on line %d\n\n", lineBuf->lineNumber);

	va_list	args;
	va_start(args, fmt);

	char	errorMsg[0x200];
	vsprintf_s(errorMsg, 0x200, fmt, args);

	strcat_s(errorHeader, 0x400, errorMsg);
	PrintConsole(errorHeader);
	PrintLog(errorHeader);

	va_end(args);
}

UInt32 GetActorValueMax(UInt32 actorValueCode) {
	switch (actorValueCode ) {
		case eActorVal_Aggression:			return   3; break;
		case eActorVal_Confidence:			return   4; break;
		case eActorVal_Energy:				return 100; break;
		case eActorVal_Responsibility:		return 100; break;
		case eActorVal_Mood:				return   8; break;

		case eActorVal_Strength:			return  10; break;
		case eActorVal_Perception:			return  10; break;
		case eActorVal_Endurance:			return  10; break;
		case eActorVal_Charisma:			return  10; break;
		case eActorVal_Intelligence:		return  10; break;
		case eActorVal_Agility:				return  10; break;
		case eActorVal_Luck:				return  10; break;

		case eActorVal_ActionPoints:		return   1; break;
		case eActorVal_CarryWeight:			return   1; break;
		case eActorVal_CritChance:			return 100; break;
		case eActorVal_HealRate:			return   1; break;
		case eActorVal_Health:				return   1; break;
		case eActorVal_MeleeDamage:			return   1; break;
		case eActorVal_DamageResistance:	return   1; break;
		case eActorVal_PoisonResistance:	return   1; break;
		case eActorVal_RadResistance:		return   1; break;
		case eActorVal_SpeedMultiplier:		return   1; break;
		case eActorVal_Fatigue:				return   1; break;
		case eActorVal_Karma:				return   1; break;
		case eActorVal_XP:					return   1; break;

		case eActorVal_Head:				return 100; break;
		case eActorVal_Torso:				return 100; break;
		case eActorVal_LeftArm:				return 100; break;
		case eActorVal_RightArm:			return 100; break;
		case eActorVal_LeftLeg:				return 100; break;
		case eActorVal_RightLeg:			return 100; break;
		case eActorVal_Brain:				return 100; break;

		case eActorVal_Barter:				return 100; break;
		case eActorVal_BigGuns:				return 100; break;
		case eActorVal_EnergyWeapons:		return 100; break;
		case eActorVal_Explosives:			return 100; break;
		case eActorVal_Lockpick:			return 100; break;
		case eActorVal_Medicine:			return 100; break;
		case eActorVal_MeleeWeapons:		return 100; break;
		case eActorVal_Repair:				return 100; break;
		case eActorVal_Science:				return 100; break;
		case eActorVal_Guns:				return 100; break;
		case eActorVal_Sneak:				return 100; break;
		case eActorVal_Speech:				return 100; break;
		case eActorVal_Survival:			return 100; break;
		case eActorVal_Unarmed:				return 100; break;

		case eActorVal_InventoryWeight:		return   1; break;
		case eActorVal_Paralysis:			return   1; break;
		case eActorVal_Invisibility:		return   1; break;
		case eActorVal_Chameleon:			return   1; break;
		case eActorVal_NightEye:			return   1; break;
		case eActorVal_Turbo:				return   1; break;
		case eActorVal_FireResistance:		return   1; break;
		case eActorVal_WaterBreathing:		return   1; break;
		case eActorVal_RadLevel:			return   1; break;
		case eActorVal_BloodyMess:			return   1; break;
		case eActorVal_UnarmedDamage:		return   1; break;
		case eActorVal_Assistance:			return   2; break;

		case eActorVal_ElectricResistance:	return   1; break;

		case eActorVal_EnergyResistance:	return   1; break;
		case eActorVal_EMPResistance:		return   1; break;
		case eActorVal_Var1Medical:			return   1; break;
		case eActorVal_Var2:				return   1; break;
		case eActorVal_Var3:				return   1; break;
		case eActorVal_Var4:				return   1; break;
		case eActorVal_Var5:				return   1; break;
		case eActorVal_Var6:				return   1; break;
		case eActorVal_Var7:				return   1; break;
		case eActorVal_Var8:				return   1; break;
		case eActorVal_Var9:				return   1; break;
		case eActorVal_Var10:				return   1; break;

		case eActorVal_IgnoreCrippledLimbs:	return   1; break;
		case eActorVal_Dehydration:			return   1; break;
		case eActorVal_Hunger:				return   1; break;
		case eActorVal_Sleepdeprevation:	return   1; break;
		case eActorVal_Damagethreshold:		return   1; break;
		default: return 1;
	}
}

typedef FontManager* (* _FontManager_GetSingleton)(void);
const _FontManager_GetSingleton FontManager_GetSingleton = (_FontManager_GetSingleton)0x011F33F8;

UInt8* GetScriptDataPosition(Script* script, void* scriptDataIn, const UInt32* opcodeOffsetPtrIn)
{
	if (scriptDataIn != script->data) // set ... to or if ..., script data is stored on stack and not heap
	{
		auto* scriptData = *(static_cast<UInt8**>(scriptDataIn) + 0x1D5);
		return scriptData + *opcodeOffsetPtrIn + 1;
	}
	return static_cast<UInt8*>(scriptDataIn) + *opcodeOffsetPtrIn;
}

TimeGlobal* TimeGlobal::GetSingleton() { return reinterpret_cast<TimeGlobal*>(0x11F6394); }

void TimeGlobal::Set(const Float32 value, const char isImmediateChange)
{
	ThisCall<void>(0xAA4DB0, this, value, isImmediateChange);
}

Float32 TimeGlobal::Get() { return *reinterpret_cast<Float32*>(0x11AC3A0); }
Float32 TimeGlobal::GetTarget() { return *reinterpret_cast<Float32*>(0x11AC3A4); }

NiTPointerMap<TESForm>* GetAllForms() { return *reinterpret_cast<NiTPointerMap<TESForm>**>(0x11C54C0); }
/*
void ConsoleManager::AppendToSentHistory(const char* str)
{
	// create text node
	auto* textNode = static_cast<TextNode*>(GameHeapAlloc(sizeof(TextNode)));
	textNode->next = 0;
	textNode->prev = 0;

	textNode->text.m_data = NULL;
	textNode->text.Init(0);
	textNode->text.Set(str);

	// append it to the input history
	this->inputHistory.Append(textNode);
}*/
