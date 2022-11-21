#include <Types.h>
#include <GameAPI.h>
#include <string>
#include <algorithm>
#include <Objects.h>
#include <GameProcess.h>
#include <unordered_set>

#include "GameData.h"
/*** String ***/

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

OSInputGlobals** g_inputGlobals = reinterpret_cast<OSInputGlobals**>(0x11F35CC);

NiExtraData* __fastcall NiObjectNET::GetExtraData(UInt32 vtbl) const
{
	for (UInt16 index = 0; index < m_extraDataListLen; index++)
	{
		const auto iter = this->m_extraDataList[index];
		if (*(UInt32*)iter == vtbl)
			return iter;
	}
	return nullptr;
}

NiControllerSequence* NiControllerManager::FindSequence(const char* seqName)
{
	for (const auto iter : sequences) if (!StrCompare(iter->sequenceName, seqName)) return iter;
	return nullptr;
}

// jazz code
__declspec(naked) bool NiControllerSequence::Play()
{
	__asm
	{
		push	ecx
		push	0
		push	0
		push	0
		push	0x3F800000
		push	0
		push	0
		mov		eax, 0xA34F20
		call	eax
		pop		ecx
		test	al, al
		jz		done
		mov		dword ptr [ecx+0x48], 0xFF7FFFFF
		mov		edx, [ecx+0x40]
		test	edx, edx
		jz		done
		or		byte ptr [edx+8], 8
	done:
		retn
	}
}

BSAnimGroupSequence* BSAnimGroupSequence::Get3rdPersonCounterpart() const
{
	const auto animData = PlayerCharacter::GetSingleton()->baseProcess->GetAnimData();
	if (const auto base = animData->mapAnimSequenceBase->Lookup(this->animGroup->groupID))
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

/*
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
	return GetGroupInfo(groupID);
}
*/

UInt8 TESAnimGroup::GetBaseGroupID() const
{
	return groupID;
}

__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds: [0x11C54C0]
		mov		eax, [esp + 4]
		xor		edx, edx
		div		dword ptr[ecx + 4]
		mov		eax, [ecx + 8]
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		edx, [esp + 4]
		ALIGN 16
	iterHead:
		cmp		[eax + 4], edx
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

extern TESDataHandler* g_TESDataHandler;
TESForm* GetRefFromString(char* mod, char* id)
{
	const auto itemID = (g_TESDataHandler->GetModIndex(mod) << 24) | strtoul(id, nullptr, 0);
	return LookupFormByRefID(itemID);
}

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