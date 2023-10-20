#pragma once
#include <NiObjects.h>
#include <Utilities.hpp>

struct ChunkAndFormType {
	UInt32		chunkType;	// ie 
	UInt32		formType;	// ie 0x2A
	const char*	formName;	// ie 'NPC_'
};

static const UInt32 _ModInfo_GetNextChunk = 0x004726B0; // args: none retn: UInt32 subrecordType (third call in TESObjectARMO_LoadForm)
static const UInt32 _ModInfo_GetChunkData = 0x00472890;	// args: void* buf, UInt32 bufSize retn: bool readSucceeded (fifth call in TESObjectARMO_LoadForm)
static const UInt32 _ModInfo_Read32		  =	0x004727F0;	// args: void* buf retn: void (find 'LPER', then next call, still in TESObjectARMO_LoadForm)
static const UInt32 _ModInfo_HasMoreSubrecords = 0x004726F0;	// Last call before "looping" to GetNextChunk in TESObjectARMO_LoadForm.
static const UInt32 _ModInfo_InitializeForm = 0x00472F60;	// args: TESForm* retn: void (second call in TESObjectARMO_LoadForm)

// addresses of static ModInfo members holding type info about currently loading form
static UInt32* s_ModInfo_CurrentChunkTypeCode = (UInt32*)0x011C54F4;
static UInt32* s_ModInfo_CurrentFormTypeEnum = (UInt32*)0x011C54F0;
// in last call (SetStaticFieldsAndGetFormTypeEnum) of first call (ModInfo__GetFormInfoTypeID) from _ModInfo_InitializeForm
		//		s_ModInfo_CurrentChunkTypeCode is first cmp
		//		s_ModInfo_CurrentChunkTypeEnum is next mov
static const ChunkAndFormType* s_ModInfo_ChunkAndFormTypes = (const ChunkAndFormType*)0x01187008;	// Array used in the loop in SetStaticFieldsAndGetFormTypeEnum, starts under dd offset aNone

static UInt8** g_CreatedObjectData = (UInt8**)0x011C54CC;	// pointer to FormInfo + form data, filled out by TESForm::SaveForm()
static UInt32* g_CreatedObjectSize = (UInt32*)0x011C54D0;
		// in first call (Form_startSaveForm) in TESObjectARMO__SaveForm:
		//		g_CreatedObjectSize is set to 18h
		//		g_CreatedObjectData is set to the eax result of the next call

// 10
class BoundObjectListHead
{
public:
	BoundObjectListHead();
	~BoundObjectListHead();

	UInt32			boundObjectCount;	// 0
	TESBoundObject*	first;				// 4
	TESBoundObject*	last;				// 8
	UInt32			unkC;				// C
};

struct FormRecordData
{
	UInt8		typeID;		// corresponds to kFormType_XXX
	UInt32		typeCode;	// i.e. 'GMST', 'FACT'
	UInt32		unk08;		// only seen zero
};

struct ChunkHeader
{
	UInt32	type : 4;	// i.e. 'XGRD', 'DATA'
	UInt16	size : 2;
};

struct ModInfo		// referred to by game as TESFile
{
	ModInfo();
	~ModInfo();

	// 18 info about currently loading form
	struct FormInfo  // Record Header in FNVEdit
	{
		UInt32		recordType;			// 00 i.e. 'FACT', 'GMST'						Signature
		UInt32		dataSize;			// 04 looks like size of entire record			Data Size
		UInt32		formFlags;			// 08 copied to TESForm->flags					Record Flags
		UInt32		formID;				// 0C											FormID
		UInt32		unk10;				// 10											Version Control Info 1
		UInt16		formVersion;		// 14 always initialized to 0F on SaveForm.		Form Version
		UInt16		unk16;				// 16                                           Version Control Info 2
	};

	// 18 info about current group of form
	struct GroupInfo  // Record Header in FNVEdit
	{
		UInt32		recordType;			// 00 'GRUP'									Signature
		UInt32		groupSize;			// 04 Size of entire record						Size
		UInt32		groupLabel;			// 08 copied to TESForm->flags					Label
		UInt32		groupType;			// 0C forms, dialog, cell...					Type
		UInt32		unk10;				// 10											Stamp
		UInt16		unk14;				// 14											Part of Unknown
		UInt16		unk16;				// 16                                           Part of Unknown
	};

	struct FileHeader	// File header in FNVEdit Signature 'HEDR'
	{
		Float32	version;		//	00
		UInt32	recordCount;	//	04
		UInt32	nextObectID;	//	08
	};

	struct	MasterSize	// Data member of the master list in WIN32_FIND_DATA format
	{
		UInt32	low;
		UInt32	high;
	};

	TList<ModInfo>						unkList;			// 000 treated as ModInfo during InitializeForm, looks to be a linked list of modInfo. Also during Save in GECK.
	UInt32 /*NiTPointerMap<TESFile*>*/	* pointerMap;		// 008
	UInt32								unk00C;				// 00C
	BSFile*								unkFile;			// 010
	UInt32								unk014;				// 014 
	void								* unk018;			// 018 seen all zeroes. size unknown, seen not valid pointer in FalloutNV.esm
	void								* unk01C;			// 01C as above
	char								name[0x104];		// 020
	char								filepath[0x104];	// 124
	UInt32								unk228;				// 228
	UInt32								unk22C;				// Masters are init'd to dword_1186740 (0x2800) same val as BSFile+10? Buffer size ?
	UInt32								unk230;				// 230
	UInt32								unk234;				// 234
	UInt32								unk238;				// 238
	UInt32								unk23C;				// 23C
	FormInfo							formInfo;			// 240
	ChunkHeader							subRecordHeader;	// 258
	UInt32								unk260;				// 260 could be file size, as it is compared with fileOffset during load module. But filesize would be an Int64 !
	UInt32								fileOffset;			// 264
	UInt32								dataOffset;			// 268 index into dataBuf
	UInt32								subrecordBytesRead;	// 26C generates error on Read if != expected length
	FormInfo							writeInfo;			// 270 "used" to add record to the plugin.
	UInt32								writeOffset;		// 288
	UInt32								subrecordBytesToWrite;	// 28C
	TList<UInt32>						TList290;			// 290 looks to be a list of form or a list of formInfo. referenced from TESForm::WriteForm
	UInt8								unk298;				// 298
	UInt8								bIsBigEndian;		// 299
	UInt8								unk29A;				// 29A
	UInt8								pad29B;
	WIN32_FIND_DATA						fileData;			// 29C
	FileHeader							header;				// 3DC
	UInt8								flags;				// 3E8	Bit 0 is ESM . Runtime: Bit 2 is Valid, Bit 3 is Unselected Editor: 2 is selected, 3 is active, 4 may be invalid, 6 is endian, 14 controls VCI.
	UInt8								pad3E9[3];
	TList<char*>						refModNames;		// 3EC
	TList<MasterSize*>					refModData;			// 3F4 most likely full of 0
	UInt32								numRefMods;			// 3FC related to modindex; see 4472D0
																// formIDs in mod are as saved in GECK, must fix up at runtime
	ModInfo								** refModInfo;		// 400 used to look up modInfo based on fixed mod index, double-check
	UInt32								unk404;				// 404
	UInt32								unk408;				// 408
	UInt8								modIndex;			// 40C init to 0xFF
	UInt8								pad40D[3];
	String								author;				// 410
	String								description;		// 418
	void								* dataBuf;			// 420 
	UInt32								dataBufSize;		// 424 looks like size of entire record
	UInt8								unk428;				// 428 decide if forms needs to be reloaded on LoadFiles
	UInt8								pad429[3];

	// In Editor: 430 = ONAM array and 434 ONAM array count. Allocated at 0438
	
	bool IsLoaded() const { return true; }

	/*** used by TESForm::LoadForm() among others ***/
	MEMBER_FN_PREFIX(ModInfo);
	DEFINE_MEMBER_FN(GetNextChunk, UInt32, _ModInfo_GetNextChunk);	// returns chunk type
	DEFINE_MEMBER_FN(GetChunkData, bool, _ModInfo_GetChunkData, UInt8* buf, UInt32 bufSize); // max size, not num to read
	DEFINE_MEMBER_FN(Read32, void, _ModInfo_Read32, void* out);
	DEFINE_MEMBER_FN(HasMoreSubrecords, bool, _ModInfo_HasMoreSubrecords);
};

static_assert(sizeof(WIN32_FIND_DATA) == 0x140);
static_assert(sizeof(ModInfo) == 0x42C);

struct ModList
{
	TList<ModInfo>		modInfoList;		// 00
	UInt32				loadedModCount;		// 08
	ModInfo*			loadedMods[0xFF];	// 0C
};
static_assert(sizeof(ModList) == 0x408);

// 5B8
class TESDataHandler
{
public:
	TESDataHandler();
	~TESDataHandler();

	UInt32							unk00;					// 000
	BoundObjectListHead*			boundObjectList;		// 004
	TList<TESPackage>				packageList;			// 008
	TList<TESWorldSpace>			worldSpaceList;			// 010
	TList<TESClimate>				climateList;			// 019
	TList<TESImageSpace>			imageSpaceList;			// 020
	TList<TESImageSpaceModifier>	imageSpaceModList;		// 028
	TList<TESWeather>				weatherList;			// 030
	TList<EnchantmentItem>			enchantmentItemList;	// 038
	TList<SpellItem>				spellItemList;			// 040
	TList<BGSHeadPart>				headPartList;			// 048
	TList<TESHair>					hairList;				// 050
	TList<TESEyes>					eyeList;				// 058
	TList<TESRace>					raceList;				// 060
	TList<BGSEncounterZone>			encounterZoneList;		// 068
	TList<TESLandTexture>			landTextureList;		// 070
	TList<BGSCameraShot>			cameraShotList;			// 078
	TList<TESClass>					classList;				// 080
	TList<TESFaction>				factionList;			// 088
	TList<TESReputation>			reputationList;			// 090
	TList<TESChallenge>				challengeList;			// 098
	TList<TESRecipe>				recipeList;				// 0A0
	TList<TESRecipeCategory>		recipeCategoryList;		// 0A8
	TList<TESAmmoEffect>			ammoEffectList;			// 0B0
	TList<TESCasino>				casinoList;				// 0B8
	TList<TESCaravanDeck>			caravanDeckList;		// 0C0
	TList<Script>					scriptList;				// 0C8
	TList<TESSound>					soundList;				// 0D0
	TList<BGSAcousticSpace>			acousticSpaceList;		// 0D8
	TList<BGSRagdoll>				ragdollList;			// 0E0
	TList<TESGlobal>				globalList;				// 0E8
	TList<BGSVoiceType>				voiceTypeList;			// 0F0
	TList<BGSImpactData>			impactDataList;			// 0F8
	TList<BGSImpactDataSet>			impactDataSetList;		// 100
	TList<TESTopic>					topicList;				// 108
	TList<TESTopicInfo>				topicInfoList;			// 110
	TList<TESQuest>					questList;				// 118
	TList<TESCombatStyle>			combatStyleList;		// 120
	TList<TESLoadScreen>			loadScreenList;			// 128
	TList<TESWaterForm>				waterFormList;			// 130
	TList<TESEffectShader>			effectShaderList;		// 138
	TList<BGSProjectile>			projectileList;			// 140
	TList<BGSExplosion>				explosionList;			// 148
	TList<BGSRadiationStage>		radiationStageList;		// 150
	TList<BGSDehydrationStage>		dehydrationStageList;	// 158
	TList<BGSHungerStage>			hungerStageList;		// 160
	TList<BGSSleepDeprivationStage>	sleepDepriveStageList;	// 168
	TList<BGSDebris>				debrisList;				// 170
	TList<BGSPerk>					perkList;				// 178
	TList<BGSBodyPartData>			bodyPartDataList;		// 180
	TList<BGSNote>					noteList;				// 188
	TList<BGSListForm>				listFormList;			// 190
	TList<BGSMenuIcon>				menuIconList;			// 198
	TList<TESObjectANIO>			anioList;				// 1A0
	TList<BGSMessage>				messageList;			// 1A8
	TList<BGSLightingTemplate>		lightningTemplateList;	// 1B0
	TList<BGSMusicType>				musicTypeList;			// 1B8
	TList<TESLoadScreenType>		loadScreenTypeList;		// 1C0
	TList<MediaSet>					mediaSetList;			// 1C8
	TList<MediaLocationController>	mediaLocControllerList;	// 1D0
	TESRegionList*					regionList;				// 1D8
	NiTArray<TESObjectCELL*>		cellArray;				// 1DC
	NiTArray<BGSAddonNode*>			addonArray;				// 1EC

	UInt32							unk1FC[0x3];			// 1FC	208 looks like next created refID
	UInt32							nextCreatedRefID;		// 208	Init'd to FF000800 (in GECK init'd to nn000800)
	ModInfo*						activeFile;					// 20C	last unselected mod in modList. GECK: active ESM
	ModList							modList;				// 210
	UInt8							unk618;					// 618	5A4
	UInt8							unk619;					// 619
	UInt8							unk61A;					// 61A	referenced during LoadForm (ie TESSpellList). bit 1 might mean refID to pointer conversion not done. For GECK means save in progress
	UInt8							unk61B;					// 61B
	UInt8							unk61C;					// 61C	5A8
	UInt8							unk61D;					// 61D
	UInt8							unk61E;					// 61E
	UInt8							unk61F;					// 61F
	UInt8							unk620;					// 620	5AC
	UInt8							loading;				// 621	Init'd to 0 after loadForms
	UInt8							unk622;					// 622	referenced during loading of modules. Compared with type = GameSetting but seems to always end up equal to 1.
	UInt8							unk623;					// 623
	TESRegionManager*				regionManager;			// 624	5B0
	UInt32							unk628;					// 628	5B4
	UInt32							unk62C;					// 62C	
	UInt32							unk630;					// 630
	UInt32							unk634;					// 634
	UInt32							unk638;					// 638

	__forceinline static TESDataHandler* GetSingleton() { return *reinterpret_cast<TESDataHandler**>(0x011C3F2C); }
	std::vector<ModInfo*> GetActiveModList();		// returns array of modEntry* corresponding to loaded mods sorted by mod index
	const ModInfo* LookupModByName(const char* modName);
	UInt8 GetModIndex(const char* modName);
	UInt8 GetActiveModCount() const;
	const char* GetNthModName(UInt32 modIndex);
	void DisableAssignFormIDs(bool shouldAsssign);

	UInt32 DoAddForm(TESForm* pForm) { return ThisCall<UInt32>(0x004603B0, this, pForm); }
	TESQuest* GetQuestByName(const char* questName);
};
static_assert(sizeof(TESDataHandler) == 0x63C);

// A0
struct WaterSurfaceManager
{
	// B0 c'tor @ 0x4ED5F0
	struct WaterGroup
	{
		TESWaterForm* waterForm;		// 00
		NiPoint4				vector04;		// 04
		NiPoint4				vector14;		// 14
		DList<TESObjectREFR>	waterPlanes;	// 24
		DList<void>				list30;			// 30
		DList<void>				list3C;			// 3C
		DList<void>				list48;			// 48
		NiAVObject* object54;		// 54
		NiAVObject* object58;		// 58
		UInt8					byte5C;			// 5C
		UInt8					byte5D;			// 5D
		UInt8					byte5E;			// 5E
		UInt8					byte5F;			// 5F
		UInt8					byte60;			// 60
		UInt8					pad61[3];		// 61
		DList<void>				list64;			// 64
		DList<void>				list70;			// 70
		DList<void>				list7C;			// 7C
		DList<void>				list88;			// 88
		NiObject* object94;		// 94
		NiObject* object98;		// 98
		UInt32					unk9C;			// 9C
		UInt32					unkA0;			// A0
		NiObject* objectA4;		// A4
		NiObject* objectA8;		// A8
		UInt32					unkAC;			// AC
	};

	struct Struct8C
	{
		UInt32		unk00;
		UInt32		unk04;
		UInt32		unk08;
	};

	UInt32								unk00;			// 00
	UInt32								unk04;			// 04
	NiObject* object08;		// 08
	NiObject* object0C;		// 0C
	NiObject* object10;		// 10
	NiObject* object14;		// 14
	NiObject* object18;		// 18
	NiObject* object1C;		// 1C	Seen NiSourceTexture
	NiObject* object20;		// 20
	UInt32								unk24;			// 24
	UInt32								unk28;			// 28
	UInt32								unk2C;			// 2C
	UInt32								unk30;			// 30
	UInt32								unk34;			// 34
	UInt32								unk38;			// 38
	DList<WaterGroup>					waterGroups;	// 3C
	WaterGroup* waterLOD;		// 48	(Assumed)
	NiTPointerMap<TESObjectREFR>		map4C;			// 4C
	NiTPointerMap<TESObjectREFR>		map5C;			// 5C
	NiTPointerMap<TESWaterForm>			map6C;			// 6C
	NiTMapBase<TESObjectREFR*, void*>	map7C;			// 7C
	Struct8C							unk8C;			// 8C
	float								flt98;			// 98
	UInt32								unk9C;			// 9C
};
static_assert(sizeof(WaterSurfaceManager) == 0xA0);

class TES
{
public:
	virtual void Fn_00(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);

	UInt32								unk04;				// 04
	GridCellArray						*gridCellArray;		// 08
	NiNode								*niNode0C;			// 0C
	NiNode								*niNode10;			// 10
	NiNode								*niNode14;			// 14
	BSTempNodeManager					*tempNodeMgr;		// 18
	NiDirectionalLight					*directionalLight;	// 1C
	void								*ptr20;				// 20
	SInt32								extGridX;			// 24
	SInt32								extGridY;			// 28
	SInt32								extCoordX;			// 2C
	SInt32								extCoordY;			// 30
	TESObjectCELL						*currentInterior;	// 34
	TESObjectCELL						**interiorsBuffer;	// 38
	TESObjectCELL						**exteriorsBuffer;	// 3C
	UInt32								unk40[9];			// 40
	WaterSurfaceManager					*waterManager;		// 64
	Sky									*sky;				// 68
	TList<ImageSpaceModifierInstance>	activeIMODs;		// 6C
	UInt32								unk74[3];			// 74
	float								flt80;				// 80	Abs X distance from centre of grid.
	float								flt84;				// 84	Abs Y distance from centre of grid.
	TESWorldSpace						*currentWrldspc;	// 88
	TList<UInt32>						list8C;				// 8C
	TList<UInt32>						list94;				// 94
	TList<UInt32>						list9C;				// 9C
	QueuedFile							*unkA4;				// A4
	NiSourceTexture						*unkA8;				// A8
	QueuedFile							*unkAC;				// AC
	void								*ptrB0;				// B0
	UInt32								unkB4[2];			// B4
	NavMeshInfoMap						*navMeshInfoMap;	// BC
	LoadedAreaBound						*areaBound;			// C0

	__forceinline static TES *GetSingleton() {return *(TES**)0x11DEA10;}

	__forceinline bool GetTerrainHeight(NiPoint2 *posXY, float *result)
	{
		return ThisCall<bool>(0x4572E0, this, posXY, result);
	}

	__forceinline void AddTempNode(NiAVObject *object, float lifetime)
	{
		ThisCall(0x458E20, this, object, lifetime);
	}
};
static_assert(sizeof(TES) == 0xC4);
