#include <GameAPI.h>
#include <GameRTTI.h>
#include <GameForms.h>
#include <GameObjects.h>
#include <GameTypes.h>
#include <GameData.h>
#include <GameScript.h>
#include <StringVar.h>

bool alternateUpdate3D = false;

// arg1 = 1, ignored if canCreateNew is false, passed to 'init' function if a new object is created
char s_tempStrArgBuffer[0x4000];

const _CreateFormInstance CreateFormInstance = reinterpret_cast<_CreateFormInstance>(0x00465110);

bool* bEchoConsole = reinterpret_cast<bool*>(0x011F158C);

const _QueueUIMessage QueueUIMessage = reinterpret_cast<_QueueUIMessage>(0x007052F0);	// Called from Cmd_AddSpell_Execute

const _ShowMessageBox ShowMessageBox = reinterpret_cast<_ShowMessageBox>(0x00703E80);
const _ShowMessageBox_Callback ShowMessageBox_Callback = reinterpret_cast<_ShowMessageBox_Callback>(0x005B4A70);
const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID = reinterpret_cast<_ShowMessageBox_pScriptRefID>(0x011CAC64);
const _ShowMessageBox_button ShowMessageBox_button = reinterpret_cast<_ShowMessageBox_button>(0x0118C684);

const _GetActorValueName GetActorValueName = reinterpret_cast<_GetActorValueName>(0x00066EAC0);	// See Cmd_GetActorValue_Eval
const UInt32 * g_TlsIndexPtr = reinterpret_cast<UInt32*>(0x0126FD98);
const _MarkBaseExtraListScriptEvent MarkBaseExtraListScriptEvent = reinterpret_cast<_MarkBaseExtraListScriptEvent>(0x005AC750);
const _DoCheckScriptRunnerAndRun DoCheckScriptRunnerAndRun = reinterpret_cast<_DoCheckScriptRunnerAndRun>(0x005AC190);

// Johnny Guitar supports this

struct TLSData
{
	// thread local storage

	UInt32			pad000[(0x260 - 0x000) >> 2];	// 000
	NiNode*			lastNiNode;						// 260	248 in FOSE
	TESObjectREFR*	lastNiNodeREFR;					// 264	24C in FOSE
	UInt8			consoleMode;					// 268
	UInt8			pad269[3];						// 269
	// 25C is used as do not head track the player , 
	// 2B8 is used to init QueudFile::unk0018, 
	// 28C might count the recursive calls to Activate, limited to 5.
};


static TLSData * GetTLSData()
{
	UInt32 TlsIndex = *g_TlsIndexPtr;
	TLSData* data = nullptr;

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

void SetConsoleEcho(const bool doEcho)
{
	*bEchoConsole = doEcho ? true : false;
}

const char* GetFullName(const TESForm * baseForm)
{
	if (baseForm)
		if (const auto fullName = baseForm->GetFullName(); fullName && fullName->name.m_data && fullName->name.m_dataLen)
			return fullName->name.m_data;
	return "<no name>";
}

ConsoleManager::TextList* ConsoleManager::TextList::Append(TextNode* append)
{
	append->prev = 0;
	append->next = this->first;
	if (this->first) this->first->prev = append;
	else this->last = append;
	this->first = append;
	++this->count;
	return this;
}

char * ConsoleManager::GetConsoleOutputFilename(void)
{
	return GetSingleton()->scofPath;
};

bool ConsoleManager::HasConsoleOutputFilename(void) {
	return 0 != GetSingleton()->scofPath[0];
}

extern DataHandler* g_dataHandler;

TESForm* GetFormByID(const char* mod, UInt32 refID)
{
	return GetFormByID(g_dataHandler->GetModIndex(mod) << 8 | refID);
}

bool s_InsideOnActorEquipHook = false;
UInt32 s_CheckInsideOnActorEquipHook = 1;

void PrintConsole(const char * fmt, ...)
{
	if (const auto mgr = ConsoleManager::GetSingleton())
	{
		va_list	args;
		va_start(args, fmt);
		mgr->Print(fmt, args);
		va_end(args);
	}
}

std::string GetSavegamePath()
{
	char path[0x104] = "\0";
	SaveGameManager::GetSingleton()->ConstructSavegamePath(path);
	return path;
}

void ScriptEventList::Destructor()
{
// OBLIVION	ThisStdCall(0x004FB4E0, this);
	if (m_eventList)
		m_eventList->RemoveAll();
	while (m_vars) {
		if (m_vars->var) FormHeap_Free(m_vars->var);
		VarEntry* next = m_vars->next;
		FormHeap_Free(m_vars);
		m_vars = next;
	}
}

TList<ScriptEventList::Var>* ScriptEventList::GetVars() const
{
	return reinterpret_cast<TList<Var>*>(m_vars);
}

void ScriptEventList::Dump(void)
{
	const auto nEvents = m_eventList->Count();
	for (UInt16 n = 0; n < nEvents; ++n)
		if (const Event* pEvent = m_eventList->GetNthItem(n))
			PrintConsole("%08X (%s) %08X", pEvent->object, GetObjectClassName(pEvent->object), pEvent->eventMask);
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
	return *(UInt32*)(&data);
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
		if (_stricmp(avStr, GetActorValueName(i)) == 0)
			return i;
	}

	return eActorVal_NoActorValue;
}

UInt32 GetActorValueForString(const char* strActorVal, bool bForScript)
{
	if (bForScript)
		return GetActorValueForScript(strActorVal);

	for (UInt32 n = 0; n <= eActorVal_FalloutMax; n++) {
		if (_stricmp(strActorVal, GetActorValueName(n)) == 0)
			return n;
	}
	return eActorVal_NoActorValue;
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
	case eActorVal_Aggression:			return   3;
	case eActorVal_Confidence:			return   4;
	case eActorVal_Energy:				return 100;
	case eActorVal_Responsibility:		return 100;
	case eActorVal_Mood:				return   8;

	case eActorVal_Strength:			return  10;
	case eActorVal_Perception:			return  10;
	case eActorVal_Endurance:			return  10;
	case eActorVal_Charisma:			return  10;
	case eActorVal_Intelligence:		return  10;
	case eActorVal_Agility:				return  10;
	case eActorVal_Luck:				return  10;

	case eActorVal_ActionPoints:		return   1;
	case eActorVal_CarryWeight:			return   1;
	case eActorVal_CritChance:			return 100;
	case eActorVal_HealRate:			return   1;
	case eActorVal_Health:				return   1;
	case eActorVal_MeleeDamage:			return   1;
	case eActorVal_DamageResistance:	return   1;
	case eActorVal_PoisonResistance:	return   1;
	case eActorVal_RadResistance:		return   1;
	case eActorVal_SpeedMultiplier:		return   1;
	case eActorVal_Fatigue:				return   1;
	case eActorVal_Karma:				return   1;
	case eActorVal_XP:					return   1;

	case eActorVal_Head:				return 100;
	case eActorVal_Torso:				return 100;
	case eActorVal_LeftArm:				return 100;
	case eActorVal_RightArm:			return 100;
	case eActorVal_LeftLeg:				return 100;
	case eActorVal_RightLeg:			return 100;
	case eActorVal_Brain:				return 100;

	case eActorVal_Barter:				return 100;
	case eActorVal_BigGuns:				return 100;
	case eActorVal_EnergyWeapons:		return 100;
	case eActorVal_Explosives:			return 100;
	case eActorVal_Lockpick:			return 100;
	case eActorVal_Medicine:			return 100;
	case eActorVal_MeleeWeapons:		return 100;
	case eActorVal_Repair:				return 100;
	case eActorVal_Science:				return 100;
	case eActorVal_Guns:				return 100;
	case eActorVal_Sneak:				return 100;
	case eActorVal_Speech:				return 100;
	case eActorVal_Survival:			return 100;
	case eActorVal_Unarmed:				return 100;

	case eActorVal_InventoryWeight:		return   1;
	case eActorVal_Paralysis:			return   1;
	case eActorVal_Invisibility:		return   1;
	case eActorVal_Chameleon:			return   1;
	case eActorVal_NightEye:			return   1;
	case eActorVal_Turbo:				return   1;
	case eActorVal_FireResistance:		return   1;
	case eActorVal_WaterBreathing:		return   1;
	case eActorVal_RadLevel:			return   1;
	case eActorVal_BloodyMess:			return   1;
	case eActorVal_UnarmedDamage:		return   1;
	case eActorVal_Assistance:			return   2;

	case eActorVal_ElectricResistance:	return   1;

	case eActorVal_EnergyResistance:	return   1;
	case eActorVal_EMPResistance:		return   1;
	case eActorVal_Var1Medical:			return   1;
	case eActorVal_Var2:				return   1;
	case eActorVal_Var3:				return   1;
	case eActorVal_Var4:				return   1;
	case eActorVal_Var5:				return   1;
	case eActorVal_Var6:				return   1;
	case eActorVal_Var7:				return   1;
	case eActorVal_Var8:				return   1;
	case eActorVal_Var9:				return   1;
	case eActorVal_Var10:				return   1;

	case eActorVal_IgnoreCrippledLimbs:	return   1;
	case eActorVal_Dehydration:			return   1;
	case eActorVal_Hunger:				return   1;
	case eActorVal_Sleepdeprevation:	return   1;
	case eActorVal_Damagethreshold:		return   1;
	default: return 1;
	}
}

typedef FontManager* (* _FontManager_GetSingleton)(void);
const _FontManager_GetSingleton FontManager_GetSingleton = reinterpret_cast<_FontManager_GetSingleton>(0x011F33F8);

UInt8* GetScriptDataPosition(Script* script, void* scriptDataIn, const UInt32* opcodeOffsetPtrIn)
{
	if (scriptDataIn != script->data) // set ... to or if ..., script data is stored on stack and not heap
	{
		auto* scriptData = *(static_cast<UInt8**>(scriptDataIn) + 0x1D5);
		return scriptData + *opcodeOffsetPtrIn + 1;
	}
	return static_cast<UInt8*>(scriptDataIn) + *opcodeOffsetPtrIn;
}

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
}
