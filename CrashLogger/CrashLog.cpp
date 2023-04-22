#include <main.h>

#include "CrashLog.h"

#include <DbgHelp.h>
#include <map>
#include <set>

#include "GameEffects.h"
#include "TESForm.h"
#include "GameTasks.h"
#include "Tile.h"
#include "Menu.h"
#include "TESObjectREFR.h"
#include "SafeWrite.h"

#define SYMOPT_EX_WINE_NATIVE_MODULES  1000

constexpr UInt32 ce_printStackCount = 256;

namespace CrashLogger::Handle
{
	typedef std::string (*_Handler)(void* ptr);

	std::string AsUInt32(void* ptr)
	{
		return FormatString("0x%08X", **static_cast<UInt32**>(ptr));
	}

	std::string AsMenu(void* ptr) { return static_cast<Menu*>(ptr)->tile->GetFullPath(); }
	std::string AsTile(void* ptr) { return static_cast<Tile*>(ptr)->GetFullPath(); }
	std::string AsStartMenuOption(void* ptr) { return static_cast<StartMenu::Option*>(ptr)->displayString; }

	std::string AsTESForm(void* ptr) { const auto form = static_cast<TESForm*>(ptr); return FormatString("%08X (%s)", form->refID, form->GetName()); }
	std::string AsTESObjectREFR(void* ptr) { const auto refr = static_cast<TESObjectREFR*>(ptr);
		return FormatString("%08X (%s), BaseForm %08X (%s)", refr->refID, refr->GetName(),
		                    refr->TryGetREFRParent()->refID, refr->TryGetREFRParent()->GetName());
	}
	std::string AsBaseProcess(void* ptr)
	{
		const auto process = static_cast<BaseProcess*>(ptr);
		for (const auto iter : *TESForm::GetAll())
		{
			if (iter->typeID == kFormType_Character && static_cast<Character*>(iter)->baseProcess == process)
				return FormatString("Form %08X (%s)", iter->refID, iter->GetName());
			if (iter->typeID == kFormType_Creature && static_cast<Creature*>(iter)->baseProcess == process)
				return FormatString("Form %08X (%s)", iter->refID, iter->GetName());
		}
		return "";
	}

	std::string AsBSAnimGroupSequence(void* ptr)
	{
		const auto sequence = static_cast<BSAnimGroupSequence*>(ptr);
		return FormatString("%s, %s, AnimGroup %04X", sequence->sequenceName, sequence->accumRootName,
		                    sequence->animGroup->groupID);
	}
	std::string AsAnimSequenceSingle(void* ptr)
	{
		const auto sequence = static_cast<AnimSequenceSingle*>(ptr);
		return FormatString("%s, %s, AnimGroup %04X", sequence->anim->sequenceName, sequence->anim->accumRootName,
		                    sequence->anim->animGroup->groupID);
	}

	std::string AsAnimSequenceMultiple(void* ptr)
	{
		const auto sequence = static_cast<AnimSequenceMultiple*>(ptr);
		std::string ret;
		for (const auto iter : *sequence->anims)
			ret += FormatString("%s, %s, AnimGroup %04X; ", iter->sequenceName, iter->accumRootName, iter->animGroup->groupID);
		return ret;
	}

	std::string AsNiObjectNET(void* ptr) { const auto object = static_cast<NiObjectNET*>(ptr); return FormatString("%s", object->m_pcName); }


	std::string AsBSFile(void* ptr) { const auto file = static_cast<BSFile*>(ptr); return FormatString("%s", file->m_path); }

	std::string AsTESModel(void* ptr) { const auto model = static_cast<TESModel*>(ptr); return FormatString("%s", model->nifPath.CStr()); }
	std::string AsQueuedModel(void* ptr) { const auto model = static_cast<QueuedModel*>(ptr); return FormatString("%s %s", model->model->path, model->tesModel->nifPath.CStr()); }

	std::string AsTESTexture(void* ptr) { const auto texture = static_cast<TESTexture*>(ptr); return FormatString("%s", texture->ddsPath.CStr()); }
	std::string AsQueuedTexture(void* ptr) { const auto texture = static_cast<QueuedTexture*>(ptr); return FormatString("%s", texture->name); }

	std::string AsActiveEffect(void* ptr) { const auto effect = static_cast<ActiveEffect*>(ptr); return effect->enchantObject ? FormatString("Enchanted Object %08X (%s)", effect->enchantObject->refID, effect->enchantObject->GetName()) : ""; }

	std::string AsScriptEffect(void* ptr) {
		const auto effect = static_cast<ScriptEffect*>(ptr);
		std::string ret;
		ret += effect->enchantObject ? FormatString("Enchanted Object %08X (%s) ", effect->enchantObject->refID, effect->enchantObject->GetName()) : "";
		ret += effect->script ? FormatString("Script %08X (%s)", effect->script->refID, effect->script->GetName()) : "";
		return ret;
	}

}
namespace CrashLogger::NVVtables
{
	Handle::_Handler lastHandler = nullptr;

	class Label {
	public:
		enum Type : UInt8 {
			kType_None = 0,
			kType_RTTIClass = 1,
			kType_Global = 2,
		};

		Type type = kType_None;

		UInt32 address;
		UInt32 size;
		Handle::_Handler function;
		std::string name;

		Label(UInt32 address, Handle::_Handler function = lastHandler, std::string name = "", Type type = kType_RTTIClass, UInt32 size = 4)
			: type(type), address(address), size(size), function(function), name(std::move(name))
		{
			lastHandler = function;
		};
		
		bool Satisfies(void* ptr) const
		{
			__try
			{
				return *static_cast<UInt32*>(ptr) >= address && *static_cast<UInt32*>(ptr) <= address + size;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return false;
			}
		}

		static std::string GetTypeName(void* ptr)
		{
			return printTypeName(ptr);
		}

		[[nodiscard]] std::string GetLabelName() const
		{
			if (type == kType_RTTIClass) return "RTTI: ";
			if (type == kType_Global) return "Global: ";
			return "None: ";
		}

		static UInt32 Dereference(void* ptr)
		{
			__try
			{
				return *static_cast<UInt32*>(ptr);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}

		std::string Get(void* ptr) const
		{
			std::string name1 = name.empty() && type == kType_RTTIClass ? GetTypeName(ptr) : name;
			if (name1.empty()) name1 = FormatString("0x%08X", Dereference(ptr));

			std::string details;
			if (function) details = function(ptr);
			if (!details.empty()) details = ": " + details;
		
			return GetLabelName() + name1 + details;
		}
	};

	std::vector<std::unique_ptr<Label>> setOfLabels;
	
	template <class... _Types> void Push(_Types... args) {
		setOfLabels.push_back(std::make_unique<Label>(std::forward<_Types>(args)...));
	}

	void FillLabels()
	{
		Push(0x10F1EE0, nullptr, "TypeInfo", Label::kType_RTTIClass);

		Push(0x11F3374, Handle::AsUInt32, "TileValueIndirectTemp", Label::kType_Global);
		Push(0x118FB0C, nullptr, "ShowWhoDetects", Label::kType_Global);
		Push(0x1042C58, nullptr, "ShowWhoDetects", Label::kType_Global);
		
		Push(0x11846D4, nullptr, "Actor", Label::kType_RTTIClass, 0x8);
		Push(0x1184920, nullptr, "MovingObject", Label::kType_RTTIClass, 0x8);
		Push(0x11840C4, nullptr, "TESAmmo", Label::kType_RTTIClass, 0x8);
		

		Push(0x1094D9C, nullptr, "Tile::MenuStringMap");
		Push(0x10472CC, nullptr, "BSSimpleArray<const char*>", Label::kType_RTTIClass,0x8);
		Push(0x106EDEC, nullptr, "BSSimpleArray<Tile::Value*>");
		Push(0x106CC64, nullptr, "BSSimpleArray<Float32>", Label::kType_RTTIClass, 0x8);
		Push(0x10C1740, nullptr, "BSTaskManagerThread<SInt64>");
		Push(0x1094E7C, nullptr, "NiTPointerMap<Tile::Value*; Tile::Reaction*>");

		Push(0x01082F04, nullptr, "BSTCommonScrapHeapMessageQueue<BSPackedTask>");

		Push(0x104A284, nullptr, "TESNPC::TESActorBase");
		Push(0x104A270, nullptr, "TESNPC::Container");
		Push(0x104A25C, nullptr, "TESNPC::TouchSpell");
		Push(0x104A23C, nullptr, "TESNPC::SpellList");
		Push(0x104A21C, nullptr, "TESNPC::AI");
		Push(0x104A204, nullptr, "TESNPC::Health");
		Push(0x104A1F0, nullptr, "TESNPC::Attributes");
		Push(0x104A1DC, nullptr, "TESNPC::Animation");
		Push(0x104A1C8, nullptr, "TESNPC::FullName");
		Push(0x104A1A4, nullptr, "TESNPC::Model");
		Push(0x104A190, nullptr, "TESNPC::Scriptable");
		Push(0x104A160, nullptr, "TESNPC::AVOwner");
		Push(0x104A14C, nullptr, "TESNPC::Destructible");
		Push(0x104A138, nullptr, "TESNPC::Race");

		Push(0x101191C, nullptr, "AlchemyItem::MagicItem");
		Push(0x1011900, nullptr, "AlchemyItem::MagicItem::TList");
		Push(0x10118DC, nullptr, "AlchemyItem::Model");
		Push(0x10118BC, nullptr, "AlchemyItem::Icon");
		Push(0x10118A8, nullptr, "AlchemyItem::MessageIcon");
		Push(0x1011894, nullptr, "AlchemyItem::Scriptable");
		Push(0x1011880, nullptr, "AlchemyItem::Weight");
		Push(0x101186C, nullptr, "AlchemyItem::EquipType");
		Push(0x1011858, nullptr, "AlchemyItem::Destructible");
		Push(0x1011844, nullptr, "AlchemyItem::PickUpPutDownSounds");


		Push(kVtbl_Menu, Handle::AsMenu);
		Push(kVtbl_TutorialMenu);
		Push(kVtbl_StatsMenu);
		Push(kVtbl_TextEditMenu);
		Push(kVtbl_SurgeryMenu);
		Push(kVtbl_VATSMenu);
		Push(kVtbl_SpecialBookMenu);
		Push(kVtbl_LoveTesterMenu);
		Push(kVtbl_QuantityMenu);
		Push(kVtbl_RecipeMenu);
		Push(kVtbl_BarterMenu);
		Push(kVtbl_BlackjackMenu);
		Push(kVtbl_BookMenu);
		Push(kVtbl_CaravanMenu);
		Push(kVtbl_CharGenMenu);
		Push(kVtbl_CompanionWheelMenu);
		Push(kVtbl_ComputersMenu);
		Push(kVtbl_ContainerMenu);
		Push(kVtbl_CreditsMenu);
		Push(kVtbl_DialogMenu);
		Push(kVtbl_HackingMenu);
		Push(kVtbl_HUDMainMenu);
		Push(kVtbl_InventoryMenu);
		Push(kVtbl_ItemModMenu);
		Push(kVtbl_LevelUpMenu);
		Push(kVtbl_LoadingMenu);
		Push(kVtbl_LockpickMenu);
		Push(kVtbl_MapMenu);
		Push(kVtbl_MessageMenu);
		Push(kVtbl_PlayerNameEntryMenu);
		Push(kVtbl_RaceSexMenu);
		Push(kVtbl_RepairMenu);
		Push(kVtbl_RepairServicesMenu);
		Push(kVtbl_RouletteMenu);
		Push(kVtbl_SleepWaitMenu);
		Push(kVtbl_SlotMachineMenu);
		Push(kVtbl_StartMenu);
		Push(kVtbl_TraitMenu);
		Push(kVtbl_TraitSelectMenu);
		Push(kVtbl_TraitSelectMenu);
		Push(kVtbl_TutorialMenu);

		Push(kVtbl_Tile, Handle::AsTile);
		Push(kVtbl_TileMenu);
		Push(kVtbl_TileRect);
		Push(kVtbl_TileImage);
		Push(kVtbl_RadialTile);
		Push(kVtbl_TileText);
		Push(kVtbl_Tile3D);

		Push(kVtbl_StartMenuOption, Handle::AsStartMenuOption, "StartMenu::Option");
		Push(kVtbl_StartMenuUserOption, Handle::AsStartMenuOption, "StartMenu::UserOption");

		Push(kVtbl_TESForm, Handle::AsTESForm);

		Push(kVtbl_AlchemyItem);

		Push(kVtbl_BGSConstructibleObject);
		Push(kVtbl_BGSDebris);
		Push(kVtbl_BGSDehydrationStage);
		Push(kVtbl_BGSExplosion);
		Push(kVtbl_BGSHeadPart);
		Push(kVtbl_BGSHungerStage);
		Push(kVtbl_BGSListForm);
		Push(kVtbl_BGSMessage);
		Push(kVtbl_BGSNote);
		Push(kVtbl_BGSMovableStatic);
		Push(kVtbl_BGSPerk);
		Push(kVtbl_BGSPlaceableWater);
		Push(kVtbl_BGSProjectile);
		Push(kVtbl_BGSSleepDeprevationStage);
		Push(kVtbl_BGSStaticCollection);
		Push(kVtbl_BGSTalkingActivator);
		Push(kVtbl_BGSTerminal);
		Push(kVtbl_EffectSetting);
		Push(kVtbl_EnchantmentItem);
		Push(kVtbl_IngredientItem);
		Push(kVtbl_MagicItemForm);
		Push(kVtbl_MagicItemObject);
		Push(kVtbl_NavMesh);
		Push(kVtbl_Script);
		Push(kVtbl_SpellItem);
		Push(kVtbl_TESAmmo);
		Push(kVtbl_TESAmmoEffect);
		Push(kVtbl_TESBoundAnimObject);
		Push(kVtbl_TESBoundObject);
		Push(kVtbl_TESBoundTreeObject);
		Push(kVtbl_TESCaravanCard);
		Push(kVtbl_TESCaravanDeck);
		Push(kVtbl_TESCaravanMoney);
		Push(kVtbl_TESCasino);
		Push(kVtbl_TESCasinoChips);
		Push(kVtbl_TESChallenge);
		Push(kVtbl_TESClass);
		Push(kVtbl_TESClimate);
		Push(kVtbl_TESCombatStyle);
		Push(kVtbl_TESCreature);
		Push(kVtbl_TESEffectShader);
		Push(kVtbl_TESEyes);
		Push(kVtbl_TESFaction);
		Push(kVtbl_TESFlora);
		Push(kVtbl_TESFurniture);
		Push(kVtbl_TESGlobal);
		Push(kVtbl_TESGrass);
		Push(kVtbl_TESHair);
		Push(kVtbl_TESImageSpace);
		Push(kVtbl_TESImageSpaceModifier);
		Push(kVtbl_TESKey);
		Push(kVtbl_TESLandTexture);
		Push(kVtbl_TESLevCharacter);
		Push(kVtbl_TESLevCreature);
		Push(kVtbl_TESLevItem);
		Push(kVtbl_TESLevSpell);
		Push(kVtbl_TESNPC);
		Push(kVtbl_TESObject);
		Push(kVtbl_TESObjectACTI);
		Push(kVtbl_TESObjectANIO);
		Push(kVtbl_TESObjectARMA);
		Push(kVtbl_TESObjectARMO);
		Push(kVtbl_TESObjectBOOK);
		Push(kVtbl_TESObjectCELL);
		Push(kVtbl_TESObjectCLOT);
		Push(kVtbl_TESObjectCONT);
		Push(kVtbl_TESObjectDOOR);
		Push(kVtbl_TESObjectIMOD);
		Push(kVtbl_TESObjectLIGH);
		Push(kVtbl_TESObjectMISC);
		Push(kVtbl_TESObjectSTAT);
		Push(kVtbl_TESObjectTREE);
		Push(kVtbl_TESObjectWEAP);
		Push(kVtbl_TESPackage);
		Push(kVtbl_AlarmPackage);
		Push(kVtbl_BackUpPackage);
		Push(kVtbl_DialoguePackage);
		Push(kVtbl_EscortActorPackageData);
		Push(kVtbl_FleePackage);
		Push(kVtbl_GuardActorPackageData);
		Push(kVtbl_PatrolActorPackageData);
		Push(kVtbl_SandBoxActorPackageData);
		Push(kVtbl_SearchPackage);
		Push(kVtbl_SpectatorPackage);
		Push(kVtbl_TrespassPackage);
		Push(kVtbl_TESQuest);
		Push(kVtbl_TESRace);
		Push(kVtbl_TESRecipe);
		Push(kVtbl_TESRegion);
		Push(kVtbl_TESReputation);
		Push(kVtbl_TESSkill);
		Push(kVtbl_TESSound);
		Push(kVtbl_TESTopic);
		Push(kVtbl_TESTopicInfo);
		Push(kVtbl_TESWaterForm);
		Push(kVtbl_TESWeather);
		Push(kVtbl_TESWorldSpace);
		Push(kVtbl_TESLoadScreen);
		Push(kVtbl_TESRecipeCategory);
		Push(kVtbl_TESActorBase);
		Push(kVtbl_TESIdleForm);


		Push(kVtbl_TESObjectREFR, Handle::AsTESObjectREFR);
		Push(kVtbl_MobileObject);
		Push(kVtbl_Actor);
		Push(kVtbl_Creature);
		Push(kVtbl_Character);
		Push(kVtbl_PlayerCharacter);
		Push(kVtbl_Explosion);
		Push(kVtbl_Projectile);
		Push(kVtbl_ArrowProjectile);
		Push(kVtbl_BeamProjectile);
		Push(kVtbl_ContinuousBeamProjectile);
		Push(kVtbl_FlameProjectile);
		Push(kVtbl_GrenadeProjectile);
		Push(kVtbl_MissileProjectile);


		Push(kVtbl_BaseProcess, Handle::AsBaseProcess);
		Push(kVtbl_LowProcess);
		Push(kVtbl_MiddleLowProcess);
		Push(kVtbl_MiddleHighProcess);
		Push(kVtbl_HighProcess);



		Push(kVtbl_TESTexture, Handle::AsTESTexture);
		Push(kVtbl_TESIcon);
		Push(kVtbl_QueuedTexture, Handle::AsQueuedTexture);


		// Ni
		Push(kVtbl_NiObjectNET, Handle::AsNiObjectNET);
		Push(kVtbl_NiNode);
		Push(kVtbl_BSFadeNode);
		Push(kVtbl_NiProperty);
		Push(kVtbl_NiAlphaProperty);
		Push(kVtbl_NiCamera);


		// animations
		Push(kVtbl_BSAnimGroupSequence, Handle::AsBSAnimGroupSequence);
		Push(kVtbl_AnimSequenceBase, nullptr);
		Push(kVtbl_AnimSequenceSingle, Handle::AsAnimSequenceSingle);
		Push(kVtbl_AnimSequenceMultiple, Handle::AsAnimSequenceMultiple);

		// model
		Push(kVtbl_BSFile, Handle::AsBSFile);
		Push(kVtbl_TESModel, Handle::AsTESModel);
		Push(kVtbl_QueuedModel, Handle::AsQueuedModel);

		// effects
		Push(kVtbl_ScriptEffect, Handle::AsScriptEffect);
		Push(kVtbl_ActiveEffect, Handle::AsActiveEffect);
		Push(kVtbl_AbsorbEffect);
		Push(kVtbl_AssociatedItemEffect);
		Push(kVtbl_BoundItemEffect);
		Push(kVtbl_CalmEffect);
		Push(kVtbl_ChameleonEffect);
		Push(kVtbl_CommandCreatureEffect);
		Push(kVtbl_CommandEffect);
		Push(kVtbl_CommandHumanoidEffect);
		Push(kVtbl_ConcussionEffect);
		Push(kVtbl_CureEffect);
		Push(kVtbl_DarknessEffect);
		Push(kVtbl_DemoralizeEffect);
		Push(kVtbl_DetectLifeEffect);
		Push(kVtbl_DisintegrateArmorEffect);
		Push(kVtbl_DisintegrateWeaponEffect);
		Push(kVtbl_DispelEffect);
		Push(kVtbl_FrenzyEffect);
		Push(kVtbl_InvisibilityEffect);
		Push(kVtbl_LightEffect);
		Push(kVtbl_LimbConditionEffect);
		Push(kVtbl_LockEffect);
		Push(kVtbl_MagicHitEffect);
		Push(kVtbl_MagicModelHitEffect);
		Push(kVtbl_MagicShaderHitEffect);
		Push(kVtbl_NightEyeEffect);
		Push(kVtbl_OpenEffect);
		Push(kVtbl_ParalysisEffect);
		Push(kVtbl_ReanimateEffect);
		Push(kVtbl_ShieldEffect);
		Push(kVtbl_SummonCreatureEffect);
		Push(kVtbl_SunDamageEffect);
		Push(kVtbl_TelekinesisEffect);
		Push(kVtbl_TurnUndeadEffect);
		Push(kVtbl_ValueAndConditionsEffect);
		Push(kVtbl_ValueModifierEffect);
		Push(kVtbl_VampirismEffect);

		Push(kVtbl_MagicItem, nullptr);

		Push(kVtbl_TileExtra);

		Push(kVtbl_TESFullName);
		Push(kVtbl_BaseFormComponent);
		Push(kVtbl_Setting);
		Push(kVtbl_GameSettingCollection);
		Push(kVtbl_EffectItemList);
		Push(kVtbl_ExtraCell3D);
		Push(kVtbl_BSExtraData);
		Push(kVtbl_ExtraHavok);
		Push(kVtbl_ExtraRegionList);
		Push(kVtbl_ExtraCellMusicType);
		Push(kVtbl_ExtraCellAcousticSpace);
		Push(kVtbl_ExtraCellClimate);
		Push(kVtbl_ExtraCellImageSpace);
		Push(kVtbl_ExtraCellImpactSwap);
		Push(kVtbl_ExtraCellWaterType);
		Push(kVtbl_ExtraProcessMiddleLow);
		Push(kVtbl_ExtraCellCanopyShadowMask);
		Push(kVtbl_ExtraSeenData);
		Push(kVtbl_ExtraNorthRotation);
		Push(kVtbl_ExtraDetachTime);
		Push(kVtbl_ExtraReflectedRefs);
		Push(kVtbl_ExtraReflectorRefs);
		Push(kVtbl_ExtraWaterLightRefs);
		Push(kVtbl_ExtraLitWaterRefs);
		Push(kVtbl_ExtraSpecialRenderFlags);
		Push(kVtbl_ExtraNavMeshPortal);
		Push(kVtbl_ExtraRadioData);
		Push(kVtbl_ExtraHealthPerc);
		Push(kVtbl_ExtraObjectHealth);
		Push(kVtbl_ExtraTerminalState);
		Push(kVtbl_ExtraEditorID);
		Push(kVtbl_ExtraOpenCloseActivateRef);
		Push(kVtbl_ExtraPrimitive);
		Push(kVtbl_ExtraPatrolRefData);
		Push(kVtbl_ExtraOcclusionPlaneRefData);
		Push(kVtbl_ExtraPortalRefData);
		Push(kVtbl_ExtraRoomRefData);
		Push(kVtbl_ExtraCollisionData);
		Push(kVtbl_ExtraPackageData);
		Push(kVtbl_ExtraRadius);
		Push(kVtbl_ExtraRadiation);
		Push(kVtbl_ExtraLock);
		Push(kVtbl_ExtraTeleport);
		Push(kVtbl_ExtraOwnership);
		Push(kVtbl_ExtraGlobal);
		Push(kVtbl_ExtraRank);
		Push(kVtbl_ExtraCount);
		Push(kVtbl_ExtraHealth);
		Push(kVtbl_ExtraUses);
		Push(kVtbl_ExtraTimeLeft);
		Push(kVtbl_ExtraCharge);
		Push(kVtbl_ExtraScript);
		Push(kVtbl_ExtraScale);
		Push(kVtbl_ExtraHotkey);
		Push(kVtbl_ExtraReferencePointer);
		Push(kVtbl_ExtraTresPassPackage);
		Push(kVtbl_ExtraLeveledItem);
		Push(kVtbl_ExtraPoison);
		Push(kVtbl_ExtraHeadingTarget);
		Push(kVtbl_ExtraHasNoRumors);
		Push(kVtbl_ExtraModelSwap);
		Push(kVtbl_ExtraActorCause);
		Push(kVtbl_ExtraAmmo);
		Push(kVtbl_ExtraWeaponModFlags);
		Push(kVtbl_ExtraSecuritronFace);
		Push(kVtbl_ExtraModdingItem);
		Push(kVtbl_ExtraCombatStyle);
		Push(kVtbl_ExtraAnim);
		Push(kVtbl_ExtraDismemberedLimbs);
		Push(kVtbl_ExtraStartingPosition);
		Push(kVtbl_ExtraStartingWorldOrCell);
		Push(kVtbl_ExtraLight);
		Push(kVtbl_ExtraFollower);
		Push(kVtbl_ExtraGuardedRefData);
		Push(kVtbl_ExtraLeveledCreature);
		Push(kVtbl_ExtraMapMarker);
		Push(kVtbl_ExtraAudioMarker);
		Push(kVtbl_ExtraAudioBuoyMarker);
		Push(kVtbl_ExtraAction);
		Push(kVtbl_ExtraContainerChanges);
		Push(kVtbl_ExtraOriginalReference);
		Push(kVtbl_ExtraGhost);
		Push(kVtbl_ExtraWorn);
		Push(kVtbl_ExtraWornLeft);
		Push(kVtbl_ExtraCannotWear);
		Push(kVtbl_ExtraInfoGeneralTopic);
		Push(kVtbl_ExtraSeed);
		Push(kVtbl_ExtraPackageStartLocation);
		Push(kVtbl_ExtraPackage);
		Push(kVtbl_ExtraPlayerCrimeList);
		Push(kVtbl_ExtraPersistentCell);
		Push(kVtbl_ExtraRagDollData);
		Push(kVtbl_ExtraEncounterZone);
		Push(kVtbl_ExtraUsedMarkers);
		Push(kVtbl_ExtraReservedMarkers);
		Push(kVtbl_ExtraRunOncePacks);
		Push(kVtbl_ExtraDistantData);
		Push(kVtbl_ExtraEnableStateParent);
		Push(kVtbl_ExtraEnableStateChildren);
		Push(kVtbl_ExtraRandomTeleportMarker);
		Push(kVtbl_ExtraLinkedRefChildren);
		Push(kVtbl_ExtraLinkedRef);
		Push(kVtbl_ExtraAshPileRef);
		Push(kVtbl_ExtraActivateRefChildren);
		Push(kVtbl_ExtraActivateRef);
		Push(kVtbl_ExtraDecalRefs);
		Push(kVtbl_ExtraMerchantContainer);
		Push(kVtbl_ExtraLevCreaModifier);
		Push(kVtbl_ExtraLastFinishedSequence);
		Push(kVtbl_ExtraXTarget);
		Push(kVtbl_ExtraEmittanceSource);
		Push(kVtbl_ExtraMultiBoundRef);
		Push(kVtbl_ExtraMultiBoundData);
		Push(kVtbl_ExtraMultiBound);
		Push(kVtbl_ExtraOcclusionPlane);
		Push(kVtbl_ExtraPortal);
		Push(kVtbl_ExtraRoom);
		Push(kVtbl_ExtraItemDropper);
		Push(kVtbl_ExtraDroppedItemList);
		Push(kVtbl_ExtraSavedAnimation);
		Push(kVtbl_ExtraSavedHavokData);
		Push(kVtbl_ExtraFriendHits);
		Push(kVtbl_ExtraRefractionProperty);
		Push(kVtbl_ExtraEditorRefMoveData);
		Push(kVtbl_ExtraSound);
		Push(kVtbl_ExtraCreatureAwakeSound);
		Push(kVtbl_ExtraCreatureMovementSound);
		Push(kVtbl_ExtraWeaponIdleSound);
		Push(kVtbl_ExtraWeaponAttackSound);
		Push(kVtbl_ExtraActivateLoopSound);
		Push(kVtbl_ExtraTalkingActor);
		Push(kVtbl_ExtraFactionChanges);
		Push(kVtbl_ExtraSayTopicInfoOnceADay);
		Push(kVtbl_ExtraSayToTopicInfo);
		Push(kVtbl_ExtraWaterZoneMap);
		Push(kVtbl_ExtraIgnoredBySandbox);
		Push(kVtbl_ExtraPatrolRefInUseData);
		Push(kVtbl_ExtraFollowerSwimBreadcrumbs);

		Push(kVtbl_BaseExtraList);
		Push(kVtbl_ExtraDataList);


		Push(kVtbl_BSStream);
		Push(kVtbl_AttachDistant3DTask);
		Push(kVtbl_IOTask);

		Push(kVtbl_QueuedTreeBillboard);
		Push(kVtbl_QueuedTreeModel);
		Push(kVtbl_QueuedMagicItem);
		Push(kVtbl_QueuedKF);
		Push(kVtbl_QueuedAnimIdle);
		Push(kVtbl_QueuedReplacementKF);
		Push(kVtbl_QueuedHead);
		Push(kVtbl_QueuedHelmet);
		Push(kVtbl_QueuedReference);
		Push(kVtbl_QueuedTree);
		Push(kVtbl_QueuedActor);
		Push(kVtbl_QueuedCharacter);
		Push(kVtbl_QueuedCreature);
		Push(kVtbl_QueuedPlayer);
		Push(kVtbl_QueuedFileLoad);
		Push(kVtbl_QueuedFaceGenFile);
		Push(kVtbl_QueuedReplacementKFList);

		Push(kVtbl_BackgroundCloneThread);
		Push(kVtbl_InterfacedClass);
		Push(kVtbl_INISettingCollection);

		Push(kVtbl_TES);

		Push(kVtbl_BSTempNodeManager);
		Push(kVtbl_BSTempNode);
		Push(kVtbl_RendererSettingCollection);

		Push(kVtbl_BGSBipedModelList);

		Push(kVtbl_BGSAmmoForm);
		Push(kVtbl_BGSClipRoundsForm);

		Push(kVtbl_BGSDestructibleObjectForm);
		Push(kVtbl_BGSEquipType);
		Push(kVtbl_BGSIdleCollection);
		Push(kVtbl_BGSMessageIcon);
		Push(kVtbl_BGSPickupPutdownSounds);
		Push(kVtbl_BGSRepairItemList);
		Push(kVtbl_BGSTouchSpellForm);
		Push(kVtbl_TESActorBaseData);
		Push(kVtbl_TESAIForm);
		Push(kVtbl_TESAnimation);
		Push(kVtbl_TESAttackDamageForm);
		Push(kVtbl_TESAttributes);
		Push(kVtbl_TESBipedModelForm);
		Push(kVtbl_TESModelRDT);
		Push(kVtbl_TESContainer);
		Push(kVtbl_TESDescription);
		Push(kVtbl_TESEnchantableForm);

		Push(kVtbl_TESImageSpaceModifiableForm);
		Push(kVtbl_TESLeveledList);

		Push(kVtbl_TESModelList);
		Push(kVtbl_TESModelTextureSwap);

		Push(kVtbl_TESHealthForm);

		Push(kVtbl_TESProduceForm);
		Push(kVtbl_TESRaceForm);
		Push(kVtbl_TESReactionForm);
		Push(kVtbl_TESScriptableForm);

		Push(kVtbl_TESValueForm);
		Push(kVtbl_TESWeightForm);


		Push(kVtbl_TESSpellList);

		Push(kVtbl_AnimIdle);
		Push(kVtbl_NiRefObject);

		Push(kVtbl_NiStencilProperty);
		Push(kVtbl_BSCullingProcess);

		Push(kVtbl_hkpAllRayHitCollector);
		Push(kVtbl_hkpRayHitCollector);

		Push(kVtbl_BGSPrimitive);
		Push(kVtbl_BGSPrimitivePlane);
		Push(kVtbl_BSOcclusionPlane);
		Push(kVtbl_BSPortal);
		Push(kVtbl_BGSPrimitiveBox);

		Push(kVtbl_BSMultiBoundAABB);
		Push(kVtbl_BSMultiBoundOBB);
		Push(kVtbl_BGSPrimitiveSphere);
		Push(kVtbl_BSMultiBoundSphere);
		Push(kVtbl_BGSSceneInfo);
		Push(kVtbl_BSAnimNoteReceiver);
		Push(kVtbl_IBSAnimNoteReceiver);

		Push(kVtbl_bhkMouseSpringAction);
		Push(kVtbl_bhkUnaryAction);
		Push(kVtbl_bhkAction);
		Push(kVtbl_bhkSerializable);
		Push(kVtbl_GridArray);
		Push(kVtbl_GridCellArray);
		Push(kVtbl_INIPrefSettingCollection);

		Push(kVtbl_BSCustomSplatterExtraData);
		Push(kVtbl_BSSplatterExtraData);
		Push(kVtbl_ImageSpaceEffectParam);
		Push(kVtbl_BGSDecalNode);

		Push(kVtbl_TESRegionData);
		Push(kVtbl_TESRegionDataGrass);
		Push(kVtbl_TESRegionDataImposter);
		Push(kVtbl_TESRegionDataLandscape);
		Push(kVtbl_TESRegionDataManager);
		Push(kVtbl_TESRegionDataMap);
		Push(kVtbl_TESRegionDataSound);
		Push(kVtbl_TESRegionDataWeather);
		Push(kVtbl_TESRegionGrassObject);
		Push(kVtbl_TESRegionObjectBase);
		Push(kVtbl_TESRegionGrassObjectList);
		Push(kVtbl_TESRegionList);

		Push(kVtbl_BGSAddonNode);
		Push(kVtbl_BGSAddonNodeSoundHandleExtra);

		Push(kVtbl_BGSPreloadable);

		Push(kVtbl_PathingLocation);

		Push(kVtbl_BGSOpenCloseForm);

		Push(kVtbl_TESModelTree);
		Push(kVtbl_TESIconTree);
		Push(kVtbl_TESTexture1024);
		Push(kVtbl_NiBillboardNode);
		Push(kVtbl_BSShaderTextureSet);
		Push(kVtbl_BSTextureSet);
		Push(kVtbl_BGSEncounterZone);
		Push(kVtbl_BGSLightingTemplate);
		Push(kVtbl_LoadedReferenceMap);
		Push(kVtbl_ExteriorCellLoaderTask);
		Push(kVtbl_ImageSpaceModifierInstanceForm);
		Push(kVtbl_ImageSpaceModifierInstance);
		Push(kVtbl_ImageSpaceModifierInstanceDOF);
		Push(kVtbl_ImageSpaceModifierInstanceRB);
		Push(kVtbl_ImageSpaceModifierInstanceDRB);
		Push(kVtbl_ImageSpaceModifierInstanceTemp);

		Push(kVtbl_NiFloatData);
		Push(kVtbl_NiColorData);
		Push(kVtbl_TESObjectLAND);
		Push(kVtbl_TESChildCell);
		Push(kVtbl_BSPackedAdditionalGeometryData);
		Push(kVtbl_NiAdditionalGeometryData);
		Push(kVtbl_hkpMoppCode);
		Push(kVtbl_hkReferencedObject);
		Push(kVtbl_hkBaseObject);
		Push(kVtbl_NiBinaryExtraData);

		Push(kVtbl_CheckWithinMultiBoundTask);
		Push(kVtbl_hkpWorldCinfo);
		Push(kVtbl_TESWindListener);
		Push(kVtbl_hkpEntityListener);

		Push(kVtbl_bhkRigidBody);
		Push(kVtbl_bhkEntity);
		Push(kVtbl_bhkWorldObject);
		Push(kVtbl_bhkCollisionObject);
		Push(kVtbl_bhkWaterPhantomCallbackShape);
		Push(kVtbl_hkpPhantomCallbackShape);
		Push(kVtbl_hkpShape);
		Push(kVtbl_bhkSimpleShapePhantom);
		Push(kVtbl_bhkShapePhantom);
		Push(kVtbl_bhkPhantom);
		Push(kVtbl_bhkAabbPhantom);
		Push(kVtbl_bhkBoxShape);
		Push(kVtbl_bhkConvexShape);
		Push(kVtbl_bhkSphereRepShape);
		Push(kVtbl_bhkShape);
		Push(kVtbl_bhkSphereShape);
		Push(kVtbl_bhkAcousticSpacePhantomCallbackShape);
		Push(kVtbl_ExtraAnimNoteReceiver);


		Push(kVtbl_BGSAcousticSpace);
		Push(kVtbl_BGSCameraPath);
		Push(kVtbl_NiFormArray);
		Push(kVtbl_BGSCameraShot);
		Push(kVtbl_BGSDefaultObjectManager);
		Push(kVtbl_BGSImpactData);
		Push(kVtbl_BGSImpactDataSet);
		Push(kVtbl_BGSMenuIcon);
		Push(kVtbl_BGSMusicType);
		Push(kVtbl_TESSoundFile);
		Push(kVtbl_BGSRadiationStage);
		Push(kVtbl_BGSTextureSet);
		Push(kVtbl_MediaLocationController);
		Push(kVtbl_MediaSet);
		// w
		Push(kVtbl_hkpClosestRayHitCollector);

		Push(kVtbl_TESLoadScreenType);

		Push(kVtbl_MobIterOperator);
		Push(kVtbl_BGSBodyPart);
		Push(kVtbl_TESModelPSA);
		Push(kVtbl_BGSBodyPartData);
		Push(kVtbl_BGSEntryPointFunctionDataOneValue);
		Push(kVtbl_BGSEntryPointFunctionData);
		Push(kVtbl_BGSEntryPointFunctionDataTwoValue);
		Push(kVtbl_BGSEntryPointFunctionDataLeveledList);
		Push(kVtbl_BGSEntryPointFunctionDataActivateChoice);
		Push(kVtbl_BGSIdleMarker);
		Push(kVtbl_BGSPerkEntry);
		Push(kVtbl_BGSQuestPerkEntry);
		Push(kVtbl_BGSAbilityPerkEntry);
		Push(kVtbl_BGSEntryPointPerkEntry);
		Push(kVtbl_BGSQuestObjective);
		Push(kVtbl_BGSRagdoll);
		Push(kVtbl_BGSVoiceType);
		Push(kVtbl_Reset3DMobIterator);
		Push(kVtbl_TESAnimGroup);

		Push(kVtbl_ActorValueOwner);

		Push(kVtbl_TESModelAnim);

		Push(kVtbl_BGSTextureModel);

		Push(kVtbl_TopicInfoArray);

		Push(kVtbl_BGSAcousticSpaceListener);
		Push(kVtbl_bhkEntityListener);
		Push(kVtbl_TargetEntry);
		Push(kVtbl_ZoneEntry);
		Push(kVtbl_TrapTargetEntry);
		Push(kVtbl_BGSZoneTargetListener);
		Push(kVtbl_hkpAllCdPointCollector);
		Push(kVtbl_hkpCdPointCollector);
		Push(kVtbl_CellMopp);
		Push(kVtbl_bhkTriSampledHeightFieldBvTreeShape);
		Push(kVtbl_bhkBvTreeShape);
		Push(kVtbl_FOCollisionListener);
		Push(kVtbl_bhkContactListener);
		Push(kVtbl_hkpContactListener);
		Push(kVtbl_FOIslandActivationListener);
		Push(kVtbl_hkpIslandActivationListener);
		Push(kVtbl_HavokStreambufFactory);
		Push(kVtbl_hkFileSystem);
		Push(kVtbl_HavokError);
		Push(kVtbl_hkError);
		Push(kVtbl_hkStreamReader);
		Push(kVtbl_hkStreamWriter);
		Push(kVtbl_HavokFileStreambufReader);
		Push(kVtbl_HavokFileStreambufWriter);
		Push(kVtbl_LoadedAreaBound);
		Push(kVtbl_SpecificItemCollector);
		Push(kVtbl_TESArrowTargetListener);
		Push(kVtbl_bhkArrowTargetListener);
		Push(kVtbl_hkpAllCdBodyPairCollector);
		Push(kVtbl_hkpCdBodyPairCollector);
		Push(kVtbl_TESTrapListener);
		Push(kVtbl_bhkTrapListener);
		Push(kVtbl_hkpPhantomListener);
		Push(kVtbl_TrapEntry);
		Push(kVtbl_WaterTargetEntry);
		Push(kVtbl_TESWaterListener);
		Push(kVtbl_WaterZone);
		Push(kVtbl_BlendSettingCollection);
		Push(kVtbl_Atmosphere);
		Push(kVtbl_Clouds);
		Push(kVtbl_Moon);
		Push(kVtbl_Precipitation);
		Push(kVtbl_Sky);
		Push(kVtbl_SkyObject);
		Push(kVtbl_SkyTask);
		Push(kVtbl_Stars);
		Push(kVtbl_Sun);
		Push(kVtbl_BSFaceGenAnimationData);
		Push(kVtbl_BSFaceGenImage);
		Push(kVtbl_BSFaceGenKeyframe);
		Push(kVtbl_BSFaceGenKeyframeMultiple);
		Push(kVtbl_BSFaceGenModelMap);
		Push(kVtbl_BSFaceGenModel);
		Push(kVtbl_BSFaceGenModelExtraData);
		Push(kVtbl_BSFaceGenMorph);
		Push(kVtbl_BSFaceGenMorphDifferential);
		Push(kVtbl_BSFaceGenMorphStatistical);
		Push(kVtbl_BSFaceGenMorphDataHead);
		Push(kVtbl_BSFaceGenMorphData);
		Push(kVtbl_BSFaceGenMorphDataHair);
		Push(kVtbl_BSFaceGenBaseMorphExtraData);
		Push(kVtbl_BSFaceGenNiNode);
		Push(kVtbl_BSTreeModel);
		Push(kVtbl_BSTreeNode);
		Push(kVtbl_bhkSPCollisionObject);
		Push(kVtbl_bhkPCollisionObject);
		Push(kVtbl_bhkMultiSphereShape);
		Push(kVtbl_bhkTransformShape);
		Push(kVtbl_bhkCapsuleShape);
		Push(kVtbl_ActorValueInfo);

		Push(kVtbl_TESAmbushPackageData);
		Push(kVtbl_TESPackageData);
		Push(kVtbl_TESDialoguePackageData);
		Push(kVtbl_TESEatPackageData);
		Push(kVtbl_TESEscortPackageData);
		Push(kVtbl_TESFollowPackageData);
		Push(kVtbl_TESPatrolPackageData);
		Push(kVtbl_TESUseItemPackageData);
		Push(kVtbl_TESUseWeaponPackageData);
		Push(kVtbl_UseWeaponActorPackageData);
		Push(kVtbl_ActorPackageData);
		Push(kVtbl_NiTask);
		Push(kVtbl_BSTempEffect);
		Push(kVtbl_BSTempEffectDebris);
		Push(kVtbl_BSTempEffectGeometryDecal);
		Push(kVtbl_BSDismemberSkinInstance);
		Push(kVtbl_NiSkinInstance);
		Push(kVtbl_BSTempEffectParticle);
		Push(kVtbl_BSTempEffectSimpleDecal);

		Push(kVtbl_NavMeshPOVSearch);
		Push(kVtbl_NavMeshSearch);
		Push(kVtbl_NavMeshSearchClosePoint);
		Push(kVtbl_NavMeshSearchFlee);
		Push(kVtbl_NavMeshSearchHide);
		Push(kVtbl_NavMeshSearchLOS);
		Push(kVtbl_NavMeshSearchMaxCost);
		Push(kVtbl_NavMeshSearchSLPoint);
		Push(kVtbl_PathSmootherPOVSearch);
		Push(kVtbl_NavMeshInfoMap);
		Push(kVtbl_NavMeshInfoSearch);
		Push(kVtbl_ObstacleUndoData);
		Push(kVtbl_bhkObstacleDeactivationListener);
		Push(kVtbl_hkpEntityActivationListener);
		Push(kVtbl_bhkObstacleRemovalListener);
		Push(kVtbl_ReferenceObstacleArray);
		Push(kVtbl_ObstacleData);
		Push(kVtbl_BSTasklet);
		Push(kVtbl_ObstacleTaskData);
		Push(kVtbl_BSWin32TaskletData);
		Push(kVtbl_BSTaskletData);
		Push(kVtbl_FindTriangleForLocationFilter);
		Push(kVtbl_CheckLineOfSightFilterWater);
		Push(kVtbl_PathingCoverLocation);
		Push(kVtbl_PathingLOSGridMap);
		Push(kVtbl_PathingLOSMap);
		Push(kVtbl_PathingRequest);
		Push(kVtbl_PathingAvoidNodeArray);
		Push(kVtbl_PathingRequestClosePoint);
		Push(kVtbl_PathingRequestCover);
		Push(kVtbl_PathingRequestCoveredMove);
		Push(kVtbl_PathingRequestFlee);
		Push(kVtbl_PathingRequestHide);
		Push(kVtbl_PathingRequestLOS);
		Push(kVtbl_PathingRequestOptimalLocation);
		Push(kVtbl_PathingRequestSafeStraightLine);
		Push(kVtbl_PathingSearchRayCast);
		Push(kVtbl_PathingSolution);
		Push(kVtbl_PathingTaskData);
		Push(kVtbl_TeleportDoorSearch);
		Push(kVtbl_BGSDistantObjectBlockLoadTask);
		Push(kVtbl_BGSDistantTreeBlockLoadTask);
		Push(kVtbl_BGSTerrainChunkLoadTask);
		Push(kVtbl_CCallbackBase);
		Push(kVtbl_FORenderedTerminal);


		Push(kVtbl_ListBoxBase);


		Push(kVtbl_LoadingMenuThread);


		Push(kVtbl_FORenderedMenuRaceSex);


		Push(kVtbl_FOPipboyManager);
		Push(kVtbl_FORenderedMenu);


		Push(kVtbl_bhkCharacterListenerSpell);
		Push(kVtbl_bhkTelekinesisListener);

		Push(kVtbl_MagicBallProjectile);
		Push(kVtbl_MagicBoltProjectile);
		Push(kVtbl_MagicCaster);
		Push(kVtbl_MagicFogProjectile);




		Push(kVtbl_MagicProjectile);
		Push(kVtbl_MagicSprayProjectile);
		Push(kVtbl_MagicTarget);
		Push(kVtbl_NonActorMagicCaster);
		Push(kVtbl_NonActorMagicTarget);

		Push(kVtbl_CBSAStream);
		Push(kVtbl_CAsyncStream);
		Push(kVtbl_CBSAReader);
		Push(kVtbl_RadioConvTask);
		Push(kVtbl_BGSReconstructFormsMap);
		Push(kVtbl_BGSReconstructCellsMap);
		Push(kVtbl_BGSReconstructCellSubBlocksMap);
		Push(kVtbl_BGSReconstructCellBlocksMap);
		Push(kVtbl_BGSReconstructWorldSpacesMap);
		Push(kVtbl_BGSReconstructFormsInFileMap);
		Push(kVtbl_BGSOffsetDataArray);
		Push(kVtbl_BGSReconstructFormsInAllFilesMap);
		Push(kVtbl_BGSSaveLoadChangesMap);
		Push(kVtbl_BGSCellNumericIDArrayMap);
		Push(kVtbl_BGSSaveLoadStatsMap);
		Push(kVtbl_ChangesMap);
		Push(kVtbl_InteriorCellNewReferencesMap);
		Push(kVtbl_ExteriorCellNewReferencesMap);
		Push(kVtbl_NumericIDBufferMap);
		Push(kVtbl_SaveGameFile);
		Push(kVtbl_BGSLoadFormBuffer);
		Push(kVtbl_BGSLoadGameBuffer);
		Push(kVtbl_BGSSaveFormBuffer);
		Push(kVtbl_BGSSaveGameBuffer);
		Push(kVtbl_BGSMoviePlayer);
		Push(kVtbl_HighActorCuller);
		Push(kVtbl_BSFadeNodeCuller);
		Push(kVtbl_RegSettingCollection);
		Push(kVtbl_MemoryManagerFile);
		Push(kVtbl_IMemoryManagerFile);
		Push(kVtbl_SceneGraph);
		Push(kVtbl_SeenData);
		Push(kVtbl_IntSeenData);

		Push(kVtbl_CachedValuesOwner);
		Push(kVtbl_bhkFilteredCdBodyCollector);
		Push(kVtbl_AILinearTaskThread);
		Push(kVtbl_BSLinearTaskThread);
		Push(kVtbl_DetectionTaskData);
		Push(kVtbl_MobileObjectTaskletData);
		Push(kVtbl_AnimationTaskData);
		Push(kVtbl_ActorUpdateTaskData);
		Push(kVtbl_PackageUpdateTaskData);
		Push(kVtbl_ActorsScriptTaskData);
		Push(kVtbl_MovementTaskData);
		Push(kVtbl_AITaskThread);

		Push(kVtbl_bhkCharacterListenerArrow);
		Push(kVtbl_BSPlayerDistanceCheckController);


		Push(kVtbl_ActorPathingMessageQueue);
		Push(kVtbl_LipTask);

		Push(kVtbl_CombatAction);
		Push(kVtbl_CombatActionAttackRanged);
		Push(kVtbl_CombatActionAttackRangedFromCover);
		Push(kVtbl_CombatActionAttackRangedExplosive);
		Push(kVtbl_CombatActionAttackRangedExplosiveFromCover);
		Push(kVtbl_CombatActionAttackMelee);
		Push(kVtbl_CombatActionAttackHandToHand);
		Push(kVtbl_CombatActionAttackGrenade);
		Push(kVtbl_CombatActionAttackGrenadeFlushTarget);
		Push(kVtbl_CombatActionAttackGrenadeFromCover);
		Push(kVtbl_CombatActionMove);
		Push(kVtbl_CombatActionMoveAndSwitchToMelee);
		Push(kVtbl_CombatActionMoveAndAttackRanged);
		Push(kVtbl_CombatActionMoveAndAttackRangedExplosive);
		Push(kVtbl_CombatActionMoveAndAttackGrenade);
		Push(kVtbl_CombatActionDrawWeapon);
		Push(kVtbl_CombatActionSwitchWeapon);
		Push(kVtbl_CombatActionAvoidThreat);
		Push(kVtbl_CombatActionSearch);
		Push(kVtbl_CombatActionInvestigate);
		Push(kVtbl_CombatActionDodge);
		Push(kVtbl_CombatActionIgnoreBlockedTarget);
		Push(kVtbl_CombatActionFlee);
		Push(kVtbl_CombatActionActivateCombatItem);
		Push(kVtbl_CombatActionUseCombatItem);
		Push(kVtbl_CombatActionApproachTarget);
		Push(kVtbl_CombatActionAcquireLineOfSight);
		Push(kVtbl_CombatActionHide);
		Push(kVtbl_CombatController);
		Push(kVtbl_CombatPlanner);
		Push(kVtbl_CombatCoverMap);
		Push(kVtbl_CombatGoal);
		Push(kVtbl_CombatGoalKillTarget);
		Push(kVtbl_CombatGoalFindTarget);
		Push(kVtbl_CombatGoalAvoidThreats);
		Push(kVtbl_CombatGoalFlee);
		Push(kVtbl_CombatGoalActivateCombatItem);
		Push(kVtbl_CombatGoalUseCombatItem);
		Push(kVtbl_CombatGoalFlank);
		Push(kVtbl_CombatGoalFallBack);
		Push(kVtbl_CombatGoalAmbush);
		Push(kVtbl_CombatGoalReachDestination);
		Push(kVtbl_CombatGoalAdvance);
		Push(kVtbl_CombatGoalGuard);
		Push(kVtbl_CombatGoalCluster);
		Push(kVtbl_CombatGoalHideFromTarget);
		Push(kVtbl_CombatGoalFlushTarget);
		Push(kVtbl_CombatGoalStayInCombatArea);
		Push(kVtbl_CombatGroupStrategyFlank);
		Push(kVtbl_CombatGroupStrategy);
		Push(kVtbl_CombatGroupStrategyFallBack);
		Push(kVtbl_CombatGroupStrategyAmbush);
		Push(kVtbl_CombatGroupStrategyAdvance);
		Push(kVtbl_CombatTargetMap);
		Push(kVtbl_CombatThreatMap);

		Push(kVtbl_bhkLiquidAction);
		Push(kVtbl_hkpFixedBufferCdPointCollector);

		Push(kVtbl_bhkHingeConstraint);
		Push(kVtbl_bhkConstraint);
		Push(kVtbl_hkHingeConstraintCinfo);
		Push(kVtbl_hkConstraintCinfo);

		Push(kVtbl_ProjectileListener);
		Push(kVtbl_CombatProcedureActivateObject);
		Push(kVtbl_CombatProcedure);
		Push(kVtbl_CombatProcedureAttackGrenade);
		Push(kVtbl_CombatProcedureAttackLow);
		Push(kVtbl_CombatProcedureAttackMelee);
		Push(kVtbl_CombatProcedureAttackRanged);
		Push(kVtbl_CombatProcedureBeInCover);
		Push(kVtbl_CombatProcedureEngageTarget);
		Push(kVtbl_CombatProcedureEvade);
		Push(kVtbl_CombatProcedureHideFromTarget);
		Push(kVtbl_CombatProcedureMove);
		Push(kVtbl_CombatProcedureSearch);
		Push(kVtbl_CombatProcedureSwitchWeapon);
		Push(kVtbl_CombatProcedureUseCombatItem);
		Push(kVtbl_ActorMover);
		Push(kVtbl_ActorPathHandler);
		Push(kVtbl_DetailedActorPathHandler);
		Push(kVtbl_DoorInPathFilter);
		Push(kVtbl_PlayerMover);
		Push(kVtbl_VirtualActorPathHandler);

		Push(kVtbl_CEnumPins);
		Push(kVtbl_CEnumMediaTypes);
		Push(kVtbl_CMediaSample);
		Push(kVtbl_CMemAllocator);
		Push(kVtbl_CAsyncOutputPin);
		Push(kVtbl_CAsyncReader);

		Push(kVtbl_DebugText);
		Push(kVtbl_IDebugText);
		Push(kVtbl_DebugTextExtraData);

		Push(kVtbl_NiAVObjectPalette);
		Push(kVtbl_NiControllerManager);
		Push(kVtbl_NiDefaultAVObjectPalette);
		Push(kVtbl_NiInterpController);
		Push(kVtbl_NiMultiTargetTransformController);
		Push(kVtbl_NiControllerSequence);
		Push(kVtbl_BSMultiTargetTreadTransfController);
		Push(kVtbl_NiBlendAccumTransformInterpolator);
		Push(kVtbl_NiTransformController);
		Push(kVtbl_NiBlendInterpolator);
		Push(kVtbl_NiSingleInterpController);
		Push(kVtbl_NiGeomMorpherController);
		Push(kVtbl_NiColorInterpolator);
		Push(kVtbl_NiFloatInterpolator);
		Push(kVtbl_NiKeyBasedInterpolator);
		Push(kVtbl_NiPosData);
		Push(kVtbl_NiPathInterpolator);
		Push(kVtbl_NiTransformInterpolator);
		Push(kVtbl_NiBlendFloatInterpolator);
		Push(kVtbl_NiBlendTransformInterpolator);
		Push(kVtbl_NiFloatExtraDataController);
		Push(kVtbl_NiMorphData);
		Push(kVtbl_BSFrustumFOVController);
		Push(kVtbl_BSBlendTreadTransfInterpolator);
		Push(kVtbl_BSTreadTransfInterpolator);
		Push(kVtbl_BSRotAccumTransfInterpolator);
		Push(kVtbl_BSAnimNote);
		Push(kVtbl_BSLookIKNote);
		Push(kVtbl_BSGrabIKNote);
		Push(kVtbl_BSAnimNotes);
		Push(kVtbl_NiVisController);
		Push(kVtbl_NiUVData);
		Push(kVtbl_NiUVController);
		Push(kVtbl_NiTextureTransformController);
		Push(kVtbl_NiTextKeyExtraData);
		Push(kVtbl_NiStringPalette);
		Push(kVtbl_NiSequenceStreamHelper);
		Push(kVtbl_NiSequence);
		Push(kVtbl_NiRotData);
		Push(kVtbl_NiFloatController);
		Push(kVtbl_NiRollController);
		Push(kVtbl_NiQuaternionInterpolator);
		Push(kVtbl_NiPoint3Interpolator);
		Push(kVtbl_NiPathController);
		Push(kVtbl_NiMaterialColorController);
		Push(kVtbl_NiLookAtInterpolator);
		Push(kVtbl_NiLookAtController);
		Push(kVtbl_NiLightDimmerController);
		Push(kVtbl_NiLightColorController);
		Push(kVtbl_NiKeyframeManager);
		Push(kVtbl_NiTransformData);
		Push(kVtbl_NiFloatsExtraDataPoint3Controller);
		Push(kVtbl_NiFloatsExtraDataController);
		Push(kVtbl_NiFlipController);
		Push(kVtbl_NiColorExtraDataController);
		Push(kVtbl_NiBSplineTransformInterpolator);
		Push(kVtbl_NiBSplinePoint3Interpolator);
		Push(kVtbl_NiBSplineFloatInterpolator);
		Push(kVtbl_NiBSplineCompTransformInterpolator);
		Push(kVtbl_NiBSplineCompPoint3Interpolator);
		Push(kVtbl_NiBSplineCompFloatInterpolator);
		Push(kVtbl_NiBSplineCompColorInterpolator);
		Push(kVtbl_NiBSplineColorInterpolator);
		Push(kVtbl_NiBSplineData);
		Push(kVtbl_NiBSplineBasisData);
		Push(kVtbl_NiBoolTimelineInterpolator);
		Push(kVtbl_NiBoolInterpolator);
		Push(kVtbl_NiBoolData);
		Push(kVtbl_NiBoneLODController);
		Push(kVtbl_NiBlendQuaternionInterpolator);
		Push(kVtbl_NiBlendPoint3Interpolator);
		Push(kVtbl_NiBlendColorInterpolator);
		Push(kVtbl_NiBlendBoolInterpolator);
		Push(kVtbl_NiAlphaController);
		Push(kVtbl_NiGeomMorpherUpdateTask);
		Push(kVtbl_NiInterpolator);
		Push(kVtbl_NiExtraDataController);
		Push(kVtbl_NiFloatInterpController);
		Push(kVtbl_NiBoolInterpController);
		Push(kVtbl_NiPoint3InterpController);
		Push(kVtbl_NiBSplineInterpolator);
		Push(kVtbl_NiAVObject);
		Push(kVtbl_NiObject);

		Push(kVtbl_NiVertexColorProperty);
		Push(kVtbl_NiZBufferProperty);
		Push(kVtbl_NiTexture);
		Push(kVtbl_NiSourceTexture);
		Push(kVtbl_NiSourceCubeMap);
		Push(kVtbl_NiRenderer);
		Push(kVtbl_NiStream);
		Push(kVtbl_NiGeometryData);
		Push(kVtbl_NiDynamicEffect);
		Push(kVtbl_NiCullingProcess);
		Push(kVtbl_NiTexturingProperty);
		Push(kVtbl_NiTimeController);

		Push(kVtbl_NiTriStrips);
		Push(kVtbl_NiLight);
		Push(kVtbl_NiStringExtraData);
		Push(kVtbl_NiTriShape);
		Push(kVtbl_NiLines);
		Push(kVtbl_NiMaterialProperty);
		Push(kVtbl_NiDirectionalLight);
		Push(kVtbl_NiTriStripsData);
		Push(kVtbl_NiImageConverter);
		Push(kVtbl_NiDevImageConverter);
		Push(kVtbl_NiExtraData);
		Push(kVtbl_NiTriShapeData);
		Push(kVtbl_NiPixelData);
		Push(kVtbl_NiPointLight);
		Push(kVtbl_NiGeometryGroupManager);
		Push(kVtbl_NiRenderedTexture);
		Push(kVtbl_NiGeometry);
		Push(kVtbl_NiCollisionObject);
		Push(kVtbl_NiShadeProperty);
		Push(kVtbl_NiTriShapeDynamicData);
		Push(kVtbl_NiSkinPartition);
		Push(kVtbl_NiSkinData);
		Push(kVtbl_BSResizableTriShape);
		Push(kVtbl_BSSegmentedTriShape);
		Push(kVtbl_NiScreenElements);
		Push(kVtbl_NiScreenElementsData);
		Push(kVtbl_NiFloatsExtraData);
		Push(kVtbl_NiColorExtraData);
		Push(kVtbl_NiTriBasedGeom);
		Push(kVtbl_NiVertWeightsExtraData);
		Push(kVtbl_NiLinesData);
		Push(kVtbl_NiTriBasedGeomData);
		Push(kVtbl_NiImageReader);
		Push(kVtbl_NiNIFImageReader);
		Push(kVtbl_NiPalette);
		Push(kVtbl_NiDDSReader);
		Push(kVtbl_NiTGAReader);
		Push(kVtbl_NiBMPReader);
		Push(kVtbl_Ni2DBuffer);
		Push(kVtbl_NiShader);
		Push(kVtbl_NiCullingProperty);
		Push(kVtbl_NiMaterial);
		Push(kVtbl_NiSingleShaderMaterialLibrary);
		Push(kVtbl_NiMaterialLibrary);
		Push(kVtbl_NiScreenGeometry);
		Push(kVtbl_NiScreenPolygon);
		Push(kVtbl_NiScreenGeometryData);
		Push(kVtbl_NiScreenSpaceCamera);
		Push(kVtbl_NiVectorExtraData);
		Push(kVtbl_NiTriStripsDynamicData);
		Push(kVtbl_NiTextureEffect);
		Push(kVtbl_NiSwitchStringExtraData);
		Push(kVtbl_NiSwitchNode);
		Push(kVtbl_NiStringsExtraData);
		Push(kVtbl_NiSpotLight);
		Push(kVtbl_NiSpecularProperty);
		Push(kVtbl_NiSortAdjustNode);
		Push(kVtbl_NiScreenTexture);
		Push(kVtbl_NiScreenLODData);
		Push(kVtbl_NiParticlesData);
		Push(kVtbl_NiRangeLODData);
		Push(kVtbl_NiPersistentSrcTextureRendererData);
		Push(kVtbl_NiParticleMeshesData);
		Push(kVtbl_NiParticleMeshes);
		Push(kVtbl_NiLODNode);
		Push(kVtbl_NiIntegersExtraData);
		Push(kVtbl_NiIntegerExtraData);
		Push(kVtbl_NiFogProperty);
		Push(kVtbl_NiFloatExtraData);
		Push(kVtbl_NiDitherProperty);
		Push(kVtbl_NiBSPNode);
		Push(kVtbl_NiBooleanExtraData);
		Push(kVtbl_NiParticles);
		Push(kVtbl_NiAmbientLight);
		Push(kVtbl_NiAlphaAccumulator);
		Push(kVtbl_NiShaderConstantMap);
		Push(kVtbl_NiDepthStencilBuffer);
		Push(kVtbl_NiGlobalConstantEntry);
		Push(kVtbl_NiShaderFactory);
		Push(kVtbl_NiSingleShaderMaterial);
		Push(kVtbl_NiLODData);
		Push(kVtbl_NiAccumulator);
		Push(kVtbl_NiBackToFrontAccumulator);
		Push(kVtbl_NiFile);
		Push(kVtbl_NiBinaryStream);
		Push(kVtbl_NiAllocator);
		Push(kVtbl_NiStandardAllocator);
		Push(kVtbl_BSNiAllocator);
		Push(kVtbl_NiSearchPath);
		Push(kVtbl_NiMemStream);
		Push(kVtbl_BSCoreMessage);
		Push(kVtbl_BSThread);
		Push(kVtbl_ZeroOverheadHeap);
		Push(kVtbl_IMemoryHeap);
		Push(kVtbl_MemoryHeap);
		Push(kVtbl_AbstractHeap);
		Push(kVtbl_FutBinaryFileC);
		Push(kVtbl_BSFaceGenBinaryFile);

		Push(kVtbl_BSAudio);
		Push(kVtbl_BSAudioManager);
		Push(kVtbl_BSAudioManagerThread);
		Push(kVtbl_AudioLoadTask);
		Push(kVtbl_BSGameSound);

		Push(kVtbl_BSWin32Audio);
		Push(kVtbl_BSWin32AudioListener);
		Push(kVtbl_BSAudioListener);
		Push(kVtbl_BSWin32GameSound);
		Push(kVtbl_BSSaveDataSystemUtility);
		Push(kVtbl_BSSystemUtility);
		Push(kVtbl_BSMsgDialogSystemUtility);
		Push(kVtbl_BSSystemUtilitiesStrings);
		Push(kVtbl_BSCacheDriveSystemUtility);
		Push(kVtbl_BSGameDataSystemUtility);
		Push(kVtbl_BSDiscBootSystemUtility);
		Push(kVtbl_BSSysInfoSystemUtility);
		Push(kVtbl_Archive);
		Push(kVtbl_ArchiveFile);
		Push(kVtbl_CompressedArchiveFile);

		Push(kVtbl_BSTaskletManagerCallback);
		Push(kVtbl_BSFileCache);
		Push(kVtbl_BSSearchPath);
		Push(kVtbl_BSWin32FileCache);
		Push(kVtbl_BSWin32TaskletManager);
		Push(kVtbl_BSTaskletManager);
		Push(kVtbl_BSWin32TaskletGroupData);
		Push(kVtbl_BSTaskletGroupData);
		Push(kVtbl_IdvFileError);
		Push(kVtbl_CTreeEngine);
		Push(kVtbl_CIdvCamera);
		Push(kVtbl_Uniform);
		Push(kVtbl_Random);
		Push(kVtbl_CBillboardLeaf);
		Push(kVtbl_PosGen);
		Push(kVtbl_Normal);
		Push(kVtbl_SymGen);

		Push(kVtbl_DistantLODShaderProperty);
		Push(kVtbl_TallGrassShaderProperty);
		Push(kVtbl_BSShaderPPLightingProperty);
		Push(kVtbl_WaterShaderProperty);
		Push(kVtbl_BSShaderNoLightingProperty);
		Push(kVtbl_BSShaderLightingProperty);
		Push(kVtbl_BSShaderBloodSplatterProperty);
		Push(kVtbl_BSBatchRenderer);
		Push(kVtbl_LightingData);
		Push(kVtbl_ParticleShader);
		Push(kVtbl_ParticleShaderProperty);
		Push(kVtbl_SpeedTreeBillboardShaderProperty);
		Push(kVtbl_BSShaderProperty);
		Push(kVtbl_TallGrassShader);
		Push(kVtbl_SkyShaderProperty);
		Push(kVtbl_PrecipitationShader);
		Push(kVtbl_PrecipitationShaderProperty);
		Push(kVtbl_SpeedTreeLeafShader);
		Push(kVtbl_SpeedTreeBranchShaderProperty);
		Push(kVtbl_SpeedTreeShaderPPLightingProperty);
		Push(kVtbl_SpeedTreeShaderLightingProperty);
		Push(kVtbl_SpeedTreeLeafShaderProperty);
		Push(kVtbl_GeometryDecalShaderProperty);
		Push(kVtbl_Lighting30ShaderProperty);
		Push(kVtbl_BSDistantTreeShader);
		Push(kVtbl_BSDistantTreeShaderProperty);
		Push(kVtbl_TileShaderProperty);
		Push(kVtbl_BSFogProperty);
		Push(kVtbl_Lighting30Shader);
		Push(kVtbl_BoltShader);
		Push(kVtbl_BoltShaderProperty);
		Push(kVtbl_BSClearZNode);
		Push(kVtbl_BSOrderedNode);
		Push(kVtbl_BSMaterialEmittanceMultController);
		Push(kVtbl_BSRefractionFirePeriodController);
		Push(kVtbl_BSRefractionStrengthController);
		Push(kVtbl_VolumetricFogShaderProperty);
		Push(kVtbl_HairShaderProperty);
		Push(kVtbl_BeamShader);
		Push(kVtbl_VolumetricFogShader);
		Push(kVtbl_BSShaderNoLighting);
		Push(kVtbl_TileShader);
		Push(kVtbl_GeometryDecalShader);
		Push(kVtbl_ParallaxShader);
		Push(kVtbl_SkinShader);
		Push(kVtbl_HairShader);
		Push(kVtbl_SpeedTreeBranchShader);
		Push(kVtbl_DistantLODShader);
		Push(kVtbl_BSCubeMapCamera);
		Push(kVtbl_NiParticleSystem);
		Push(kVtbl_NiPSysEmitterCtlr);
		Push(kVtbl_NiPSysMeshEmitter);
		Push(kVtbl_NiPSysCylinderEmitter);
		Push(kVtbl_NiPSysSphereEmitter);
		Push(kVtbl_NiPSysBoxEmitter);
		Push(kVtbl_NiPSysRotationModifier);
		Push(kVtbl_NiPSysMeshUpdateModifier);
		Push(kVtbl_NiMeshParticleSystem);
		Push(kVtbl_NiPSysEmitter);
		Push(kVtbl_NiPSysGravityModifier);
		Push(kVtbl_NiPSysSpawnModifier);
		Push(kVtbl_NiPSysData);
		Push(kVtbl_BSStripParticleSystem);
		Push(kVtbl_NiPSysUpdateTask);
		Push(kVtbl_BSStripPSysData);
		Push(kVtbl_NiPSysVortexFieldModifier);
		Push(kVtbl_NiPSysUpdateCtlr);
		Push(kVtbl_NiPSysTurbulenceFieldModifier);
		Push(kVtbl_NiPSysSphericalCollider);
		Push(kVtbl_NiPSysResetOnLoopCtlr);
		Push(kVtbl_NiPSysRadialFieldModifier);
		Push(kVtbl_NiPSysPositionModifier);
		Push(kVtbl_NiPSysPlanarCollider);
		Push(kVtbl_NiPSysModifierActiveCtlr);
		Push(kVtbl_NiPSysInitialRotSpeedVarCtlr);
		Push(kVtbl_NiPSysInitialRotSpeedCtlr);
		Push(kVtbl_NiPSysInitialRotAngleVarCtlr);
		Push(kVtbl_NiPSysInitialRotAngleCtlr);
		Push(kVtbl_NiPSysGrowFadeModifier);
		Push(kVtbl_NiPSysGravityStrengthCtlr);
		Push(kVtbl_NiPSysGravityFieldModifier);
		Push(kVtbl_NiPSysFieldMaxDistanceCtlr);
		Push(kVtbl_NiPSysFieldMagnitudeCtlr);
		Push(kVtbl_NiPSysFieldAttenuationCtlr);
		Push(kVtbl_NiPSysEmitterSpeedCtlr);
		Push(kVtbl_NiPSysEmitterPlanarAngleVarCtlr);
		Push(kVtbl_NiPSysEmitterPlanarAngleCtlr);
		Push(kVtbl_NiPSysEmitterLifeSpanCtlr);
		Push(kVtbl_NiPSysEmitterInitialRadiusCtlr);
		Push(kVtbl_NiPSysEmitterDeclinationVarCtlr);
		Push(kVtbl_NiPSysEmitterDeclinationCtlr);
		Push(kVtbl_NiPSysEmitterCtlrData);
		Push(kVtbl_NiPSysDragModifier);
		Push(kVtbl_NiPSysDragFieldModifier);
		Push(kVtbl_NiPSysColorModifier);
		Push(kVtbl_NiPSysColliderManager);
		Push(kVtbl_NiPSysBoundUpdateModifier);
		Push(kVtbl_NiPSysBombModifier);
		Push(kVtbl_NiPSysAgeDeathModifier);
		Push(kVtbl_NiPSysAirFieldSpreadCtlr);
		Push(kVtbl_NiPSysAirFieldModifier);
		Push(kVtbl_NiPSysAirFieldInheritVelocityCtlr);
		Push(kVtbl_NiPSysAirFieldAirFrictionCtlr);
		Push(kVtbl_NiMeshPSysData);
		Push(kVtbl_NiPSysModifierCtlr);
		Push(kVtbl_NiPSysModifier);
		Push(kVtbl_NiPSysVolumeEmitter);
		Push(kVtbl_NiPSysFieldModifier);
		Push(kVtbl_NiPSysCollider);
		Push(kVtbl_NiPSysModifierBoolCtlr);
		Push(kVtbl_NiPSysModifierFloatCtlr);
		Push(kVtbl_BSMultiBound);
		Push(kVtbl_BSMultiBoundRoom);
		Push(kVtbl_BSReference);
		Push(kVtbl_BSNodeReferences);
		Push(kVtbl_MessageHandler);

		Push(kVtbl_QueuedChildren);
		Push(kVtbl_QueuedParents);
		Push(kVtbl_QueuedFile);
		Push(kVtbl_QueuedFileEntry);

		Push(kVtbl_IOManager);
		Push(kVtbl_BSTaskThread);
		Push(kVtbl_BSXFlags);
		Push(kVtbl_BSValueNode);
		Push(kVtbl_BSParallelTaskThread);
		Push(kVtbl_BSRangeNode);
		Push(kVtbl_BSWindModifier);
		Push(kVtbl_BSMultiBoundNode);
		Push(kVtbl_BSBlastNode);
		Push(kVtbl_BSDebrisNode);
		Push(kVtbl_BSDamageStage);
		Push(kVtbl_BSPortalSharedNode);
		Push(kVtbl_BSMultiBoundShape);
		Push(kVtbl_BSParticleSystemManager);
		Push(kVtbl_UpdateMasterPsysTaskletData);
		Push(kVtbl_BSSceneGraph);
		Push(kVtbl_NiBSBoneLODController);
		Push(kVtbl_BSFurnitureMarker);
		Push(kVtbl_BSBound);
		Push(kVtbl_BSPortalGraph);
		Push(kVtbl_BSPSysMultiTargetEmitterCtlr);
		Push(kVtbl_BSScissorTriShape);
		Push(kVtbl_BSMasterParticleSystem);
		Push(kVtbl_BSTextureData);
		Push(kVtbl_BSNiNode);
		Push(kVtbl_BSPSysStripUpdateModifier);
		Push(kVtbl_BSPSysSimpleColorModifier);
		Push(kVtbl_BSMultiBoundCapsule);
		Push(kVtbl_BSWArray);
		Push(kVtbl_PArrayPoint);
		Push(kVtbl_BSPSysArrayEmitter);
		Push(kVtbl_BSParentVelocityModifier);
		Push(kVtbl_BSDecalPlacementVectorExtraData);
		Push(kVtbl_BSAttachPoints);
		Push(kVtbl_bhkMemoryAllocator);
		Push(kVtbl_hkMemoryAllocator);
		Push(kVtbl_bhkOnStartupCallback);
		Push(kVtbl_bhkOnExitCallback);
		Push(kVtbl_hkpCollidableCollidableFilter);
		Push(kVtbl_hkpRayCollidableFilter);
		Push(kVtbl_hkpShapeCollectionFilter);
		Push(kVtbl_hkpRayShapeCollectionFilter);
		Push(kVtbl_hkpBroadPhaseCastCollector);
		Push(kVtbl_ahkpWorld);
		Push(kVtbl_hkpWorldRayCaster);
		Push(kVtbl_bhkCollisionFilter);
		Push(kVtbl_bhkWorldCinfo);
		Push(kVtbl_bhkWorld);
		Push(kVtbl_bhkNiCollisionObject);
		Push(kVtbl_hkpCharacterProxyListener);
		Push(kVtbl_bhkCharacterListener);
		Push(kVtbl_bhkShapeCollection);
		Push(kVtbl_bhkCharacterProxy);
		Push(kVtbl_bhkCachingShapePhantom);
		Push(kVtbl_hkpClosestCdPointCollector);
		Push(kVtbl_bhkCollisionBox);
		Push(kVtbl_bhkListShape);
		Push(kVtbl_bhkCharacterStateClimbing);
		Push(kVtbl_bhkCharacterStateFlying);
		Push(kVtbl_bhkCharacterController);
		Push(kVtbl_bhkWindListener);
		Push(kVtbl_hkaRaycastInterface);
		Push(kVtbl_bhkRagdollShareData);
		Push(kVtbl_bhkRagdollController);
		Push(kVtbl_hkaSkeleton);
		Push(kVtbl_bhkStiffSpringConstraint);
		Push(kVtbl_hkStiffSpringConstraintCinfo);
		Push(kVtbl_bhkBlendCollisionObject);
		Push(kVtbl_hkpMotion);
		Push(kVtbl_bhkRefObject);
		Push(kVtbl_WeaponObject);
		Push(kVtbl_bhkLimitedHingeConstraint);
		Push(kVtbl_hkLimitedHingeConstraintCinfo);
		Push(kVtbl_bhkBinaryAction);
		Push(kVtbl_bhkBlendCollisionObjectAddRotation);
		Push(kVtbl_hkReferencedObjectLock);
		Push(kVtbl_hkpNullContactMgr);
		Push(kVtbl_hkpBroadPhaseListener);
		Push(kVtbl_hkpDefaultConvexListFilter);
		Push(kVtbl_hkpPhantomBroadPhaseListener);
		Push(kVtbl_hkpBroadPhaseBorderListener);
		Push(kVtbl_hkpNullContactMgrFactory);
		Push(kVtbl_hkpNullCollisionFilter);
		Push(kVtbl_hkpSimpleWorldRayCaster);
		Push(kVtbl_hkpWorldLinearCaster);
		Push(kVtbl_hkpWorld);
		Push(kVtbl_hkDebugDisplay);
		Push(kVtbl_bhkWorldM);
		Push(kVtbl_bhkBlendController);
		Push(kVtbl_hkpKeyframedRigidMotion);
		Push(kVtbl_hkpMaxSizeMotion);
		Push(kVtbl_hkpFixedRigidMotion);
		Push(kVtbl_hkpSphereMotion);
		Push(kVtbl_hkpBoxMotion);
		Push(kVtbl_hkpThinBoxMotion);
		Push(kVtbl_hkpCharacterMotion);
		Push(kVtbl_hkpWorldObject);
		Push(kVtbl_hkpEntity);
		Push(kVtbl_hkpShapeContainer);
		Push(kVtbl_hkpSingleShapeContainer);
		Push(kVtbl_hkpBvShape);
		Push(kVtbl_hkpBoxShape);
		Push(kVtbl_hkpConvexTransformShapeBase);
		Push(kVtbl_hkpPhantom);
		Push(kVtbl_hkpShapePhantom);
		Push(kVtbl_hkaRagdollRaycastInterface);
		Push(kVtbl_bhkRagdollPenetrationUtil);
		Push(kVtbl_hkPackedNiTriStripsData);
		Push(kVtbl_hkpTriangleShape);
		Push(kVtbl_hkNormalTriangleShape);
		Push(kVtbl_hkPackedNiTriStripsShape);
		Push(kVtbl_hkNiTriStripsShape);
		Push(kVtbl_bhkPackedNiTriStripsShape);
		Push(kVtbl_bhkNiTriStripsShape);
		Push(kVtbl_hkpRigidBody);
		Push(kVtbl_hkpCharacterContext);
		Push(kVtbl_bhkMalleableConstraint);
		Push(kVtbl_hkMalleableConstraintCinfo);
		Push(kVtbl_hkpMalleableConstraintData);
		Push(kVtbl_ahkpMalleableConstraintData);
		Push(kVtbl_ahkpCharacterProxy);
		Push(kVtbl_hkpCharacterProxy);
		Push(kVtbl_hkBSHeightFieldShape);
		Push(kVtbl_hkErrStream);
		Push(kVtbl_bhkRigidBodyT);
		Push(kVtbl_bhkWaterListener);
		Push(kVtbl_bhkOrientHingedBodyAction);
		Push(kVtbl_hkpLimitedHingeConstraintData);
		Push(kVtbl_bhkForceController);
		Push(kVtbl_bhkBallAndSocketConstraint);
		Push(kVtbl_bhkRagdollConstraint);
		Push(kVtbl_hkBallAndSocketConstraintCinfo);
		Push(kVtbl_hkRagdollConstraintCinfo);
		Push(kVtbl_bhkRagdollTemplateData);
		Push(kVtbl_bhkRagdollTemplate);
		Push(kVtbl_hkpMouseSpringAction);
		Push(kVtbl_hkNativeFileSystem);
		Push(kVtbl_hkDummySingleton);
		Push(kVtbl_hkMemorySystem);
		Push(kVtbl_hkpCharacterStateManager);
		Push(kVtbl_hkpHingeConstraintData);
		Push(kVtbl_hkpPhantomOverlapListener);
		Push(kVtbl_hkpCollisionFilter);
		Push(kVtbl_hkOstream);
		Push(kVtbl_hkMemoryTrackStreamWriter);
		Push(kVtbl_hkArrayStreamWriter);
		Push(kVtbl_bhkPoseArray);
		Push(kVtbl_bhkExtraData);
		Push(kVtbl_bhkSpringAction);
		Push(kVtbl_bhkMotorAction);
		Push(kVtbl_bhkDashpotAction);
		Push(kVtbl_bhkAngularDashpotAction);
		Push(kVtbl_bhkBreakableConstraint);
		Push(kVtbl_ahkpBreakableConstraintData);
		Push(kVtbl_hkBreakableConstraintCinfo);
		Push(kVtbl_bhkWheelConstraint);
		Push(kVtbl_hkWheelConstraintCinfo);
		Push(kVtbl_bhkRagdollLimitsConstraint);
		Push(kVtbl_hkRagdollLimitsConstraintCinfo);
		Push(kVtbl_bhkPrismaticConstraint);
		Push(kVtbl_hkPrismaticConstraintCinfo);
		Push(kVtbl_bhkFixedConstraint);
		Push(kVtbl_bhkGenericConstraint);
		Push(kVtbl_hkGenericConstraintCinfo);
		Push(kVtbl_hkFixedConstraintCinfo);
		Push(kVtbl_bhkHingeLimitsConstraint);
		Push(kVtbl_hkHingeLimitsConstraintCinfo);
		Push(kVtbl_bhkBallSocketConstraintChain);
		Push(kVtbl_bhkConstraintChain);
		Push(kVtbl_hkBallSocketChainCinfo);
		Push(kVtbl_bhkMoppBvTreeShape);
		Push(kVtbl_hkScaledMoppBvTreeShape);
		Push(kVtbl_bhkTriangleShape);
		Push(kVtbl_bhkPlaneShape);
		Push(kVtbl_bhkHeightFieldShape);
		Push(kVtbl_bhkExtendedMeshShapeData);
		Push(kVtbl_bhkExtendedMeshShape);
		Push(kVtbl_bhkCylinderShape);
		Push(kVtbl_bhkConvexVerticesShape);
		Push(kVtbl_bhkConvexTransformShape);
		Push(kVtbl_bhkConvexSweepShape);
		Push(kVtbl_bhkConvexListShape);
		Push(kVtbl_hkThreadMemory);
		Push(kVtbl_hkMemoryBlockServer);
		Push(kVtbl_hkLeakDetectAllocator);
		Push(kVtbl_hkFreeListMemorySystem);
		Push(kVtbl_hkVisualDebugger);
		Push(kVtbl_hkpWorldDeletionListener);
		Push(kVtbl_hkpActionListener);
		Push(kVtbl_hkpConstraintListener);
		Push(kVtbl_hkProcessContext);
		Push(kVtbl_hkpPhysicsContext);
		Push(kVtbl_bhkCharacterPointCollector);
		Push(kVtbl_bhkCharacterStateProjectile);
		Push(kVtbl_bhkCharacterStateInAir);
		Push(kVtbl_bhkCharacterStateJumping);
		Push(kVtbl_bhkCharacterStateSwimming);
		Push(kVtbl_bhkCharacterStateOnGround);
		Push(kVtbl_hkCharControllerShape);
		Push(kVtbl_bhkCharControllerShape);
		Push(kVtbl_hkpSphereShape);
		Push(kVtbl_hkaFootPlacementIkSolver);
		Push(kVtbl_hkpRagdollConstraintData);
		Push(kVtbl_hkpConstraintMotor);
		Push(kVtbl_hkpLimitedForceConstraintMotor);
		Push(kVtbl_hkpPositionConstraintMotor);
		Push(kVtbl_hkaRagdollInstance);
		Push(kVtbl_hkaSkeletonMapper);
		Push(kVtbl_hkpAabbPhantom);
		Push(kVtbl_hkpConstraintInstance);
		Push(kVtbl_bhkPositionConstraintMotor);
		Push(kVtbl_hkpVelocityConstraintMotor);
		Push(kVtbl_bhkVelocityConstraintMotor);
		Push(kVtbl_bhkSpringDamperConstraintMotor);
		Push(kVtbl_hkPointToPathConstraintCinfo);
		Push(kVtbl_hkpAction);
		Push(kVtbl_hkpBinaryAction);
		Push(kVtbl_hkpShapeCollection);
		Push(kVtbl_hkpListShape);
		Push(kVtbl_hkpTransformShape);
		Push(kVtbl_hkpNullBroadPhaseListener);
		Push(kVtbl_hkpSimulation);
		Push(kVtbl_hkpCollisionDispatcher);
		Push(kVtbl_hkpReportContactMgr);
		Push(kVtbl_hkpSimpleContactConstraintData);
		Push(kVtbl_hkpSimpleConstraintContactMgr);
		Push(kVtbl_hkpMultiThreadedSimulation);
		Push(kVtbl_hkpPhysicsSystem);
		Push(kVtbl_hkStatisticsCollector);
		Push(kVtbl_hkpConstraintOwner);
		Push(kVtbl_hkpSimulationIsland);
		Push(kVtbl_hkpWorldPostSimulationListener);
		Push(kVtbl_hkKdTreeCastCollector);
		Push(kVtbl_hkpKdTreeCollidableRaycaster);
		Push(kVtbl_hkpKdTreeWorldLinearCaster);
		Push(kVtbl_hkpAabbTreeWorldManager);
		Push(kVtbl_hkpKdTreeWorldManager);
		Push(kVtbl_hkpCollisionCallbackUtil);
		Push(kVtbl_hkpDefaultWorldMaintenanceMgr);
		Push(kVtbl_hkpBroadPhaseBorder);
		Push(kVtbl_hkpContinuousSimulation);
		Push(kVtbl_hkpNullAgent);
		Push(kVtbl_hkpEntityEntityBroadPhaseListener);
		Push(kVtbl_hkp3AxisSweep);
		Push(kVtbl_hkpSimpleShapePhantom);
		Push(kVtbl_hkaDetectRagdollPenetration);
		Push(kVtbl_hkpCapsuleShape);
		Push(kVtbl_hkDisplayGeometry);
		Push(kVtbl_hkDisplayCapsule);
		Push(kVtbl_hkpSampledHeightFieldShape);
		Push(kVtbl_hkpTriSampledHeightFieldCollection);
		Push(kVtbl_hkpBvTreeShape);
		Push(kVtbl_hkpTriSampledHeightFieldBvTreeShape);
		Push(kVtbl_hkBufferedStreamWriter);
		Push(kVtbl_hkpMultiSphereShape);
		Push(kVtbl_hkOrientHingedBodyAction);
		Push(kVtbl_hkpUnaryAction);
		Push(kVtbl_hkDefaultError);
		Push(kVtbl_hkSocket);
		Push(kVtbl_hkBufferedStreamReader);
		Push(kVtbl_hkStdioStreamReader);
		Push(kVtbl_hkStdioStreamWriter);
		Push(kVtbl_hkLiquidAction);
		Push(kVtbl_hkCpuJobThreadPool);
		Push(kVtbl_hkpSymmetricAgentFlipCollector);
		Push(kVtbl_hkpSymmetricAgentFlipCastCollector);
		Push(kVtbl_hkpSymmetricAgentFlipBodyCollector);
		Push(kVtbl_hkpBvTreeStreamAgent);
		Push(kVtbl_hkpBvTreeAgent);
		Push(kVtbl_hkpMoppAgent);
		Push(kVtbl_hkHeightFieldRayForwardingCollector);
		Push(kVtbl_hkpHeightFieldAgent);
		Push(kVtbl_hkpMapPointsToSubShapeContactMgr);
		Push(kVtbl_hkpPredGskfAgent);
		Push(kVtbl_hkpFlagCdBodyPairCollector);
		Push(kVtbl_hkpSimpleClosestContactCollector);
		Push(kVtbl_hkpConvexListAgent);
		Push(kVtbl_hkpListAgent);
		Push(kVtbl_hkpMultiRayConvexAgent);
		Push(kVtbl_hkpMultiSphereTriangleAgent);
		Push(kVtbl_hkpCapsuleTriangleAgent);
		Push(kVtbl_hkpCapsuleCapsuleAgent);
		Push(kVtbl_hkpSphereBoxAgent);
		Push(kVtbl_hkpSphereTriangleAgent);
		Push(kVtbl_hkpSphereCapsuleAgent);
		Push(kVtbl_hkpSphereSphereAgent);
		Push(kVtbl_hkpBoxBoxAgent);
		Push(kVtbl_hkpPhantomAgent);
		Push(kVtbl_hkpTransformAgent);
		Push(kVtbl_hkpShapeCollectionAgent);
		Push(kVtbl_hkpMultiSphereAgent);
		Push(kVtbl_hkpBvAgent);
		Push(kVtbl_hkMemoryTrackStreamReader);
		Push(kVtbl_hkDefaultBuiltinTypeRegistry);
		Push(kVtbl_hkTypeInfoRegistry);
		Push(kVtbl_hkDynamicClassNameRegistry);
		Push(kVtbl_hkDefaultClassNameRegistry);
		Push(kVtbl_hkVtableClassRegistry);
		Push(kVtbl_hkpPhysicsData);
		Push(kVtbl_hkBinaryTagfileWriter);
		Push(kVtbl_hkpStorageSampledHeightFieldShape);
		Push(kVtbl_hkpStorageExtendedMeshShape);
		Push(kVtbl_hkpStorageMeshShape);
		Push(kVtbl_hkpPhysicsSystemWithContacts);
		Push(kVtbl_hkpCachingShapePhantom);
		Push(kVtbl_hkpSpringAction);
		Push(kVtbl_hkpMotorAction);
		Push(kVtbl_hkpDashpotAction);
		Push(kVtbl_hkpAngularDashpotAction);
		Push(kVtbl_hkpBreakableConstraintData);
		Push(kVtbl_hkpWheelConstraintData);
		Push(kVtbl_hkpStiffSpringConstraintData);
		Push(kVtbl_hkpRagdollLimitsData);
		Push(kVtbl_hkpPrismaticConstraintData);
		Push(kVtbl_hkpGenericConstraintData);
		Push(kVtbl_hkpHingeLimitsData);
		Push(kVtbl_hkpBallSocketChainData);
		Push(kVtbl_hkpBallAndSocketConstraintData);
		Push(kVtbl_hkMoppBvTreeShapeBase);
		Push(kVtbl_hkpMoppBvTreeShape);
		Push(kVtbl_hkpPlaneShape);
		Push(kVtbl_hkpConvexTranslateShape);
		Push(kVtbl_hkpExtendedMeshShape);
		Push(kVtbl_hkpCylinderShape);
		Push(kVtbl_hkDisplayCylinder);
		Push(kVtbl_hkpConvexVerticesShape);
		Push(kVtbl_hkpConvexTransformShape);
		Push(kVtbl_hkpConvexListShape);
		Push(kVtbl_hkSolverAllocator);
		Push(kVtbl_hkFreeListAllocator);
		Push(kVtbl_hkSystemMemoryBlockServer);
		Push(kVtbl_hkOArchive);
		Push(kVtbl_hkProcess);
		Push(kVtbl_hkProcessHandler);
		Push(kVtbl_hkCommandRouter);
		Push(kVtbl_hkServerProcessHandler);
		Push(kVtbl_hkpWeldingViewer);
		Push(kVtbl_hkpWorldSnapshotViewer);
		Push(kVtbl_hkpWorldMemoryViewer);
		Push(kVtbl_hkpVehicleViewer);
		Push(kVtbl_hkpSweptTransformDisplayViewer);
		Push(kVtbl_hkpSimulationIslandViewer);
		Push(kVtbl_hkpShapeDisplayViewer);
		Push(kVtbl_hkpPhantomDisplayViewer);
		Push(kVtbl_hkpUserShapePhantomTypeIdentifier);
		Push(kVtbl_hkpMousePickingViewer);
		Push(kVtbl_hkpRigidBodyInertiaViewer);
		Push(kVtbl_hkpToiCountViewer);
		Push(kVtbl_hkpToiContactPointViewer);
		Push(kVtbl_hkpInactiveContactPointViewer);
		Push(kVtbl_hkpActiveContactPointViewer);
		Push(kVtbl_hkpConvexRadiusViewer);
		Push(kVtbl_hkpConstraintViewer);
		Push(kVtbl_hkpRigidBodyCentreOfMassViewer);
		Push(kVtbl_hkpMidphaseViewer);
		Push(kVtbl_hkpBroadphaseViewer);
		Push(kVtbl_hkgpConvexHull);
		Push(kVtbl_hkpPointToPlaneConstraintData);
		Push(kVtbl_hkpSpringDamperConstraintMotor);
		Push(kVtbl_hkTraceStream);
		Push(kVtbl_hkpConstraintCollisionFilter);
		Push(kVtbl_hkpDeferredConstraintOwner);
		Push(kVtbl_hkpBroadPhase);
		Push(kVtbl_hkKdTreeBundleCastCollector);
		Push(kVtbl_hkKdTreeCollidableRayBundleCaster);
		Push(kVtbl_hkpAabbTreeCollidableRaycaster);
		Push(kVtbl_hkpCollidableMediator);
		Push(kVtbl_hkpEndOfStepCallbackUtil);
		Push(kVtbl_hkpDefaultToiResourceMgr);
		Push(kVtbl_hkLocalFrameGroup);
		Push(kVtbl_hkSimpleLocalFrame);
		Push(kVtbl_hkStackTracer);
		Push(kVtbl_hkBsdSocket);
		Push(kVtbl_hkpGskBaseAgent);
		Push(kVtbl_hkpGskfAgent);
		Push(kVtbl_hkCrc32StreamWriter);
		Push(kVtbl_hkStaticClassNameRegistry);
		Push(kVtbl_hkVersionPatchManager);
		Push(kVtbl_hkSerializeDeprecated);
		Push(kVtbl_hkDataRefCounted);
		Push(kVtbl_hkDataClassNative);
		Push(kVtbl_hkDataObjectNative);
		Push(kVtbl_hkDataArrayNative);
		Push(kVtbl_hkDataWorldNative);
		Push(kVtbl_hkBinaryPackfileWriter);
		Push(kVtbl_hkSubStreamWriter);
		Push(kVtbl_hkPackfileWriter);
		Push(kVtbl_hkBinaryTagfileReader);
		Push(kVtbl_ArrayOfTuplesImplementation);
		Push(kVtbl_hkDataObjectDict);
		Push(kVtbl_VecArrayImplementation);
		Push(kVtbl_PointerArrayImplementation);
		Push(kVtbl_ByteArrayImplementation);
		Push(kVtbl_IntArrayImplementation);
		Push(kVtbl_RealArrayImplementation);
		Push(kVtbl_CstringArrayImplementation);
		Push(kVtbl_hkDataClassDict);
		Push(kVtbl_StructArrayImplementation);
		Push(kVtbl_hkDataWorldDict);
		Push(kVtbl_hkIstream);
		Push(kVtbl_hkpMeshShape);
		Push(kVtbl_hkpSerializedAgentNnEntry);
		Push(kVtbl_hkpConstraintChainInstanceAction);
		Push(kVtbl_hkpConstraintChainInstance);
		Push(kVtbl_hkpMoppCostFunction);
		Push(kVtbl_hkpMoppShapeMediator);
		Push(kVtbl_hkpMoppCachedShapeMediator);
		Push(kVtbl_hkFreeListAllocatorServer);
		Push(kVtbl_hkLargeBlockAllocator);
		Push(kVtbl_hkWindowsSystemClock);
		Push(kVtbl_hkIArchive);
		Push(kVtbl_hkProcessFactory);
		Push(kVtbl_hkDebugDisplayHandler);
		Push(kVtbl_hkServerDebugDisplayHandler);
		Push(kVtbl_hkDisplaySerializeOStream);
		Push(kVtbl_hkDisplaySerializeIStream);
		Push(kVtbl_hkInspectProcess);
		Push(kVtbl_hkStatisticsProcess);
		Push(kVtbl_hkDebugDisplayProcess);
		Push(kVtbl_hkpPhysicsContextWorldListener);
		Push(kVtbl_hkpWorldViewerBase);
		Push(kVtbl_hkStreamStatisticsCollector);
		Push(kVtbl_hkpShapeDisplayBuilder);
		Push(kVtbl_hkpUserShapeDisplayBuilder);
		Push(kVtbl_hkDisplayAABB);
		Push(kVtbl_hkDisplayConvex);
		Push(kVtbl_hkDisplayBox);
		Push(kVtbl_hkpContactPointViewer);
		Push(kVtbl_hkpConvexRadiusBuilder);
		Push(kVtbl_hkpUserConvexRadiusBuilder);
		Push(kVtbl_hkDisplaySemiCircle);
		Push(kVtbl_hkDisplayPlane);
		Push(kVtbl_hkDisplayCone);
		Push(kVtbl_hkpPointToPathConstraintData);
		Push(kVtbl_hkpPulleyConstraintData);
		Push(kVtbl_hkpPairCollisionFilter);
		Push(kVtbl_hkCpuWorldGetClosestPointsCollector);
		Push(kVtbl_hkCpuWorldLinearCastCollector);
		Push(kVtbl_hkCpuWorldRayCastCollector);
		Push(kVtbl_hkpFixedBufferRayHitCollector);
		Push(kVtbl_hkaAnimation);
		Push(kVtbl_hkaWaveletCompressedAnimation);
		Push(kVtbl_hkaSplineCompressedAnimation);
		Push(kVtbl_hkaDeltaCompressedAnimation);
		Push(kVtbl_hkpRejectChassisListener);
		Push(kVtbl_hkpVehicleWheelCollide);
		Push(kVtbl_hkpVehicleLinearCastWheelCollide);
		Push(kVtbl_hkpVehicleInstance);
		Push(kVtbl_hkpVehicleDefaultSuspension);
		Push(kVtbl_hkpVehicleDefaultBrake);
		Push(kVtbl_hkpVehicleData);
		Push(kVtbl_hkpTyremarksWheel);
		Push(kVtbl_hkpTyremarksInfo);
		Push(kVtbl_hkpSerializedDisplayRbTransforms);
		Push(kVtbl_hkpPoweredChainMapper);
		Push(kVtbl_hkpRotationalConstraintData);
		Push(kVtbl_hkpSimpleMeshShape);
		Push(kVtbl_hkpMultiRayShape);
		Push(kVtbl_hkxAttributeHolder);
		Push(kVtbl_hkxNode);
		Push(kVtbl_hkxMaterial);
		Push(kVtbl_hkxSparselyAnimatedInt);
		Push(kVtbl_hkxSparselyAnimatedEnum);
		Push(kVtbl_hkxEnum);
		Push(kVtbl_hkMonitorStreamColorTable);
		Push(kVtbl_hkaInterleavedUncompressedAnimation);
		Push(kVtbl_hkaFootstepAnalysisInfoContainer);
		Push(kVtbl_hkaDefaultAnimatedReferenceFrame);
		Push(kVtbl_hkaBoneAttachment);
		Push(kVtbl_hkaAnimationPreviewColorContainer);
		Push(kVtbl_hkaAnimationContainer);
		Push(kVtbl_hkaAnimationBinding);
		Push(kVtbl_hkpVehicleRayCastWheelCollide);
		Push(kVtbl_hkpVehicleManager);
		Push(kVtbl_hkpVehicleRayCastBatchingManager);
		Push(kVtbl_hkpVehicleLinearCastBatchingManager);
		Push(kVtbl_hkpVehicleDriverInputAnalogStatus);
		Push(kVtbl_hkpVehicleDefaultAnalogDriverInput);
		Push(kVtbl_hkpVehicleDefaultVelocityDamper);
		Push(kVtbl_hkpVehicleDefaultTransmission);
		Push(kVtbl_hkpVehicleDefaultSteering);
		Push(kVtbl_hkpVehicleDefaultEngine);
		Push(kVtbl_hkpVehicleDefaultAerodynamics);
		Push(kVtbl_hkpSerializedDisplayMarkerList);
		Push(kVtbl_hkpSerializedDisplayMarker);
		Push(kVtbl_hkpReorientAction);
		Push(kVtbl_hkpGroupCollisionFilter);
		Push(kVtbl_hkpDisplayBindingData);
		Push(kVtbl_hkpDisableEntityCollisionFilter);
		Push(kVtbl_hkpConstrainedSystemFilter);
		Push(kVtbl_hkpConvexPieceStreamData);
		Push(kVtbl_hkpLinearParametricCurve);
		Push(kVtbl_hkpDefaultWorldMemoryWatchDog);
		Push(kVtbl_hkpCallbackConstraintMotor);
		Push(kVtbl_hkpShapeInfo);
		Push(kVtbl_hkpMoppModifier);
		Push(kVtbl_hkpRemoveTerminalsMoppModifier);
		Push(kVtbl_hkpGroupFilter);
		Push(kVtbl_hkpFastMeshShape);
		Push(kVtbl_hkpConvexVerticesConnectivity);
		Push(kVtbl_hkpConvexPieceMeshShape);
		Push(kVtbl_hkpCompressedSampledHeightFieldShape);
		Push(kVtbl_hkpCollisionFilterList);
		Push(kVtbl_hkMemoryMeshShape);
		Push(kVtbl_hkxVertexVectorDataChannel);
		Push(kVtbl_hkxVertexSelectionChannel);
		Push(kVtbl_hkxVertexIntDataChannel);
		Push(kVtbl_hkxVertexFloatDataChannel);
		Push(kVtbl_hkxTriangleSelectionChannel);
		Push(kVtbl_hkxTextureInplace);
		Push(kVtbl_hkxTextureFile);
		Push(kVtbl_hkxSparselyAnimatedString);
		Push(kVtbl_hkxSparselyAnimatedBool);
		Push(kVtbl_hkxSkinBinding);
		Push(kVtbl_hkxScene);
		Push(kVtbl_hkxNodeSelectionSet);
		Push(kVtbl_hkxMeshSection);
		Push(kVtbl_hkxMaterialShader);
		Push(kVtbl_hkxMaterialEffect);
		Push(kVtbl_hkxLight);
		Push(kVtbl_hkxIndexBuffer);
		Push(kVtbl_hkxEdgeSelectionChannel);
		Push(kVtbl_hkxCamera);
		Push(kVtbl_hkxAnimatedVector);
		Push(kVtbl_hkxAnimatedQuaternion);
		Push(kVtbl_hkxAnimatedMatrix);
		Push(kVtbl_hkxAnimatedFloat);
		Push(kVtbl_hkPlatformObjectWriter);
		Push(kVtbl_hkOffsetOnlyStreamWriter);
		Push(kVtbl_hkObjectCopier);
		Push(kVtbl_hkObjectResource);
		Push(kVtbl_hkPackfileData);
		Push(kVtbl_hkMemoryStreamReader);
		Push(kVtbl_hkpMoppCodeGenerator);
		Push(kVtbl_hkpMoppDefaultAssembler);
		Push(kVtbl_hkpMoppDefaultSplitter);
		Push(kVtbl_hkDisplaySphere);
		Push(kVtbl_IConvexOverlapImpl);
		Push(kVtbl_hkGskShape);
		Push(kVtbl_hkgpMesh);
		Push(kVtbl_hkResourceMap);
		Push(kVtbl_hkMemoryResourceHandle);
		Push(kVtbl_hkMemoryResourceContainer);
		Push(kVtbl_hkContainerResourceMap);
		Push(kVtbl_hkaMeshBinding);
		Push(kVtbl_hkpStiffSpringChainData);
		Push(kVtbl_hkpPoweredChainData);
		Push(kVtbl_hkpCompressedMeshShape);
		Push(kVtbl_hkxVertexBuffer);
		Push(kVtbl_hkxEnvironment);
		Push(kVtbl_hkaFootstepAnalysisInfo);
		Push(kVtbl_hkpGunProjectile);
		Push(kVtbl_hkpProjectileGun);
		Push(kVtbl_hkpMountedBallGun);
		Push(kVtbl_hkpGravityGun);
		Push(kVtbl_hkpFirstPersonGun);
		Push(kVtbl_hkpBallGun);
		Push(kVtbl_hkpRemoveTerminalsMoppModifier2);
		Push(kVtbl_hkxMesh);
		Push(kVtbl_hkxMaterialShaderSet);
		Push(kVtbl_hkAlignSceneToNodeOptions);
		Push(kVtbl_hkKdTreeCpuBroadphaseBundleCastCollector);
		Push(kVtbl_hkpConvexPieceShape);
		Push(kVtbl_NiDX9SourceTextureData);
		Push(kVtbl_NiDX9Renderer);
		Push(kVtbl_NiDX9ShaderDeclaration);
		Push(kVtbl_NiD3DShaderDeclaration);
		Push(kVtbl_NiD3DDefaultShader);
		Push(kVtbl_NiD3DShader);
		Push(kVtbl_NiDX92DBufferData);
		Push(kVtbl_NiDX9TextureBufferData);
		Push(kVtbl_NiDX9ImplicitBufferData);
		Push(kVtbl_NiDX9ImplicitDepthStencilBufferData);
		Push(kVtbl_NiDX9AdditionalDepthStencilBufferData);
		Push(kVtbl_NiD3DPass);
		Push(kVtbl_NiD3DShaderInterface);
		Push(kVtbl_NiShaderConstantMapEntry);
		Push(kVtbl_NiD3DShaderConstantMap);
		Push(kVtbl_NiD3DRenderState);
		Push(kVtbl_NiD3DGeometryGroupManager);
		Push(kVtbl_NiD3DShaderProgram);
		Push(kVtbl_NiD3DPixelShader);
		Push(kVtbl_NiD3DVertexShader);
		Push(kVtbl_NiDX9TextureData);
		Push(kVtbl_NiDX9SourceCubeMapData);
		Push(kVtbl_NiDX9VertexBufferManager);
		Push(kVtbl_NiFragmentShaderInstanceDescriptor);
		Push(kVtbl_NiDX9FragmentShader);
		Push(kVtbl_NiD3DGPUProgramCache);
		Push(kVtbl_NiGeometryBufferData);
		Push(kVtbl_NiDX9IndexBufferManager);
		Push(kVtbl_NiDX9RenderedTextureData);
		Push(kVtbl_NiDX9RenderedCubeMapData);
		Push(kVtbl_NiDX9DynamicTextureData);
		Push(kVtbl_NiDX9ErrorShader);
		Push(kVtbl_NiDX9TextureManager);
		Push(kVtbl_NiDX9RenderState);
		Push(kVtbl_NiDX9PersistentSrcTextureRendererData);
		Push(kVtbl_NiD3DShaderProgramCreator);
		Push(kVtbl_NiD3DShaderProgramCreatorHLSL);
		Push(kVtbl_NiD3DShaderProgramCreatorAsm);
		Push(kVtbl_NiD3DShaderProgramCreatorObj);
		Push(kVtbl_NiD3DShaderFactory);
		Push(kVtbl_NiD3DShaderProgramFactory);
		Push(kVtbl_NiStaticGeometryGroup);
		Push(kVtbl_NiUnsharedGeometryGroup);
		Push(kVtbl_NiDynamicGeometryGroup);
		Push(kVtbl_NiD3DHLSLPixelShader);
		Push(kVtbl_NiD3DHLSLVertexShader);
		Push(kVtbl_NiDX9ShaderConstantManager);
		Push(kVtbl_NiGeometryGroup);
		Push(kVtbl_NiD3DShaderConstantManager);
		Push(kVtbl_NiBoundingVolume);
		Push(kVtbl_NiHalfSpaceBV);
		Push(kVtbl_NiUnionBV);
		Push(kVtbl_NiCapsuleBV);
		Push(kVtbl_NiBoxBV);
		Push(kVtbl_NiSphereBV);
		Push(kVtbl_NiCollisionData);
		Push(kVtbl_NiIntersector);
		Push(kVtbl_NiCapsuleSphereIntersector);
		Push(kVtbl_NiCapsuleCapsuleIntersector);
		Push(kVtbl_NiCapsuleTriIntersector);
		Push(kVtbl_NiBoxCapsuleIntersector);
		Push(kVtbl_NiBoxSphereIntersector);
		Push(kVtbl_NiSphereTriIntersector);
		Push(kVtbl_NiOBBRoot);
		Push(kVtbl_NiOBBNode);
		Push(kVtbl_NiOBBLeaf);
		Push(kVtbl_MoviePlayer);
		Push(kVtbl_NiRenderedCubeMap);
		Push(kVtbl_NiRenderTargetGroup);
		Push(kVtbl_NiShaderDeclaration);
		Push(kVtbl_NiSCMExtraData);
		Push(kVtbl_NiGPUProgramCache);
		Push(kVtbl_NiMaterialResource);
	};

	std::string GetStringForVTBL(void* ptr, UInt32 vtbl)
	{

		for (const auto& iter : setOfLabels)
			if (iter->Satisfies(ptr)) return iter->Get(ptr);

		if (vtbl > 0x1000000 && vtbl < 0x1200000)
			return FormatString("¯\\_(Ö)_/¯: 0x%08X", vtbl);

		return "";
	}
}

// take a look inside
namespace CrashLogger::YvilesMagicBox
{

	UInt32 DereferenceExcept(UInt32 ptr)
	{
		__try { UInt32 vtbl = *(UInt32*)(ptr); return vtbl; }
		__except (EXCEPTION_EXECUTE_HANDLER) { return 0; }
	}

	UInt32 DereferenceCatch(UInt32 ptr)
	{
		try { return DereferenceExcept(ptr); }
		catch (...) { return 0; }
	}


	

	std::string DecryptPtr(UInt32 ptr, UInt32 depth = 5)
	{
		try
		{
			if (!ptr) return "";

			UInt32 vtbl;

			std::string full;

			while (vtbl = DereferenceCatch(ptr))
			{
				if (std::string onetime = NVVtables::GetStringForVTBL((void*)ptr, vtbl); !onetime.empty()) return full + onetime;

				if (depth == 0) break;

				depth--;

				full += FormatString("NOT_VTBL: 0x%08X; ", vtbl);

				ptr = vtbl;
			}

			return "";

		}
		catch (...)
		{
			return "";
		}
	}
}

namespace CrashLogger::Calltrace
{
	void Get(EXCEPTION_POINTERS* info, HANDLE process, HANDLE thread) {

		Log() << FormatString("Exception %08X caught!", info->ExceptionRecord->ExceptionCode);
		Log() << FormatString("\nCalltrace: ");

		DWORD machine = IMAGE_FILE_MACHINE_I386;
		CONTEXT context = {};
		memcpy(&context, info->ContextRecord, sizeof(CONTEXT));
		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);
		//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
		if (SymInitialize(process, NULL, TRUE) != TRUE) {
			Log() << FormatString("Error initializing symbol store");
		}
		//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;
		DWORD eip = 0;
		while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) {
			/*
		Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
		When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
			*/
			if (frame.AddrPC.Offset == eip) break;
			eip = frame.AddrPC.Offset;

			/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
				   if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
			}*/
			IMAGEHLP_MODULE module = { 0 };
			module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

			BOOL moduleAv = SymGetModuleInfo(process, frame.AddrPC.Offset, &module);
			std::string functioName;
			char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
			IMAGEHLP_SYMBOL* symbol = (IMAGEHLP_SYMBOL*)symbolBuffer;
			symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
			symbol->MaxNameLength = 254;
			DWORD  offset = 0;
			if (!moduleAv) {
				//    Log() << FormatString("porcoddio2 %0X", GetLastError());
				Log() << FormatString("0x%08X ==> %20s (0x%08X) : (Corrupt stack or heap?)", frame.AddrPC.Offset, "¯\\(°_o)/¯", frame.AddrFrame.Offset);
				continue;
			}

			if (!SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol)) {
				//   Log() << FormatString("porcoddio1 %0X", GetLastError());
				//WIne dbghelp and Windows dbghelp operate totally differently
				Log() << FormatString("0x%08X ==> %20s (0x%08X) : %s+0x%0X ", frame.AddrPC.Offset, module.ModuleName, frame.AddrFrame.Offset, "EntryPoint", (UInt32)-1);
				continue;
			}


			DWORD offset2 = 0;
			char lineBuffer[sizeof(IMAGEHLP_LINE) + 255];
			IMAGEHLP_LINE* line = (IMAGEHLP_LINE*)lineBuffer;
			line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

			functioName = symbol->Name;

			if (!SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset2, line)) {
				Log() << FormatString("0x%08X ==> %20s (0x%08X) : %s+0x%0X", frame.AddrPC.Offset, module.ModuleName, frame.AddrFrame.Offset, functioName.c_str(), offset);
				continue;
			}

			Log() << FormatString("0x%08X ==> %-80s <== <%s:%d>", frame.AddrPC.Offset, FormatString("%20s (0x%08X) : %s+0x%0X", module.ModuleName, frame.AddrFrame.Offset, functioName.c_str(), offset).c_str(), line->FileName, line->LineNumber);
		}
	}

}

namespace CrashLogger::Registry
{
	void Get(EXCEPTION_POINTERS* info)
	{
		Log() << FormatString("\nRegistry: ");
		Log() << FormatString("REG |    VALUE   | DEREFERENCE INFO");
		Log() << FormatString("%s | 0x%08X | %s ", "eax", info->ContextRecord->Eax, YvilesMagicBox::DecryptPtr(info->ContextRecord->Eax).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "ebx", info->ContextRecord->Ebx, YvilesMagicBox::DecryptPtr(info->ContextRecord->Ebx).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "ecx", info->ContextRecord->Ecx, YvilesMagicBox::DecryptPtr(info->ContextRecord->Ecx).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "edx", info->ContextRecord->Edx, YvilesMagicBox::DecryptPtr(info->ContextRecord->Edx).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "edi", info->ContextRecord->Edi, YvilesMagicBox::DecryptPtr(info->ContextRecord->Edi).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "esi", info->ContextRecord->Esi, YvilesMagicBox::DecryptPtr(info->ContextRecord->Esi).c_str());
		Log() << FormatString("%s | 0x%08X | %s ", "ebp", info->ContextRecord->Ebp, YvilesMagicBox::DecryptPtr(info->ContextRecord->Ebp).c_str());
		Log() << FormatString("%s | 0x%08X |", "esp", info->ContextRecord->Esp);
		Log() << FormatString("%s | 0x%08X |", "eip", info->ContextRecord->Eip);
	}
}

namespace CrashLogger::Stack
{
	void Get(EXCEPTION_POINTERS* info)
	{
		Log() << FormatString("\nStack: ");
		Log() << FormatString(" # |    VALUE   | DEREFERENCE INFO");

		UInt32* esp = (UInt32*)info->ContextRecord->Esp;
		for (unsigned int i = 0; i < ce_printStackCount; i += 1) {

			if (i <= 8)
			{
				Log() << FormatString("%2X | 0x%08X | %s", i, esp[i], YvilesMagicBox::DecryptPtr(esp[i]).c_str());
			}
			else
			{
				std::string result = YvilesMagicBox::DecryptPtr(esp[i], 0);
				if (!result.empty()) Log() << FormatString("%2X | 0x%08X | %s", i, esp[i], result.c_str());

			}
		}
	}
}

namespace CrashLogger::ModuleBases
{
	struct UserContext {
		UInt32 eip;
		UInt32 moduleBase;
		CHAR* name;
	};

	BOOL CALLBACK EumerateModulesCallback(PCSTR name, ULONG moduleBase, ULONG moduleSize, PVOID context) {
		UserContext* info = (UserContext*)context;
		if (info->eip >= (UInt32)moduleBase && info->eip <= (UInt32)moduleBase + (UInt32)moduleSize) {
			//Log() << FormatString("%0X %0X  %0X", (UInt32)moduleBase, info->eip, (UInt32)moduleBase + (UInt32) moduleSize);
			info->moduleBase = moduleBase;
			strcpy_s(info->name, 100, name);
		}
		Log() << FormatString(" - 0x%08X - 0x%08X: %s", (UInt32)moduleBase, (UInt32)moduleBase + (UInt32)moduleSize, name);
		return TRUE;
	}

	BOOL CALLBACK EumerateModulesCallbackSym(PCSTR name, ULONG moduleBase, PVOID context) {
		UserContext* info = (UserContext*)context;
		IMAGEHLP_MODULE modu;
		SymGetModuleInfo(GetCurrentProcess(), moduleBase, &modu);
		if (info->eip >= (UInt32)moduleBase && info->eip <= (UInt32)moduleBase + modu.ImageSize) {
			//			   Log() << FormatString("%0X %0X  %0X", (UInt32)moduleBase, info->eip, (UInt32)moduleBase + moduleSize);
			info->moduleBase = moduleBase;
			strcpy_s(info->name, 100, name);
		}
		Log() << FormatString(" - 0x%08X - 0x%08X: %s", (UInt32)moduleBase, (UInt32)moduleBase + modu.ImageSize, name);
		return TRUE;
	}

	void Get(EXCEPTION_POINTERS* info, HANDLE processHandle)
	{
		const UInt32 eip = info->ContextRecord->Eip;

		Log() << FormatString("\nModule bases:");
		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };
		EnumerateLoadedModules(processHandle, EumerateModulesCallback, &infoUser);
		//        SymEnumerateModules(processHandle, EumerateModulesCallbackSym, &infoUser);
		if (infoUser.moduleBase) {
			Log() << FormatString("\nGAME CRASHED AT INSTRUCTION Base+0x%08X IN MODULE: %s", (infoUser.eip - infoUser.moduleBase), infoUser.name);
			Log() << FormatString("Please note that this does not automatically mean that that module is responsible. \n"
				"It may have been supplied bad data or program state as the result of an issue in \n"
				"the base game or a different DLL.");
		}
		else {
			Log() << FormatString("\nUNABLE TO IDENTIFY MODULE CONTAINING THE CRASH ADDRESS.");
			Log() << FormatString("This can occur if the crashing instruction is located in the vanilla address space, \n"
				"but it can also occur if there are too many DLLs for us to list, and if the crash \n"
				"occurred in one of their address spaces. Please note that even if the crash occurred \n"
				"in vanilla code, that does not necessarily mean that it is a vanilla problem. The \n"
				"vanilla code may have been supplied bad data or program state as the result of an \n"
				"issue in a loaded DLL.");
		}
	}
}

namespace CrashLogger
{
	void Get(EXCEPTION_POINTERS* info) {
		HANDLE  processHandle = GetCurrentProcess();
		HANDLE  threadHandle = GetCurrentThread();

		 { Calltrace::Get(info, processHandle, threadHandle); }

		 { Registry::Get(info); }

		 __try { Stack::Get(info); }
		 __except (EXCEPTION_EXECUTE_HANDLER) {}

		 { ModuleBases::Get(info, processHandle); }

		SymCleanup(processHandle);
	};

	static LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

	LONG WINAPI Filter(EXCEPTION_POINTERS* info) {
		static bool caught = false;
		bool ignored = false;
		//
		if (caught) ignored = true;
		else
		{
			caught = true;
			try { Get(info); }
			catch (...) {};
		}
		if (s_originalFilter) s_originalFilter(info); // don't return
		return !ignored ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
	};

	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI FakeSetUnhandledExceptionFilter(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) {
		s_originalFilter = lpTopLevelExceptionFilter;
		return nullptr;
	}
	//

	void QuickTest() {
		const UInt32* playerptr = (UInt32*) g_player->baseProcess;
		const UInt32 size = sizeof HighProcess;

		for (unsigned int i = 0x97; 4 * i < size; i += 1)
		{
			try
			{
				std::string result = YvilesMagicBox::DecryptPtr(playerptr[i]);;
				//if (!result.empty())
					Log(true, Log::kLog) << FormatString("0x%03X ", 4 * i) + YvilesMagicBox::DecryptPtr(playerptr[i]);
			}
			catch (...) {
			}
		}

	}

	void Apply() {
		s_originalFilter = SetUnhandledExceptionFilter(&Filter);
		//
		// Prevent disabling the filter:
		//
		SafeWrite32(0x00FDF180, (UInt32)&FakeSetUnhandledExceptionFilter);
		//else SafeWrite32(0x00A281B4, (UInt32)&FakeSetUnhandledExceptionFilter);


//		mainLoopDoOnce.push_back(QuickTest);
	};
};