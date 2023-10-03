﻿#include <main.h>
#include <Formatter.h>

#include <SafeWrite.h>
#include <CrashLogger.h>
#include <DbgHelpWrap.h>
#include <set>

#define SYMOPT_EX_WINE_NATIVE_MODULES 1000

constexpr UInt32 ce_printStackCount = 256;

namespace CrashLogger
{
	template<typename T>
	class Dereference {
		intptr_t pointer;
		std::size_t size;

	public:
		Dereference(intptr_t pointer, std::size_t size) : pointer(pointer), size(size) {}

		Dereference(intptr_t pointer) : pointer(pointer), size(sizeof(T)) {}
		Dereference(const void* pointer) : pointer((intptr_t) pointer), size(sizeof(T)) {}

		operator bool()
		{
			return IsValidPointer();
		}

		operator T* () {
			if (IsValidPointer()) {
				return reinterpret_cast<T*>(pointer);
			}

			throw std::runtime_error("Bad dereference");
		}

		T* operator->() {
			if (IsValidPointer()) {
				return reinterpret_cast<T*>(pointer);
			}

			throw std::runtime_error("Bad dereference");
		}

	private:
		bool IsValidAddress() const
		{
			MEMORY_BASIC_INFORMATION mbi;
			if (::VirtualQuery((void*)pointer, &mbi, sizeof(mbi)))
			{
				if (mbi.State != MEM_COMMIT) return false;

				DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
				if ((mbi.Protect & mask) == 0) return false;

				if (mbi.Protect & PAGE_GUARD) return false;
				if (mbi.Protect & PAGE_NOACCESS) return false;

				if (size_t(mbi.RegionSize) < size) return false;

				return true;
			}
			return false;
		}

		bool AttemptDereference() const 
		{
			__try {
				// Attempt to read the address as a UInt32
				volatile UInt32 temp = *reinterpret_cast<const volatile UInt32*>(pointer);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return false;
			}
		}

		bool IsVtableValid() const
		{
//			if (vtables_.find(vtable) == vtables_.end()) return false;

			UInt32 vtable = *reinterpret_cast<UInt32*>(pointer);

			if (vtable > 0xFE0000 && vtable < 0x1200000)
				return true;

			return false;
		}

		bool IsValidPointer() const 
		{
			if (!IsValidAddress()) return false;

			if (!AttemptDereference()) return false;

			if (!IsVtableValid()) return false;

			return true;
		}
	};
}

namespace CrashLogger::PDBHandling
{
	std::string GetAddress(UInt32 eip)
	{
		return std::format("0x{:08X}", eip);
	}

	std::string GetModule(UInt32 eip, HANDLE process)
	{
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!Safe_SymGetModuleInfo(process, eip, &module)) return "";

		return module.ModuleName;
	}

	UInt32 GetModuleBase(UInt32 eip, HANDLE process)
	{
		IMAGEHLP_MODULE module = { 0 };
		module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
		if (!Safe_SymGetModuleInfo(process, eip, &module)) return 0;

		return module.BaseOfImage;
	}

	std::string GetSymbol(UInt32 eip, HANDLE process)
	{
		char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
		const auto symbol = (IMAGEHLP_SYMBOL*)symbolBuffer;

		symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		symbol->MaxNameLength = 254;
		DWORD offset = 0;

		if (!Safe_SymGetSymFromAddr(process, eip, &offset, symbol)) return "";
		
		const std::string functioName = symbol->Name;

		return std::format("{}+0x{:0X}", functioName, offset);
	}

	std::string GetLine(UInt32 eip, HANDLE process)
	{
		char lineBuffer[sizeof(IMAGEHLP_LINE) + 255];
		const auto line = (IMAGEHLP_LINE*)lineBuffer;
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD offset = 0;

		if (!Safe_SymGetLineFromAddr(process, eip, &offset, line)) return "";

		return std::format("<{}:{:d}>", line->FileName, line->LineNumber);
	}

	std::string GetCalltraceFunction(UInt32 eip, UInt32 ebp, HANDLE process)
	{
		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
			   if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/
		
		const auto moduleBase = GetModuleBase(eip, process);

		std::string begin = std::format("0x{:08X} ==> ", (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip);

		std::string middle;

		if (const auto module = GetModule(eip, process); module.empty()) 
			middle = std::format("{:20s} ({}) : (Corrupt stack or heap?)", "-\\(°_o)/-", GetAddress(ebp));
		else if (const auto symbol = GetSymbol(eip, process); symbol.empty())
			middle = std::format("{:20s} ({}) : EntryPoint+0xFFFFFFFF", module, GetAddress(ebp));
		else
			middle = std::format("{:20s} ({}) : {}", module, GetAddress(ebp), symbol);

		std::string end;

		if (const auto line = GetLine(eip, process); !line.empty())
		{
			middle = std::format("{:<80s}", middle);
			end = " <== " + line;
		} 

		return begin + middle + end;
	}

	std::string GetSymbolForAddress(UInt32 address, HANDLE process)
	{
		std::string module;
		if (module = GetModule(address, process); module.empty()) return "";

		std::string symbol;
		if (symbol = GetSymbol(address, process); symbol.empty()) return "";

		std::string end;

		if (const auto line = GetLine(address, process); !line.empty())
		{
			end = " <== " + line;
		}

		return std::format("{:>20} : {:<40}{}", module, symbol, end);
	}
}

namespace CrashLogger::VirtualTables
{
	typedef void (*FormattingHandler)(void* ptr, std::string& buffer);
	
	void AsUInt32(void* ptr, std::string& buffer) { buffer += std::format("{:#08X}", **static_cast<UInt32**>(ptr)); }

	template<typename T> std::ostream& AsInternal(std::ostream& os, void* ptr)
	{
		if (auto sanitized = Dereference<T>((UInt32)ptr))
			os << *sanitized;
		return os;
	}

	template<typename T> std::ostream& AsSEH(std::ostream& os, void* ptr)
	{
		__try {
			AsInternal<T>(os, ptr);
		}
		__except (1) {
			if (os.rdbuf()->in_avail()) os << ", ";
			os << "failed to format";
		}
		return os;
	}

	template<typename T> void As(void* ptr, std::string& buffer)
	{
		std::stringstream ss;
		AsSEH<T>(ss, ptr);
		if (!ss.str().empty()) buffer += ": " + ss.str();
	}

	FormattingHandler lastHandler = nullptr;

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
		VirtualTables::FormattingHandler function;
		std::string name;

		Label(UInt32 address, VirtualTables::FormattingHandler function = lastHandler, std::string name = "", Type type = kType_RTTIClass, UInt32 size = 4)
			: type(type), address(address), size(size), function(function), name(std::move(name))
		{
			lastHandler = function;
		};
		
		bool Satisfies(const void* ptr) const
		{
			return *Dereference<UInt32>(ptr) >= address && *Dereference<UInt32>(ptr) <= address + size;
		}

		static std::string GetTypeName(void* ptr)
		{
			return GetObjectClassName(ptr);
		}

		[[nodiscard]] std::string GetLabelName() const
		{
			if (type == kType_RTTIClass) return "RTTI: ";
			if (type == kType_Global) return "Global: ";
			return "";
		}

		void Get(void* ptr, std::string& buffer) const
		{
			buffer.clear();
			buffer += GetLabelName();

			std::string name1 = name.empty() && type == kType_RTTIClass ? GetTypeName(ptr) : name;
			if (name1.empty() && type != kType_None) name1 = std::format("0x{:08X}", *Dereference<UInt32>((UInt32)ptr));
			buffer += name1;

			if (function) function(ptr, buffer);
		}
	};

	std::vector<std::unique_ptr<Label>> setOfLabels;
	
	template <class... _Types> void Push(_Types... args) {
		setOfLabels.push_back(std::make_unique<Label>(std::forward<_Types>(args)...));
	}

	void FillNVSELabels()
	{
		Push(0x10F1EE0, nullptr, "TypeInfo", Label::kType_RTTIClass);

		Push(0x11F3374, VirtualTables::AsUInt32, "TileValueIndirectTemp", Label::kType_Global);
		Push(0x118FB0C, nullptr, "ShowWhoDetects", Label::kType_Global);
		Push(0x1042C58, nullptr, "ShowWhoDetects", Label::kType_Global);
		Push(0x011F6238, nullptr, "HeapManager", Label::kType_Global);

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

		Push(0x108C0D0, nullptr, "NiTPrimitiveArray<MobileObject*>");
		Push(0x10C1760, nullptr, "LockFreeQueue<NiPointer<IOTask>>");
		Push(0x101747C, nullptr, "LockFreeMap<TESObjectREFR*, NiPointer<QueuedReference>>");

		Push(0x1000000, nullptr, "", Label::kType_None); // integer that is often encountered
		Push(0x11C0000, nullptr, "", Label::kType_None);
		
		Push(kVtbl_Menu, As<Menu>);
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

		Push(kVtbl_Tile, As<Tile>);
		Push(kVtbl_TileMenu);
		Push(kVtbl_TileRect);
		Push(kVtbl_TileImage);
		Push(kVtbl_RadialTile);
		Push(kVtbl_TileText);
		Push(kVtbl_Tile3D);

		Push(kVtbl_StartMenuOption, As<StartMenu::Option>, "StartMenu::Option");
		Push(kVtbl_StartMenuUserOption, As<StartMenu::Option>, "StartMenu::UserOption");

		Push(kVtbl_Setting, As<Setting>);
		Push(kVtbl_GameSettingCollection, nullptr);
		Push(kVtbl_INIPrefSettingCollection);
		Push(kVtbl_INISettingCollection);

		Push(kVtbl_NavMesh, As<NavMesh>);
		Push(kVtbl_TESForm, As<TESForm>);
		Push(kVtbl_AlchemyItem);
		Push(kVtbl_BGSConstructibleObject);
		Push(kVtbl_BGSDebris);
		Push(kVtbl_BGSDehydrationStage);
		Push(kVtbl_BGSExplosion);
		Push(kVtbl_BGSHeadPart);
		Push(kVtbl_BGSHungerStage);
		Push(kVtbl_BGSListForm);
		Push(kVtbl_BGSMessage);
		Push(kVtbl_BGSMovableStatic);
		Push(kVtbl_BGSNote);
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
		Push(kVtbl_Script, As<Script>);
		Push(kVtbl_SpellItem, As<TESForm>);
		Push(kVtbl_TESActorBase);
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
		Push(kVtbl_TESIdleForm);
		Push(kVtbl_TESImageSpace);
		Push(kVtbl_TESImageSpaceModifier);
		Push(kVtbl_TESKey);
		Push(kVtbl_TESLandTexture);
		Push(kVtbl_TESLevCharacter);
		Push(kVtbl_TESLevCreature);
		Push(kVtbl_TESLevItem);
		Push(kVtbl_TESLevSpell);
		Push(kVtbl_TESLoadScreen);
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
		Push(kVtbl_TESQuest);
		Push(kVtbl_TESRace);
		Push(kVtbl_TESRecipe);
		Push(kVtbl_TESRecipeCategory);
		Push(kVtbl_TESRegion);
		Push(kVtbl_TESReputation);
		Push(kVtbl_TESSkill);
		Push(kVtbl_TESSound);
		Push(kVtbl_TESTopic);
		Push(kVtbl_TESTopicInfo);
		Push(kVtbl_TESWaterForm);
		Push(kVtbl_TESWeather);
		Push(kVtbl_TESWorldSpace);

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

		Push(kVtbl_TESObjectREFR, As<TESObjectREFR>);
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

		Push(kVtbl_QueuedReference, As<QueuedReference>);
		Push(kVtbl_QueuedCharacter);
		Push(kVtbl_QueuedActor);
		Push(kVtbl_QueuedCreature);
		Push(kVtbl_QueuedPlayer);

		Push(kVtbl_ActorMover, As<ActorMover>);
		Push(kVtbl_PlayerMover);

		Push(kVtbl_BaseProcess, As<BaseProcess>);
		Push(kVtbl_LowProcess);
		Push(kVtbl_MiddleLowProcess);
		Push(kVtbl_MiddleHighProcess);
		Push(kVtbl_HighProcess);



		Push(kVtbl_TESTexture, As<TESTexture>);
		Push(kVtbl_TESIcon);
		Push(kVtbl_QueuedTexture, As<QueuedTexture>);


		// Ni
		Push(kVtbl_NiObjectNET, As<NiObjectNET>);

		// NiProperty
		Push(kVtbl_NiProperty);
		Push(kVtbl_NiTexturingProperty);
		Push(kVtbl_NiVertexColorProperty);
//		Push(kVtbl_NiWireframeProperty);
		Push(kVtbl_NiZBufferProperty);
		Push(kVtbl_NiMaterialProperty);
		Push(kVtbl_NiAlphaProperty);
		Push(kVtbl_NiStencilProperty);
//		Push(kVtbl_NiRendererSpecificProperty);
		Push(kVtbl_NiShadeProperty);

		Push(kVtbl_BSShaderProperty);
		Push(kVtbl_SkyShaderProperty);
		Push(kVtbl_ParticleShaderProperty);
		Push(kVtbl_BSShaderLightingProperty);
		Push(kVtbl_DistantLODShaderProperty);
		Push(kVtbl_TallGrassShaderProperty);
		Push(kVtbl_BSShaderPPLightingProperty);
		Push(kVtbl_SpeedTreeShaderPPLightingProperty);
		Push(kVtbl_SpeedTreeBranchShaderProperty);
		Push(kVtbl_Lighting30ShaderProperty);
		Push(kVtbl_HairShaderProperty);
		Push(kVtbl_SpeedTreeShaderLightingProperty);
		Push(kVtbl_SpeedTreeLeafShaderProperty);
//		Push(kVtbl_SpeedTreeFrondShaderProperty);
		Push(kVtbl_GeometryDecalShaderProperty);
		Push(kVtbl_PrecipitationShaderProperty);
		Push(kVtbl_BoltShaderProperty);
		Push(kVtbl_WaterShaderProperty);
		Push(kVtbl_NiSpecularProperty);
		Push(kVtbl_NiFogProperty);
		Push(kVtbl_BSFogProperty);
		Push(kVtbl_NiDitherProperty);

		// unk
		Push(kVtbl_SpeedTreeBillboardShaderProperty);
		Push(kVtbl_TileShaderProperty);
		Push(kVtbl_VolumetricFogShaderProperty);
		Push(kVtbl_BSDistantTreeShaderProperty);
		Push(kVtbl_BSShaderBloodSplatterProperty);
		Push(kVtbl_BSShaderNoLighting);
		Push(kVtbl_BSShaderNoLightingProperty);
		Push(kVtbl_ExtraRefractionProperty);
		Push(kVtbl_NiCullingProperty);
		
		// NiTexture
		Push(kVtbl_NiTexture);
//		Push(kVtbl_NiDX9Direct3DTexture);
		Push(kVtbl_NiSourceTexture);
		Push(kVtbl_NiSourceCubeMap);
		Push(kVtbl_NiRenderedTexture);
		Push(kVtbl_NiRenderedCubeMap);

		// NiAVObject
		Push(kVtbl_NiAVObject);
		Push(kVtbl_NiDynamicEffect);
		Push(kVtbl_NiDirectionalLight);
		Push(kVtbl_NiPointLight);
		Push(kVtbl_NiSpotLight);
		Push(kVtbl_NiAmbientLight);
		Push(kVtbl_NiTextureEffect);

		Push(kVtbl_NiNode, As<NiNode>);
		Push(kVtbl_SceneGraph);
		Push(kVtbl_BSTempNode);
		Push(kVtbl_BSTempNodeManager);
//		Push(kVtbl_BSCellNode);
		Push(kVtbl_BSClearZNode);
		Push(kVtbl_BSFadeNode, nullptr, "BSFadeNode"); // missing RTTI name
//		Push(kVtbl_BSScissorNode);
//		Push(kVtbl_BSTimingNode);
		Push(kVtbl_BSFaceGenNiNode, As<NiNode>);
		Push(kVtbl_NiBillboardNode);
		Push(kVtbl_NiSwitchNode);
		Push(kVtbl_NiLODNode);
//		Push(kVtbl_NiBSLODNode);
		Push(kVtbl_NiSortAdjustNode);
		Push(kVtbl_NiBSPNode);
		Push(kVtbl_ShadowSceneNode); // 10ADCE0
		Push(kVtbl_NiCamera);
		Push(kVtbl_BSCubeMapCamera);
		Push(kVtbl_NiScreenSpaceCamera);

		// NiGeometry
		Push(kVtbl_NiGeometry, As<NiObjectNET>);
		Push(kVtbl_NiLines);
		Push(kVtbl_NiTriBasedGeom);
		Push(kVtbl_NiTriShape);
		Push(kVtbl_BSScissorTriShape);
		Push(kVtbl_NiScreenElements);
		Push(kVtbl_NiScreenGeometry);
//		Push(kVtbl_TallGrassTriShape);
		Push(kVtbl_NiTriStrips);
//		Push(kVtbl_TallGrassTriStrips);
		Push(kVtbl_NiParticles);
		Push(kVtbl_NiParticleSystem);
		Push(kVtbl_NiMeshParticleSystem);
		Push(kVtbl_NiParticleMeshes);
		Push(kVtbl_NiSequenceStreamHelper);

		// NiRenderer
		Push(kVtbl_NiRenderer);
		Push(kVtbl_NiDX9Renderer);

		// Misc
		Push(kVtbl_NiPixelData);
		Push(kVtbl_NiCollisionObject);
		Push(kVtbl_NiCollisionData);
		Push(kVtbl_bhkNiCollisionObject);
		Push(kVtbl_bhkCollisionObject);
		Push(kVtbl_bhkBlendCollisionObject);
		Push(kVtbl_bhkBlendCollisionObjectAddRotation);
		Push(kVtbl_WeaponObject);
		Push(kVtbl_bhkPCollisionObject);
		Push(kVtbl_bhkSPCollisionObject);

		// Animations
		// TODO:: figure out why the hell the source is wrong and parent of NiController is NiObject not NiObjectNET
		Push(kVtbl_NiControllerSequence);

		// NiTimeController
		Push(kVtbl_NiTimeController);
//		Push(kVtbl_BSDoorHavokController);
		Push(kVtbl_BSPlayerDistanceCheckController);
//		Push(kVtbl_NiD3DController);
		Push(kVtbl_NiControllerManager);
		Push(kVtbl_NiInterpController);
		Push(kVtbl_NiSingleInterpController);
		Push(kVtbl_NiTransformController);
		Push(kVtbl_NiPSysModifierCtlr);
		Push(kVtbl_NiPSysEmitterCtlr);
		Push(kVtbl_NiPSysModifierBoolCtlr);
		Push(kVtbl_NiPSysModifierActiveCtlr);
		Push(kVtbl_NiPSysModifierFloatCtlr);
		Push(kVtbl_NiPSysInitialRotSpeedVarCtlr);
		Push(kVtbl_NiPSysInitialRotSpeedCtlr);
		Push(kVtbl_NiPSysInitialRotAngleVarCtlr);
		Push(kVtbl_NiPSysInitialRotAngleCtlr);
		Push(kVtbl_NiPSysGravityStrengthCtlr);
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
		Push(kVtbl_NiPSysAirFieldSpreadCtlr);
		Push(kVtbl_NiPSysAirFieldInheritVelocityCtlr);
		Push(kVtbl_NiPSysAirFieldAirFrictionCtlr);

		Push(kVtbl_NiFloatInterpController);
		Push(kVtbl_NiFlipController);
		Push(kVtbl_NiAlphaController);
		Push(kVtbl_NiTextureTransformController);
		Push(kVtbl_NiLightDimmerController);

		Push(kVtbl_NiBoolInterpController);
		Push(kVtbl_NiVisController);
		Push(kVtbl_NiPoint3InterpController);
		Push(kVtbl_NiMaterialColorController);
		Push(kVtbl_NiLightColorController);
		Push(kVtbl_NiExtraDataController);
		Push(kVtbl_NiFloatsExtraDataPoint3Controller);
		Push(kVtbl_NiFloatsExtraDataController);
		Push(kVtbl_NiFloatExtraDataController);
		Push(kVtbl_NiColorExtraDataController);
		Push(kVtbl_NiMultiTargetTransformController);
		Push(kVtbl_NiGeomMorpherController);

		Push(kVtbl_bhkBlendController);
		Push(kVtbl_bhkForceController);
		Push(kVtbl_NiBSBoneLODController);
		Push(kVtbl_NiUVController);
		Push(kVtbl_NiPathController);
		Push(kVtbl_NiLookAtController);
		Push(kVtbl_NiKeyframeManager);
		Push(kVtbl_NiBoneLODController);
		Push(kVtbl_NiPSysUpdateCtlr);
		Push(kVtbl_NiPSysResetOnLoopCtlr);
		Push(kVtbl_NiFloatController);
		Push(kVtbl_NiRollController);

		Push(kVtbl_bhkSerializable);
		Push(kVtbl_bhkWorld);
		Push(kVtbl_bhkWorldM);
		Push(kVtbl_bhkAction);
		Push(kVtbl_bhkUnaryAction);
		Push(kVtbl_bhkMouseSpringAction);
		Push(kVtbl_bhkMotorAction);
		Push(kVtbl_bhkBinaryAction);
		Push(kVtbl_bhkSpringAction);
		Push(kVtbl_bhkAngularDashpotAction);
		Push(kVtbl_bhkDashpotAction);
		Push(kVtbl_bhkWorldObject);
		Push(kVtbl_bhkPhantom);
		Push(kVtbl_bhkShapePhantom);
		Push(kVtbl_bhkSimpleShapePhantom);
		Push(kVtbl_bhkCachingShapePhantom);
		Push(kVtbl_bhkAabbPhantom);
//		Push(kVtbl_bhkAvoidBox);
		Push(kVtbl_bhkEntity);
		Push(kVtbl_bhkRigidBody);
		Push(kVtbl_bhkRigidBodyT);
		Push(kVtbl_bhkConstraint);
		Push(kVtbl_bhkLimitedHingeConstraint);
		Push(kVtbl_bhkMalleableConstraint);
		Push(kVtbl_bhkBreakableConstraint);
		Push(kVtbl_bhkWheelConstraint);
		Push(kVtbl_bhkStiffSpringConstraint);
		Push(kVtbl_bhkRagdollConstraint);
		Push(kVtbl_bhkPrismaticConstraint);
		Push(kVtbl_bhkHingeConstraint);
		Push(kVtbl_bhkBallAndSocketConstraint);
		Push(kVtbl_bhkGenericConstraint);
		Push(kVtbl_bhkFixedConstraint);
//		Push(kVtbl_bhkPointToPathConstraint);
//		Push(kVtbl_bhkPoweredHingeConstraint);

		Push(kVtbl_bhkShape);
		Push(kVtbl_bhkTransformShape);
		Push(kVtbl_bhkSphereRepShape);
		Push(kVtbl_bhkConvexShape);
		Push(kVtbl_bhkSphereShape);
		Push(kVtbl_bhkCapsuleShape);
		Push(kVtbl_bhkBoxShape);
		Push(kVtbl_bhkTriangleShape);
		Push(kVtbl_bhkCylinderShape);
		Push(kVtbl_bhkConvexVerticesShape);
		Push(kVtbl_bhkCharControllerShape);
		Push(kVtbl_bhkConvexTransformShape);
		Push(kVtbl_bhkConvexSweepShape);
		Push(kVtbl_bhkMultiSphereShape);
		Push(kVtbl_bhkBvTreeShape);
		Push(kVtbl_bhkTriSampledHeightFieldBvTreeShape);
		Push(kVtbl_bhkMoppBvTreeShape);
		Push(kVtbl_bhkShapeCollection);
		Push(kVtbl_bhkListShape);
		Push(kVtbl_bhkPackedNiTriStripsShape);
		Push(kVtbl_bhkNiTriStripsShape);
		Push(kVtbl_bhkHeightFieldShape);
		Push(kVtbl_bhkPlaneShape);
		Push(kVtbl_bhkCharacterProxy);
		Push(kVtbl_bhkCharacterListenerArrow);
		Push(kVtbl_bhkCharacterListenerSpell);
		Push(kVtbl_bhkCharacterController);

		Push(kVtbl_NiExtraData);
//		Push(kVtbl_TESObjectExtraData);
		Push(kVtbl_BSFaceGenAnimationData);
		Push(kVtbl_BSFaceGenModelExtraData);
		Push(kVtbl_BSFaceGenBaseMorphExtraData);
		Push(kVtbl_DebugTextExtraData);
		Push(kVtbl_NiStringExtraData);
		Push(kVtbl_NiFloatExtraData);
//		Push(kVtbl_FadeNodeMaxAlphaExtraData);
		Push(kVtbl_BSFurnitureMarker);
		Push(kVtbl_NiBinaryExtraData);
		Push(kVtbl_BSBound);
		Push(kVtbl_NiSCMExtraData);
		Push(kVtbl_NiTextKeyExtraData);
		Push(kVtbl_NiVertWeightsExtraData);
		Push(kVtbl_bhkExtraData);
		Push(kVtbl_PArrayPoint);
		Push(kVtbl_NiIntegerExtraData);
		Push(kVtbl_BSXFlags);
		Push(kVtbl_NiFloatsExtraData);
		Push(kVtbl_NiColorExtraData);
		Push(kVtbl_NiVectorExtraData);
		Push(kVtbl_NiSwitchStringExtraData);
		Push(kVtbl_NiStringsExtraData);
		Push(kVtbl_NiIntegersExtraData);
		Push(kVtbl_NiBooleanExtraData);
		Push(kVtbl_NiAdditionalGeometryData);
		Push(kVtbl_BSPackedAdditionalGeometryData);
		Push(kVtbl_NiGeometryData);
		Push(kVtbl_NiLinesData);
		Push(kVtbl_NiTriBasedGeomData);
		Push(kVtbl_NiTriStripsData);
		Push(kVtbl_NiTriStripsDynamicData);
		Push(kVtbl_NiTriShapeData);
		Push(kVtbl_NiScreenElementsData);
		Push(kVtbl_NiTriShapeDynamicData);
		Push(kVtbl_NiScreenGeometryData);
		Push(kVtbl_NiParticlesData);
		Push(kVtbl_NiPSysData);
		Push(kVtbl_NiMeshPSysData);
		Push(kVtbl_NiParticleMeshesData);

		Push(kVtbl_NiTask);
//		Push(kVtbl_BSTECreateTask);
//		Push(kVtbl_NiParallelUpdateTaskManager::SignalTask);
		Push(kVtbl_NiGeomMorpherUpdateTask);
		Push(kVtbl_NiPSysUpdateTask);
		Push(kVtbl_NiSkinInstance);
		Push(kVtbl_NiSkinPartition);
		Push(kVtbl_NiSkinData);
		Push(kVtbl_NiRenderTargetGroup);
		Push(kVtbl_Ni2DBuffer);
		Push(kVtbl_NiDepthStencilBuffer);
		Push(kVtbl_NiUVData);
		Push(kVtbl_NiStringPalette);
		Push(kVtbl_NiSequence);
		Push(kVtbl_NiRotData);
		Push(kVtbl_NiPosData);
		Push(kVtbl_NiMorphData);
		Push(kVtbl_NiTransformData);
		Push(kVtbl_NiFloatData);
		Push(kVtbl_NiColorData);
		Push(kVtbl_NiBSplineData);
		Push(kVtbl_NiBSplineBasisData);
		Push(kVtbl_NiBoolData);
//		Push(kVtbl_NiTaskManager);
//		Push(kVtbl_NiParallelUpdateTaskManager);
		Push(kVtbl_hkPackedNiTriStripsData);
		Push(kVtbl_NiInterpolator);
		Push(kVtbl_NiBlendInterpolator);
		Push(kVtbl_NiBlendTransformInterpolator);
		Push(kVtbl_NiBlendAccumTransformInterpolator);
		Push(kVtbl_NiBlendFloatInterpolator);
		Push(kVtbl_NiBlendQuaternionInterpolator);
		Push(kVtbl_NiBlendPoint3Interpolator);
		Push(kVtbl_NiBlendColorInterpolator);
		Push(kVtbl_NiBlendBoolInterpolator);
		Push(kVtbl_NiLookAtInterpolator);
		Push(kVtbl_NiKeyBasedInterpolator);
		Push(kVtbl_NiFloatInterpolator);
		Push(kVtbl_NiTransformInterpolator);
		Push(kVtbl_NiQuaternionInterpolator);
		Push(kVtbl_NiPoint3Interpolator);
		Push(kVtbl_NiPathInterpolator);
		Push(kVtbl_NiColorInterpolator);
		Push(kVtbl_NiBoolInterpolator);
		Push(kVtbl_NiBoolTimelineInterpolator);

		Push(kVtbl_NiBSplineInterpolator);
		Push(kVtbl_NiBSplineTransformInterpolator);
		Push(kVtbl_NiBSplineCompTransformInterpolator);
		Push(kVtbl_NiBSplinePoint3Interpolator);
		Push(kVtbl_NiBSplineCompPoint3Interpolator);
		Push(kVtbl_NiBSplineFloatInterpolator);
		Push(kVtbl_NiBSplineCompFloatInterpolator);
		Push(kVtbl_NiBSplineColorInterpolator);
		Push(kVtbl_NiBSplineCompColorInterpolator);

		Push(kVtbl_NiAVObjectPalette);
		Push(kVtbl_NiDefaultAVObjectPalette);
		Push(kVtbl_BSReference);
		Push(kVtbl_BSNodeReferences);
		Push(kVtbl_NiPalette);
		Push(kVtbl_NiLODData);
		Push(kVtbl_NiRangeLODData);
		Push(kVtbl_NiScreenLODData);
		Push(kVtbl_NiPSysModifier);
		Push(kVtbl_BSWindModifier);
		Push(kVtbl_NiPSysMeshUpdateModifier);
		Push(kVtbl_NiPSysRotationModifier);
		Push(kVtbl_NiPSysEmitter);
		Push(kVtbl_NiPSysMeshEmitter);
		Push(kVtbl_NiPSysVolumeEmitter);
		Push(kVtbl_NiPSysCylinderEmitter);
		Push(kVtbl_NiPSysSphereEmitter);
		Push(kVtbl_NiPSysBoxEmitter);
		Push(kVtbl_BSPSysArrayEmitter);
		Push(kVtbl_NiPSysGravityModifier);
		Push(kVtbl_NiPSysSpawnModifier);
		Push(kVtbl_BSParentVelocityModifier);
		Push(kVtbl_NiPSysPositionModifier);
		Push(kVtbl_NiPSysGrowFadeModifier);
		Push(kVtbl_NiPSysDragModifier);
		Push(kVtbl_NiPSysColorModifier);
		Push(kVtbl_NiPSysColliderManager);
		Push(kVtbl_NiPSysBoundUpdateModifier);
		Push(kVtbl_NiPSysBombModifier);
		Push(kVtbl_NiPSysAgeDeathModifier);
		Push(kVtbl_NiPSysVortexFieldModifier);
		Push(kVtbl_NiPSysTurbulenceFieldModifier);
		Push(kVtbl_NiPSysRadialFieldModifier);
		Push(kVtbl_NiPSysGravityFieldModifier);
		Push(kVtbl_NiPSysDragFieldModifier);
		Push(kVtbl_NiPSysAirFieldModifier);
		Push(kVtbl_NiPSysEmitterCtlrData);
		Push(kVtbl_NiAccumulator);
		Push(kVtbl_NiBackToFrontAccumulator);
		Push(kVtbl_NiAlphaAccumulator);
//		Push(kVtbl_BSShaderAccumulator);
		Push(kVtbl_NiScreenPolygon);
		Push(kVtbl_NiScreenTexture);
		Push(kVtbl_NiPSysCollider);
		Push(kVtbl_NiPSysSphericalCollider);
		Push(kVtbl_NiPSysPlanarCollider);

		Push(kVtbl_NiShader, nullptr);

		// NiStream
		Push(kVtbl_NiStream, As<NiStream>);
		Push(kVtbl_BSStream);


		// animations
		Push(kVtbl_BSAnimGroupSequence, As<BSAnimGroupSequence>);
		Push(kVtbl_AnimSequenceBase);
		Push(kVtbl_AnimSequenceSingle, As<AnimSequenceSingle>);
		Push(kVtbl_AnimSequenceMultiple, As<AnimSequenceMultiple>);

		Push(kVtbl_QueuedKF, As<QueuedKF>);
		Push(kVtbl_QueuedAnimIdle);

		Push(kVtbl_BSFile, As<BSFile>);
		Push(kVtbl_ArchiveFile);
		Push(kVtbl_CompressedArchiveFile);

		// model
		Push(kVtbl_TESModel, As<TESModel>);
		Push(kVtbl_QueuedModel, As<QueuedModel>);

		// effects
		Push(kVtbl_ScriptEffect, As<ScriptEffect>);
		Push(kVtbl_ActiveEffect, As<ActiveEffect>);
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

		// w
		Push(kVtbl_AILinearTaskThread);
		Push(kVtbl_AITaskThread);
		Push(kVtbl_AbstractHeap);
		Push(kVtbl_ActorPackageData);
		Push(kVtbl_ActorPathHandler);
		Push(kVtbl_ActorPathingMessageQueue);
		Push(kVtbl_ActorUpdateTaskData);
		Push(kVtbl_ActorValueInfo);
		Push(kVtbl_ActorValueOwner);
		Push(kVtbl_ActorsScriptTaskData);
		Push(kVtbl_AnimIdle);
		Push(kVtbl_AnimationTaskData);
		Push(kVtbl_Archive);
		Push(kVtbl_ArrayOfTuplesImplementation);
		Push(kVtbl_Atmosphere);
		Push(kVtbl_AttachDistant3DTask);
		Push(kVtbl_AudioLoadTask);

		Push(kVtbl_BGSAbilityPerkEntry);
		Push(kVtbl_BGSAcousticSpace);
		Push(kVtbl_BGSAcousticSpaceListener);
		Push(kVtbl_BGSAddonNode);
		Push(kVtbl_BGSAddonNodeSoundHandleExtra);
		Push(kVtbl_BGSAmmoForm);
		Push(kVtbl_BGSBipedModelList);
		Push(kVtbl_BGSBodyPart);
		Push(kVtbl_BGSBodyPartData);
		Push(kVtbl_BGSCameraPath);
		Push(kVtbl_BGSCameraShot);
		Push(kVtbl_BGSCellNumericIDArrayMap);
		Push(kVtbl_BGSClipRoundsForm);
		Push(kVtbl_BGSDecalNode);
		Push(kVtbl_BGSDefaultObjectManager);
		Push(kVtbl_BGSDestructibleObjectForm);
		Push(kVtbl_BGSDistantObjectBlockLoadTask);
		Push(kVtbl_BGSDistantTreeBlockLoadTask);
		Push(kVtbl_BGSEncounterZone);
		Push(kVtbl_BGSEntryPointFunctionData);
		Push(kVtbl_BGSEntryPointFunctionDataActivateChoice);
		Push(kVtbl_BGSEntryPointFunctionDataLeveledList);
		Push(kVtbl_BGSEntryPointFunctionDataOneValue);
		Push(kVtbl_BGSEntryPointFunctionDataTwoValue);
		Push(kVtbl_BGSEntryPointPerkEntry);
		Push(kVtbl_BGSEquipType);
		Push(kVtbl_BGSIdleCollection);
		Push(kVtbl_BGSIdleMarker);
		Push(kVtbl_BGSImpactData);
		Push(kVtbl_BGSImpactDataSet);
		Push(kVtbl_BGSLightingTemplate);
		Push(kVtbl_BGSLoadFormBuffer);
		Push(kVtbl_BGSLoadGameBuffer);
		Push(kVtbl_BGSMenuIcon);
		Push(kVtbl_BGSMessageIcon);
		Push(kVtbl_BGSMoviePlayer);
		Push(kVtbl_BGSMusicType);
		Push(kVtbl_BGSOffsetDataArray);
		Push(kVtbl_BGSOpenCloseForm);
		Push(kVtbl_BGSPerkEntry);
		Push(kVtbl_BGSPickupPutdownSounds);
		Push(kVtbl_BGSPreloadable);
		Push(kVtbl_BGSPrimitive);
		Push(kVtbl_BGSPrimitiveBox);
		Push(kVtbl_BGSPrimitivePlane);
		Push(kVtbl_BGSPrimitiveSphere);
		Push(kVtbl_BGSQuestObjective);
		Push(kVtbl_BGSQuestPerkEntry);
		Push(kVtbl_BGSRadiationStage);
		Push(kVtbl_BGSRagdoll);
		Push(kVtbl_BGSReconstructCellBlocksMap);
		Push(kVtbl_BGSReconstructCellSubBlocksMap);
		Push(kVtbl_BGSReconstructCellsMap);
		Push(kVtbl_BGSReconstructFormsInAllFilesMap);
		Push(kVtbl_BGSReconstructFormsInFileMap);
		Push(kVtbl_BGSReconstructFormsMap);
		Push(kVtbl_BGSReconstructWorldSpacesMap);
		Push(kVtbl_BGSRepairItemList);
		Push(kVtbl_BGSSaveFormBuffer);
		Push(kVtbl_BGSSaveGameBuffer);
		Push(kVtbl_BGSSaveLoadChangesMap);
		Push(kVtbl_BGSSaveLoadStatsMap);
		Push(kVtbl_BGSSceneInfo);
		Push(kVtbl_BGSTerrainChunkLoadTask);
		Push(kVtbl_BGSTextureModel);
		Push(kVtbl_BGSTextureSet);
		Push(kVtbl_BGSTouchSpellForm);
		Push(kVtbl_BGSVoiceType);
		Push(kVtbl_BGSZoneTargetListener);

		Push(kVtbl_BSAnimNote);
		Push(kVtbl_BSAnimNoteReceiver);
		Push(kVtbl_BSAnimNotes);
		Push(kVtbl_BSAttachPoints);
		Push(kVtbl_BSAudio);
		Push(kVtbl_BSAudioListener);
		Push(kVtbl_BSAudioManager);
		Push(kVtbl_BSAudioManagerThread);
		Push(kVtbl_BSBatchRenderer);
		Push(kVtbl_BSBlastNode);
		Push(kVtbl_BSBlendTreadTransfInterpolator);
		Push(kVtbl_BSCacheDriveSystemUtility);
		Push(kVtbl_BSCoreMessage);
		Push(kVtbl_BSCullingProcess);
		Push(kVtbl_BSCustomSplatterExtraData);
		Push(kVtbl_BSDamageStage);
		Push(kVtbl_BSDebrisNode);
		Push(kVtbl_BSDecalPlacementVectorExtraData);
		Push(kVtbl_BSDiscBootSystemUtility);
		Push(kVtbl_BSDismemberSkinInstance);
		Push(kVtbl_BSDistantTreeShader);
		Push(kVtbl_BSExtraData);
		Push(kVtbl_BSFaceGenBinaryFile);
		Push(kVtbl_BSFaceGenImage);
		Push(kVtbl_BSFaceGenKeyframe);
		Push(kVtbl_BSFaceGenKeyframeMultiple);
		Push(kVtbl_BSFaceGenModel);
		Push(kVtbl_BSFaceGenModelMap);
		Push(kVtbl_BSFaceGenMorph);
		Push(kVtbl_BSFaceGenMorphData);
		Push(kVtbl_BSFaceGenMorphDataHair);
		Push(kVtbl_BSFaceGenMorphDataHead);
		Push(kVtbl_BSFaceGenMorphDifferential);
		Push(kVtbl_BSFaceGenMorphStatistical);
		Push(kVtbl_BSFadeNodeCuller);
		Push(kVtbl_BSFileCache);
		Push(kVtbl_BSFrustumFOVController);
		Push(kVtbl_BSGameDataSystemUtility);
		Push(kVtbl_BSGameSound);
		Push(kVtbl_BSGrabIKNote);
		Push(kVtbl_BSLinearTaskThread);
		Push(kVtbl_BSLookIKNote);
		Push(kVtbl_BSMasterParticleSystem);
		Push(kVtbl_BSMaterialEmittanceMultController);
		Push(kVtbl_BSMsgDialogSystemUtility);
		Push(kVtbl_BSMultiBound);
		Push(kVtbl_BSMultiBoundAABB);
		Push(kVtbl_BSMultiBoundCapsule);
		Push(kVtbl_BSMultiBoundNode);
		Push(kVtbl_BSMultiBoundOBB);
		Push(kVtbl_BSMultiBoundRoom);
		Push(kVtbl_BSMultiBoundShape);
		Push(kVtbl_BSMultiBoundSphere);
		Push(kVtbl_BSMultiTargetTreadTransfController);
		Push(kVtbl_BSNiAllocator);
		Push(kVtbl_BSNiNode);
		Push(kVtbl_BSOcclusionPlane);
		Push(kVtbl_BSOrderedNode);
		Push(kVtbl_BSPSysMultiTargetEmitterCtlr);
		Push(kVtbl_BSPSysSimpleColorModifier);
		Push(kVtbl_BSPSysStripUpdateModifier);
		Push(kVtbl_BSParallelTaskThread);
		Push(kVtbl_BSParticleSystemManager);
		Push(kVtbl_BSPortal);
		Push(kVtbl_BSPortalGraph);
		Push(kVtbl_BSPortalSharedNode);
		Push(kVtbl_BSRangeNode);
		Push(kVtbl_BSRefractionFirePeriodController);
		Push(kVtbl_BSRefractionStrengthController);
		Push(kVtbl_BSResizableTriShape);
		Push(kVtbl_BSRotAccumTransfInterpolator);
		Push(kVtbl_BSSaveDataSystemUtility);
		Push(kVtbl_BSSceneGraph);
		Push(kVtbl_BSSearchPath);
		Push(kVtbl_BSSegmentedTriShape);
		Push(kVtbl_BSShaderTextureSet);
		Push(kVtbl_BSSplatterExtraData);
		Push(kVtbl_BSStripPSysData);
		Push(kVtbl_BSStripParticleSystem);
		Push(kVtbl_BSSysInfoSystemUtility);
		Push(kVtbl_BSSystemUtilitiesStrings);
		Push(kVtbl_BSSystemUtility);
		Push(kVtbl_BSTaskThread);
		Push(kVtbl_BSTasklet);
		Push(kVtbl_BSTaskletData);
		Push(kVtbl_BSTaskletGroupData);
		Push(kVtbl_BSTaskletManager);
		Push(kVtbl_BSTaskletManagerCallback);
		Push(kVtbl_BSTempEffect);
		Push(kVtbl_BSTempEffectDebris);
		Push(kVtbl_BSTempEffectGeometryDecal);
		Push(kVtbl_BSTempEffectParticle);
		Push(kVtbl_BSTempEffectSimpleDecal);
		Push(kVtbl_BSTextureData);
		Push(kVtbl_BSTextureSet);
		Push(kVtbl_BSThread);
		Push(kVtbl_BSTreadTransfInterpolator);
		Push(kVtbl_BSTreeModel);
		Push(kVtbl_BSTreeNode);
		Push(kVtbl_BSValueNode);
		Push(kVtbl_BSWArray);
		Push(kVtbl_BSWin32Audio);
		Push(kVtbl_BSWin32AudioListener);
		Push(kVtbl_BSWin32FileCache);
		Push(kVtbl_BSWin32GameSound);
		Push(kVtbl_BSWin32TaskletData);
		Push(kVtbl_BSWin32TaskletGroupData);
		Push(kVtbl_BSWin32TaskletManager);
		Push(kVtbl_BackgroundCloneThread);
		Push(kVtbl_BaseExtraList);
		Push(kVtbl_BaseFormComponent);
		Push(kVtbl_BeamShader);
		Push(kVtbl_BlendSettingCollection);
		Push(kVtbl_BoltShader);

		Push(kVtbl_ByteArrayImplementation);
		Push(kVtbl_CAsyncOutputPin);
		Push(kVtbl_CAsyncReader);
		Push(kVtbl_CAsyncStream);
		Push(kVtbl_CBSAReader);
		Push(kVtbl_CBSAStream);
		Push(kVtbl_CBillboardLeaf);
		Push(kVtbl_CCallbackBase);
		Push(kVtbl_CEnumMediaTypes);
		Push(kVtbl_CEnumPins);
		Push(kVtbl_CIdvCamera);
		Push(kVtbl_CMediaSample);
		Push(kVtbl_CMemAllocator);
		Push(kVtbl_CTreeEngine);
		Push(kVtbl_CachedValuesOwner);
		Push(kVtbl_CellMopp);
		Push(kVtbl_ChangesMap);
		Push(kVtbl_CheckLineOfSightFilterWater);
		Push(kVtbl_CheckWithinMultiBoundTask);
		Push(kVtbl_Clouds);
		Push(kVtbl_CombatAction);
		Push(kVtbl_CombatActionAcquireLineOfSight);
		Push(kVtbl_CombatActionActivateCombatItem);
		Push(kVtbl_CombatActionApproachTarget);
		Push(kVtbl_CombatActionAttackGrenade);
		Push(kVtbl_CombatActionAttackGrenadeFlushTarget);
		Push(kVtbl_CombatActionAttackGrenadeFromCover);
		Push(kVtbl_CombatActionAttackHandToHand);
		Push(kVtbl_CombatActionAttackMelee);
		Push(kVtbl_CombatActionAttackRanged);
		Push(kVtbl_CombatActionAttackRangedExplosive);
		Push(kVtbl_CombatActionAttackRangedExplosiveFromCover);
		Push(kVtbl_CombatActionAttackRangedFromCover);
		Push(kVtbl_CombatActionAvoidThreat);
		Push(kVtbl_CombatActionDodge);
		Push(kVtbl_CombatActionDrawWeapon);
		Push(kVtbl_CombatActionFlee);
		Push(kVtbl_CombatActionHide);
		Push(kVtbl_CombatActionIgnoreBlockedTarget);
		Push(kVtbl_CombatActionInvestigate);
		Push(kVtbl_CombatActionMove);
		Push(kVtbl_CombatActionMoveAndAttackGrenade);
		Push(kVtbl_CombatActionMoveAndAttackRanged);
		Push(kVtbl_CombatActionMoveAndAttackRangedExplosive);
		Push(kVtbl_CombatActionMoveAndSwitchToMelee);
		Push(kVtbl_CombatActionSearch);
		Push(kVtbl_CombatActionSwitchWeapon);
		Push(kVtbl_CombatActionUseCombatItem);
		Push(kVtbl_CombatController);
		Push(kVtbl_CombatCoverMap);
		Push(kVtbl_CombatGoal);
		Push(kVtbl_CombatGoalActivateCombatItem);
		Push(kVtbl_CombatGoalAdvance);
		Push(kVtbl_CombatGoalAmbush);
		Push(kVtbl_CombatGoalAvoidThreats);
		Push(kVtbl_CombatGoalCluster);
		Push(kVtbl_CombatGoalFallBack);
		Push(kVtbl_CombatGoalFindTarget);
		Push(kVtbl_CombatGoalFlank);
		Push(kVtbl_CombatGoalFlee);
		Push(kVtbl_CombatGoalFlushTarget);
		Push(kVtbl_CombatGoalGuard);
		Push(kVtbl_CombatGoalHideFromTarget);
		Push(kVtbl_CombatGoalKillTarget);
		Push(kVtbl_CombatGoalReachDestination);
		Push(kVtbl_CombatGoalStayInCombatArea);
		Push(kVtbl_CombatGoalUseCombatItem);
		Push(kVtbl_CombatGroupStrategy);
		Push(kVtbl_CombatGroupStrategyAdvance);
		Push(kVtbl_CombatGroupStrategyAmbush);
		Push(kVtbl_CombatGroupStrategyFallBack);
		Push(kVtbl_CombatGroupStrategyFlank);
		Push(kVtbl_CombatPlanner);
		Push(kVtbl_CombatProcedure);
		Push(kVtbl_CombatProcedureActivateObject);
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
		Push(kVtbl_CombatTargetMap);
		Push(kVtbl_CombatThreatMap);
		Push(kVtbl_CstringArrayImplementation);
		Push(kVtbl_DebugText);
		Push(kVtbl_DetailedActorPathHandler);
		Push(kVtbl_DetectionTaskData);
		Push(kVtbl_DistantLODShader);

		Push(kVtbl_DoorInPathFilter);
		Push(kVtbl_EffectItemList);
		Push(kVtbl_ExteriorCellLoaderTask);
		Push(kVtbl_ExteriorCellNewReferencesMap);

		Push(kVtbl_ExtraAction);
		Push(kVtbl_ExtraActivateLoopSound);
		Push(kVtbl_ExtraActivateRef);
		Push(kVtbl_ExtraActivateRefChildren);
		Push(kVtbl_ExtraActorCause);
		Push(kVtbl_ExtraAmmo);
		Push(kVtbl_ExtraAnim);
		Push(kVtbl_ExtraAnimNoteReceiver);
		Push(kVtbl_ExtraAshPileRef);
		Push(kVtbl_ExtraAudioBuoyMarker);
		Push(kVtbl_ExtraAudioMarker);
		Push(kVtbl_ExtraCannotWear);
		Push(kVtbl_ExtraCell3D);
		Push(kVtbl_ExtraCellAcousticSpace);
		Push(kVtbl_ExtraCellCanopyShadowMask);
		Push(kVtbl_ExtraCellClimate);
		Push(kVtbl_ExtraCellImageSpace);
		Push(kVtbl_ExtraCellImpactSwap);
		Push(kVtbl_ExtraCellMusicType);
		Push(kVtbl_ExtraCellWaterType);
		Push(kVtbl_ExtraCharge);
		Push(kVtbl_ExtraCollisionData);
		Push(kVtbl_ExtraCombatStyle);
		Push(kVtbl_ExtraContainerChanges);
		Push(kVtbl_ExtraCount);
		Push(kVtbl_ExtraCreatureAwakeSound);
		Push(kVtbl_ExtraCreatureMovementSound);
		Push(kVtbl_ExtraDataList);
		Push(kVtbl_ExtraDecalRefs);
		Push(kVtbl_ExtraDetachTime);
		Push(kVtbl_ExtraDismemberedLimbs);
		Push(kVtbl_ExtraDistantData);
		Push(kVtbl_ExtraDroppedItemList);
		Push(kVtbl_ExtraEditorID);
		Push(kVtbl_ExtraEditorRefMoveData);
		Push(kVtbl_ExtraEmittanceSource);
		Push(kVtbl_ExtraEnableStateChildren);
		Push(kVtbl_ExtraEnableStateParent);
		Push(kVtbl_ExtraEncounterZone);
		Push(kVtbl_ExtraFactionChanges);
		Push(kVtbl_ExtraFollower);
		Push(kVtbl_ExtraFollowerSwimBreadcrumbs);
		Push(kVtbl_ExtraFriendHits);
		Push(kVtbl_ExtraGhost);
		Push(kVtbl_ExtraGlobal);
		Push(kVtbl_ExtraGuardedRefData);
		Push(kVtbl_ExtraHasNoRumors);
		Push(kVtbl_ExtraHavok);
		Push(kVtbl_ExtraHeadingTarget);
		Push(kVtbl_ExtraHealth);
		Push(kVtbl_ExtraHealthPerc);
		Push(kVtbl_ExtraHotkey);
		Push(kVtbl_ExtraIgnoredBySandbox);
		Push(kVtbl_ExtraInfoGeneralTopic);
		Push(kVtbl_ExtraItemDropper);
		Push(kVtbl_ExtraLastFinishedSequence);
		Push(kVtbl_ExtraLevCreaModifier);
		Push(kVtbl_ExtraLeveledCreature);
		Push(kVtbl_ExtraLeveledItem);
		Push(kVtbl_ExtraLight);
		Push(kVtbl_ExtraLinkedRef);
		Push(kVtbl_ExtraLinkedRefChildren);
		Push(kVtbl_ExtraLitWaterRefs);
		Push(kVtbl_ExtraLock);
		Push(kVtbl_ExtraMapMarker);
		Push(kVtbl_ExtraMerchantContainer);
		Push(kVtbl_ExtraModdingItem);
		Push(kVtbl_ExtraModelSwap);
		Push(kVtbl_ExtraMultiBound);
		Push(kVtbl_ExtraMultiBoundData);
		Push(kVtbl_ExtraMultiBoundRef);
		Push(kVtbl_ExtraNavMeshPortal);
		Push(kVtbl_ExtraNorthRotation);
		Push(kVtbl_ExtraObjectHealth);
		Push(kVtbl_ExtraOcclusionPlane);
		Push(kVtbl_ExtraOcclusionPlaneRefData);
		Push(kVtbl_ExtraOpenCloseActivateRef);
		Push(kVtbl_ExtraOriginalReference);
		Push(kVtbl_ExtraOwnership);
		Push(kVtbl_ExtraPackage);
		Push(kVtbl_ExtraPackageData);
		Push(kVtbl_ExtraPackageStartLocation);
		Push(kVtbl_ExtraPatrolRefData);
		Push(kVtbl_ExtraPatrolRefInUseData);
		Push(kVtbl_ExtraPersistentCell);
		Push(kVtbl_ExtraPlayerCrimeList);
		Push(kVtbl_ExtraPoison);
		Push(kVtbl_ExtraPortal);
		Push(kVtbl_ExtraPortalRefData);
		Push(kVtbl_ExtraPrimitive);
		Push(kVtbl_ExtraProcessMiddleLow);
		Push(kVtbl_ExtraRadiation);
		Push(kVtbl_ExtraRadioData);
		Push(kVtbl_ExtraRadius);
		Push(kVtbl_ExtraRagDollData);
		Push(kVtbl_ExtraRandomTeleportMarker);
		Push(kVtbl_ExtraRank);
		Push(kVtbl_ExtraReferencePointer);
		Push(kVtbl_ExtraReflectedRefs);


		Push(kVtbl_ExtraRegionList);
		Push(kVtbl_ExtraReservedMarkers);
		Push(kVtbl_ExtraRoom);
		Push(kVtbl_ExtraRoomRefData);
		Push(kVtbl_ExtraRunOncePacks);
		Push(kVtbl_ExtraSavedAnimation);
		Push(kVtbl_ExtraSavedHavokData);
		Push(kVtbl_ExtraSayToTopicInfo);
		Push(kVtbl_ExtraSayTopicInfoOnceADay);
		Push(kVtbl_ExtraScale);
		Push(kVtbl_ExtraScript);
		Push(kVtbl_ExtraSecuritronFace);
		Push(kVtbl_ExtraSeed);
		Push(kVtbl_ExtraSeenData);
		Push(kVtbl_ExtraSound);
		Push(kVtbl_ExtraSpecialRenderFlags);
		Push(kVtbl_ExtraStartingPosition);
		Push(kVtbl_ExtraStartingWorldOrCell);
		Push(kVtbl_ExtraTalkingActor);
		Push(kVtbl_ExtraTeleport);
		Push(kVtbl_ExtraTerminalState);
		Push(kVtbl_ExtraTimeLeft);
		Push(kVtbl_ExtraTresPassPackage);
		Push(kVtbl_ExtraUsedMarkers);
		Push(kVtbl_ExtraUses);
		Push(kVtbl_ExtraWaterLightRefs);
		Push(kVtbl_ExtraWaterZoneMap);
		Push(kVtbl_ExtraWeaponAttackSound);
		Push(kVtbl_ExtraWeaponIdleSound);
		Push(kVtbl_ExtraWeaponModFlags);
		Push(kVtbl_ExtraWorn);
		Push(kVtbl_ExtraWornLeft);
		Push(kVtbl_ExtraXTarget);

		Push(kVtbl_FOCollisionListener);
		Push(kVtbl_FOIslandActivationListener);
		Push(kVtbl_FOPipboyManager);
		Push(kVtbl_FORenderedMenu);
		Push(kVtbl_FORenderedMenuRaceSex);
		Push(kVtbl_FORenderedTerminal);
		Push(kVtbl_FindTriangleForLocationFilter);
		Push(kVtbl_FutBinaryFileC);
		Push(kVtbl_GeometryDecalShader);

		Push(kVtbl_GridArray);
		Push(kVtbl_GridCellArray);
		Push(kVtbl_HairShader);
		Push(kVtbl_HavokError);
		Push(kVtbl_HavokFileStreambufReader);
		Push(kVtbl_HavokFileStreambufWriter);
		Push(kVtbl_HavokStreambufFactory);
		Push(kVtbl_HighActorCuller);
		Push(kVtbl_IBSAnimNoteReceiver);
		Push(kVtbl_IConvexOverlapImpl);
		Push(kVtbl_IDebugText);
		Push(kVtbl_IMemoryHeap);
		Push(kVtbl_IMemoryManagerFile);

		Push(kVtbl_IOManager);
		Push(kVtbl_IOTask);
		Push(kVtbl_IdvFileError);
		Push(kVtbl_ImageSpaceEffectParam);
		Push(kVtbl_ImageSpaceModifierInstance);
		Push(kVtbl_ImageSpaceModifierInstanceDOF);
		Push(kVtbl_ImageSpaceModifierInstanceDRB);
		Push(kVtbl_ImageSpaceModifierInstanceForm);
		Push(kVtbl_ImageSpaceModifierInstanceRB);
		Push(kVtbl_ImageSpaceModifierInstanceTemp);
		Push(kVtbl_IntArrayImplementation);
		Push(kVtbl_IntSeenData);
		Push(kVtbl_InterfacedClass);
		Push(kVtbl_InteriorCellNewReferencesMap);
		Push(kVtbl_Lighting30Shader);
		Push(kVtbl_LightingData);
		Push(kVtbl_LipTask);
		Push(kVtbl_ListBoxBase);
		Push(kVtbl_LoadedAreaBound);
		Push(kVtbl_LoadedReferenceMap);
		Push(kVtbl_LoadingMenuThread);
		Push(kVtbl_MagicBallProjectile);
		Push(kVtbl_MagicBoltProjectile);
		Push(kVtbl_MagicCaster);
		Push(kVtbl_MagicFogProjectile);
		Push(kVtbl_MagicProjectile);
		Push(kVtbl_MagicSprayProjectile);
		Push(kVtbl_MagicTarget);
		Push(kVtbl_MediaLocationController);
		Push(kVtbl_MediaSet);
		Push(kVtbl_MemoryHeap);
		Push(kVtbl_MemoryManagerFile);
		Push(kVtbl_MessageHandler);
		Push(kVtbl_MobIterOperator);
		Push(kVtbl_MobileObjectTaskletData);
		Push(kVtbl_Moon);
		Push(kVtbl_MovementTaskData);
		Push(kVtbl_MoviePlayer);

		Push(kVtbl_NavMeshInfoMap);
		Push(kVtbl_NavMeshInfoSearch);
		Push(kVtbl_NavMeshPOVSearch);
		Push(kVtbl_NavMeshSearch);
		Push(kVtbl_NavMeshSearchClosePoint);
		Push(kVtbl_NavMeshSearchFlee);
		Push(kVtbl_NavMeshSearchHide);
		Push(kVtbl_NavMeshSearchLOS);
		Push(kVtbl_NavMeshSearchMaxCost);
		Push(kVtbl_NavMeshSearchSLPoint);

		Push(kVtbl_NiAllocator);
		Push(kVtbl_NiBMPReader);
		Push(kVtbl_NiBinaryStream);
		Push(kVtbl_NiBoundingVolume);
		Push(kVtbl_NiBoxBV);
		Push(kVtbl_NiBoxCapsuleIntersector);
		Push(kVtbl_NiBoxSphereIntersector);
		Push(kVtbl_NiCapsuleBV);
		Push(kVtbl_NiCapsuleCapsuleIntersector);
		Push(kVtbl_NiCapsuleSphereIntersector);
		Push(kVtbl_NiCapsuleTriIntersector);
		Push(kVtbl_NiCullingProcess);
		Push(kVtbl_NiD3DDefaultShader);
		Push(kVtbl_NiD3DGPUProgramCache);
		Push(kVtbl_NiD3DGeometryGroupManager);
		Push(kVtbl_NiD3DHLSLPixelShader);
		Push(kVtbl_NiD3DHLSLVertexShader);
		Push(kVtbl_NiD3DPass);
		Push(kVtbl_NiD3DPixelShader);
		Push(kVtbl_NiD3DRenderState);
		Push(kVtbl_NiD3DShader);
		Push(kVtbl_NiD3DShaderConstantManager);
		Push(kVtbl_NiD3DShaderConstantMap);
		Push(kVtbl_NiD3DShaderDeclaration);
		Push(kVtbl_NiD3DShaderFactory);
		Push(kVtbl_NiD3DShaderInterface);
		Push(kVtbl_NiD3DShaderProgram);
		Push(kVtbl_NiD3DShaderProgramCreator);
		Push(kVtbl_NiD3DShaderProgramCreatorAsm);
		Push(kVtbl_NiD3DShaderProgramCreatorHLSL);
		Push(kVtbl_NiD3DShaderProgramCreatorObj);
		Push(kVtbl_NiD3DShaderProgramFactory);
		Push(kVtbl_NiD3DVertexShader);
		Push(kVtbl_NiDDSReader);
		Push(kVtbl_NiDX92DBufferData);
		Push(kVtbl_NiDX9AdditionalDepthStencilBufferData);
		Push(kVtbl_NiDX9DynamicTextureData);
		Push(kVtbl_NiDX9ErrorShader);
		Push(kVtbl_NiDX9FragmentShader);
		Push(kVtbl_NiDX9ImplicitBufferData);
		Push(kVtbl_NiDX9ImplicitDepthStencilBufferData);
		Push(kVtbl_NiDX9IndexBufferManager);
		Push(kVtbl_NiDX9PersistentSrcTextureRendererData);
		Push(kVtbl_NiDX9RenderState);
		Push(kVtbl_NiDX9RenderedCubeMapData);
		Push(kVtbl_NiDX9RenderedTextureData);
		Push(kVtbl_NiDX9ShaderConstantManager);
		Push(kVtbl_NiDX9ShaderDeclaration);
		Push(kVtbl_NiDX9SourceCubeMapData);
		Push(kVtbl_NiDX9SourceTextureData);
		Push(kVtbl_NiDX9TextureBufferData);
		Push(kVtbl_NiDX9TextureData);
		Push(kVtbl_NiDX9TextureManager);
		Push(kVtbl_NiDX9VertexBufferManager);
		Push(kVtbl_NiDevImageConverter);

		Push(kVtbl_NiDynamicGeometryGroup);
		Push(kVtbl_NiFile);

		Push(kVtbl_NiFormArray);
		Push(kVtbl_NiFragmentShaderInstanceDescriptor);
		Push(kVtbl_NiGPUProgramCache);
		Push(kVtbl_NiGeometryBufferData);
		Push(kVtbl_NiGeometryGroup);
		Push(kVtbl_NiGeometryGroupManager);
		Push(kVtbl_NiGlobalConstantEntry);
		Push(kVtbl_NiHalfSpaceBV);
		Push(kVtbl_NiImageConverter);
		Push(kVtbl_NiImageReader);
		Push(kVtbl_NiIntersector);
		Push(kVtbl_NiLight);
		Push(kVtbl_NiMaterial);
		Push(kVtbl_NiMaterialLibrary);

		Push(kVtbl_NiMaterialResource);
		Push(kVtbl_NiMemStream);
		Push(kVtbl_NiNIFImageReader);
		Push(kVtbl_NiOBBLeaf);
		Push(kVtbl_NiOBBNode);
		Push(kVtbl_NiOBBRoot);
		Push(kVtbl_NiObject);
		Push(kVtbl_NiPSysFieldModifier);
		Push(kVtbl_NiPersistentSrcTextureRendererData);
		Push(kVtbl_NiRangeLODData);
		Push(kVtbl_NiRefObject);
		Push(kVtbl_NiSearchPath);
		Push(kVtbl_NiShaderConstantMap);
		Push(kVtbl_NiShaderConstantMapEntry);
		Push(kVtbl_NiShaderDeclaration);
		Push(kVtbl_NiShaderFactory);
		Push(kVtbl_NiSingleShaderMaterial);
		Push(kVtbl_NiSingleShaderMaterialLibrary);

		Push(kVtbl_NiSphereBV);
		Push(kVtbl_NiSphereTriIntersector);
		Push(kVtbl_NiStandardAllocator);
		Push(kVtbl_NiStaticGeometryGroup);

		Push(kVtbl_NiTGAReader);
		Push(kVtbl_NiUnionBV);
		Push(kVtbl_NiUnsharedGeometryGroup);

		Push(kVtbl_NonActorMagicCaster);
		Push(kVtbl_NonActorMagicTarget);
		Push(kVtbl_Normal);
		Push(kVtbl_NumericIDBufferMap);
		Push(kVtbl_ObstacleData);
		Push(kVtbl_ObstacleTaskData);
		Push(kVtbl_ObstacleUndoData);
		Push(kVtbl_PackageUpdateTaskData);
		Push(kVtbl_ParallaxShader);
		Push(kVtbl_ParticleShader);
		Push(kVtbl_PathSmootherPOVSearch);
		Push(kVtbl_PathingAvoidNodeArray);
		Push(kVtbl_PathingCoverLocation);
		Push(kVtbl_PathingLOSGridMap);
		Push(kVtbl_PathingLOSMap);
		Push(kVtbl_PathingLocation);
		Push(kVtbl_PathingRequest);
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

		Push(kVtbl_PointerArrayImplementation);
		Push(kVtbl_PosGen);
		Push(kVtbl_Precipitation);
		Push(kVtbl_PrecipitationShader);
		Push(kVtbl_ProjectileListener);

		Push(kVtbl_QueuedChildren);
		Push(kVtbl_QueuedFaceGenFile);
		Push(kVtbl_QueuedFile);
		Push(kVtbl_QueuedFileEntry);
		Push(kVtbl_QueuedFileLoad);
		Push(kVtbl_QueuedHead);
		Push(kVtbl_QueuedHelmet);
		Push(kVtbl_QueuedMagicItem);
		Push(kVtbl_QueuedParents);
		Push(kVtbl_QueuedReplacementKF);
		Push(kVtbl_QueuedReplacementKFList);
		Push(kVtbl_QueuedTree);
		Push(kVtbl_QueuedTreeBillboard);
		Push(kVtbl_QueuedTreeModel);

		Push(kVtbl_RadioConvTask);
		Push(kVtbl_Random);
		Push(kVtbl_RealArrayImplementation);
		Push(kVtbl_ReferenceObstacleArray);
		Push(kVtbl_RegSettingCollection);
		Push(kVtbl_RendererSettingCollection);
		Push(kVtbl_Reset3DMobIterator);
		Push(kVtbl_SaveGameFile);
		Push(kVtbl_SeenData);
		Push(kVtbl_SkinShader);
		Push(kVtbl_Sky);
		Push(kVtbl_SkyObject);
		Push(kVtbl_SkyTask);
		Push(kVtbl_SpecificItemCollector);
		Push(kVtbl_SpeedTreeBranchShader);
		Push(kVtbl_SpeedTreeLeafShader);
		Push(kVtbl_Stars);
		Push(kVtbl_StructArrayImplementation);
		Push(kVtbl_Sun);
		Push(kVtbl_SymGen);
		Push(kVtbl_TES);
		Push(kVtbl_TESAIForm);
		Push(kVtbl_TESActorBaseData);
		Push(kVtbl_TESAmbushPackageData);
		Push(kVtbl_TESAnimGroup);
		Push(kVtbl_TESAnimation);
		Push(kVtbl_TESArrowTargetListener);
		Push(kVtbl_TESAttackDamageForm);
		Push(kVtbl_TESAttributes);
		Push(kVtbl_TESBipedModelForm);
		Push(kVtbl_TESChildCell);
		Push(kVtbl_TESContainer);
		Push(kVtbl_TESDescription);
		Push(kVtbl_TESDialoguePackageData);
		Push(kVtbl_TESEatPackageData);
		Push(kVtbl_TESEnchantableForm);
		Push(kVtbl_TESEscortPackageData);
		Push(kVtbl_TESFollowPackageData);
		Push(kVtbl_TESFullName);
		Push(kVtbl_TESHealthForm);
		Push(kVtbl_TESIconTree);
		Push(kVtbl_TESImageSpaceModifiableForm);
		Push(kVtbl_TESLeveledList);
		Push(kVtbl_TESLoadScreenType);
		Push(kVtbl_TESModelAnim);
		Push(kVtbl_TESModelList);
		Push(kVtbl_TESModelPSA);
		Push(kVtbl_TESModelRDT);
		Push(kVtbl_TESModelTextureSwap);
		Push(kVtbl_TESModelTree);
		Push(kVtbl_TESObjectLAND);
		Push(kVtbl_TESPackageData);
		Push(kVtbl_TESPatrolPackageData);
		Push(kVtbl_TESProduceForm);
		Push(kVtbl_TESRaceForm);
		Push(kVtbl_TESReactionForm);
		Push(kVtbl_TESRegionData);
		Push(kVtbl_TESRegionDataGrass);
		Push(kVtbl_TESRegionDataImposter);
		Push(kVtbl_TESRegionDataLandscape);
		Push(kVtbl_TESRegionDataManager);
		Push(kVtbl_TESRegionDataMap);
		Push(kVtbl_TESRegionDataSound);
		Push(kVtbl_TESRegionDataWeather);
		Push(kVtbl_TESRegionGrassObject);
		Push(kVtbl_TESRegionGrassObjectList);
		Push(kVtbl_TESRegionList);
		Push(kVtbl_TESRegionObjectBase);
		Push(kVtbl_TESScriptableForm);
		Push(kVtbl_TESSoundFile);
		Push(kVtbl_TESSpellList);
		Push(kVtbl_TESTexture1024);
		Push(kVtbl_TESTrapListener);
		Push(kVtbl_TESUseItemPackageData);
		Push(kVtbl_TESUseWeaponPackageData);
		Push(kVtbl_TESValueForm);
		Push(kVtbl_TESWaterListener);
		Push(kVtbl_TESWeightForm);
		Push(kVtbl_TESWindListener);
		Push(kVtbl_TallGrassShader);
		Push(kVtbl_TargetEntry);
		Push(kVtbl_TeleportDoorSearch);
		Push(kVtbl_TileExtra);
		Push(kVtbl_TileShader);
		Push(kVtbl_TopicInfoArray);
		Push(kVtbl_TrapEntry);
		Push(kVtbl_TrapTargetEntry);
		Push(kVtbl_Uniform);
		Push(kVtbl_UpdateMasterPsysTaskletData);
		Push(kVtbl_UseWeaponActorPackageData);
		Push(kVtbl_VecArrayImplementation);
		Push(kVtbl_VirtualActorPathHandler);
		Push(kVtbl_VolumetricFogShader);
		Push(kVtbl_WaterTargetEntry);
		Push(kVtbl_WaterZone);
		Push(kVtbl_ZeroOverheadHeap);
		Push(kVtbl_ZoneEntry);

		Push(kVtbl_ahkpBreakableConstraintData);
		Push(kVtbl_ahkpCharacterProxy);
		Push(kVtbl_ahkpMalleableConstraintData);
		Push(kVtbl_ahkpWorld);

		Push(kVtbl_bhkAcousticSpacePhantomCallbackShape);
		Push(kVtbl_bhkArrowTargetListener);
		Push(kVtbl_bhkBallSocketConstraintChain);
		Push(kVtbl_bhkCharacterListener);
		Push(kVtbl_bhkCharacterPointCollector);
		Push(kVtbl_bhkCharacterStateClimbing);
		Push(kVtbl_bhkCharacterStateFlying);
		Push(kVtbl_bhkCharacterStateInAir);
		Push(kVtbl_bhkCharacterStateJumping);
		Push(kVtbl_bhkCharacterStateOnGround);
		Push(kVtbl_bhkCharacterStateProjectile);
		Push(kVtbl_bhkCharacterStateSwimming);
		Push(kVtbl_bhkCollisionBox);
		Push(kVtbl_bhkCollisionFilter);
		Push(kVtbl_bhkConstraintChain);
		Push(kVtbl_bhkContactListener);
		Push(kVtbl_bhkConvexListShape);
		Push(kVtbl_bhkEntityListener);
		Push(kVtbl_bhkExtendedMeshShape);
		Push(kVtbl_bhkExtendedMeshShapeData);
		Push(kVtbl_bhkFilteredCdBodyCollector);
		Push(kVtbl_bhkHingeLimitsConstraint);
		Push(kVtbl_bhkLiquidAction);
		Push(kVtbl_bhkMemoryAllocator);
		Push(kVtbl_bhkObstacleDeactivationListener);
		Push(kVtbl_bhkObstacleRemovalListener);
		Push(kVtbl_bhkOnExitCallback);
		Push(kVtbl_bhkOnStartupCallback);
		Push(kVtbl_bhkOrientHingedBodyAction);
		Push(kVtbl_bhkPoseArray);
		Push(kVtbl_bhkPositionConstraintMotor);
		Push(kVtbl_bhkRagdollController);
		Push(kVtbl_bhkRagdollLimitsConstraint);
		Push(kVtbl_bhkRagdollPenetrationUtil);
		Push(kVtbl_bhkRagdollShareData);
		Push(kVtbl_bhkRagdollTemplate);
		Push(kVtbl_bhkRagdollTemplateData);
		Push(kVtbl_bhkRefObject);
		Push(kVtbl_bhkSpringDamperConstraintMotor);
		Push(kVtbl_bhkTelekinesisListener);
		Push(kVtbl_bhkTrapListener);
		Push(kVtbl_bhkVelocityConstraintMotor);
		Push(kVtbl_bhkWaterListener);
		Push(kVtbl_bhkWaterPhantomCallbackShape);
		Push(kVtbl_bhkWindListener);
		Push(kVtbl_bhkWorldCinfo);

		Push(kVtbl_hkAlignSceneToNodeOptions);
		Push(kVtbl_hkArrayStreamWriter);
		Push(kVtbl_hkBSHeightFieldShape);
		Push(kVtbl_hkBallAndSocketConstraintCinfo);
		Push(kVtbl_hkBallSocketChainCinfo);
		Push(kVtbl_hkBaseObject);
		Push(kVtbl_hkBinaryPackfileWriter);
		Push(kVtbl_hkBinaryTagfileReader);
		Push(kVtbl_hkBinaryTagfileWriter);
		Push(kVtbl_hkBreakableConstraintCinfo);
		Push(kVtbl_hkBsdSocket);
		Push(kVtbl_hkBufferedStreamReader);
		Push(kVtbl_hkBufferedStreamWriter);
		Push(kVtbl_hkCharControllerShape);
		Push(kVtbl_hkCommandRouter);
		Push(kVtbl_hkConstraintCinfo);
		Push(kVtbl_hkContainerResourceMap);
		Push(kVtbl_hkCpuJobThreadPool);
		Push(kVtbl_hkCpuWorldGetClosestPointsCollector);
		Push(kVtbl_hkCpuWorldLinearCastCollector);
		Push(kVtbl_hkCpuWorldRayCastCollector);
		Push(kVtbl_hkCrc32StreamWriter);
		Push(kVtbl_hkDataArrayNative);
		Push(kVtbl_hkDataClassDict);
		Push(kVtbl_hkDataClassNative);
		Push(kVtbl_hkDataObjectDict);
		Push(kVtbl_hkDataObjectNative);
		Push(kVtbl_hkDataRefCounted);
		Push(kVtbl_hkDataWorldDict);
		Push(kVtbl_hkDataWorldNative);
		Push(kVtbl_hkDebugDisplay);
		Push(kVtbl_hkDebugDisplayHandler);
		Push(kVtbl_hkDebugDisplayProcess);
		Push(kVtbl_hkDefaultBuiltinTypeRegistry);
		Push(kVtbl_hkDefaultClassNameRegistry);
		Push(kVtbl_hkDefaultError);
		Push(kVtbl_hkDisplayAABB);
		Push(kVtbl_hkDisplayBox);
		Push(kVtbl_hkDisplayCapsule);
		Push(kVtbl_hkDisplayCone);
		Push(kVtbl_hkDisplayConvex);
		Push(kVtbl_hkDisplayCylinder);
		Push(kVtbl_hkDisplayGeometry);
		Push(kVtbl_hkDisplayPlane);
		Push(kVtbl_hkDisplaySemiCircle);
		Push(kVtbl_hkDisplaySerializeIStream);
		Push(kVtbl_hkDisplaySerializeOStream);
		Push(kVtbl_hkDisplaySphere);
		Push(kVtbl_hkDummySingleton);
		Push(kVtbl_hkDynamicClassNameRegistry);
		Push(kVtbl_hkErrStream);
		Push(kVtbl_hkError);
		Push(kVtbl_hkFileSystem);
		Push(kVtbl_hkFixedConstraintCinfo);
		Push(kVtbl_hkFreeListAllocator);
		Push(kVtbl_hkFreeListAllocatorServer);
		Push(kVtbl_hkFreeListMemorySystem);
		Push(kVtbl_hkGenericConstraintCinfo);
		Push(kVtbl_hkGskShape);
		Push(kVtbl_hkHeightFieldRayForwardingCollector);
		Push(kVtbl_hkHingeConstraintCinfo);
		Push(kVtbl_hkHingeLimitsConstraintCinfo);
		Push(kVtbl_hkIArchive);
		Push(kVtbl_hkInspectProcess);
		Push(kVtbl_hkIstream);
		Push(kVtbl_hkKdTreeBundleCastCollector);
		Push(kVtbl_hkKdTreeCastCollector);
		Push(kVtbl_hkKdTreeCollidableRayBundleCaster);
		Push(kVtbl_hkKdTreeCpuBroadphaseBundleCastCollector);
		Push(kVtbl_hkLargeBlockAllocator);
		Push(kVtbl_hkLeakDetectAllocator);
		Push(kVtbl_hkLimitedHingeConstraintCinfo);
		Push(kVtbl_hkLiquidAction);
		Push(kVtbl_hkLocalFrameGroup);
		Push(kVtbl_hkMalleableConstraintCinfo);
		Push(kVtbl_hkMemoryAllocator);
		Push(kVtbl_hkMemoryBlockServer);
		Push(kVtbl_hkMemoryMeshShape);
		Push(kVtbl_hkMemoryResourceContainer);
		Push(kVtbl_hkMemoryResourceHandle);
		Push(kVtbl_hkMemoryStreamReader);
		Push(kVtbl_hkMemorySystem);
		Push(kVtbl_hkMemoryTrackStreamReader);
		Push(kVtbl_hkMemoryTrackStreamWriter);
		Push(kVtbl_hkMonitorStreamColorTable);
		Push(kVtbl_hkMoppBvTreeShapeBase);
		Push(kVtbl_hkNativeFileSystem);
		Push(kVtbl_hkNiTriStripsShape);
		Push(kVtbl_hkNormalTriangleShape);
		Push(kVtbl_hkOArchive);
		Push(kVtbl_hkObjectCopier);
		Push(kVtbl_hkObjectResource);
		Push(kVtbl_hkOffsetOnlyStreamWriter);
		Push(kVtbl_hkOrientHingedBodyAction);
		Push(kVtbl_hkOstream);
		Push(kVtbl_hkPackedNiTriStripsShape);
		Push(kVtbl_hkPackfileData);
		Push(kVtbl_hkPackfileWriter);
		Push(kVtbl_hkPlatformObjectWriter);
		Push(kVtbl_hkPointToPathConstraintCinfo);
		Push(kVtbl_hkPrismaticConstraintCinfo);
		Push(kVtbl_hkProcess);
		Push(kVtbl_hkProcessContext);
		Push(kVtbl_hkProcessFactory);
		Push(kVtbl_hkProcessHandler);
		Push(kVtbl_hkRagdollConstraintCinfo);
		Push(kVtbl_hkRagdollLimitsConstraintCinfo);
		Push(kVtbl_hkReferencedObject);
		Push(kVtbl_hkReferencedObjectLock);
		Push(kVtbl_hkResourceMap);
		Push(kVtbl_hkScaledMoppBvTreeShape);
		Push(kVtbl_hkSerializeDeprecated);
		Push(kVtbl_hkServerDebugDisplayHandler);
		Push(kVtbl_hkServerProcessHandler);
		Push(kVtbl_hkSimpleLocalFrame);
		Push(kVtbl_hkSocket);
		Push(kVtbl_hkSolverAllocator);
		Push(kVtbl_hkStackTracer);
		Push(kVtbl_hkStaticClassNameRegistry);
		Push(kVtbl_hkStatisticsCollector);
		Push(kVtbl_hkStatisticsProcess);
		Push(kVtbl_hkStdioStreamReader);
		Push(kVtbl_hkStdioStreamWriter);
		Push(kVtbl_hkStiffSpringConstraintCinfo);
		Push(kVtbl_hkStreamReader);
		Push(kVtbl_hkStreamStatisticsCollector);
		Push(kVtbl_hkStreamWriter);
		Push(kVtbl_hkSubStreamWriter);
		Push(kVtbl_hkSystemMemoryBlockServer);
		Push(kVtbl_hkThreadMemory);
		Push(kVtbl_hkTraceStream);
		Push(kVtbl_hkTypeInfoRegistry);
		Push(kVtbl_hkVersionPatchManager);
		Push(kVtbl_hkVisualDebugger);
		Push(kVtbl_hkVtableClassRegistry);
		Push(kVtbl_hkWheelConstraintCinfo);
		Push(kVtbl_hkWindowsSystemClock);

		Push(kVtbl_hkaAnimation);
		Push(kVtbl_hkaAnimationBinding);
		Push(kVtbl_hkaAnimationContainer);
		Push(kVtbl_hkaAnimationPreviewColorContainer);
		Push(kVtbl_hkaBoneAttachment);
		Push(kVtbl_hkaDefaultAnimatedReferenceFrame);
		Push(kVtbl_hkaDeltaCompressedAnimation);
		Push(kVtbl_hkaDetectRagdollPenetration);
		Push(kVtbl_hkaFootPlacementIkSolver);
		Push(kVtbl_hkaFootstepAnalysisInfo);
		Push(kVtbl_hkaFootstepAnalysisInfoContainer);
		Push(kVtbl_hkaInterleavedUncompressedAnimation);
		Push(kVtbl_hkaMeshBinding);
		Push(kVtbl_hkaRagdollInstance);
		Push(kVtbl_hkaRagdollRaycastInterface);
		Push(kVtbl_hkaRaycastInterface);
		Push(kVtbl_hkaSkeleton);
		Push(kVtbl_hkaSkeletonMapper);
		Push(kVtbl_hkaSplineCompressedAnimation);
		Push(kVtbl_hkaWaveletCompressedAnimation);

		Push(kVtbl_hkgpConvexHull);
		Push(kVtbl_hkgpMesh);

		Push(kVtbl_hkp3AxisSweep);
		Push(kVtbl_hkpAabbPhantom);
		Push(kVtbl_hkpAabbTreeCollidableRaycaster);
		Push(kVtbl_hkpAabbTreeWorldManager);
		Push(kVtbl_hkpAction);
		Push(kVtbl_hkpActionListener);
		Push(kVtbl_hkpActiveContactPointViewer);
		Push(kVtbl_hkpAllCdBodyPairCollector);
		Push(kVtbl_hkpAllCdPointCollector);
		Push(kVtbl_hkpAllRayHitCollector);
		Push(kVtbl_hkpAngularDashpotAction);
		Push(kVtbl_hkpBallAndSocketConstraintData);
		Push(kVtbl_hkpBallGun);
		Push(kVtbl_hkpBallSocketChainData);
		Push(kVtbl_hkpBinaryAction);
		Push(kVtbl_hkpBoxBoxAgent);
		Push(kVtbl_hkpBoxMotion);
		Push(kVtbl_hkpBoxShape);
		Push(kVtbl_hkpBreakableConstraintData);
		Push(kVtbl_hkpBroadPhase);
		Push(kVtbl_hkpBroadPhaseBorder);
		Push(kVtbl_hkpBroadPhaseBorderListener);
		Push(kVtbl_hkpBroadPhaseCastCollector);
		Push(kVtbl_hkpBroadPhaseListener);
		Push(kVtbl_hkpBroadphaseViewer);
		Push(kVtbl_hkpBvAgent);
		Push(kVtbl_hkpBvShape);
		Push(kVtbl_hkpBvTreeAgent);
		Push(kVtbl_hkpBvTreeShape);
		Push(kVtbl_hkpBvTreeStreamAgent);
		Push(kVtbl_hkpCachingShapePhantom);
		Push(kVtbl_hkpCallbackConstraintMotor);
		Push(kVtbl_hkpCapsuleCapsuleAgent);
		Push(kVtbl_hkpCapsuleShape);
		Push(kVtbl_hkpCapsuleTriangleAgent);
		Push(kVtbl_hkpCdBodyPairCollector);
		Push(kVtbl_hkpCdPointCollector);
		Push(kVtbl_hkpCharacterContext);
		Push(kVtbl_hkpCharacterMotion);
		Push(kVtbl_hkpCharacterProxy);
		Push(kVtbl_hkpCharacterProxyListener);
		Push(kVtbl_hkpCharacterStateManager);
		Push(kVtbl_hkpClosestCdPointCollector);
		Push(kVtbl_hkpClosestRayHitCollector);
		Push(kVtbl_hkpCollidableCollidableFilter);
		Push(kVtbl_hkpCollidableMediator);
		Push(kVtbl_hkpCollisionCallbackUtil);
		Push(kVtbl_hkpCollisionDispatcher);
		Push(kVtbl_hkpCollisionFilter);
		Push(kVtbl_hkpCollisionFilterList);
		Push(kVtbl_hkpCompressedMeshShape);
		Push(kVtbl_hkpCompressedSampledHeightFieldShape);
		Push(kVtbl_hkpConstrainedSystemFilter);
		Push(kVtbl_hkpConstraintChainInstance);
		Push(kVtbl_hkpConstraintChainInstanceAction);
		Push(kVtbl_hkpConstraintCollisionFilter);
		Push(kVtbl_hkpConstraintInstance);
		Push(kVtbl_hkpConstraintListener);
		Push(kVtbl_hkpConstraintMotor);
		Push(kVtbl_hkpConstraintOwner);
		Push(kVtbl_hkpConstraintViewer);
		Push(kVtbl_hkpContactListener);
		Push(kVtbl_hkpContactPointViewer);
		Push(kVtbl_hkpContinuousSimulation);
		Push(kVtbl_hkpConvexListAgent);
		Push(kVtbl_hkpConvexListShape);
		Push(kVtbl_hkpConvexPieceMeshShape);
		Push(kVtbl_hkpConvexPieceShape);
		Push(kVtbl_hkpConvexPieceStreamData);
		Push(kVtbl_hkpConvexRadiusBuilder);
		Push(kVtbl_hkpConvexRadiusViewer);
		Push(kVtbl_hkpConvexTransformShape);
		Push(kVtbl_hkpConvexTransformShapeBase);
		Push(kVtbl_hkpConvexTranslateShape);
		Push(kVtbl_hkpConvexVerticesConnectivity);
		Push(kVtbl_hkpConvexVerticesShape);
		Push(kVtbl_hkpCylinderShape);
		Push(kVtbl_hkpDashpotAction);
		Push(kVtbl_hkpDefaultConvexListFilter);
		Push(kVtbl_hkpDefaultToiResourceMgr);
		Push(kVtbl_hkpDefaultWorldMaintenanceMgr);
		Push(kVtbl_hkpDefaultWorldMemoryWatchDog);
		Push(kVtbl_hkpDeferredConstraintOwner);
		Push(kVtbl_hkpDisableEntityCollisionFilter);
		Push(kVtbl_hkpDisplayBindingData);
		Push(kVtbl_hkpEndOfStepCallbackUtil);
		Push(kVtbl_hkpEntity);
		Push(kVtbl_hkpEntityActivationListener);
		Push(kVtbl_hkpEntityEntityBroadPhaseListener);
		Push(kVtbl_hkpEntityListener);
		Push(kVtbl_hkpExtendedMeshShape);
		Push(kVtbl_hkpFastMeshShape);
		Push(kVtbl_hkpFirstPersonGun);
		Push(kVtbl_hkpFixedBufferCdPointCollector);
		Push(kVtbl_hkpFixedBufferRayHitCollector);
		Push(kVtbl_hkpFixedRigidMotion);
		Push(kVtbl_hkpFlagCdBodyPairCollector);
		Push(kVtbl_hkpGenericConstraintData);
		Push(kVtbl_hkpGravityGun);
		Push(kVtbl_hkpGroupCollisionFilter);
		Push(kVtbl_hkpGroupFilter);
		Push(kVtbl_hkpGskBaseAgent);
		Push(kVtbl_hkpGskfAgent);
		Push(kVtbl_hkpGunProjectile);
		Push(kVtbl_hkpHeightFieldAgent);
		Push(kVtbl_hkpHingeConstraintData);
		Push(kVtbl_hkpHingeLimitsData);
		Push(kVtbl_hkpInactiveContactPointViewer);
		Push(kVtbl_hkpIslandActivationListener);
		Push(kVtbl_hkpKdTreeCollidableRaycaster);
		Push(kVtbl_hkpKdTreeWorldLinearCaster);
		Push(kVtbl_hkpKdTreeWorldManager);
		Push(kVtbl_hkpKeyframedRigidMotion);
		Push(kVtbl_hkpLimitedForceConstraintMotor);
		Push(kVtbl_hkpLimitedHingeConstraintData);
		Push(kVtbl_hkpLinearParametricCurve);
		Push(kVtbl_hkpListAgent);
		Push(kVtbl_hkpListShape);
		Push(kVtbl_hkpMalleableConstraintData);
		Push(kVtbl_hkpMapPointsToSubShapeContactMgr);
		Push(kVtbl_hkpMaxSizeMotion);
		Push(kVtbl_hkpMeshShape);
		Push(kVtbl_hkpMidphaseViewer);
		Push(kVtbl_hkpMoppAgent);
		Push(kVtbl_hkpMoppBvTreeShape);
		Push(kVtbl_hkpMoppCachedShapeMediator);
		Push(kVtbl_hkpMoppCode);
		Push(kVtbl_hkpMoppCodeGenerator);
		Push(kVtbl_hkpMoppCostFunction);
		Push(kVtbl_hkpMoppDefaultAssembler);
		Push(kVtbl_hkpMoppDefaultSplitter);
		Push(kVtbl_hkpMoppModifier);
		Push(kVtbl_hkpMoppShapeMediator);
		Push(kVtbl_hkpMotion);
		Push(kVtbl_hkpMotorAction);
		Push(kVtbl_hkpMountedBallGun);
		Push(kVtbl_hkpMousePickingViewer);
		Push(kVtbl_hkpMouseSpringAction);
		Push(kVtbl_hkpMultiRayConvexAgent);
		Push(kVtbl_hkpMultiRayShape);
		Push(kVtbl_hkpMultiSphereAgent);
		Push(kVtbl_hkpMultiSphereShape);
		Push(kVtbl_hkpMultiSphereTriangleAgent);
		Push(kVtbl_hkpMultiThreadedSimulation);
		Push(kVtbl_hkpNullAgent);
		Push(kVtbl_hkpNullBroadPhaseListener);
		Push(kVtbl_hkpNullCollisionFilter);
		Push(kVtbl_hkpNullContactMgr);
		Push(kVtbl_hkpNullContactMgrFactory);
		Push(kVtbl_hkpPairCollisionFilter);
		Push(kVtbl_hkpPhantom);
		Push(kVtbl_hkpPhantomAgent);
		Push(kVtbl_hkpPhantomBroadPhaseListener);
		Push(kVtbl_hkpPhantomCallbackShape);
		Push(kVtbl_hkpPhantomDisplayViewer);
		Push(kVtbl_hkpPhantomListener);
		Push(kVtbl_hkpPhantomOverlapListener);
		Push(kVtbl_hkpPhysicsContext);
		Push(kVtbl_hkpPhysicsContextWorldListener);
		Push(kVtbl_hkpPhysicsData);
		Push(kVtbl_hkpPhysicsSystem);
		Push(kVtbl_hkpPhysicsSystemWithContacts);
		Push(kVtbl_hkpPlaneShape);
		Push(kVtbl_hkpPointToPathConstraintData);
		Push(kVtbl_hkpPointToPlaneConstraintData);
		Push(kVtbl_hkpPositionConstraintMotor);
		Push(kVtbl_hkpPoweredChainData);
		Push(kVtbl_hkpPoweredChainMapper);
		Push(kVtbl_hkpPredGskfAgent);
		Push(kVtbl_hkpPrismaticConstraintData);
		Push(kVtbl_hkpProjectileGun);
		Push(kVtbl_hkpPulleyConstraintData);
		Push(kVtbl_hkpRagdollConstraintData);
		Push(kVtbl_hkpRagdollLimitsData);
		Push(kVtbl_hkpRayCollidableFilter);
		Push(kVtbl_hkpRayHitCollector);
		Push(kVtbl_hkpRayShapeCollectionFilter);
		Push(kVtbl_hkpRejectChassisListener);
		Push(kVtbl_hkpRemoveTerminalsMoppModifier);
		Push(kVtbl_hkpRemoveTerminalsMoppModifier2);
		Push(kVtbl_hkpReorientAction);
		Push(kVtbl_hkpReportContactMgr);
		Push(kVtbl_hkpRigidBody);
		Push(kVtbl_hkpRigidBodyCentreOfMassViewer);
		Push(kVtbl_hkpRigidBodyInertiaViewer);
		Push(kVtbl_hkpRotationalConstraintData);
		Push(kVtbl_hkpSampledHeightFieldShape);
		Push(kVtbl_hkpSerializedAgentNnEntry);
		Push(kVtbl_hkpSerializedDisplayMarker);
		Push(kVtbl_hkpSerializedDisplayMarkerList);
		Push(kVtbl_hkpSerializedDisplayRbTransforms);
		Push(kVtbl_hkpShape);
		Push(kVtbl_hkpShapeCollection);
		Push(kVtbl_hkpShapeCollectionAgent);
		Push(kVtbl_hkpShapeCollectionFilter);
		Push(kVtbl_hkpShapeContainer);
		Push(kVtbl_hkpShapeDisplayBuilder);
		Push(kVtbl_hkpShapeDisplayViewer);
		Push(kVtbl_hkpShapeInfo);
		Push(kVtbl_hkpShapePhantom);
		Push(kVtbl_hkpSimpleClosestContactCollector);
		Push(kVtbl_hkpSimpleConstraintContactMgr);
		Push(kVtbl_hkpSimpleContactConstraintData);
		Push(kVtbl_hkpSimpleMeshShape);
		Push(kVtbl_hkpSimpleShapePhantom);
		Push(kVtbl_hkpSimpleWorldRayCaster);
		Push(kVtbl_hkpSimulation);
		Push(kVtbl_hkpSimulationIsland);
		Push(kVtbl_hkpSimulationIslandViewer);
		Push(kVtbl_hkpSingleShapeContainer);
		Push(kVtbl_hkpSphereBoxAgent);
		Push(kVtbl_hkpSphereCapsuleAgent);
		Push(kVtbl_hkpSphereMotion);
		Push(kVtbl_hkpSphereShape);
		Push(kVtbl_hkpSphereSphereAgent);
		Push(kVtbl_hkpSphereTriangleAgent);
		Push(kVtbl_hkpSpringAction);
		Push(kVtbl_hkpSpringDamperConstraintMotor);
		Push(kVtbl_hkpStiffSpringChainData);
		Push(kVtbl_hkpStiffSpringConstraintData);
		Push(kVtbl_hkpStorageExtendedMeshShape);
		Push(kVtbl_hkpStorageMeshShape);
		Push(kVtbl_hkpStorageSampledHeightFieldShape);
		Push(kVtbl_hkpSweptTransformDisplayViewer);
		Push(kVtbl_hkpSymmetricAgentFlipBodyCollector);
		Push(kVtbl_hkpSymmetricAgentFlipCastCollector);
		Push(kVtbl_hkpSymmetricAgentFlipCollector);
		Push(kVtbl_hkpThinBoxMotion);
		Push(kVtbl_hkpToiContactPointViewer);
		Push(kVtbl_hkpToiCountViewer);
		Push(kVtbl_hkpTransformAgent);
		Push(kVtbl_hkpTransformShape);
		Push(kVtbl_hkpTriSampledHeightFieldBvTreeShape);
		Push(kVtbl_hkpTriSampledHeightFieldCollection);
		Push(kVtbl_hkpTriangleShape);
		Push(kVtbl_hkpTyremarksInfo);
		Push(kVtbl_hkpTyremarksWheel);
		Push(kVtbl_hkpUnaryAction);
		Push(kVtbl_hkpUserConvexRadiusBuilder);
		Push(kVtbl_hkpUserShapeDisplayBuilder);
		Push(kVtbl_hkpUserShapePhantomTypeIdentifier);
		Push(kVtbl_hkpVehicleData);
		Push(kVtbl_hkpVehicleDefaultAerodynamics);
		Push(kVtbl_hkpVehicleDefaultAnalogDriverInput);
		Push(kVtbl_hkpVehicleDefaultBrake);
		Push(kVtbl_hkpVehicleDefaultEngine);
		Push(kVtbl_hkpVehicleDefaultSteering);
		Push(kVtbl_hkpVehicleDefaultSuspension);
		Push(kVtbl_hkpVehicleDefaultTransmission);
		Push(kVtbl_hkpVehicleDefaultVelocityDamper);
		Push(kVtbl_hkpVehicleDriverInputAnalogStatus);
		Push(kVtbl_hkpVehicleInstance);
		Push(kVtbl_hkpVehicleLinearCastBatchingManager);
		Push(kVtbl_hkpVehicleLinearCastWheelCollide);
		Push(kVtbl_hkpVehicleManager);
		Push(kVtbl_hkpVehicleRayCastBatchingManager);
		Push(kVtbl_hkpVehicleRayCastWheelCollide);
		Push(kVtbl_hkpVehicleViewer);
		Push(kVtbl_hkpVehicleWheelCollide);
		Push(kVtbl_hkpVelocityConstraintMotor);
		Push(kVtbl_hkpWeldingViewer);
		Push(kVtbl_hkpWheelConstraintData);
		Push(kVtbl_hkpWorld);
		Push(kVtbl_hkpWorldCinfo);
		Push(kVtbl_hkpWorldDeletionListener);
		Push(kVtbl_hkpWorldLinearCaster);
		Push(kVtbl_hkpWorldMemoryViewer);
		Push(kVtbl_hkpWorldObject);
		Push(kVtbl_hkpWorldPostSimulationListener);
		Push(kVtbl_hkpWorldRayCaster);
		Push(kVtbl_hkpWorldSnapshotViewer);
		Push(kVtbl_hkpWorldViewerBase);

		Push(kVtbl_hkxAnimatedFloat);
		Push(kVtbl_hkxAnimatedMatrix);
		Push(kVtbl_hkxAnimatedQuaternion);
		Push(kVtbl_hkxAnimatedVector);
		Push(kVtbl_hkxAttributeHolder);
		Push(kVtbl_hkxCamera);
		Push(kVtbl_hkxEdgeSelectionChannel);
		Push(kVtbl_hkxEnum);
		Push(kVtbl_hkxEnvironment);
		Push(kVtbl_hkxIndexBuffer);
		Push(kVtbl_hkxLight);
		Push(kVtbl_hkxMaterial);
		Push(kVtbl_hkxMaterialEffect);
		Push(kVtbl_hkxMaterialShader);
		Push(kVtbl_hkxMaterialShaderSet);
		Push(kVtbl_hkxMesh);
		Push(kVtbl_hkxMeshSection);
		Push(kVtbl_hkxNode);
		Push(kVtbl_hkxNodeSelectionSet);
		Push(kVtbl_hkxScene);
		Push(kVtbl_hkxSkinBinding);
		Push(kVtbl_hkxSparselyAnimatedBool);
		Push(kVtbl_hkxSparselyAnimatedEnum);
		Push(kVtbl_hkxSparselyAnimatedInt);
		Push(kVtbl_hkxSparselyAnimatedString);
		Push(kVtbl_hkxTextureFile);
		Push(kVtbl_hkxTextureInplace);
		Push(kVtbl_hkxTriangleSelectionChannel);
		Push(kVtbl_hkxVertexBuffer);
		Push(kVtbl_hkxVertexFloatDataChannel);
		Push(kVtbl_hkxVertexIntDataChannel);
		Push(kVtbl_hkxVertexSelectionChannel);
		Push(kVtbl_hkxVertexVectorDataChannel);

	};

	void FillFOSELabels()
	{
		
	}

	void FillOBSELabels()
	{
		
	}


	void GetStringForLabelInternal(void* ptr, UInt32 vtbl, std::string& buffer)
	{
		for (const auto& iter : setOfLabels)
			if (iter->Satisfies(ptr)) { iter->Get(ptr, buffer); return; }

		if (vtbl > 0xFE0000 && vtbl < 0x1200000)
			buffer = std::format("-\\_(Ö)_/-: 0x{:08X}", vtbl);

//		if (std::string one = PDBHandling::GetSymbolForAddress((UInt32)ptr, GetCurrentProcess()); !one.empty()) return "Function:" + one;
	}

	void GetStringForLabelSEH(void* ptr, UInt32 vtbl, std::string& buffer)
	{
		__try
		{
			GetStringForLabelInternal(ptr, vtbl, buffer);
		}
		__except (1)
		{
			buffer += ", failed to format";
		}
	}

	std::string GetStringForLabel(void* ptr, UInt32 vtbl)
	{
		std::string buffer;
		GetStringForLabelInternal(ptr, vtbl, buffer);
		return buffer;
	}

	std::string Get(UInt32 ptr, UInt32 depth = 5)
	{
		if (!ptr) return "";

		UInt32 vtbl;

		std::string full;

		while (vtbl = Dereference<UInt32>(ptr) ? *(UInt32*)ptr : 0)
		{
			if (std::string onetime = GetStringForLabel((void*)ptr, vtbl); !onetime.empty())
				return full + onetime;

			if (depth == 0) break;

			depth--;

			full += std::format("Pointer: 0x{:08X}; ", vtbl);

			ptr = vtbl;
		}
		// 70E943 
		// 0D2E7379
		// 0D2E7369
		return "";
	}
}

namespace CrashLogger::Calltrace
{

	void Get(EXCEPTION_POINTERS* info) 
	{
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		Log() << std::format("Exception {:08X} caught!\n", info->ExceptionRecord->ExceptionCode);
		Log() << "Calltrace:";

		DWORD machine = IMAGE_FILE_MACHINE_I386;
		CONTEXT context = {};
		memcpy(&context, info->ContextRecord, sizeof(CONTEXT));

		Safe_SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
		if (!Safe_SymInitialize(process, NULL, true))
			Log() << "Error initializing symbol store";

		//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;
		DWORD eip = 0;

		while (Safe_StackWalk(machine, process, thread, &frame, &context, NULL, Safe_SymFunctionTableAccess, Safe_SymGetModuleBase, NULL)) {
			/*
		Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
		When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
			*/
			if (frame.AddrPC.Offset == eip) break;
			eip = frame.AddrPC.Offset;
			Log() << PDBHandling::GetCalltraceFunction(frame.AddrPC.Offset, frame.AddrFrame.Offset, process);
		}

		Log();
	}

}

namespace CrashLogger::Registry
{
	void Get(EXCEPTION_POINTERS* info)
	{
		Log() << "Registry:" << std::endl
			<< ("REG |    VALUE   | DEREFERENCE INFO") << std::endl
			<< std::format("eax | 0x{:08X} | {}", info->ContextRecord->Eax, VirtualTables::Get(info->ContextRecord->Eax)) << std::endl
			<< std::format("ebx | 0x{:08X} | {}", info->ContextRecord->Ebx, VirtualTables::Get(info->ContextRecord->Ebx)) << std::endl
			<< std::format("ecx | 0x{:08X} | {}", info->ContextRecord->Ecx, VirtualTables::Get(info->ContextRecord->Ecx)) << std::endl
			<< std::format("edx | 0x{:08X} | {}", info->ContextRecord->Edx, VirtualTables::Get(info->ContextRecord->Edx)) << std::endl
			<< std::format("edi | 0x{:08X} | {}", info->ContextRecord->Edi, VirtualTables::Get(info->ContextRecord->Edi)) << std::endl
			<< std::format("esi | 0x{:08X} | {}", info->ContextRecord->Esi, VirtualTables::Get(info->ContextRecord->Esi)) << std::endl
			<< std::format("ebp | 0x{:08X} | {}", info->ContextRecord->Ebp, VirtualTables::Get(info->ContextRecord->Ebp)) << std::endl
			<< std::format("esp | 0x{:08X} | {}", info->ContextRecord->Esp, VirtualTables::Get(info->ContextRecord->Esp)) << std::endl
			<< std::format("eip | 0x{:08X} | {}", info->ContextRecord->Eip, VirtualTables::Get(info->ContextRecord->Eip)) << std::endl;
	}
}

namespace CrashLogger::Stack
{
	UInt32 GetESPi(UInt32* esp, UInt32 i)
	{
		__try
		{
			return esp[i];
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return 0;
		}
	}

	void Get(EXCEPTION_POINTERS* info)
	{
		Log() << "Stack:" << std::endl << " # |    VALUE   | DEREFERENCE INFO";

		const auto esp = reinterpret_cast<UInt32*>(info->ContextRecord->Esp);

		for (unsigned int i = 0; i < 0x100; i++) {
			const auto espi = GetESPi(esp, i);
			if (i <= 8)
				Log() << std::format("{:2X} | 0x{:08X} | {}", i, espi, VirtualTables::Get(espi));
			else if (std::string result = VirtualTables::Get(espi); !result.empty())
				Log() << std::format("{:2X} | 0x{:08X} | {}", i, espi, result);
		}
		Log();
	}
}

namespace CrashLogger::ModuleBases
{
	struct UserContext {
		UInt32 eip;
		UInt32 moduleBase;
		CHAR* name;
	};

	struct Module
	{
		UInt32 moduleBase;
		UInt32 moduleEnd;
		std::filesystem::path path;

		bool operator<(const Module& other) const {
			if (path < other.path) return true;
			return false;
		};
	};

	std::set<Module> enumeratedModules;

	BOOL CALLBACK EumerateModulesCallback(PCSTR name, ULONG moduleBase, ULONG moduleSize, PVOID context) {
		UserContext* info = (UserContext*)context;
		if (info->eip >= (UInt32)moduleBase && info->eip <= (UInt32)moduleBase + (UInt32)moduleSize) {
			//Log() << FormatString("%0X %0X  %0X", (UInt32)moduleBase, info->eip, (UInt32)moduleBase + (UInt32) moduleSize);
			info->moduleBase = moduleBase;
			strcpy_s(info->name, 100, name);
		}

		enumeratedModules.emplace((UInt32)moduleBase, (UInt32)moduleBase + (UInt32)moduleSize, std::string(name));

		return TRUE;
	}

	std::unordered_map<std::string, std::string> pluginNames = {
		{"hot_reload_editor", "hot_reload"},
		{"ilsfix", "ILS Fix"},
		{"improved_console", "Improved Console"},
		{"ImprovedLightingShaders", "Improved Lighting Shaders for FNV"},
		{"jip_nvse", "JIP LN NVSE"},
		{"johnnyguitar", "JohnnyGuitarNVSE"},
		{"MCM", "MCM Extensions"},
		{"mod_limit_fix", "Mod Limit Fix"},
		{"nvse_console_clipboard", "Console Clipboard"},
		{"nvse_stewie_tweaks", "lStewieAl's Tweaks"},
		{"ShowOffNVSE", "ShowOffNVSE Plugin"},
		{"supNVSE", "SUP NVSE Plugin"},
		{"ui_organizer", "UI Organizer Plugin"},
	};

	std::string GetPluginNameForFileName(std::string name)
	{
		if (pluginNames.contains(name))
			return pluginNames[name];
		return name;
	}

	void Get(EXCEPTION_POINTERS* info)
	{
		HANDLE process = GetCurrentProcess();

		const UInt32 eip = info->ContextRecord->Eip;

		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };

		Safe_EnumerateLoadedModules(process, EumerateModulesCallback, &infoUser);
	
		if (infoUser.moduleBase)
			Log() << std::format("GAME CRASHED AT INSTRUCTION Base+0x{:08X} IN MODULE: {}", (infoUser.eip - infoUser.moduleBase), infoUser.name) << std::endl
				<< "Please note that this does not automatically mean that that module is responsible. It may have been supplied bad data or" << std::endl
				<< "program state as the result of an issue in the base game or a different DLL.";
		else
			Log() << "UNABLE TO IDENTIFY MODULE CONTAINING THE CRASH ADDRESS." << std::endl
				<< "This can occur if the crashing instruction is located in the vanilla address space, but it can also occur if there are too many" << std::endl
				<< "DLLs for us to list, and if the crash occurred in one of their address spaces. Please note that even if the crash occurred" << std::endl
				<< "in vanilla code, that does not necessarily mean that it is a vanilla problem. The vanilla code may have been supplied bad data" << std::endl
				<< "or program state as the result of an issue in a loaded DLL.";

		Log();

		Log() << "Module bases:";
		for (const auto& [moduleBase, moduleEnd, path] : enumeratedModules)
		{
			std::string version;

			if (const auto info = g_commandInterface->GetPluginInfoByName(GetPluginNameForFileName(path.stem().generic_string()).c_str()))
				version = std::format("NVSE plugin version: {:>4d}, ", info->version);

			Log() << std::format("0x{:08X} - 0x{:08X} ==> {:25s}{:>30s}{}", moduleBase, moduleEnd, path.stem().generic_string() + ",", version, path.generic_string());

		}

		Log();
	}
}

namespace CrashLogger
{

	void Get(EXCEPTION_POINTERS* info) {

		Calltrace::Get(info);

		Registry::Get(info);

		Stack::Get(info);

		ModuleBases::Get(info);

		Logger::Copy();

		Safe_SymCleanup(GetCurrentProcess());
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

	void ApplyNVSE()
	{
		s_originalFilter = SetUnhandledExceptionFilter(&Filter);
		SafeWrite32(0x00FDF180, (UInt32)&FakeSetUnhandledExceptionFilter);
	}

	// thanks Stewie
	void ApplyFOSE()
	{
		s_originalFilter = SetUnhandledExceptionFilter(&Filter);
		SafeWrite32(0x00D9B180, (UInt32)&FakeSetUnhandledExceptionFilter);
	}

	void ApplyOBSE()
	{
		s_originalFilter = SetUnhandledExceptionFilter(&Filter);
		SafeWrite32(0x00A281B4, (UInt32)&FakeSetUnhandledExceptionFilter);
	}
};