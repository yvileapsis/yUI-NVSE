#include <format>

#include "bhkRefObject.hpp"
#include "bhkCharacterController.hpp"

#include "NiExtraData.hpp"
#include "NiStream.hpp"
#include "NiCollisionObject.hpp"
#include "NiTimeController.hpp"

#include "Setting.hpp"

#include "TESForm.hpp"
#include "TESObjectREFR.hpp"
#include "NavMesh.hpp"

#include "Character.hpp"
#include "Creature.hpp"

#include "BaseProcess.hpp"

#include "TESAnimGroup.hpp"
#include "NiControllerSequence.hpp"
#include "BSAnimGroupSequence.hpp"
#include "AnimSequence.hpp"

#include "ActorMover.hpp"
#include "QueuedReference.hpp"
#include "QueuedModel.hpp"
#include "QueuedTexture.hpp"
#include "QueuedKF.hpp"

#include "ActiveEffect.hpp"
#include "ScriptEffect.hpp"

#include "TESScript.hpp"

#include "Tile.hpp"
#include "Menu.hpp"
#include "StartMenu.hpp"

#include <set>

#define FORMAT_CLASS(CLASS)											\
template <>															\
struct std::formatter<CLASS> {										\
	constexpr auto parse(format_parse_context &ctx) {				\
		return ctx.begin();											\
	}																\
	template <typename FormatContext>								\
	auto format(const CLASS& custom_obj, FormatContext &ctx) {		\
		std::stringstream ss;										\
		ss << custom_obj;											\
		return format_to(ctx.out(), "{}", ss.str());				\
	}																\
};																	\

#define FORMAT_CLASS_AND_DERIVED(CLASS)								\
template <std::derived_from<CLASS> Derived>							\
struct std::formatter<Derived> {									\
	constexpr auto parse(format_parse_context &ctx) {				\
		return ctx.begin();											\
	}																\
	template <typename FormatContext>								\
	auto format(const Derived& custom_obj, FormatContext &ctx) {	\
		std::stringstream ss;										\
		ss << custom_obj;											\
		return format_to(ctx.out(), "{}", ss.str());				\
	}																\
};																	\

std::ostream &operator<<(std::ostream &os, const Tile& obj) 
{
	os << std::format(R"(Path: "{}")", obj.GetFullPath()); 
	return os;
}

std::ostream &operator<<(std::ostream &os, const Menu& obj) 
{
	os << std::format("MenuMode: {:d}", (UInt32) obj.eID);
	os << std::format(", visible: {:b}", obj.IsVisible());
	os << std::format(", visibilityState : 0x{:X}", (UInt32) obj.eVisibilityState);
	std::stringstream ss;
	ss << ", TileMenu: " << (const Tile&) *obj.pkRootTile;
	os << ss.str();
	return os; 
}
std::ostream &operator<<(std::ostream &os, const StartMenu::Option& obj) { os << obj.pcDisplayString; return os; }

std::ostream &operator<<(std::ostream &os, const Setting& obj) 
{ 
	os << obj.pKey << ": ";
	if (obj.GetType() == Setting::kSetting_String)
		os << obj.GetAsString();
	else 
		os << std::to_string(obj.GetAsFloat());
	return os;
}

std::ostream& operator<<(std::ostream& os, const TESForm& obj) 
{ 
	UInt32 refID = obj.uiFormID;

	std::string modName;
	const char* refName = obj.GetEditorID();

	if (obj.ucModIndex == 0xFF)
	{
		if (!refName)
			refName = "Temp Form";
		else
			refName = obj.eTypeID == TESForm::kType_Script ? "Script Runner" : "Temp Form";
	}
	os << std::format("{:08X} ({})", refID, refName);

	if (obj.ucModIndex != 0xFF) {
		TESFile* sourceMod = obj.kMods.m_item;
		TESFile* lastMod = obj.kMods.TailItem();

		if (sourceMod == lastMod)
			modName = std::format(R"(Plugin: "{}")", sourceMod->m_Filename);
		else
			modName = std::format(R"(Plugin: "{}", Last modified by: "{}")", sourceMod->m_Filename, lastMod->m_Filename);

		os << ", " << modName;
	}

	return os; 
}

std::ostream& operator<<(std::ostream& os, const TESObjectREFR& obj)
{
	os << (const TESForm&) obj;
	TESForm* baseForm = obj.TryGetREFRParent();
	if (baseForm) {
//		std::stringstream ss;
		os << ", BaseForm: " << (const TESForm&) *baseForm;
//		os << ss.str();
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const ActorMover& obj)
{
	if (obj.pkActor) os << (const TESObjectREFR&)*obj.pkActor;
	return os;
}

std::ostream& operator<<(std::ostream& os, const QueuedReference& obj)
{ 
	if (obj.pRefr) os << (const TESObjectREFR&)*obj.pRefr;
	return os;
}

std::ostream& operator<<(std::ostream& os, const NavMesh& obj)
{
	os << (const TESForm&)obj;
	if (obj.pParentCell) {
		std::stringstream ss;
		ss << ", Cell " << (const TESForm&)*obj.pParentCell;
		os << ss.str();
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const BaseProcess& obj)
{
	for (const auto iter : *TESForm::GetAll())
		if ((iter->eTypeID == TESForm::kType_Creature && static_cast<Creature*>(iter)->pkBaseProcess == &obj)
			|| (iter->eTypeID == TESForm::kType_Character && static_cast<Character*>(iter)->pkBaseProcess == &obj))
			os << (const TESObjectREFR&)*iter;
	return os;
}

std::ostream& operator<<(std::ostream& os, const NiControllerSequence& obj)
{
	os << SanitizeString(std::format("{}, {}", reinterpret_cast<const char*>(&obj.m_kName), reinterpret_cast<const char*>(&obj.m_kAccumRootName)));
	return os;
}

std::ostream& operator<<(std::ostream& os, const BSAnimGroupSequence& obj)
{
	os << (const NiControllerSequence&) obj 
		<< SanitizeString(std::format("AnimGroup {:04X}", obj.spAnimGroup->groupID));
	return os;
}

std::ostream& operator<<(std::ostream& os, const AnimSequenceSingle& obj)
{
	os << (const BSAnimGroupSequence&)*obj.pkAnim;
	return os;
}

std::ostream& operator<<(std::ostream& os, const AnimSequenceMultiple& obj)
{
	for (const auto iter : *obj.pkAnims)
		os << (const BSAnimGroupSequence&)iter << "; ";
	return os;
}

std::ostream& operator<<(std::ostream& os, const NiObjectNET& obj) 
{
	const auto name = obj.m_kName.GetStd();
	if (!name.empty()) os << SanitizeString(std::format(R"("{}")", name)); 
	return os; 
}

std::ostream& operator<<(std::ostream& os, const NiNode& obj)
{
	os << (const NiObjectNET&)obj;
	if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj)) 
	{
		const char* separator = !obj.m_kName.GetStd().empty() ? ", " : " ";
		os << separator << (const TESObjectREFR&)*ref;
	}
	return os; 
}

std::ostream& operator<<(std::ostream& os, const NiExtraData& obj)
{
	const auto name = obj.m_kName.GetStd();
	if (!name.empty()) os << SanitizeString(std::format(R"("{}")", name));
	return os; 
}

std::ostream& operator<<(std::ostream& os, const BSFile& obj) 
{ 
	os << SanitizeString(std::format("Path: {}", obj.cFileName));
	return os;
}
std::ostream& operator<<(std::ostream& os, const TESModel& obj)
{
	os << SanitizeString(std::format("Path: {}", obj.kModel.StdStr()));
	return os; 
}

std::ostream& operator<<(std::ostream& os, const QueuedModel& obj) 
{
	if (obj.spModel)
		os << SanitizeString("Path: " + std::string(obj.spModel->pcPath) + " ");
	if (obj.pTESModel)
		os << (TESModel&)*obj.pTESModel;
	return os;
}

std::ostream& operator<<(std::ostream& os, const TESTexture& obj) 
{
	os << SanitizeString(std::format(R"(Path: "{}")", obj.kTexturePath.StdStr()));
	return os;
}

std::ostream& operator<<(std::ostream& os, const QueuedTexture& obj) 
{
	os << SanitizeString(std::format(R"(Path: "{}")", obj.pFileName));
	return os; 
}

std::ostream& operator<<(std::ostream& os, const NiStream& obj) 
{
	os << SanitizeString(std::format(R"(Path: "{}")", obj.m_acFileName));
	return os; 
}

std::ostream& operator<<(std::ostream& os, const ActiveEffect& obj)
{
	if (obj.pkEnchantObject)
		os << "Enchanted Object: " << (const TESForm&)*obj.pkEnchantObject;
	return os; 
}

std::set<Script*> crashedScripts;

std::ostream& operator<<(std::ostream& os, const Script& obj)
{
	os << (const TESForm&)obj;
	if (obj.m_val && std::string(obj.GetEditorID()).empty())
		if (!crashedScripts.contains(const_cast<Script*>(&obj)))
		{
			crashedScripts.emplace(const_cast<Script*>(&obj));
			os << ", Source: " << DecompileScriptToFolder("UnknownScript" + std::to_string(crashedScripts.size()), const_cast<Script*>(&obj), "gek", "Crash Logger");
		}
	return os;
}

std::ostream& operator<<(std::ostream& os, const ScriptEffect& obj)
{
	os << (const ActiveEffect&)obj << " ";
	if (obj.pkScript)
	{
		std::stringstream ss;
		ss << "Script: " << (const Script&)*obj.pkScript;
		os << SanitizeString(ss.str());
	}
	return os; 
}

std::ostream& operator<<(std::ostream& os, const QueuedKF& obj)
{
	if (obj.pkKFModel) os << SanitizeString(std::format(R"(Path: "{}")", obj.pkKFModel->pcPath));
	return os; 
}

std::ostream& operator<<(std::ostream& os, const bhkRefObject& obj)
{
	NiAVObject* object = obj.GetAVObject();
	if (object) {
		os << object;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const NiCollisionObject& obj)
{
	NiAVObject* object = obj.m_pkSceneObject;
	if (object) {
		if (object->IsNiNode())
			os << (const NiNode&)*object;
		else
			os << object;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const NiTimeController& obj)
{
	NiObjectNET* object = obj.m_pkTarget;
	if (object) {
		if (object->IsNiNode())
			os << "Target: " << (const NiNode&)*object;
		else
			os << "Target: " << object;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const bhkCharacterController& obj)
{
	NiAVObject* object = obj.GetNiObject();
	if (object) {
		if (object->IsNiNode())
			os << "Target: " << (const NiNode&)*object;
		else
			os << "Target: " << object;
	}
	return os;
}


FORMAT_CLASS(Tile)
FORMAT_CLASS(Menu)
FORMAT_CLASS(StartMenu::Option)
FORMAT_CLASS(Setting)

FORMAT_CLASS(TESForm)
FORMAT_CLASS(TESObjectREFR)

FORMAT_CLASS(ActorMover)
FORMAT_CLASS(QueuedReference)

FORMAT_CLASS(NavMesh)
FORMAT_CLASS(BaseProcess);

FORMAT_CLASS(BSAnimGroupSequence);
FORMAT_CLASS(AnimSequenceSingle);
FORMAT_CLASS(AnimSequenceMultiple);
FORMAT_CLASS(NiObjectNET);

FORMAT_CLASS(NiNode);
FORMAT_CLASS(NiExtraData);
FORMAT_CLASS(BSFile);
FORMAT_CLASS(TESModel);
FORMAT_CLASS(QueuedModel);
FORMAT_CLASS(TESTexture);
FORMAT_CLASS(QueuedTexture);
FORMAT_CLASS(NiStream);

FORMAT_CLASS(ActiveEffect);
FORMAT_CLASS(Script);
FORMAT_CLASS(ScriptEffect);
FORMAT_CLASS(QueuedKF);
FORMAT_CLASS(bhkRefObject);
FORMAT_CLASS(NiCollisionObject);
FORMAT_CLASS(bhkCharacterController);
FORMAT_CLASS(NiTimeController);

#if 0

void DumpAnimGroups()
{
	for (UInt32 i = 0; i < TESAnimGroup::kAnimGroup_Max; i++) {
		Log() << (std::format("%d,%s", i, s_animGroupInfos[i].name));
		//if (!_stricmp(s_animGroupInfos[i].name, "JumpLandRight"))
		//	break;
	}
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