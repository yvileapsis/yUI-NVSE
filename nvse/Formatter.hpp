#include <format>
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

inline std::ostream &operator<<(std::ostream &os, const Tile& obj) 
{
	os << std::format(R"(Path: "{}")", obj.GetFullPath()); 
	return os;
}

inline std::ostream &operator<<(std::ostream &os, const Menu& obj) 
{
	os << std::format("MenuMode: {:d}", (UInt32) obj.eID);
	os << std::format(", visible: {:b}", obj.IsVisible());
	os << std::format(", visibilityState : 0x{:X}", (UInt32) obj.eVisibilityState);
	std::stringstream ss;
	ss << ", TileMenu: " << (const Tile&) *obj.pkRootTile;
	os << ss.str();
	return os; 
}
inline std::ostream &operator<<(std::ostream &os, const StartMenu::Option& obj) { os << obj.pcDisplayString; return os; }

inline std::ostream &operator<<(std::ostream &os, const Setting& obj) 
{ 
	os << obj.pKey << ": ";
	if (obj.GetType() == Setting::kSetting_String)
		os << obj.GetAsString();
	else 
		os << std::to_string(obj.GetAsFloat());
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const TESForm& obj) 
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

inline std::ostream& operator<<(std::ostream& os, const TESObjectREFR& obj)
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

inline std::ostream& operator<<(std::ostream& os, const ActorMover& obj)
{
	if (obj.pkActor) os << (const TESObjectREFR&)*obj.pkActor;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const QueuedReference& obj)
{ 
	if (obj.pRefr) os << (const TESObjectREFR&)*obj.pRefr;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const NavMesh& obj)
{
	os << (const TESForm&)obj;
	if (obj.pParentCell) {
		std::stringstream ss;
		ss << ", Cell " << (const TESForm&)*obj.pParentCell;
		os << ss.str();
	}
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const BaseProcess& obj)
{
	for (const auto iter : *TESForm::GetAll())
		if ((iter->eTypeID == TESForm::kType_Creature && static_cast<Creature*>(iter)->pkBaseProcess == &obj)
			|| (iter->eTypeID == TESForm::kType_Character && static_cast<Character*>(iter)->pkBaseProcess == &obj))
			os << (const TESObjectREFR&)*iter;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const NiControllerSequence& obj)
{
	os << SanitizeString(std::format("{}, {}", reinterpret_cast<const char*>(&obj.m_kName), reinterpret_cast<const char*>(&obj.m_kAccumRootName)));
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const BSAnimGroupSequence& obj)
{
	os << (const NiControllerSequence&) obj 
		<< SanitizeString(std::format("AnimGroup {:04X}", obj.spAnimGroup->groupID));
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const AnimSequenceSingle& obj)
{
	os << (const BSAnimGroupSequence&)*obj.pkAnim;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const AnimSequenceMultiple& obj)
{
	for (const auto iter : *obj.pkAnims)
		os << (const BSAnimGroupSequence&)iter << "; ";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const NiObjectNET& obj) 
{
	const auto name = obj.m_kName.GetStd();
	if (!name.empty()) os << SanitizeString(std::format(R"("{}")", name)); 
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const NiNode& obj)
{
	os << (const NiObjectNET&)obj;
	if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj)) 
	{
		const char* separator = !obj.m_kName.GetStd().empty() ? ", " : " ";
		os << separator << (const TESObjectREFR&)*ref;
	}
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const NiExtraData& obj)
{
	const auto name = obj.m_kName.GetStd();
	if (!name.empty()) os << SanitizeString(std::format(R"("{}")", name));
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const BSFile& obj) 
{ 
	os << SanitizeString(std::format("Path: {}", obj.cFileName));
	return os;
}
inline std::ostream& operator<<(std::ostream& os, const TESModel& obj)
{
	os << SanitizeString(std::format("Path: {}", obj.kModel.StdStr()));
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const QueuedModel& obj) 
{
	if (obj.spModel)
		os << SanitizeString("Path: " + std::string(obj.spModel->pcPath) + " ");
	if (obj.pTESModel)
		os << (TESModel&)*obj.pTESModel;
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const TESTexture& obj) 
{
	os << SanitizeString(std::format(R"(Path: "{}")", obj.kTexturePath.StdStr()));
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const QueuedTexture& obj) 
{
	os << SanitizeString(std::format(R"(Path: "{}")", obj.pFileName));
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const NiStream& obj) 
{
	os << SanitizeString(std::format(R"(Path: "{}")", obj.m_acFileName));
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const ActiveEffect& obj)
{
	if (obj.pkEnchantObject)
		os << "Enchanted Object: " << (const TESForm&)*obj.pkEnchantObject;
	return os; 
}

static std::set<Script*> crashedScripts;

inline std::ostream& operator<<(std::ostream& os, const Script& obj)
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

inline std::ostream& operator<<(std::ostream& os, const ScriptEffect& obj)
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

inline std::ostream& operator<<(std::ostream& os, const QueuedKF& obj)
{
	if (obj.pkKFModel) os << SanitizeString(std::format(R"(Path: "{}")", obj.pkKFModel->pcPath));
	return os; 
}

inline std::ostream& operator<<(std::ostream& os, const bhkRefObject& obj)
{
	NiAVObject* object = obj.GetAVObject();
	if (object) {
		os << object;
	}
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const NiCollisionObject& obj)
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

inline std::ostream& operator<<(std::ostream& os, const NiTimeController& obj)
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

inline std::ostream& operator<<(std::ostream& os, const bhkCharacterController& obj)
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