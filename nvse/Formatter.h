#include <format>

#include "Setting.h"
#include "GameEffects.h"
#include "TESForm.h"
#include "TESObjectREFR.h"
#include "GameTasks.h"
#include "Tile.h"
#include "Menu.h"

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

std::ostream &operator<<(std::ostream &os, const Tile& obj) { os << "Path: " << obj.GetFullPath(); return os; }
std::ostream &operator<<(std::ostream &os, const Menu& obj) 
{
	os << std::format("MenuMode: {:d}, visible: {:b}, visibilityState: 0x{:X}", obj.id, obj.IsVisible(), obj.visibilityState);
	return os; 
}
std::ostream &operator<<(std::ostream &os, const StartMenu::Option& obj) { os << obj.displayString; return os; }

std::ostream &operator<<(std::ostream &os, const Setting& obj) 
{ 
	os << obj.name << ": ";
	if (obj.GetType() == Setting::kSetting_String)
		os << obj.GetAsString();
	else 
		os << std::to_string(obj.GetAsFloat());
	return os;
}

std::ostream& operator<<(std::ostream& os, const TESForm& obj) { os << std::format("{:08X} ({})", obj.refID, obj.GetName()); return os; }
std::ostream& operator<<(std::ostream& os, const TESObjectREFR& obj) { os << (const TESForm&)obj << ", Baseform " << obj.TryGetREFRParent(); return os; }

std::ostream& operator<<(std::ostream& os, const ActorMover& obj) { if (obj.actor) os << (const TESObjectREFR&)obj.actor; return os; }
std::ostream& operator<<(std::ostream& os, const QueuedReference& obj) { if (obj.refr) os << (const TESObjectREFR&)obj.refr; return os; }

std::ostream& operator<<(std::ostream& os, const NavMesh& obj) { os << (const TESForm&)obj << ", Cell " << (const TESForm&)obj.parentCell; return os; }

std::ostream& operator<<(std::ostream& os, const BaseProcess& obj)
{
	for (const auto iter : *TESForm::GetAll())
		if ((iter->typeID == kFormType_Creature || iter->typeID == kFormType_Character) && static_cast<Character*>(iter)->baseProcess == &obj)
			os << (const TESObjectREFR&)iter;
	return os;
}

std::ostream& operator<<(std::ostream& os, const BSAnimGroupSequence& obj)
{
	os << std::format("{}, {}, AnimGroup {:04X}", obj.sequenceName, obj.accumRootName, obj.animGroup->groupID);
	return os;
}

std::ostream& operator<<(std::ostream& os, const AnimSequenceSingle& obj)
{
	os << (const BSAnimGroupSequence&)obj.anim;
	return os;
}

std::ostream& operator<<(std::ostream& os, const AnimSequenceMultiple& obj)
{
	for (const auto iter : *obj.anims)
		os << (const BSAnimGroupSequence&)iter << "; ";
	return os;
}

std::ostream& operator<<(std::ostream& os, const NiObjectNET& obj) { os << std::format(R"(Name: "{}")", obj.m_pcName); return os; }

std::ostream& operator<<(std::ostream& os, const NiNode& obj)
{
	os << (const NiObjectNET&)obj;
	if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj))
		os << ", " << (const TESObjectREFR&)ref;
	return os; 
}

std::ostream& operator<<(std::ostream& os, const BSFile& obj) { os << std::format("Path: {}", obj.m_path); return os; }
std::ostream& operator<<(std::ostream& os, const TESModel& obj) { os << std::format("Path: {}", obj.nifPath.CStr()); return os; }

std::ostream& operator<<(std::ostream& os, const QueuedModel& obj) 
{
	if (obj.model)
		os << "Path: " << obj.model->path << " ";
	if (obj.tesModel)
		os << (TESModel&)obj.tesModel;
	return os;
}

std::ostream& operator<<(std::ostream& os, const TESTexture& obj) { os << std::format("Path: {}", obj.ddsPath.CStr()); return os; }
std::ostream& operator<<(std::ostream& os, const QueuedTexture& obj) { os << std::format("Path: {}", obj.name); return os; }
std::ostream& operator<<(std::ostream& os, const NiStream& obj) { os << std::format("Path: {}", obj.path); return os; }

std::ostream& operator<<(std::ostream& os, const ActiveEffect& obj)
{
	if (obj.enchantObject)
		os << "Enchanted Object: " << (const TESForm&)obj.enchantObject;
	return os; 
}

std::ostream& operator<<(std::ostream& os, const ScriptEffect& obj)
{
	os << (const ActiveEffect&)obj << " ";
	if (obj.script)
		os << "Script: " << (const TESForm&)obj.script;
	return os; 
}

std::ostream& operator<<(std::ostream& os, const QueuedKF& obj)
{
	if (obj.kf)
		os << "Path: " << obj.kf->path;
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
FORMAT_CLASS(BSFile);
FORMAT_CLASS(TESModel);
FORMAT_CLASS(QueuedModel);
FORMAT_CLASS(TESTexture);
FORMAT_CLASS(QueuedTexture);
FORMAT_CLASS(NiStream);

FORMAT_CLASS(ActiveEffect);
FORMAT_CLASS(ScriptEffect);
FORMAT_CLASS(QueuedKF);

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