#include <bhkUtilFunctions.hpp>
#include <ModelLoader.hpp>
#include <TESWorldSpace.hpp>
#include <NiControllerManager.hpp>
#include <TESObjectACTI.hpp>

#include <RTTI.hpp>

#define DYNAMIC_CAST(obj, from, to) ( ## to *) Fallout_DynamicCast((void*)(obj), 0, RTTI_ ## from, RTTI_ ## to, 0)

// If class is described by a single line, no need to name the variable
// If there is a member class, if it's one-line, leave it as one-line, if there are several, prepend the name and add offset

const char* pNiFlagNames[] = {
	"APP_CULLED",
	"SELECTIVE_UPDATE",
	"SELECTIVE_UPDATE_TRANSFORMS",
	"SELECTIVE_UPDATE_CONTROLLER",
	"SELECTIVE_UPDATE_RIGID",
	"DISPLAY_OBJECT",
	"DISABLE_SORTING",
	"SELECTIVE_UPDATE_TRANSFORMS_OVERRIDE",
	"OBJECT_PREPARED",
	"TRANS_IS_IDENTITY",
	"NO_DECALS",
	"ALWAYS_DRAW",
	"PREPROCESSED_NODE",
	"FIXED_BOUND",
	"FADED_IN",
	"IGNORE_FADE",
	"LOD_FADING_OUT",
	"HAS_MOVING_SOUND",
	"HAS_PROPERTY_CONTROLLER",
	"ORPHANED_BOUND",
	"NOT_VISIBLE",
	"IGNORES_PICKING",
	"WORLD_BOUND_CHANGE",
	"NOT_SHADOW_RECEIVER",
	"HIGH_DETAIL",
	"ALREADY_TRAVERSED",
	"",
	"PLAYER_BONE",
	"RELEVANT_IMPOSTER",
	"IS_POINTLIGHT",
	"DONE_INIT_LIGHTS",
	"IS_INSERTED",
};

void  __fastcall AddFlag(char* apBuffer, size_t astBufferSize, const char* apFlagName) {
	if (apBuffer[0] != '\0')
		strcat_s(apBuffer, astBufferSize, " | ");
	strcat_s(apBuffer, astBufferSize, apFlagName);
}

void __fastcall GetNiFlags(uint32_t auiFlags, char* apBuffer, size_t astBufferSize) {
	if (auiFlags == 0) {
		strcpy_s(apBuffer, astBufferSize, "NONE");
		return;
	}

	for (uint32_t i = 0; i < 32; i++){
		if (auiFlags & (1u << i)) {
			AddFlag(apBuffer, astBufferSize, pNiFlagNames[i]);
		}
	}
}

void __fastcall GetFormFlags(const TESForm* apForm, char* apBuffer, size_t astBufferSize) noexcept {
	const auto& uiFormFlags = apForm->uiFormFlags;
	if (uiFormFlags.Get() == 0) {
		strcpy_s(apBuffer, astBufferSize, "NONE");
		return;
	}

	const bool bRef = apForm->IsReference();
	const bool bBoundObj = apForm->IsBoundObject();
	const bool bMobileObj = apForm->IsMobileObject();
	const bool bActor = apForm->IsActor();
	const bool bActorBase = apForm->IsActorBase();
	const auto eFormType = apForm->eTypeID;
	const TESBoundObject* pBase = bRef ? static_cast<const TESObjectREFR*>(apForm)->GetObjectReference() : nullptr;
	if (uiFormFlags.GetBit(TESForm::FormFlags::MASTER))
		AddFlag(apBuffer, astBufferSize, "MASTER");

	if (uiFormFlags.GetBit(TESForm::FormFlags::ALTERED))
		AddFlag(apBuffer, astBufferSize, "ALTERED");

	if (uiFormFlags.GetBit(TESForm::FormFlags::INITIALIZED))
		AddFlag(apBuffer, astBufferSize, "INITIALIZED");

	if (uiFormFlags.GetBit(TESForm::FormFlags::NO_COLLISION))
		AddFlag(apBuffer, astBufferSize, "NO_COLLISION");

	if (uiFormFlags.GetBit(TESForm::FormFlags::DELETED))
		AddFlag(apBuffer, astBufferSize, "DELETED");

	if (uiFormFlags.GetBit(TESForm::FormFlags::HAS_SPOKEN)) {
		if (bActor)
			AddFlag(apBuffer, astBufferSize, "HAS_SPOKEN");
		else if (eFormType == TESForm::kType_TESRegion)
			AddFlag(apBuffer, astBufferSize, "BORDER_REGION");
		else if (eFormType == TESForm::kType_TESGlobal)
			AddFlag(apBuffer, astBufferSize, "CONSTANT");
		else if (bBoundObj) {
			if (eFormType == TESForm::kType_TESObjectTREE)
				AddFlag(apBuffer, astBufferSize, "TREE_LOD");
			else if (eFormType == TESForm::kType_TESObjectDOOR)
				AddFlag(apBuffer, astBufferSize, "HIDDEN_DOOR");
			else
				AddFlag(apBuffer, astBufferSize, "IN_PLACEABLE_WATER");
		}
	}

	if (uiFormFlags.GetBit(TESForm::FormFlags::FIRE_OFF))
		AddFlag(apBuffer, astBufferSize, "FIRE_OFF");

	if (uiFormFlags.GetBit(TESForm::FormFlags::DROPPED)) {
		if (bRef && pBase) {
			if (pBase->eTypeID != TESForm::kType_TESObjectDOOR)
				AddFlag(apBuffer, astBufferSize, "DROPPED");
			else
				AddFlag(apBuffer, astBufferSize, "INACCESSIBLE");
		}
	}

	if (bRef && pBase && uiFormFlags.GetBit(TESForm::FormFlags::CASTS_SHADOWS)) {
		if (pBase->eTypeID == TESForm::kType_TESObjectLIGH)
			AddFlag(apBuffer, astBufferSize, "CASTS_SHADOWS");
		else if (pBase->eTypeID == TESForm::kType_BGSMovableStatic)
			AddFlag(apBuffer, astBufferSize, "MOTION_BLUR");
	}
	else if (bBoundObj)
		AddFlag(apBuffer, astBufferSize, "ON_LOCAL_MAP");

	if (uiFormFlags.GetBit(TESForm::FormFlags::PERSISTENT)) {
		if (bRef || eFormType == TESForm::kType_TESObjectCELL)
			AddFlag(apBuffer, astBufferSize, "PERSISTENT");
		else if (eFormType == TESForm::kType_TESLoadScreen)
			AddFlag(apBuffer, astBufferSize, "DISPLAY_ON_MAIN_MENU");
		else if (bBoundObj)
			AddFlag(apBuffer, astBufferSize, "QUEST_ITEM");
	}

	if (uiFormFlags.GetBit(TESForm::FormFlags::DISABLED))
		AddFlag(apBuffer, astBufferSize, "DISABLED");

	if (uiFormFlags.GetBit(TESForm::FormFlags::EMPTY)) {
		if (eFormType == TESForm::kType_TESFlora)
			AddFlag(apBuffer, astBufferSize, "EMPTY");
		else if (eFormType == TESForm::kType_BGSTalkingActivator)
			AddFlag(apBuffer, astBufferSize, "NO_VOICE_FILTER");
		else if (eFormType == TESForm::kType_TESTopicInfo || eFormType == TESForm::kType_TESTopic)
			AddFlag(apBuffer, astBufferSize, "HAS_RESULTS");
		else if (!bRef && DYNAMIC_CAST(apForm, TESForm, BGSDestructibleObjectForm))
			AddFlag(apBuffer, astBufferSize, "RESET_DESTRUCT");
	}

	if (uiFormFlags.GetBit(TESForm::FormFlags::TEMPORARY))
		AddFlag(apBuffer, astBufferSize, "TEMPORARY");

	if (uiFormFlags.GetBit(TESForm::FormFlags::VISIBLE_DISTANT))
		AddFlag(apBuffer, astBufferSize, "VISIBLE_DISTANT");

	if (uiFormFlags.GetBit(TESForm::FormFlags::RANDOM_ANIM_START)) {
		if (bActor)
			AddFlag(apBuffer, astBufferSize, "HAVOK_DEATH");
		else if (bRef)
			AddFlag(apBuffer, astBufferSize, "HIGH_PRIORITY_LOD");
		else if (bBoundObj)
			AddFlag(apBuffer, astBufferSize, "RANDOM_ANIM_START");
	}

	if (uiFormFlags.GetBit(TESForm::FormFlags::NEED_TO_CHANGE_PROCESS)) {
		if (bMobileObj)
			AddFlag(apBuffer, astBufferSize, "NEED_TO_CHANGE_PROCESS");
		else if (eFormType == TESForm::kType_BGSTalkingActivator)
			AddFlag(apBuffer, astBufferSize, "RADIO_STATION");
		else if (eFormType == TESForm::kType_TESObjectCELL)
			AddFlag(apBuffer, astBufferSize, "OFF_LIMITS");
		else if (!bRef && DYNAMIC_CAST(apForm, TESForm, TESObjectACTI))
			AddFlag(apBuffer, astBufferSize, "DANGEROUS");
	}

	if (uiFormFlags.GetBit(TESForm::FormFlags::COMPRESSED))
		AddFlag(apBuffer, astBufferSize, bRef ? "TARGETED" : "COMPRESSED");

	if (uiFormFlags.GetBit(TESForm::FormFlags::STARTS_DEAD)) {
		if (bRef)
			AddFlag(apBuffer, astBufferSize, "HAS_TEMP_3D");
		else if (eFormType == TESForm::kType_TESObjectCELL || eFormType == TESForm::kType_TESWorldSpace)
			AddFlag(apBuffer, astBufferSize, "CANT_WAIT");
		else if (bBoundObj) {
			if (bActorBase)
				AddFlag(apBuffer, astBufferSize, "STARTS_DEAD");
			else
				AddFlag(apBuffer, astBufferSize, "SPECIFIC_TEXTURES");
		}
	}

	if (bRef && !bActor && uiFormFlags.GetBit(TESForm::FormFlags::CENTER_ON_CREATION))
		AddFlag(apBuffer, astBufferSize, "CENTER_ON_CREATION");

	if (uiFormFlags.GetBit(TESForm::FormFlags::STILL_LOADING))
		AddFlag(apBuffer, astBufferSize, "STILL_LOADING");

	if (bRef && uiFormFlags.GetBit(TESForm::FormFlags::BEING_DROPPED))
		AddFlag(apBuffer, astBufferSize, "BEING_DROPPED");

	if (bActor && uiFormFlags.GetBit(TESForm::FormFlags::IGNORE_FRIENDLY_HITS))
		AddFlag(apBuffer, astBufferSize, "IGNORE_FRIENDLY_HITS");

	if (bRef && uiFormFlags.GetBit(TESForm::FormFlags::DESTROYED))
		AddFlag(apBuffer, astBufferSize, "DESTROYED");

	if (bRef && uiFormFlags.GetBit(TESForm::FormFlags::DESTRUCTIBLE))
		AddFlag(apBuffer, astBufferSize, "DESTRUCTIBLE");

	if (uiFormFlags.GetBit(TESForm::FormFlags::OBSTACLE)) {
		if (bBoundObj)
			AddFlag(apBuffer, astBufferSize, "OBSTACLE");
		else if (bRef)
			AddFlag(apBuffer, astBufferSize, "NO_AI_ACQUIRE");
	}

	if (bRef && uiFormFlags.GetBit(TESForm::FormFlags::VATS_TARGET_OVERRIDE))
		AddFlag(apBuffer, astBufferSize, "VATS_TARGET_OVERRIDE");

	if (bRef && uiFormFlags.GetBit(TESForm::FormFlags::DISABLE_FADE))
		AddFlag(apBuffer, astBufferSize, "DISABLE_FADE");

	if (uiFormFlags.GetBit(TESForm::FormFlags::REFLECTED_BY_AUTO_WATER)) {
		if (bRef)
			AddFlag(apBuffer, astBufferSize, "REFLECTED_BY_AUTO_WATER");
		else if (eFormType == TESForm::kType_BGSTalkingActivator)
			AddFlag(apBuffer, astBufferSize, "NON_PIPBOY_RADIO");
	}

	if (uiFormFlags.GetBit(TESForm::FormFlags::REFRACTED_BY_AUTO_WATER)) {
		if (bRef)
			AddFlag(apBuffer, astBufferSize, "REFRACTED_BY_AUTO_WATER");
		else if (bBoundObj) {
			if (eFormType == TESForm::kType_TESObjectACTI || eFormType == TESForm::kType_TESFurniture || eFormType == TESForm::kType_BGSIdleMarker)
				AddFlag(apBuffer, astBufferSize, "CHILD_CAN_USE");
		}
	}

	if (eFormType == TESForm::kType_BGSTalkingActivator && uiFormFlags.GetBit(TESForm::FormFlags::CONTINUOUS_BROADCAST))
		AddFlag(apBuffer, astBufferSize, "CONTINUOUS_BROADCAST");
}

template<class Member> void __fastcall LogMember(const char* name, const Member& member, bool nested = true) {
	if (name)
		_MESSAGE(name);

	AutoIndent indent;
	try {
		LogClass(member, nested);
	}
	catch (...) {
		if (name)
			_MESSAGE("Exception occurred while logging member \"%s\"", name);
		else
			_MESSAGE("Exception occurred while logging member");
	}
}

template<class Member> void __fastcall LogClassLineByLine(Member& member) {
	AutoIndent indent;
	try {
		LogClass(member);
	}
	catch (...) {
		_MESSAGE("Exception occurred while logging");
	}
}

inline void __fastcall LogClass(const NiRefObject& obj, bool nested = false) {
	_MESSAGE("Reference Count: %i", obj.m_uiRefCount);
	if (obj.m_uiRefCount == 0)
		_MESSAGE("WARNING: Reference Count is zero, object may be deleted!");
}

inline void __fastcall LogClass(const Tile& obj, bool nested = false) { 
	char finalPath[MAX_PATH];
	char swapPath[MAX_PATH];
	const char* start = obj.kName.CStr();
	strcpy_s(finalPath, sizeof(finalPath), start);
	if ((obj.GetType() != Tile::kValue_tilemenu) && obj.pkParent) {
		strcpy_s(swapPath, sizeof(swapPath), start);

		auto tile = &obj;
		while ((tile->GetType() != Tile::kValue_tilemenu) && (tile = tile->pkParent)) {
			sprintf_s(finalPath, "%s/%s", tile->kName.CStr(), swapPath);
			strcpy_s(swapPath, sizeof(swapPath), finalPath);
		}
	}

	_MESSAGE("XML Path: \"%s\"", finalPath);
}

inline void __fastcall LogClass(const Menu& obj, bool nested = false) {
	_MESSAGE("Menu ID: %d", obj.eID);
	_MESSAGE("Visible: %s", obj.IsVisible() ? "True" : "False");
	_MESSAGE("Visibility State: 0x%X", obj.eVisibilityState);
	LogMember("Root Tile:", static_cast<Tile&>(*obj.pkRootTile));
}

inline void __fastcall LogClass(const StartMenu::Option& obj, bool nested = false) {
	_MESSAGE("\"%s\"", obj.pcDisplayString);
}

inline void __fastcall LogClass(const Setting& obj, bool nested = false) {
#if 0
	return std::vector { '"' + std::string(obj.pKey) + '"' + ": " + (obj.GetType() == Setting::kSetting_String ? obj.GetAsString() : std::to_string(obj.GetAsFloat()))
	};
#endif
}

inline void __fastcall LogClass(const TESForm& obj, bool nested = false) {
	const UInt32 refID = obj.uiFormID;

	const char* pEDID = obj.GetEditorID();
	const char* pName = nullptr;
	if (obj.eTypeID == TESForm::kType_TESObjectCELL)
		pName = static_cast<const TESObjectCELL&>(obj).GetFullName();
	else if (obj.IsReference())
		pName = static_cast<const TESObjectREFR&>(obj).GetFullName();
	else
		pName = TESFullName::GetFullName(&obj);

	if (pName && pName[0])
		_MESSAGE("Full Name: \"%s\"", pName);
	else
		_MESSAGE("Full Name: None");

	// Temporary forms don't have actual FormIDs, so logging them is useless
	if (!pEDID || !pEDID[0]) {
		if (obj.IsTemporary())
			_MESSAGE("Temporary Form");
		else
			_MESSAGE("FormID: %08X", refID);
	}
	else {
		if (obj.IsTemporary())
			_MESSAGE("Temporary Form: \"%s\"", pEDID);
		else
			_MESSAGE("FormID: %08X (\"%s\")", refID, pEDID);
	}

	{
		char cFlagsBuffer[512] = {};
		GetFormFlags(&obj, cFlagsBuffer, sizeof(cFlagsBuffer));
		_MESSAGE("Flags: %s", cFlagsBuffer);
	}

	TESFile* pLastMod = obj.kMods.TailItem();
	if (pLastMod) {
		TESFile* pSourceMod = TESDataHandler::GetSingleton()->kMods.pLoadedMods[obj.ucModIndex];

		_MESSAGE("Plugin: \"%s\"", pSourceMod->m_Filename);

		if (pSourceMod != pLastMod) {
			_MESSAGE("Last modified by: \"%s\"", pLastMod->m_Filename);
		}
	}
}

inline void __fastcall LogClass(const TESBoundObject& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);
	if (!nested) {
		try {
			TESModel* pModel = ModelLoader::GetSingleton()->GetModelForBoundObject(&obj, nullptr);
			if (pModel) {
				LogMember("Model:", *pModel);
			}
		}
		catch (...) {
			_MESSAGE("Failed to retrieve model for bound object");
		}
	}
}

inline void __fastcall LogClass(const TESObjectREFR& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);
	if (!obj.IsTemporary() && obj.uiFormFlags.IsClear(TESForm::FormFlags::INITIALIZED) && obj.pkParentCell) {
		_MESSAGE("ERROR: Reference is not initialized! This can be caused by a corrupted plugin!\nOpen xEdit, and resave plugins that modify %08X (\"%s\")\nTo resave a plugin, right-click on it and mark as modified!\nDo not modify vanilla ESMs!", obj.pkParentCell->GetFormID(), obj.pkParentCell->GetEditorID());
	}

	const auto baseForm = obj.GetObjectReference();
	if (baseForm) {
		char cBuffer[128];
		sprintf_s(cBuffer, "BaseForm: %s", TESForm::TypeNames[baseForm->eTypeID]);
		if (!nested)
			LogMember(cBuffer, *baseForm);
		else
			_MESSAGE(cBuffer);
	}

	if (!nested) {
		if (obj.pkParentCell) {
			LogMember("Cell:", *obj.pkParentCell);
		}

		try {
			TESModel* pModel = ModelLoader::GetSingleton()->GetModelForBoundObject(baseForm, &obj);
			if (pModel) {
				LogMember("Model:", *pModel);
			}
		}
		catch (...) {
			_MESSAGE("Failed to retrieve model for reference");
		}
	}
}

inline void __fastcall LogClass(const ActorMover& obj, bool nested = false)  { 
	if (obj.pkActor)
		LogClass(*obj.pkActor);
}

inline void __fastcall LogClass(const QueuedReference& obj, bool nested = false) {
	if (obj.pRefr)
		LogClass(*obj.pRefr);
}

inline void __fastcall LogClass(const NavMesh& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);
	if (obj.pParentCell)
		LogMember("Cell:", *obj.pParentCell);
}

inline void __fastcall LogClass(const TESPackage& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);
#if 0
	if (obj.kData.eFlags & TESPackage::kPackageFlag_Created) {
		if (obj.kData.eType >= 0 && obj.kData.eType < 40) {
			const char* pObjectTypeName = ThisStdCall<const char*>(0x67ACD0, &obj);
			char textBuffer[128];
			_MESSAGE("Package Type: %s", pObjectTypeName);
		
		}
	}
	else {
		_MESSAGE("Package has not been created");
	}
#endif
}

inline void __fastcall LogClass(const BaseProcess& obj, bool nested = false) {
	{
		const TESPackage* pPackage = obj.GetCurrentPackage();
		if (pPackage) {
			LogMember("Current Package:", *pPackage);
		}
	}

	{
		const TESPackage* pPackage = obj.GetRunOncePackage();
		if (pPackage) {
			LogMember("Run-Once Package:", *pPackage);
		}
	}

	if (!nested) {
		const auto& array = ProcessLists::GetSingleton()->kAllProcessArrays;
		for (uint32_t i = 0; i < array.kMobileObjects.GetSize(); i++) {
			const MobileObject* pObject = array.GetAt(i);
			if (pObject && pObject->pkBaseProcess == &obj) {
				LogMember("Owner:", reinterpret_cast<const TESForm&>(*pObject));
			}
		}
	}
}

inline void __fastcall LogClass(const NiControllerSequence& obj, bool nested = false) {
	LogClass(static_cast<const NiRefObject&>(obj), nested);

	_MESSAGE("Name: \"%s\"", obj.m_kName.m_kHandle);

	_MESSAGE("Accum Root Name: \"%s\"", obj.m_kAccumRootName.m_kHandle);

	if (obj.m_pkOwner && !nested) {
		LogMember("Owner:", *static_cast<const NiTimeController*>(obj.m_pkOwner));
	}
}

inline void __fastcall LogClass(const BSAnimGroupSequence& obj, bool nested = false) {
	LogClass(static_cast<const NiControllerSequence&>(obj), nested);
	_MESSAGE("Anim Group: %04X", obj.spAnimGroup->groupID);

}

inline void __fastcall LogClass(const AnimSequenceSingle& obj, bool nested = false) { 
	if (obj.pkAnim) 
		LogClass(*obj.pkAnim);
}

inline void __fastcall LogClass(const AnimSequenceMultiple& obj, bool nested = false) {
	UInt32 i = 0;
	for (const auto iter : *obj.pkAnims) {
		char cBuffer[32];
		sprintf_s(cBuffer, "Anim Sequence %d:", i);
		LogMember(cBuffer, *iter);
		i++;
	}
}

inline void __fastcall LogClass(const NiObjectNET& obj, bool nested = false) {
	LogClass(static_cast<const NiRefObject&>(obj), nested);
	const std::string_view name = obj.m_kName;
	if (!name.empty()) {
		_MESSAGE("Name: \"%s\"", name.data());
	}
}

inline void __fastcall LogClass(const NiAVObject& obj, bool nested = false) {
	LogClass(static_cast<const NiObjectNET&>(obj), nested);

	{
		char cFlagsBuffer[512] = {};
		GetNiFlags(obj.m_uiFlags.Get(), cFlagsBuffer, sizeof(cFlagsBuffer));
		_MESSAGE("Flags: %s", cFlagsBuffer);
	}

	if (!nested) {
		try {
			if (const auto ref = TESObjectREFR::FindReferenceFor3D(&obj))
				LogMember("Owner Reference:", *ref);
		}
		catch (...) {
			_MESSAGE("Failed to retrieve owner reference");
		}
	}

	if (obj.GetParent()) {
		const NiNode* parent = obj.GetParent();
		char cBuffer[256];
		sprintf_s(cBuffer, "Parent: %s", parent->GetRTTIName());
		if (!nested) {
			LogMember(cBuffer, *parent);
		}
		else {
			_MESSAGE("%s -> \"%s\"", cBuffer, parent->GetName());
		}
	}
}

inline void __fastcall LogClass(const NiNode& obj, bool nested = false) {
	LogClass(static_cast<const NiAVObject&>(obj), nested);
	if (obj.GetChildCount() > 0) {
		_MESSAGE("Children: %d", obj.GetChildCount());
		AutoIndent indent;
		for (UInt32 i = 0; i < obj.GetArrayCount(); i++) {
			const NiAVObject* child = obj.GetAt(i);
			char cBuffer[256];
			snprintf(cBuffer, sizeof(cBuffer), "Child %d: %s", i, child ? child->GetRTTIName() : "<Empty>");
			if (child) {

				if (!nested) {
					LogMember(cBuffer, (static_cast<const NiObjectNET&>(*child)));
				}
				else {
					_MESSAGE("%s -> \"%s\"", cBuffer, child->GetName());
				}
			}
			else {
				_MESSAGE(cBuffer);
			}
		}
	}
}

inline void __fastcall LogClass(const NiExtraData& obj, bool nested = false) {
	LogClass(static_cast<const NiRefObject&>(obj), nested);
	if (const std::string_view name = obj.m_kName; !name.empty()) {
		_MESSAGE("Name: \"%s\"", name.data());
	}
}

inline void __fastcall LogClass(const BSFile& obj, bool nested = false) {
	char sanitizedPath[MAX_PATH] = {};
	_MESSAGE("File Path: \"%s\"", SanitizeString(obj.cFileName, sanitizedPath, sizeof(sanitizedPath)));

}
inline void __fastcall LogClass(const TESModel& obj, bool nested = false) {
	_MESSAGE("Model Path: \"%s\"", obj.GetModel());

}

inline void __fastcall LogClass(const QueuedModel& obj, bool nested = false) {
	if (obj.spModel) {
		char sanitizedPath[MAX_PATH] = {};
		_MESSAGE("File Path: \"%s\"", SanitizeString(obj.spModel->pcPath, sanitizedPath, sizeof(sanitizedPath)));
	}
	if (obj.pTESModel)
		LogMember("Model:", *obj.pTESModel);
}

inline void __fastcall LogClass(const TESTexture& obj, bool nested = false) {
	char sanitizedPath[MAX_PATH] = {};
	_MESSAGE("Texture Path: \"%s\"", SanitizeString(obj.kTexturePath.CStr(), sanitizedPath, sizeof(sanitizedPath)));
}

inline void __fastcall LogClass(const QueuedTexture& obj, bool nested = false) {
	char sanitizedPath[MAX_PATH] = {};
	_MESSAGE("\"%s\"", SanitizeString(obj.pFileName, sanitizedPath, sizeof(sanitizedPath)));
}

inline void __fastcall LogClass(const NiStream& obj, bool nested = false) {
	char sanitizedPath[MAX_PATH] = {};
	_MESSAGE("File Path: \"%s\"", SanitizeString(obj.m_acFileName, sanitizedPath, sizeof(sanitizedPath)));
}

inline void __fastcall LogClass(const ActiveEffect& obj, bool nested = false) { 
	if (obj.pkEnchantObject) 
		LogClass(*obj.pkEnchantObject);
}

inline void __fastcall LogClass(const Script& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);
	if (obj.m_val && obj.IsTemporary() || obj.kMods.IsEmpty()) {
		char textBuffer[256] = {};
		const char* pEDID = obj.GetEditorID();
		if (pEDID && pEDID[0])
			sprintf_s(textBuffer, "Script %s 0x%08X", pEDID, obj.uiFormID);
		else if (!obj.IsTemporary())
			sprintf_s(textBuffer, "Unknown Script 0x%08X", obj.uiFormID);
		else
			sprintf_s(textBuffer, "Temporary Script");

		try {
			DecompileScriptToFolder(textBuffer, const_cast<Script*>(&obj), "gek", "Crash Logger");
		}
		catch (...) {
			_MESSAGE("Failed to decompile script");
		}
	}
}

inline void __fastcall LogClass(const ScriptEffect& obj, bool nested = false) {
	LogClass(static_cast<const ActiveEffect&>(obj), nested);
	if (obj.pkScript)
		LogMember("Script:", *obj.pkScript);
}

inline void __fastcall LogClass(const QueuedKF& obj, bool nested = false) { 
	if (obj.pkKFModel) {
		_MESSAGE("File Path: \"%s\"", obj.pkKFModel->pcPath);
	
	}
}
inline void __fastcall LogClass(const bhkRefObject& obj, bool nested = false) {
	if (const auto object = obj.GetAVObject()) 
		LogMember("Owner:", *object);
}

inline void __fastcall LogClass(const NiCollisionObject& obj, bool nested = false) {
	LogClass(static_cast<const NiRefObject&>(obj), nested);
	if (const auto object = obj.m_pkSceneObject) {
		if (object->IsNiNode())
			LogMember("Scene Object:", reinterpret_cast<const NiNode&>(*object));
		else
			LogMember("Scene Object:", *object);
	}
}

inline void __fastcall LogClass(const NiTimeController& obj, bool nested = false) {
	LogClass(static_cast<const NiRefObject&>(obj), nested);
	if (obj.m_pkTarget) {
		LogMember("Target:", *obj.m_pkTarget);
	}
}

inline void __fastcall LogClass(const bhkCharacterController& obj, bool nested = false) {
	if (const auto object = obj.GetNiObject()) {
		if (object->IsNiNode())
			LogMember("Target:", reinterpret_cast<const NiNode&>(*object));
		else
			LogMember("Target:", *object);
	}
}

inline void __fastcall LogClass(const hkpWorldObject& obj, bool nested = false) {
	const char* name = obj.GetName();

	if (name && name[0]) {
		_MESSAGE("Name: %s", name);
	}

	bhkNiCollisionObject* object = bhkUtilFunctions::GetbhkNiCollisionObject(&obj);
	if (object)
		LogMember("Collision Object:", reinterpret_cast<const NiCollisionObject&>(*object));
}

inline void __fastcall LogClass(const IMemoryHeap& obj, bool nested = false) {
	HeapStats stats;
	obj.GetHeapStats(&stats, true);
	UInt32 total = stats.uiMemHeapSize;
	UInt32 used = stats.uiMemUsedInBlocks;
	char cMemBuffer[128];
	GetMemoryUsageString(used, total, cMemBuffer, sizeof(cMemBuffer));
	_MESSAGE("%s: %s", obj.GetName(), cMemBuffer);
}

inline void __fastcall LogClass(const MobileObject& obj, bool nested = false) {
	LogClass(static_cast<const TESObjectREFR&>(obj), nested);
	const BaseProcess* pCurrProcess = obj.pkBaseProcess;
	if (pCurrProcess) {
		const char* pProcessName = "Invalid";
		switch (pCurrProcess->eProcessLevel) {
			case BaseProcess::kProcessLevel_High:
				pProcessName = "High";
				break;
			case BaseProcess::kProcessLevel_MiddleHigh:
				pProcessName = "Middle High";
				break;
			case BaseProcess::kProcessLevel_MiddleLow:
				pProcessName = "Middle Low";
				break;
			case BaseProcess::kProcessLevel_Low:
				pProcessName = "Low";
				break;
		}

		char cBuffer[64];
		sprintf_s(cBuffer, "Process: %s", pProcessName);
		LogMember(cBuffer, *pCurrProcess);
	}
}

inline void __fastcall LogClass(const TESObjectCELL& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);

	_MESSAGE("Type: %s", obj.IsInterior() ? "Interior" : "Exterior");

	const char* pCellStateName = "Unknown";
	switch (obj.cCellState) {
		case TESObjectCELL::CS_NOTLOADED:
			pCellStateName = "Not Loaded";
			break;
		case TESObjectCELL::CS_UNLOADING:
			pCellStateName = "Unloading";
			break;
		case TESObjectCELL::CS_LOADING:
			pCellStateName = "Loading";
			break;
		case TESObjectCELL::CS_LOADED:
			pCellStateName = "Loaded";
			break;
		case TESObjectCELL::CS_DETACHING:
			pCellStateName = "Detaching";
			break;
		case TESObjectCELL::CS_ATTACHING:
			pCellStateName = "Attaching";
			break;
		case TESObjectCELL::CS_ATTACHED:
			pCellStateName = "Attached";
			break;
		default:
			pCellStateName = "Invalid";
			break;
	}
	_MESSAGE("State: %s", pCellStateName);

	if (obj.GetWorldspace())
		LogMember("Worldspace:", *obj.GetWorldspace());
}

inline void __fastcall LogClass(const TESWorldSpace& obj, bool nested = false) {
	LogClass(static_cast<const TESForm&>(obj), nested);
	if (obj.pCellMap) {
		_MESSAGE("Loaded Cells: %i", obj.pCellMap->GetCount());
	}
}

inline void __fastcall LogClass(const Projectile& obj, bool nested = false) {
	LogClass(static_cast<const TESObjectREFR&>(obj), nested);
	if (obj.pSourceRef) {
		LogMember("Shooter:", *obj.pSourceRef);
	}
	if (obj.pSourceWeapon) {
		LogMember("Source Weapon:", static_cast<const TESForm&>(*obj.pSourceWeapon));
	}
}