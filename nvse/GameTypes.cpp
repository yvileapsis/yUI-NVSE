#include <GameTypes.h>
#include <GameAPI.h>
#include <string>
#include <algorithm>
#include <GameObjects.h>
#include <GameProcess.h>
#include <unordered_set>
/*** String ***/

String::String() : m_data(NULL), m_dataLen(0), m_bufLen(0) {}

String::~String()
{
	if (m_data)
	{
		FormHeap_Free(m_data);
		m_data = NULL;
	}
	m_bufLen = m_dataLen = 0;
}

bool String::Set(const char * src)
{
	if (!src) {
		FormHeap_Free(m_data);
		m_data = 0;
		m_bufLen = 0;
		m_dataLen = 0;
		return true;
	}
	
	UInt32	srcLen = strlen(src);

	// realloc if needed
	if(srcLen > m_bufLen)
	{
		FormHeap_Free(m_data);
		m_data = (char *)FormHeap_Allocate(srcLen + 1);
		m_bufLen = m_data ? srcLen : 0;
	}

	if(m_data)
	{
		strcpy_s(m_data, m_bufLen + 1, src);
		m_dataLen = srcLen;
	}
	else
	{
		m_dataLen = 0;
	}

	return m_data != NULL;
}

bool String::Includes(const char *toFind) const
{
	if (!m_data || !toFind)		//passing null ptr to std::string c'tor = CRASH
		return false;
	std::string curr(m_data, m_dataLen);
	std::string str2(toFind);
	std::string::iterator currEnd = curr.end();
	return (std::search(curr.begin(), currEnd, str2.begin(), str2.end(), ci_equal) != currEnd);
}

bool String::Replace(const char *_toReplace, const char *_replaceWith)
{
	if (!m_data || !_toReplace)
		return false;

	std::string curr(m_data, m_dataLen);
	std::string toReplace(_toReplace);

	std::string::iterator currBegin = curr.begin();
	std::string::iterator currEnd = curr.end();
	std::string::iterator replaceIt = std::search(currBegin, currEnd, toReplace.begin(), toReplace.end(), ci_equal);
	if (replaceIt != currEnd) {
		std::string replaceWith(_replaceWith);
		// we found the substring, now we need to do the modification
		std::string::size_type replacePos = distance(currBegin, replaceIt);
		curr.replace(replacePos, toReplace.size(), replaceWith);
		Set(curr.c_str());
		return true;
	}
	return false;
}

bool String::Append(const char* toAppend)
{
	std::string curr("");
	if (m_data)
		curr = std::string(m_data, m_dataLen);

	curr += toAppend;
	Set(curr.c_str());
	return true;
}

double String::Compare(const String& compareTo, bool caseSensitive)
{
	if (!m_data)
		return -2;		//signal value if comparison could not be made

	std::string first(m_data, m_dataLen);
	std::string second(compareTo.m_data, compareTo.m_dataLen);

	if (!caseSensitive)
	{
		std::transform(first.begin(), first.end(), first.begin(), tolower);
		std::transform(second.begin(), second.end(), second.begin(), tolower);
	}

	double comp = 0;
	if (first < second)
		comp = -1;
	else if (first > second)
		comp = 1;
	
	return comp;
}

const char * String::CStr(void)
{

	return (m_data && m_dataLen) ? m_data : "";
}

void String::Init(UInt32 bufSize)
{
	if (m_data) GameHeapFree(m_data);
	m_bufLen = bufSize;
	m_data = (char*)GameHeapAlloc(m_bufLen + 1);
	*m_data = 0;
	m_dataLen = 0;
}

AnimGroupID AnimData::GetNextAttackGroupID() const
{
	const auto type = ThisStdCall<char>(0x495E40, this, 0);
	switch (type)
	{
	case '3':
		return kAnimGroup_Attack3;
	case '4':
		return kAnimGroup_Attack4;
	case '5':
		return kAnimGroup_Attack5;
	case '6':
		return kAnimGroup_Attack6;
	case '7':
		return kAnimGroup_Attack7;
	case '8':
		return kAnimGroup_Attack8;
	case 'l':
		return kAnimGroup_AttackLeft;
	default:
		TESObjectWEAP* weap;
		if (this->actor->baseProcess->GetWeaponInfo() && (weap = this->actor->baseProcess->GetWeaponInfo()->weapon))
		{
			if (weap->attackAnim != 0xFF)
				return static_cast<AnimGroupID>(weap->attackAnim);
		}
		return kAnimGroup_AttackRight;
	}
}

#if 0
void Actor::FireWeapon()
{
	animGroupId110 = static_cast<UInt32>(GetAnimData()->GetNextAttackGroupID());
	this->baseProcess->SetQueuedIdleFlag(kIdleFlag_FireWeapon);
	GameFuncs::HandleQueuedAnimFlags(this); //Actor::HandleQueuedIdleFlags
}

void Actor::EjectFromWeapon(TESObjectWEAP* weapon)
{
	if (weapon && GameFuncs::IsDoingAttackAnimation(this) && !weapon->IsMeleeWeapon() && !weapon->IsAutomatic())
	{
		// eject
		baseProcess->SetQueuedIdleFlag(kIdleFlag_AttackEjectEaseInFollowThrough);
		GameFuncs::HandleQueuedAnimFlags(this);
	}
}
#endif

TESObjectWEAP* Actor::GetWeaponForm() const
{
	auto* weaponInfo = this->baseProcess->GetWeaponInfo();
	if (!weaponInfo)
		return nullptr;
	return weaponInfo->weapon;
}

OSInputGlobals** g_inputGlobals = reinterpret_cast<OSInputGlobals**>(0x11F35CC);


bool TESObjectWEAP::IsMeleeWeapon() const
{
	return this->eWeaponType >= 0 && this->eWeaponType <= 2;
}

BSAnimGroupSequence* BSAnimGroupSequence::Get3rdPersonCounterpart() const
{
	auto* animData = g_thePlayer->baseProcess->GetAnimData();
	if (auto* base = animData->mapAnimSequenceBase->Lookup(this->animGroup->groupID))
		return base->GetSequenceByIndex(0);
	return nullptr;
}


bool TESAnimGroup::IsAttackIS()
{
	const auto idMinor = static_cast<AnimGroupID>(groupID & 0xFF);
	switch (idMinor)
	{
	case kAnimGroup_AttackLeftIS: break;
	case kAnimGroup_AttackRightIS: break;
	case kAnimGroup_Attack3IS: break;
	case kAnimGroup_Attack4IS: break;
	case kAnimGroup_Attack5IS: break;
	case kAnimGroup_Attack6IS: break;
	case kAnimGroup_Attack7IS: break;
	case kAnimGroup_Attack8IS: break;
	case kAnimGroup_AttackLoopIS: break;
	case kAnimGroup_AttackSpinIS: break;
	case kAnimGroup_AttackSpin2IS: break;
	case kAnimGroup_PlaceMineIS: break;
	case kAnimGroup_PlaceMine2IS: break;
	case kAnimGroup_AttackThrowIS: break;
	case kAnimGroup_AttackThrow2IS: break;
	case kAnimGroup_AttackThrow3IS: break;
	case kAnimGroup_AttackThrow4IS: break;
	case kAnimGroup_AttackThrow5IS: break;
	case kAnimGroup_Attack9IS: break;
	case kAnimGroup_AttackThrow6IS: break;
	case kAnimGroup_AttackThrow7IS: break;
	case kAnimGroup_AttackThrow8IS: break;
	default: return false;
	}
	return true;
}

bool TESAnimGroup::IsAttackNonIS()
{
	const auto idMinor = static_cast<AnimGroupID>(groupID & 0xFF);
	switch (idMinor)
	{
	case kAnimGroup_AttackLeft: break;
	case kAnimGroup_AttackRight: break;
	case kAnimGroup_Attack3: break;
	case kAnimGroup_Attack4: break;
	case kAnimGroup_Attack5: break;
	case kAnimGroup_Attack6: break;
	case kAnimGroup_Attack7: break;
	case kAnimGroup_Attack8: break;
	case kAnimGroup_AttackLoop: break;
	case kAnimGroup_AttackSpin: break;
	case kAnimGroup_AttackSpin2: break;
	case kAnimGroup_AttackPower: break;
	case kAnimGroup_AttackForwardPower: break;
	case kAnimGroup_AttackBackPower: break;
	case kAnimGroup_AttackLeftPower: break;
	case kAnimGroup_AttackRightPower: break;
	case kAnimGroup_AttackCustom1Power: break;
	case kAnimGroup_AttackCustom2Power: break;
	case kAnimGroup_AttackCustom3Power: break;
	case kAnimGroup_AttackCustom4Power: break;
	case kAnimGroup_AttackCustom5Power: break;
	case kAnimGroup_PlaceMine: break;
	case kAnimGroup_PlaceMine2: break;
	case kAnimGroup_AttackThrow: break;
	case kAnimGroup_AttackThrow2: break;
	case kAnimGroup_AttackThrow3: break;
	case kAnimGroup_AttackThrow4: break;
	case kAnimGroup_AttackThrow5: break;
	case kAnimGroup_Attack9: break;
	case kAnimGroup_AttackThrow6: break;
	case kAnimGroup_AttackThrow7: break;
	case kAnimGroup_AttackThrow8: break;

	default: return false;
	}
	return true;
}

#if 0
TESAnimGroup::AnimGroupInfo* GetGroupInfo(UInt8 groupId)
{
	return &g_animGroupInfos[groupId];
}

UInt32 GetSequenceType(UInt8 groupId)
{
	return GetGroupInfo(groupId)->sequenceType;
}

TESAnimGroup::AnimGroupInfo* TESAnimGroup::GetGroupInfo() const
{
	return ::GetGroupInfo(groupID);
}
#endif

UInt8 TESAnimGroup::GetBaseGroupID() const
{
	return groupID;
}

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx

#if 0
__declspec(naked) NiAVObject* __fastcall NiNode::GetBlockByName(const char* nameStr)	//	str of NiString
{
	__asm
	{
		movzx	eax, word ptr[ecx + 0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx + 0xA0]
		mov		edi, eax
		ALIGN 16
		iterHead:
		dec		edi
			js		iterEnd
			mov		eax, [esi]
			add		esi, 4
			test	eax, eax
			jz		iterHead
			cmp[eax + 8], edx
			jz		found
			mov		ecx, [eax]
			cmp		dword ptr[ecx + 0xC], kAddr_ReturnThis
			jnz		iterHead
			mov		ecx, eax
			call	NiNode::GetBlockByName
			test	eax, eax
			jz		iterHead
			found :
		pop		edi
			pop		esi
			retn
			ALIGN 16
			iterEnd :
			xor eax, eax
			pop		edi
			pop		esi
			done :
		retn
	}
}

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlock(const char* blockName)
{
	__asm
	{
		cmp[edx], 0
		jz		retnNULL
		push	ecx
		push	edx
		CALL_EAX(0xA5B690)
		pop		ecx
		pop		ecx
		test	eax, eax
		jz		done
		lock dec dword ptr[eax - 8]
		jz		retnNULL
		cmp[ecx + 8], eax
		jz		found
		mov		edx, eax
		call	NiNode::GetBlockByName
		retn
		found :
		mov		eax, ecx
			retn
			retnNULL :
		xor eax, eax
			done :
		retn
	}
}

__declspec(naked) NiNode* __fastcall NiNode::GetNode(const char* nodeName)
{
	__asm
	{
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		xor edx, edx
		mov		ecx, [eax]
		cmp		dword ptr[ecx + 0xC], kAddr_ReturnThis
		cmovnz	eax, edx
		done :
		retn
	}
}


__declspec(naked) NiAVObject* __fastcall GetNifBlock(TESObjectREFR* thisObj, UInt32 pcNode, const char* blockName)
{
	__asm
	{
		test	dl, dl
		jz		notPlayer
		cmp		dword ptr[ecx + 0xC], 0x14
		jnz		notPlayer
		test	dl, 1
		jz		get1stP
		mov		eax, [ecx + 0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax + 0x14]
		jmp		gotRoot
		get1stP :
		mov		eax, [ecx + 0x694]
			jmp		gotRoot
			notPlayer :
		mov		eax, [ecx]
			call	dword ptr[eax + 0x1D0]
			gotRoot :
			test	eax, eax
			jz		done
			mov		edx, [esp + 4]
			cmp[edx], 0
			jz		done
			mov		ecx, eax
			call	NiNode::GetBlock
			done :
		retn	4
	}
}
#endif

float TESObjectWEAP::GetModBonuses(UInt32 effectID)
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if (effectMods[idx] == effectID)
			result += value1Mod[idx];
	return result;
}

bool Actor::IsAnimActionReload() const
{
	const auto currentAnimAction = static_cast<AnimAction>(baseProcess->GetCurrentAnimAction());
	const static std::unordered_set s_reloads = { kAnimAction_Reload, kAnimAction_ReloadLoop, kAnimAction_ReloadLoopStart, kAnimAction_ReloadLoopEnd };
	return s_reloads.contains(currentAnimAction);
}

__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds: [0x11C54C0]
		mov		eax, [esp + 4]
		xor edx, edx
		div		dword ptr[ecx + 4]
		mov		eax, [ecx + 8]
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		edx, [esp + 4]
		ALIGN 16
	iterHead:
		cmp[eax + 4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		retn	4
		found:
		mov		eax, [eax + 8]
	done :
		retn	4
	}
}

#if 0
Script* Script::CompileFromText(const std::string& scriptSource, const std::string& name)
{
	ScriptBuffer buffer;
	DataHandler::GetSingleton()->DisableAssignFormIDs(true);
	auto condition = MakeUnique<Script, 0x5AA0F0, 0x5AA1A0>();
	DataHandler::GetSingleton()->DisableAssignFormIDs(false);
	buffer.scriptName.Set(("kNVSEScript_" + name).c_str());
	buffer.scriptText = scriptSource.data();
	buffer.partialScript = true;
	buffer.currentScript = condition.get();
	const auto* ctx = ConsoleManager::GetSingleton()->scriptContext;
	const auto result = ThisStdCall<bool>(0x5AEB90, ctx, condition.get(), &buffer);
	condition->text = nullptr;
	buffer.scriptText = nullptr;
	if (!result)
	{
		DebugPrint("Failed to compile script");
		return nullptr;
	}
	return condition.release();
}

bool TESForm::IsTypeActor()
{
	return IS_ID(this, Character) || IS_ID(this, Creature);
}
#endif

void FormatScriptText(std::string& str)
{
	UInt32 pos = 0;

	while (((pos = str.find('%', pos)) != -1) && pos < str.length() - 1)
	{
		char toInsert = 0;
		switch (str[pos + 1])
		{
		case '%':
			pos += 2;
			continue;
		case 'r':
		case 'R':
			toInsert = '\n';
			break;
		case 'q':
		case 'Q':
			toInsert = '"';
			break;
		default:
			pos += 1;
			continue;
		}

		str.insert(pos, 1, toInsert); // insert char at current pos
		str.erase(pos + 1, 2);		  // erase format specifier
		pos += 1;
	}
}

UInt32* g_weaponTypeToAnim = reinterpret_cast<UInt32*>(0x118A838);

UInt16 GetActorRealAnimGroup(Actor* actor, UInt8 groupID)
{
	UInt8 animHandType = 0;
	if (auto* form = actor->GetWeaponForm())
		animHandType = g_weaponTypeToAnim[form->eWeaponType];
	auto moveFlags = actor->actorMover->GetMovementFlags();
	UInt8 moveType = 0;
	if ((moveFlags & 0x800) != 0)
		moveType = kAnimMoveType_Swimming;
	else if ((moveFlags & 0x2000) != 0)
		moveType = kAnimMoveType_Flying;
	else if ((moveFlags & 0x400) != 0)
		moveType = kAnimMoveType_Sneaking;
	const auto isPowerArmor = ThisStdCall<bool>(0x8BA3E0, actor) || ThisStdCall<bool>(0x8BA410, actor);
	return (moveType << 12) + (isPowerArmor << 15) + (animHandType << 8) + groupID;
}