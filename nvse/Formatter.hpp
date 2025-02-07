#include <format>
#include <set>
#include <bhkUtilFunctions.hpp>

// If class is described by a single line, no need to name the variable
// If there is a member class, if it's one-line, leave it as one-line, if there are several, prepend the name and add offset

inline auto Offset(std::vector<std::string> vector)
{
	for (auto i : vector) i.insert(0, "    ");
	return vector;
}

template<class Member> auto LogMember(const std::string& name, Member& member)
{
	std::vector<std::string> vec = LogClass(member);
	if (vec.size() == 1) return std::vector { name + " " + vec[0] };
	vec = Offset(vec);
	vec.insert(vec.begin(), name);
	return vec;
}

template<class Member> std::string LogClassLineByLine(Member& member)
{
	std::string output;
	std::vector<std::string> vec = LogClass(member);
	for (const auto& i : vec)
		output += i + '\n';
	return output;
}

inline std::vector<std::string> LogClass(const Tile& obj) { return std::vector { '"' + obj.GetFullPath() + '"' }; }

inline std::vector<std::string> LogClass(const Menu& obj)
{
	std::vector<std::string> vec;
	char textBuffer[256];
	sprintf_s(textBuffer, "MenuMode: %d", obj.eID);
	vec.push_back(textBuffer);

	sprintf_s(textBuffer, "Visible: %d", obj.IsVisible());
	vec.push_back(textBuffer);

	sprintf_s(textBuffer, "VisibilityState: 0x%X", obj.eVisibilityState);
	vec.push_back(textBuffer);

	vec.append_range(LogMember("TileMenu:", static_cast<Tile&>(*obj.pkRootTile)));
	return vec;
}

inline std::vector<std::string> LogClass(const StartMenu::Option& obj) { return std::vector { '"' + std::string(obj.pcDisplayString) + '"' }; }

inline std::vector<std::string> LogClass(const Setting& obj)
{
	return std::vector {
		'"' + std::string(obj.pKey) + '"' + ": "
		+ (obj.GetType() == Setting::kSetting_String
		? obj.GetAsString()
		: std::to_string(obj.GetAsFloat()))
	};
}

inline auto LogClass(const TESForm& obj)
{
	std::vector<std::string> vec;

	UInt32 refID = obj.uiFormID;

	char buffer[256];

	const char* refName = obj.GetEditorID();

	if (!refName || !refName[0])
		sprintf_s(buffer, "\nID: %08X %s", refID, obj.IsTemporary() ? "Temporary" : "");
	else
		sprintf_s(buffer, "\nID: %08X (%s) %s", refID, refName, obj.IsTemporary() ? "Temporary" : "");
	vec.push_back(buffer);

	if (obj.ucModIndex != 0xFF) {
		TESFile* sourceMod = obj.kMods.m_item;
		TESFile* lastMod = obj.kMods.TailItem();

		sprintf_s(buffer, "Plugin: \"%s\"", sourceMod->m_Filename);
		vec.push_back(buffer);

		if (sourceMod != lastMod) {
			sprintf_s(buffer, "Last modified by: \"%s\"", lastMod->m_Filename);
			vec.push_back(buffer);
		}
	}

	return vec;
}

inline auto LogClass(const TESObjectREFR& obj)
{
	auto vec = LogClass(static_cast<const TESForm&>(obj));
	const auto baseForm = obj.TryGetREFRParent();
	if (baseForm) {
		char textBuffer[128];
		sprintf_s(textBuffer, "\nBaseForm: %s", TESForm::TypeNames[baseForm->eTypeID]);
		vec.append_range(LogMember(textBuffer, *baseForm));
	}
	return vec;
}

inline std::vector<std::string> LogClass(const ActorMover& obj) { if (obj.pkActor) return LogClass(*obj.pkActor); return {}; }
inline std::vector<std::string> LogClass(const QueuedReference& obj) { if (obj.pRefr) return LogClass(*obj.pRefr); return {}; }

inline auto LogClass(const NavMesh& obj)
{
	auto vec = LogClass(static_cast<const TESForm&>(obj));
	if (obj.pParentCell)
		vec.append_range(LogMember("Cell:", static_cast<const TESForm&>(*obj.pParentCell)));
	return vec;
}

inline std::vector<std::string>  LogClass(const BaseProcess& obj)
{
	for (const auto iter : *TESForm::GetAll())
		if ((iter->eTypeID == TESForm::kType_Creature || iter->eTypeID == TESForm::kType_Character)
			&& reinterpret_cast<Actor*>(iter)->pkBaseProcess == &obj)
			return LogClass(reinterpret_cast<const TESObjectREFR&>(*iter));
	return {};
}

inline auto LogClass(const NiControllerSequence& obj)
{
	return std::vector {
		SanitizeString(std::string("Name: ") + std::string(obj.m_kName.m_kHandle)),
		SanitizeString(std::string("RootName: ") + std::string(obj.m_kAccumRootName.m_kHandle))
	};
}

inline auto LogClass(const BSAnimGroupSequence& obj)
{
	auto vec = LogClass(static_cast<const NiControllerSequence&>(obj));
	char textBuffer[32];
	sprintf_s(textBuffer, "AnimGroup: %04X", obj.spAnimGroup->groupID);
	vec.push_back(textBuffer);
	return vec;
}

inline std::vector<std::string> LogClass(const AnimSequenceSingle& obj) { if (obj.pkAnim) return LogClass(*obj.pkAnim); return {}; }

inline std::vector<std::string> LogClass(const AnimSequenceMultiple& obj)
{
	std::vector<std::string> vec;
	UInt32 i = 0;
	char textBuffer[256];
	for (const auto iter : *obj.pkAnims)
	{
		i++;
		sprintf_s(textBuffer, "AnimSequence%d:", i);
		vec.append_range(LogMember(textBuffer, *iter));
	}
	return vec;
}

inline std::vector<std::string> LogClass(const NiObjectNET& obj)
{
	const auto name = obj.m_kName.GetStd();
	if (!name.empty())
		return std::vector { '"' + SanitizeString(name.c_str()) + '"' };
	return {};
}

inline std::vector<std::string> LogClass(const NiNode& obj)
{
	std::vector<std::string> vec;
	if (const auto name = obj.m_kName.GetStd(); !name.empty())
		vec = LogMember("Name:", static_cast<const NiObjectNET&>(obj));
	if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj)) 
		vec.append_range(LogMember("Reference:", *ref));
	return vec;
}

inline std::vector<std::string> LogClass(const NiExtraData& obj)
{
	if (const auto name = obj.m_kName.GetStd(); !name.empty())
		return std::vector { '"' + SanitizeString(name.c_str()) + '"' };
	return {};
}

inline std::vector<std::string> LogClass(const BSFile& obj) { return std::vector { '"' + SanitizeString(obj.cFileName) + '"' }; }
inline std::vector<std::string> LogClass(const TESModel& obj) { return std::vector { '"' + SanitizeString(obj.kModel.StdStr()) + '"' }; }

inline std::vector<std::string> LogClass(const QueuedModel& obj)
{
	std::vector<std::string> vec;
	if (obj.spModel)
		vec.push_back(std::string("Path: ") + '"' + SanitizeString(obj.spModel->pcPath) + '"');
	if (obj.pTESModel)
		vec.append_range(LogMember("Model:", *obj.pTESModel));
	return vec;
}

inline std::vector<std::string> LogClass(const TESTexture& obj) { return std::vector { '"' + SanitizeString(obj.kTexturePath.StdStr()) + '"' }; }
inline std::vector<std::string> LogClass(const QueuedTexture& obj) { return std::vector { '"' + SanitizeString(obj.pFileName) + '"' }; }
inline std::vector<std::string> LogClass(const NiStream& obj) { return std::vector { '"' + SanitizeString(obj.m_acFileName) + '"' }; }
inline std::vector<std::string> LogClass(const ActiveEffect& obj) { if (obj.pkEnchantObject) return LogClass(*obj.pkEnchantObject); return {}; }

inline std::vector<std::string> LogClass(const Script& obj)
{
	auto vec = LogClass(static_cast<const TESForm&>(obj));
	if (obj.m_val && std::string(obj.GetEditorID()).empty())
	{
		char textBuffer[256];
		sprintf_s(textBuffer, "UnknownScript 0x%08X", obj.uiFormID);
		const auto str = DecompileScriptToFolder(textBuffer, const_cast<Script*>(&obj), "gek", "Crash Logger");
		vec.push_back(str);
	}
	return vec;
}

inline std::vector<std::string> LogClass(const ScriptEffect& obj)
{
	auto vec = LogClass(static_cast<const ActiveEffect&>(obj));
	if (obj.pkScript)
		vec.append_range(LogMember("Script:", *obj.pkScript));
	return vec;
}

inline std::vector<std::string> LogClass(const QueuedKF& obj) { if (obj.pkKFModel) return std::vector { '"' + SanitizeString(obj.pkKFModel->pcPath) + '"' }; return {}; }
inline std::vector<std::string> LogClass(const bhkRefObject& obj) { if (const auto object = obj.GetAVObject()) return LogClass(*object); return {}; }

inline std::vector<std::string> LogClass(const NiCollisionObject& obj)
{
	if (const auto object = obj.m_pkSceneObject) {
		if (object->IsNiNode())
			return LogClass(reinterpret_cast<const NiNode&>(*object));
		return LogClass(*object);
	}
	return {};
}

inline std::vector<std::string> LogClass(const NiTimeController& obj)
{
	if (const auto object = obj.m_pkTarget) {
		if (object->IsNiNode())
			return LogMember("Target:", reinterpret_cast<const NiNode&>(*object));
		return LogMember("Target:", *object);
	}
	return {};
}

inline std::vector<std::string> LogClass(const bhkCharacterController& obj)
{
	if (const auto object = obj.GetNiObject()) {
		if (object->IsNiNode())
			return LogMember("Target:", reinterpret_cast<const NiNode&>(*object));
		return LogMember("Target:", *object);
	}
	return {};
}

inline std::vector<std::string> LogClass(const hkpWorldObject& obj)
{
	std::vector<std::string> vec;
	const char* name = obj.GetName();

	if (name && name[0]) {
		char textBuffer[256];
		sprintf_s(textBuffer, "Name: %s", name);
		vec.push_back(textBuffer);
	}

	bhkNiCollisionObject* object = bhkUtilFunctions::GetbhkNiCollisionObject(&obj);
	if (object)
		vec.append_range(LogMember("Collision Object:", reinterpret_cast<const NiCollisionObject&>(*object)));

	return vec;
}

inline std::vector<std::string> LogClass(const IMemoryHeap& obj)
{
	HeapStats stats;
	std::string name = obj.GetName();
	obj.GetHeapStats(&stats, true);
	UInt32 total = stats.uiMemHeapSize;
	UInt32 used = stats.uiMemUsedInBlocks;
	char cMemBuffer[128];
	char textBuffer[256];
	GetMemoryUsageString(used, total, cMemBuffer, sizeof(cMemBuffer));
	sprintf_s(textBuffer, "%s: %s", name.c_str(), cMemBuffer);

	return std::vector{ std::string(textBuffer) };
}
