#include "GameForms.h"

#include "GameAPI.h"
#include "GameRTTI.h"
#include "GameObjects.h"
#include "GameData.h"
#include "GameProcess.h"
#include "GameSettings.h"

static const ActorValueInfo** ActorValueInfoPointerArray = (const ActorValueInfo**)0x0011D61C8;		// See GetActorValueInfo
static const _GetActorValueInfo GetActorValueInfo = (_GetActorValueInfo)0x00066E920;	// See GetActorValueName
BGSDefaultObjectManager ** g_defaultObjectManager = (BGSDefaultObjectManager**)0x011CA80C;

TESForm* TESForm::TryGetREFRParent()
{
	auto result = this;
	if (const auto refr = DYNAMIC_CAST(this, TESForm, TESObjectREFR); refr && refr->baseForm)
		result = refr->baseForm;
	return result;
}

TESFullName* TESForm::GetFullName() const
{
	if (typeID == kFormType_TESObjectCELL)		// some exterior cells inherit name of parent worldspace
	{
		const auto cell = DYNAMIC_CAST(this, TESForm, TESObjectCELL);
		TESFullName *fullName = &cell->fullName;
		return fullName->name.m_data && fullName->name.m_dataLen || !cell->worldSpace ? fullName : &cell->worldSpace->fullName;
	}
	const TESForm *baseForm = GetIsReference() ? (DYNAMIC_CAST(this, TESForm, TESObjectREFR))->baseForm : this;
	return DYNAMIC_CAST(baseForm, TESForm, TESFullName);
}

const char* TESForm::GetTheName()
{
	const auto fullName = GetFullName();
	return fullName ? fullName->name.CStr() : "";
}

void TESForm::DoAddForm(TESForm* newForm, bool persist, bool record)
{
	CALL_MEMBER_FN(DataHandler::GetSingleton(), DoAddForm)(newForm);

	if (persist)
	{
		// Only some forms can be safely saved as SaveForm. ie TESPackage at the moment.
		bool canSave = false;
		const auto package = DYNAMIC_CAST(newForm, TESForm, TESPackage);
		if (package)
			canSave = true;
		// ... more ?

		if (canSave)
			CALL_MEMBER_FN(TESSaveLoadGame::Get(), AddCreatedForm)(newForm);
	}
}

TESForm* TESForm::CloneForm(bool persist) const
{
	TESForm* result = CreateFormInstance(typeID);
	if (result)
	{
		result->CopyFrom(this);
		// it looks like some fields are not copied, case in point: TESObjectCONT does not copy BoundObject information.
		const auto boundObject = DYNAMIC_CAST(result, TESForm, TESBoundObject);
		if (boundObject)
		{
			const auto boundSource = DYNAMIC_CAST(this, TESForm, TESBoundObject);
			if (boundSource)
			{
				for (UInt8 i = 0; i < 6; i++)
					boundObject->bounds[i] = boundSource->bounds[i];
			}
		}
		DoAddForm(result, persist);
	}

	return result;
}

bool TESForm::IsCloned() const
{
	return this->modIndex == 0xFF;
}

std::string TESForm::GetStringRepresentation() const
{
	return FormatString(R"([id: %X, edid: "%s", name: "%s"])", refID, GetName(), GetFullName()->name.CStr());
}

// static
UInt32 TESBipedModelForm::MaskForSlot(UInt32 slot)
{
	switch(slot) {
		case ePart_Head:		return eSlot_Head;
		case ePart_Hair:		return eSlot_Hair;
		case ePart_UpperBody:	return eSlot_UpperBody;
		case ePart_LeftHand:	return eSlot_LeftHand;
		case ePart_RightHand:	return eSlot_RightHand;
		case ePart_Weapon:		return eSlot_Weapon;
		case ePart_PipBoy:		return eSlot_PipBoy;
		case ePart_Backpack:	return eSlot_Backpack;
		case ePart_Necklace:	return eSlot_Necklace;
		case ePart_Headband:	return eSlot_Headband;
		case ePart_Hat:			return eSlot_Hat;
		case ePart_Eyeglasses:	return eSlot_Eyeglasses;
		case ePart_Nosering:	return eSlot_Nosering;
		case ePart_Earrings:	return eSlot_Earrings;
		case ePart_Mask:		return eSlot_Mask;
		case ePart_Choker:		return eSlot_Choker;
		case ePart_MouthObject:	return eSlot_MouthObject;
		case ePart_BodyAddon1:	return eSlot_BodyAddon1;
		case ePart_BodyAddon2:	return eSlot_BodyAddon2;
		case ePart_BodyAddon3:	return eSlot_BodyAddon3;
		default:				return -1;
	}
}

UInt32 TESBipedModelForm::GetSlotsMask() const {
	return partMask;
}

void TESBipedModelForm::SetSlotsMask(UInt32 mask)
{
	partMask = (mask & ePartBitMask_Full);
}

UInt32 TESBipedModelForm::GetBipedMask() const {
	return bipedFlags & 0xFF;
}

void TESBipedModelForm::SetBipedMask(UInt32 mask)
{
	bipedFlags = mask & 0xFF;
}

void  TESBipedModelForm::SetPath(const char* newPath, UInt32 whichPath, bool bFemalePath)
{
	String* toSet = nullptr;

	switch (whichPath)
	{
	case ePath_Biped:
		toSet = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		toSet = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		toSet = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	default: break;
	}

	if (toSet) toSet->Set(newPath);
}

const char* TESBipedModelForm::GetPath(UInt32 whichPath, bool bFemalePath)
{
	String* pathStr = nullptr;

	switch (whichPath)
	{
	case ePath_Biped:
		pathStr = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		pathStr = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		pathStr = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	default: break;
	}

	return pathStr ? pathStr->m_data : "";
}

SInt8 TESActorBaseData::GetFactionRank(TESFaction* faction)
{
	for (const auto iter : factionList)
		if (iter && (iter->faction == faction))
			return iter->rank;

	return -1;
}

static constexpr UInt8 kHandGripTable[] =
{
	TESObjectWEAP::eHandGrip_Default,
	TESObjectWEAP::eHandGrip_1,
	TESObjectWEAP::eHandGrip_2,
	TESObjectWEAP::eHandGrip_3,
	TESObjectWEAP::eHandGrip_4,
	TESObjectWEAP::eHandGrip_5,
	TESObjectWEAP::eHandGrip_6,
};

bool TESObjectWEAP::IsMeleeWeapon() const
{
	return eWeaponType >= 0 && eWeaponType <= 2;
}

float TESObjectWEAP::GetModBonuses(UInt32 effectID)
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++) if (effectMods[idx] == effectID) result += value1Mod[idx];
	return result;
}

UInt8 TESObjectWEAP::HandGrip() const
{
	for (UInt32 i = 0; i < std::size(kHandGripTable); i++) if (handGrip == kHandGripTable[i]) return i;
	return 0;
}

void TESObjectWEAP::SetHandGrip(UInt8 _handGrip)
{
	if (_handGrip < std::size(kHandGripTable)) handGrip = kHandGripTable[_handGrip];
}

UInt8 TESObjectWEAP::AttackAnimation() const
{
	switch(attackAnim) {
		case eAttackAnim_Default:		return 0;
		case eAttackAnim_Attack3:		return 1;
		case eAttackAnim_Attack4:		return 2;
		case eAttackAnim_Attack5:		return 3;
		case eAttackAnim_Attack6:		return 4;
		case eAttackAnim_Attack7:		return 5;
		case eAttackAnim_Attack8:		return 6;
		case eAttackAnim_AttackLeft:	return 7;
		case eAttackAnim_AttackLoop:	return 8;
		case eAttackAnim_AttackRight:	return 9;
		case eAttackAnim_AttackSpin:	return 10;
		case eAttackAnim_AttackSpin2:	return 11;
		case eAttackAnim_AttackThrow:	return 12;
		case eAttackAnim_AttackThrow2:	return 13;
		case eAttackAnim_AttackThrow3:	return 14;
		case eAttackAnim_AttackThrow4:	return 15;
		case eAttackAnim_AttackThrow5:	return 16;
		case eAttackAnim_PlaceMine:		return 17;
		case eAttackAnim_PlaceMine2:	return 18;
		default:						return -1;
	}
}

void TESObjectWEAP::SetAttackAnimation(UInt8 _attackAnim)
{
	switch(_attackAnim) {
		case 0: {
			attackAnim = eAttackAnim_Default; 
			break;
		}
		case 1: {
			attackAnim = eAttackAnim_Attack3; 
			break;
		}
		case 2: {
			attackAnim = eAttackAnim_Attack4;
			break;
		}
		case 3: {
			attackAnim = eAttackAnim_Attack5;
			break;
		}
		case 4: {
			attackAnim = eAttackAnim_Attack6;
			break;
		}
		case 5: {
			attackAnim = eAttackAnim_Attack7;
			break;
		}
		case 6: {
			attackAnim = eAttackAnim_Attack8;
			break;
		}
		
		case 7: {
			attackAnim = eAttackAnim_AttackLeft;
			break;
		}
		case 8: {
			attackAnim = eAttackAnim_AttackLoop;
			break;
		}
		case 9: {
			attackAnim = eAttackAnim_AttackRight;
			break;
		}
		case 10: {
			attackAnim = eAttackAnim_AttackSpin;
			break;
		}
		case 11: {
			attackAnim = eAttackAnim_AttackSpin2;
			break;
		}
		case 12: {
			attackAnim = eAttackAnim_AttackThrow;
			break;
		}
		case 13: {
			attackAnim = eAttackAnim_AttackThrow2;
			break;
		}
		case 14: {
			attackAnim = eAttackAnim_AttackThrow3;
			break;
		}
		case 15: {
			attackAnim = eAttackAnim_AttackThrow4;
			break;
		}
		case 16: {
			attackAnim = eAttackAnim_AttackThrow5;
			break;
		}
		case 17: {
			attackAnim = eAttackAnim_PlaceMine;
			break;
		}
		case 18: {
			attackAnim = eAttackAnim_PlaceMine2;
			break;
		}
		default:
			break;
	}
}

TESObjectIMOD* TESObjectWEAP::GetItemMod(UInt8 which)
{
	TESObjectIMOD* pMod = nullptr;
	switch (which) {
	case 1: pMod = itemMod1; break;
	case 2: pMod = itemMod2; break;
	case 3: pMod = itemMod3; break;
	default: break;
	}
	return pMod;
}

bool TESObjectWEAP::IsMeleeWeapon()
{
	return eWeaponType == kWeapType_HandToHandMelee || eWeaponType == kWeapType_OneHandMelee || eWeaponType == kWeapType_TwoHandMelee;
}

bool TESObjectWEAP::IsRangedWeapon()
{
	return (this->projectile);
}

class FindByForm {
	TESForm* m_pForm;
public:
	FindByForm(TESForm* pForm) : m_pForm(pForm) {}
	bool Accept(TESForm* pForm) const {
		return pForm && (pForm->refID == m_pForm->refID) ? true : false;
	}
};

SInt32 BGSListForm::GetIndexOf(TESForm* pForm)
{
	return list.GetIndexOf(FindByForm(pForm));
}

SInt32 BGSListForm::RemoveForm(TESForm* pForm)
{
	const SInt32 index = GetIndexOf(pForm);
	if (index >= 0) RemoveNthForm(index);
	return index;
}

SInt32 BGSListForm::ReplaceForm(TESForm* pForm, TESForm* pReplaceWith)
{
	const SInt32 index = GetIndexOf(pForm);
	if (index >= 0) list.ReplaceNth(index, pReplaceWith);
	return index;
}

bool BGSListForm::Contains(TESForm* form)
{
	if (!form) return false;
	for (const auto iter : this->list) if (iter->refID == form->refID) return true;
	return false;
}

bool BGSListForm::ContainsRecursive(TESForm* form, UInt32 reclvl)
{
	if (!form) return false;
	if (this->Contains(form)) return true;
	if (reclvl > 100) return false; [[unlikely]]
	for (const auto iter : this->list)
		if (iter->typeID == kFormType_BGSListForm)
			if (const auto list = DYNAMIC_CAST(this, TESForm, BGSListForm);  list && list->ContainsRecursive(form, reclvl++)) return true;
	return false;
}

bool FormContainsRecusive(TESForm* parent, TESForm* child)
{
	if (!parent || !child) return false;
	if (parent->refID == child->refID) return true;
	if (parent->typeID == kFormType_BGSListForm)
	{
		const auto list = DYNAMIC_CAST(parent, TESForm, BGSListForm);
		if (list && list->ContainsRecursive(child)) return true;
	}
	return false;
}

bool TESForm::IsInventoryObject() const
{
	typedef bool (* _IsInventoryObjectType)(UInt32 formType);
#if RUNTIME
	static _IsInventoryObjectType IsInventoryObjectType = (_IsInventoryObjectType)0x00481F30;	// first call from first case of main switch in _ExtractArg
#elif EDITOR
	static _IsInventoryObjectType IsInventoryObjectType = (_IsInventoryObjectType)0x004F4100;	// first call from first case of main switch in Cmd_DefaultParse
#endif
	return IsInventoryObjectType(typeID);
}

bool TESForm::IsInventoryObjectAlt()
{
	if (!this) return false;
	switch (this->typeID)
	{
	case kFormType_TESObjectARMO:
	case kFormType_TESObjectBOOK:
	case kFormType_TESObjectCLOT:
	case kFormType_IngredientItem:
	case kFormType_TESObjectMISC:
	case kFormType_TESObjectWEAP:
	case kFormType_TESAmmo:
	case kFormType_TESKey:
	case kFormType_AlchemyItem:
	case kFormType_BGSNote:
	case kFormType_TESObjectARMA:
	case kFormType_TESObjectIMOD:
	case kFormType_TESCasinoChips:
	case kFormType_TESCaravanCard:
	case kFormType_TESCaravanMoney:
		return true;
	case kFormType_TESObjectLIGH:
		if (const auto light = DYNAMIC_CAST(this, TESForm, TESObjectLIGH))
			if (light->lightFlags & 2)
				return true;
	[[fallthrough]];
	default: return false;
	}
}

const char* TESPackage::TargetData::StringForTargetCode(UInt8 targetCode)
{
	switch (targetCode)	{
	case kTargetType_Refr:			return "Reference";
	case kTargetType_BaseObject:	return "Object";
	case kTargetType_TypeCode:		return "ObjectType";
	default:						return nullptr;
	}
}

UInt8 TESPackage::TargetData::TargetCodeForString(const char* targetStr)
{
	if (!_stricmp(targetStr, "REFERENCE"))
		return kTargetType_Refr;
	if (!_stricmp(targetStr, "OBJECT"))
		return kTargetType_BaseObject;
	if (!_stricmp(targetStr, "OBJECTTYPE"))
		return kTargetType_TypeCode;
	return 0xFF;
}

TESPackage::TargetData* TESPackage::TargetData::Create()
{
	const auto data = (TargetData*)FormHeap_Allocate(sizeof(TargetData));

	// fill out with same defaults as editor uses
	data->count = 0;
	data->target.objectCode = kObjectType_Activators;
	data->targetType = kTargetType_TypeCode;

	return data;
}

TESPackage::TargetData* TESPackage::GetTargetData()
{
	if (!target) target = TargetData::Create();
	return target;
}

void TESPackage::SetTarget(TESObjectREFR* refr)
{
	TargetData* tdata = GetTargetData();
	tdata->targetType = kTargetType_Refr;
	tdata->target.refr = refr;
	tdata->count = 150;	//DefaultDistance
}

void TESPackage::SetCount(UInt32 aCount)
{
	if (target) {
		TargetData* tdata = GetTargetData();
		tdata->count = aCount;
	}
}

void TESPackage::SetTarget(TESForm* baseForm, UInt32 count)
{
	TargetData* tdata = GetTargetData();
	tdata->targetType = kTargetType_BaseObject;
	tdata->count = count;
	tdata->target.form = baseForm;
}

void TESPackage::SetTarget(eObjectType typeCode, UInt32 count)
{
	if (typeCode > 0 && typeCode < kObjectType_Max)
	{
		TargetData* tdata = GetTargetData();
		tdata->targetType = kTargetType_TypeCode;
		tdata->target.objectCode = typeCode;
		tdata->count= count;
	}
}

TESPackage::LocationData* TESPackage::LocationData::Create()
{
	const auto data = (LocationData*)FormHeap_Allocate(sizeof(LocationData));

	data->locationType = kPackLocation_CurrentLocation;
	data->object.form = nullptr;
	data->radius = 0;

	return data;
}

TESPackage::LocationData* TESPackage::GetLocationData()
{
	if (!location) location = LocationData::Create();
	return location;
}

bool TESPackage::IsFlagSet(UInt32 flag)
{
	return (packageFlags & flag) == flag;
}

void TESPackage::SetFlag(UInt32 flag, bool bSet)
{
	if (bSet)
		packageFlags |= flag;
	else
		packageFlags &= ~flag;

	// handle either-or flags
	switch (flag)
	{
	case kPackageFlag_LockDoorsAtStart:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtStart) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtStart, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtStart:
		if (IsFlagSet(kPackageFlag_LockDoorsAtStart) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtStart, !bSet);
		break;
	case kPackageFlag_LockDoorsAtEnd:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtEnd) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtEnd, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtEnd:
		if (IsFlagSet(kPackageFlag_LockDoorsAtEnd) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtEnd, !bSet);
		break;
	case kPackageFlag_LockDoorsAtLocation:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtLocation) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtLocation, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtLocation:
		if (IsFlagSet(kPackageFlag_LockDoorsAtLocation) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtLocation, !bSet);
		break;
	}
}

static const char* TESPackage_ObjectTypeStrings[TESPackage::kObjectType_Max] =
{
	"NONE", "Activators", "Armors", "Books", "Clothing", "Containers", "Doors", "Ingredients", "Lights", "Miscellaneous", "Flora", "Furniture",
	"Weapons: Any", "Ammo", "NPCs", "Creatures", "Keys", "Alchemy", "Food", "All: Combat Wearable", "All: Wearable", "Weapons: Ranged", "Weapons: Melee",
	"Weapons: NONE", "Actor Effects: Any", "Actor Effects: Range Target", "Actor Effects: Range Touch", "Actor Effects: Range Self"
};

// add 1 to code before indexing
static const char* TESPackage_DayStrings[] = {
	"Any", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Weekdays", "Weekends", "MWF", "TT"
};

// add 1
static const char* TESPackage_MonthString[] = {
	"Any", "January", "February", "March", "April", "May", "June", "July", "August", "September",
	"October", "November", "December", "Spring", "Summer", "Autumn", "Winter"
};

static const char* TESPackage_LocationStrings[] = {
	"Reference", "Cell", "Current", "Editor", "Object", "ObjectType"
};

static const char* TESPackage_TypeStrings[TESPackage::kPackageType_Max] = {
	"Find", "Follow", "Escort", "Eat", "Sleep", "Wander", "Travel", "Accompany", "UseItemAt", "Ambush",
	"FleeNotCombat", "Sandbox", "Patrol", "Guard", "Dialogue", "UseWeapon"
};

static const char* TESPackage_ProcedureStrings[TESPackage::kProcedure_Max] = {
	"TRAVEL", "ACTIVATE", "ACQUIRE", "WAIT", "DIALOGUE", "GREET", "GREET DEAD", "WANDER", "SLEEP", 
	"OBSERVE COMBAT", "EAT", "FOLLOW", "ESCORT", "COMBAT", "ALARM", "PURSUE", "FLEE", "DONE", "YELD", 
	"TRAVEL TARGET", "CREATE FOLLOW", "GET UP", "MOUNT HORSE", "DISMOUNT HORSE", "DO NOTHING", "UNKNOWN 019", "UNKNOWN 01A",
	"ACCOMPANY", "USE ITEM AT", "AIM", "NOTIFY", "SANDMAN", "WAIT AMBUSH", "SURFACE", "WAIT FOR SPELL", "CHOOSE CAST", 
	"FLEE NON COMBAT", "REMOVE WORN ITEMS", "SEARCH", "CLEAR MOUNT POSITION", "SUMMON CREATURE DEFEND", "AVOID AREA", 
	"UNEQUIP ARMOR", "PATROL", "USE WEAPON", "DIALOGUE ACTIVATE", "GUARD", "SANDBOX", "USE IDLE MARKER", "TAKE BACK ITEM", 
	"SITTING", "MOVEMENT BLOCKED", "CANIBAL FEED", 
};

const char* TESPackage::StringForPackageType(const ePackageType pkgType)
{
	return pkgType >= kPackageType_Min && pkgType < kPackageType_Max ? TESPackage_TypeStrings[pkgType] : "";
}

const char* TESPackage::StringForObjectCode(const eObjectType objCode)
{
	return objCode >= kObjectType_Min && objCode < kObjectType_Max ? TESPackage_ObjectTypeStrings[objCode] : "";
}

UInt8 TESPackage::ObjectCodeForString(const char* objString)
{
	for (UInt32 i = 0; i < kObjectType_Max; i++) {
		if (!_stricmp(objString, TESPackage_ObjectTypeStrings[i]))
			return i;
	}

	return kObjectType_Max;
}

static const char** s_procNames = reinterpret_cast<const char**>(0x011A3CC0);

const char* TESPackage::StringForProcedureCode(const eProcedure proc)
{
	return proc >= kProcedure_Min && proc < kProcedure_Max ? TESPackage_ProcedureStrings[proc] : "";
}

const char* TESPackage::StringForProcedureCode(eProcedure proc, bool bRemovePrefix)
{
	static size_t prefixLen = strlen("PROCEDURE_");
	const char* name;
	// special-case "AQUIRE" (sic) to fix typo in game executable
	if (proc == kProcedure_ACQUIRE)
		name = "PROCEDURE_ACQUIRE";
	else
		name = proc >= kProcedure_Min && proc < kProcedure_Max ? s_procNames[proc] : nullptr;

	if (name && bRemovePrefix) name += prefixLen;

	return name;
}

const char* TESPackage::PackageTime::DayForCode(UInt8 dayCode)
{
	dayCode += 1;
	return dayCode < sizeof TESPackage_DayStrings ? TESPackage_DayStrings[dayCode] : "";
}

BGSQuestObjective* TESQuest::GetObjective(UInt32 objectiveID) const
{
	auto iter = lVarOrObjectives.Head();
	do
	{
		const auto objective = (BGSQuestObjective*)iter->data;
		if (objective && IS_TYPE(objective, BGSQuestObjective) && (objective->objectiveId == objectiveID))
			return objective;
	} while (iter = iter->next);
	return NULL;
}

const char* TESPackage::PackageTime::MonthForCode(UInt8 monthCode)
{
	monthCode += 1;
	return monthCode < sizeof TESPackage_MonthString ? TESPackage_MonthString[monthCode] : "";
}

UInt8 TESPackage::PackageTime::CodeForDay(const char* dayStr)
{
	for (UInt8 i = 0; i < sizeof(TESPackage_DayStrings); i++)
		if (!_stricmp(dayStr, TESPackage_DayStrings[i])) return i-1;
	return kWeekday_Any;
}

UInt8 TESPackage::PackageTime::CodeForMonth(const char* monthStr)
{
	for (UInt8 i = 0; i < sizeof(TESPackage_MonthString); i++)
		if (!_stricmp(monthStr, TESPackage_MonthString[i]))	return i-1;
	return kMonth_Any;
}

const char* TESPackage::LocationData::StringForLocationCode(const ePackageLocation locCode)
{
	return locCode >= kPackLocation_Min && locCode < kPackLocation_Max ? TESPackage_LocationStrings[locCode] : "";
}

const char* TESPackage::LocationData::StringForLocationCodeAndData()
{
#define resultSize 256
	static char result[resultSize];
	if (locationType < kPackLocation_Max) {
		switch (locationType) { 
			case kPackLocation_NearReference:
			case kPackLocation_InCell:
			case kPackLocation_ObjectID:
				if (object.form)
					sprintf_s(result, resultSize, "%s \"%s\" [%08X] with a radius of %u", TESPackage_LocationStrings[locationType], object.form->GetTheName(), 
						object.form->refID, radius);
				else
					sprintf_s(result, resultSize, "%s \"\" [%08X] with a radius of %u", TESPackage_LocationStrings[locationType], 0, radius);
				break;
			case kPackLocation_ObjectType:
				sprintf_s(result, resultSize, "%s \"%s\" [%04X] with a radius of %u", TESPackage_LocationStrings[locationType], StringForObjectCode(object.objectCode),
					object.objectCode, radius);
				break;
			default:
				sprintf_s(result, resultSize, "%s with a radius of %u", TESPackage_LocationStrings[locationType], radius);
				break;
		}
		return result;
	}
	return "";
}

const char* TESPackage::TargetData::StringForTargetCodeAndData(void)
{
#define resultSize 256
	static char result[resultSize];
	if (targetType < kTargetType_Max) {
		switch (targetType) { 
			case kTargetType_Refr:
				if (target.refr)
					sprintf_s(result, resultSize, "%s \"%s\" [%08X] with a distance of %u", StringForTargetCode(targetType), target.refr->GetTheName(),
						target.refr->refID, count);
				else
					sprintf_s(result, resultSize, "%s [%08X] with a distance of %u", StringForTargetCode(targetType), 0, count);
				break;
			case kTargetType_BaseObject:
				if (target.form)
					sprintf_s(result, resultSize, "%s \"%s\" [%08X] with a count of %u", StringForTargetCode(targetType), target.form->GetTheName(), target.form->refID, count);
				else
					sprintf_s(result, resultSize, "%s [%08X] with a count of %u", StringForTargetCode(targetType), 0, count);
				break;
			case kTargetType_TypeCode:
				sprintf_s(result, resultSize, "%s \"%s\" [%04X] with a radius of %u", StringForTargetCode(targetType), StringForObjectCode(target.objectCode),
						target.objectCode, count);
				break;
			default:
				sprintf_s(result, resultSize, "%s with a radius of %u", StringForTargetCode(targetType), count);
				break;
		}
		return result;
	}
	return "";
}

UInt8 TESPackage::LocationData::LocationCodeForString(const char* locStr)
{
	for (UInt32 i = 0; i < kPackLocation_Max; i++)
		if (!_stricmp(locStr, TESPackage_LocationStrings[i]))
			return i;
	return kPackLocation_Max;
}

const char* TESFaction::GetNthRankName(const UInt32 whichRank, const bool bFemale)
{
	Rank* rank = ranks.GetNthItem(whichRank);
	return rank ? bFemale ? rank->femaleName.CStr() : rank->name.CStr() : nullptr;
}

void TESFaction::SetNthRankName(const char* newName, const UInt32 whichRank, const bool bFemale)
{
	Rank* rank = ranks.GetNthItem(whichRank);
	if (rank) (bFemale ? rank->femaleName : rank->name).Set(newName);
}

UInt32 EffectItemList::CountItems() const
{
	return list.Count();
}

EffectItem* EffectItemList::ItemAt(const UInt32 whichItem)
{
	return list.GetNthItem(whichItem);
}

const char* EffectItemList::GetNthEIName(const UInt32 whichEffect) const
{
	const EffectItem* effItem = list.GetNthItem(whichEffect);
	return effItem->setting ? GetFullName(effItem->setting) : "<no name>";
}

BGSDefaultObjectManager* BGSDefaultObjectManager::GetSingleton()
{
	return *g_defaultObjectManager;
}

Script* EffectSetting::SetScript(Script* newScript)
{
	Script* oldScript = nullptr;
	if (1 == archtype )
	{
		oldScript = DYNAMIC_CAST(associatedItem, TESForm, Script);
		associatedItem = newScript;
	}
	return oldScript;
};

Script* EffectSetting::	RemoveScript()
{
	return SetScript(nullptr);
};

SInt32 TESContainer::GetCountForForm(TESForm *form)
{
	SInt32 result = 0;
	for (const auto iter : formCountList)
		if (iter->form == form) result += iter->count;
	return result;
}