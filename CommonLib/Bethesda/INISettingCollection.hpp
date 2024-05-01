#pragma once

#include "Setting.hpp"
#include "SettingCollectionList.hpp"

class INISettingCollection : public SettingCollectionList<Setting> {
public:
	INISettingCollection();
	~INISettingCollection() override;

	static INISettingCollection* GetSingleton();

	struct AnimationWarning {
		static bool* const bClampWarning;
		static bool* const bPriorityWarning;
	};

	struct Archive {
		static bool* const bCheckRuntimeCollisions;
		static bool* const bInvalidateOlderFiles;
		static bool* const bUseArchives;

		static SInt32* const iRetainDirectoryStringTable;
		static SInt32* const iRetainFilenameOffsetTable;
		static SInt32* const iRetainFilenameStringTable;

		static char* const sArchiveList;
		static char* const sInvalidationFile;
	};

	struct Audio {
		static bool* const bEnableAudio;
		static bool* const bEnableAudioCache;
		static bool* const bEnableEnviroEffectsOnPC;
		static bool* const bMultiThreadAudio;
		static bool* const bUseAudioDebugInformation;

		static SInt32* const iAttenuationDialogMenuMute;
		static SInt32* const iAttenuationEffectiveSilence;
		static SInt32* const iAudioCacheSize;
		static SInt32* const iAudioHWThread;
		static SInt32* const iCollisionSoundTimeDelta;
		static SInt32* const iMaxFX;
		static SInt32* const iMaxImpactSoundCount;
		static SInt32* const iMaxSizeForCachedSound;
		static SInt32* const iMusicSynchOverride;
		static SInt32* const iMusicTransitionInMS;
		static SInt32* const iRadioStationTimeout;
		static SInt32* const iRadioUpdateInterval;
		static SInt32* const iVoiceDecodeBufferSize;

		static float* const fASFadeInTime;
		static float* const fASFadeOutTime;
		static float* const fASFadeStartTime;
		static float* const fAudioDebugDelay;
		static float* const fChainLargeMassMin;
		static float* const fChainMediumMassMin;
		static float* const fCollisionSoundHeavyThreshold;
		static float* const fCreatureRadioMax;
		static float* const fCreatureRadioMin;
		static float* const fDBVoiceAttenuationIn2D;
		static float* const fDawnTime;
		static float* const fDayTime;
		static float* const fDialogHitSoundCooldownMax;
		static float* const fDialogHitSoundCooldownMin;
		static float* const fDialogMaxDistance;
		static float* const fDialogMinDistance;
		static float* const fDialogueFadeDecibels;
		static float* const fDialogueFadeSecondsIn;
		static float* const fDialogueFadeSecondsOut;
		static float* const fDialogueHeadPitchExaggeration;
		static float* const fDialogueHeadRollExaggeration;
		static float* const fDialogueHeadYawExaggeration;
		static float* const fDuskTime;
		static float* const fEarthLargeMassMin;
		static float* const fEarthMediumMassMin;
		static float* const fFilterDistortionGain;
		static float* const fFilterPEQGain;
		static float* const fFilterdBAttenuation;
		static float* const fGlassLargeMassMin;
		static float* const fGlassMediumMassMin;
		static float* const fHardLandingDamageThreshold;
		static float* const fLargeWeaponSpeedMax;
		static float* const fLargeWeaponWeightMin;
		static float* const fMainMenuMusicVolume;
		static float* const fMasterVolumeMult;
		static float* const fMaxFootstepDistance;
		static float* const fMediumWeaponSpeedMax;
		static float* const fMediumWeaponWeightMin;
		static float* const fMetalLargeMassMin;
		static float* const fMetalMediumMassMin;
		static float* const fMinSoundVel;
		static float* const fNightTime;
		static float* const fPlayerFootVolume;
		static float* const fRadioDialogMute;
		static float* const fRadioStaticAtOuterRadiusPct;
		static float* const fRegionLoopFadeInTime;
		static float* const fRegionLoopFadeOutTime;
		static float* const fSkinLargeMassMin;
		static float* const fSkinMediumMassMin;
		static float* const fStoneLargeMassMin;
		static float* const fStoneMediumMassMin;
		static float* const fWoodLargeMassMin;
		static float* const fWoodMediumMassMin;
	};

	struct BackgroundLoad {
		static bool* const bBackgroundCellLoads;
		static bool* const bBackgroundLoadLipFiles;
		static bool* const bLoadBackgroundFaceGen;
		static bool* const bLoadHelmetsInBackground;
		static bool* const bSelectivePurgeUnusedOnFastTravel;
		static bool* const bUseBackgroundFileLoader;
		static bool* const bUseMultiThreadedFaceGen;
		static bool* const bUseMultiThreadedTrees;

		static SInt32* const iAnimationClonePerLoop;
		static SInt32* const iPostProcessMilliseconds;
		static SInt32* const iPostProcessMillisecondsEditor;
		static SInt32* const iPostProcessMillisecondsLoadingQueuedPriority;
		static SInt32* const iPostProcessTaskWarningMilliseconds;
	};

	struct bLightAttenuation {
		static bool* const bOutQuadInLin;
		static bool* const bUseConstant;
		static bool* const bUseLinear;
		static bool* const bUseQuadratic;

		static UInt32* const uLinearMethod;
		static UInt32* const uQuadraticMethod;

		static float* const fConstantValue;
		static float* const fFlickerMovement;
		static float* const fLinearRadiusMult;
		static float* const fLinearValue;
		static float* const fQuadraticRadiusMult;
		static float* const fQuadraticValue;
	};

	struct BlurShaderHDRInterior {
		static SInt32* const iNumBlurpasses;

		static float* const fBlurRadius;
		static float* const fBrightClamp;
		static float* const fBrightScale;
		static float* const fEmissiveHDRMult;
		static float* const fEyeAdaptSpeed;
		static float* const fTargetLUM;
		static float* const fUpperLUMClamp;
	};

	struct BlurShaderHDR {
		static SInt32* const iBlendType;
		static SInt32* const iNumBlurpasses;

		static float* const fBlurRadius;
		static float* const fBrightClamp;
		static float* const fBrightScale;
		static float* const fEmissiveHDRMult;
		static float* const fEyeAdaptSpeed;
		static float* const fGrassDimmer;
		static float* const fSIEmmisiveMult;
		static float* const fSISpecularMult;
		static float* const fSkyBrightness;
		static float* const fSunBrightness;
		static float* const fSunlightDimmer;
		static float* const fTargetLUM;
		static float* const fTreeDimmer;
		static float* const fUpperLUMClamp;
	};

	struct BlurShader {
		static SInt32* const iBlendType;
		static SInt32* const iBlurTexSize;
		static SInt32* const iNumBlurpasses;

		static float* const fAlphaAddExterior;
		static float* const fAlphaAddInterior;
		static float* const fBlurRadius;
		static float* const fSIEmmisiveMult;
		static float* const fSISpecularMult;
		static float* const fSkyBrightness;
		static float* const fSunBrightness;
		static float* const fSunlightDimmer;
	};

	struct BudgetCaps {
		static bool* const bIncludeWaterInMNumberCalculations;

		static UInt32* const uActiveRefCount;
		static UInt32* const uActiveRefCountInterior;
		static UInt32* const uActorMemoryBudgetCap;
		static UInt32* const uActorRefCount;
		static UInt32* const uActorRefCountInterior;
		static UInt32* const uAnimatedObjectsCount;
		static UInt32* const uAnimatedObjectsCountInterior;
		static UInt32* const uCityLODBudgetAdjustment;
		static UInt32* const uDecalCount;
		static UInt32* const uDecalCountInterior;
		static UInt32* const uEmittersCount;
		static UInt32* const uEmittersCountInterior;
		static UInt32* const uGeometryCount;
		static UInt32* const uGeometryCountInterior;
		static UInt32* const uGeometryMemory;
		static UInt32* const uGeometryMemoryInterior;
		static UInt32* const uHavokTriCount;
		static UInt32* const uHavokTriCountInterior;
		static UInt32* const uLightCount;
		static UInt32* const uLightCountInterior;
		static UInt32* const uLightExcessGeometry;
		static UInt32* const uLightExcessGeometryInterior;
		static UInt32* const uLoadedAreaNonActorMemoryBudgetCap;
		static UInt32* const uParticlesCount;
		static UInt32* const uParticlesCountInterior;
		static UInt32* const uRefCount;
		static UInt32* const uRefCountInterior;
		static UInt32* const uTextureMemory;
		static UInt32* const uTextureMemoryInterior;
		static UInt32* const uTriangleCount;
		static UInt32* const uTriangleCountInterior;
		static UInt32* const uWastelandLODBudgetAdjustment;
		static UInt32* const uWaterCount;
		static UInt32* const uWaterCountInterior;
		static UInt32* const uWaterMemory;
		static UInt32* const uWaterMemoryInterior;

		static float* const fMaxMsUsagePerFrame;
		static float* const fMsActiveRefCount;
		static float* const fMsActorRefCount;
		static float* const fMsAnimatedObjectsCount;
		static float* const fMsDecalCount;
		static float* const fMsEmittersCount;
		static float* const fMsGeometryCount;
		static float* const fMsHavokTriCount;
		static float* const fMsLightCount;
		static float* const fMsLightExcessGeometry;
		static float* const fMsParticlesCount;
		static float* const fMsRefCount;
		static float* const fMsTriangleCount;
		static float* const fMsWaterCount;
	};

	struct CameraPath {
		static SInt32* const iFPS;
		static SInt32* const iTake;

		static char* const sDirectoryName;
		static char* const sNif;
	};

	struct Combat {
		static bool* const bAimSights;
		static bool* const bDebugCombatArea;
		static bool* const bDebugCombatAttackerText;
		static bool* const bDebugCombatCoverReservations;
		static bool* const bDebugCombatCoverReservationsText;
		static bool* const bDebugCombatCoverSearch2;
		static bool* const bDebugCombatCoverSearch;
		static bool* const bDebugCombatCoverSearchText;
		static bool* const bDebugCombatDetectionEvents;
		static bool* const bDebugCombatGroupClusters;
		static bool* const bDebugCombatGroups2;
		static bool* const bDebugCombatGroups;
		static bool* const bDebugCombatGuardRadius;
		static bool* const bDebugCombatProjectileLOS;
		static bool* const bDebugCombatSearch;
		static bool* const bDebugCombatTargets;
		static bool* const bDebugCombatTextColorDark;
		static bool* const bDebugCombatThreats;
		static bool* const bDebugCombatUnreachableLocations;
		static bool* const bDisableCombatDialogue;
		static bool* const bDisableCombatGroupStrategies;
		static bool* const bDisableNPCAttacks;
		static bool* const bDismemberOneLimb;
		static bool* const bEncounterZoneTargetRestrict;
		static bool* const bForceNPCsUseAmmo;
		static bool* const bIronSightsZoomEnable;
		static bool* const bLaserSights;
		static bool* const bPlayHitLocationIdles;
		static bool* const bPlayStaggers;
		static bool* const bProjectileDebug;
		static bool* const bVATSProjectileDebug;
		static bool* const bVatsAlwaysHit;

		static SInt32* const iMaxHiPerfCombatCount;
		static SInt32* const iMaxHiPerfNPCTargetCount;
		static SInt32* const iMaxHiPerfPCTargetCount;
		static SInt32* const iShowHitVector;

		static float* const fAimChaseLookingMult;
		static float* const fAimDownDegrees;
		static float* const fAimUpDegrees;
		static float* const fDebugCombatProjectileLOSTime;
		static float* const fDebugCombatTextSize;
		static float* const fHiPerfNPCTargetLOSTimer;
		static float* const fHiPerfPCTargetLOSTimer;
		static float* const fHitVectorDelay;
		static float* const fIronSightsZoomDefault;
		static float* const fLowPerfNPCTargetLOSTimer;
		static float* const fLowPerfPCTargetLOSTimer;
		static float* const fMinBloodDamage;
		static float* const fProjectileDebugDuration;
	};

	struct CopyProtectionStrings {
		static char* const sCopyProtectionMessage2;
		static char* const sCopyProtectionMessage;
		static char* const sCopyProtectionTitle2;
		static char* const sCopyProtectionTitle;
	};

	struct Controls {
		static bool* const bAlwaysRunByDefault;
		static bool* const bBackgroundKeyboard;
		static bool* const bBackgroundMouse;
		static bool* const bUseJoystick;

		static SInt32* const iJoystickLookLeftRight;
		static SInt32* const iJoystickLookUpDown;
		static SInt32* const iJoystickMoveFrontBack;
		static SInt32* const iJoystickMoveLeftRight;
		static SInt32* const iLanguage;
		static SInt32* const iXenonHorizLookAccel;
		static SInt32* const iXenonMenuStickDeadZone;
		static SInt32* const iXenonVertLookAccel;

		static float* const fAnalogMaxRunZonePercent;
		static float* const fAnalogRunZonePercent;
		static float* const fForegroundMouseAccelBase;
		static float* const fForegroundMouseAccelTop;
		static float* const fForegroundMouseBase;
		static float* const fForegroundMouseMult;
		static float* const fForwardBias;
		static float* const fJoystickLookLRMult;
		static float* const fJoystickLookUDMult;
		static float* const fJoystickMoveFBMult;
		static float* const fJoystickMoveLRMult;
		static float* const fMaxAnalogRunSpeed;
		static float* const fMaxAnalogWalkSpeed;
		static float* const fMinAnalogRunSpeed;
		static float* const fMinAnalogWalkSpeed;
		static float* const fPlayerCharacterTurnMult;
		static float* const fUFOCamSpeedMult;
		static float* const fXenonMenuDpadRepeatSpeed;
		static float* const fXenonMenuMouseXYMult;
		static float* const fXenonMenuStickSpeed;
		static float* const fXenonMenuStickSpeedMaxMod;
		static float* const fXenonMenuStickSpeedPlayerRotMod;
		static float* const fXenonMinLookSpeed;
	};
	
	struct Decals {
		static bool* const bDebugDecals;
		static bool* const bDecalMultithreaded;
		static bool* const bDecalOcclusionQuery;
		static bool* const bDecals;
		static bool* const bProfileDecals;
		static bool* const bSkinnedDecals;

		static UInt32* const uMaxSkinDecalPerActor;
		static UInt32* const uMaxSkinDecals;
	};
	
	struct Debug {
		static bool* const bDebugFaceGenCriticalSection;
		static bool* const bDebugFaceGenMultithreading;
		static bool* const bDebugSaveBuffer;
	};

	struct Display {
		static bool* const bActorSelfShadowing;
		static bool* const bAllow20HairShader;
		static bool* const bAllow30Shaders;
		static bool* const bAllowPartialPrecision;
		static bool* const bAllowScreenShot;
		static bool* const bAutoViewDistance;
		static bool* const bDecalsOnSkinnedGeometry;
		static bool* const bDo30VFog;
		static bool* const bDoActorShadows;
		static bool* const bDoAmbientPass;
		static bool* const bDoCanopyShadowPass;
		static bool* const bDoDiffusePass;
		static bool* const bDoSpecularPass;
		static bool* const bDoStaticAndArchShadows;
		static bool* const bDoTallGrassEffect;
		static bool* const bDoTestHDR;
		static bool* const bDoTexturePass;
		static bool* const bDynamicWindowReflections;
		static bool* const bEquippedTorchesCastShadows;
		static bool* const bForce1XShaders;
		static bool* const bForceMultiPass;
		static bool* const bForcePow2Textures;
		static bool* const bIgnoreResolutionCheck;
		static bool* const bImageSpaceEffects;
		static bool* const bLODNoiseAniso;
		static bool* const bMTRendering;
		static bool* const bReportBadTangentSpace;
		static bool* const bShadowsOnGrass;
		static bool* const bShowMenuTextureUse;
		static bool* const bStaticMenuBackground;
		static bool* const bUseFakeFullScreenMotionBlur;
		static bool* const bUseRefractionShader;
		static bool* const bUseResolvableDepth;
		static bool* const bUseShaders;
		static bool* const bUseSunbeams;

		static SInt32* const iAutoViewHiFrameRate;
		static SInt32* const iAutoViewLowFrameRate;
		static SInt32* const iAutoViewMinDistance;
		static SInt32* const iDebugTextLeftRightOffset;
		static SInt32* const iDebugTextTopBottomOffset;
		static SInt32* const iLocationX;
		static SInt32* const iLocationY;
		static SInt32* const iNPatchNOrder;
		static SInt32* const iNPatchPOrder;
		static SInt32* const iNPatches;
		static SInt32* const iPresentInterval;

		static UInt32* const uVideoDeviceIdentifierPart1;
		static UInt32* const uVideoDeviceIdentifierPart2;
		static UInt32* const uVideoDeviceIdentifierPart3;
		static UInt32* const uVideoDeviceIdentifierPart4;

		static char* const sDebugText;
		static char* const sScreenShotBaseName;

		static float* const fDecalLOD0;
		static float* const fDecalLifetime;
		static float* const fDefault1stPersonFOV;
		static float* const fDefaultFOV;
		static float* const fDefaultWorldFOV;
		static float* const fEnvMapLOD1;
		static float* const fEnvMapLOD2;
		static float* const fEyeEnvMapLOD1;
		static float* const fEyeEnvMapLOD2;
		static float* const fGammaMax;
		static float* const fGammaMin;
		static float* const fLODNoiseMipBias;
		static float* const fLandLOFadeSeconds;
		static float* const fLightLODDefaultStartFade;
		static float* const fLightLODMaxStartFade;
		static float* const fLightLODMinStartFade;
		static float* const fLightLODRange;
		static float* const fNearDistance;
		static float* const fNoLODFarDistanceMax;
		static float* const fNoLODFarDistanceMin;
		static float* const fNoLODFarDistancePct;
		static float* const fPipboy1stPersonFOV;
		static float* const fScopeScissorAmount;
		static float* const fShadowFadeTime;
		static float* const fShadowLODDefaultStartFade;
		static float* const fShadowLODMaxStartFade;
		static float* const fShadowLODMinStartFade;
		static float* const fShadowLODRange;
		static float* const fSkinnedDecalLOD0;
		static float* const fSkinnedDecalLOD1;
		static float* const fSkinnedDecalLOD2;
		static float* const fSpecularLODDefaultStartFade;
		static float* const fSpecularLODMaxStartFade;
		static float* const fSpecularLODMinStartFade;
		static float* const fSpecularLODRange;
	};

	struct DistantLOD {
		static SInt32* const iDistantLODGroupWidth;
	};

	struct FootIK {
		static bool* const bFootPlacementOn;
		static bool* const bRigidBodyController;

		static SInt32* const iNumFramesFootEaseOut;

		static float* const fAnkleOffset;
		static float* const fControllerTetherLen;
		static float* const fFootPlantedGain;
		static float* const fFootRaisedGain;
		static float* const fGroundAscendingGain;
		static float* const fGroundDescendingGain;
		static float* const fMaxFootCastMilliSec;
		static float* const fMaxStepVertError;
		static float* const fOnOffGain;
		static float* const fOriginalGroundHeightMS;
		static float* const fPelvisOffsetDamping;
		static float* const fPelvisUpDownBias;
		static float* const fRagdollFeedback;
		static float* const fVertErrorGain;
	};

	struct Fonts {
		static char* const sFontFile_1;
		static char* const sFontFile_2;
		static char* const sFontFile_3;
		static char* const sFontFile_4;
		static char* const sFontFile_5;
		static char* const sFontFile_6;
		static char* const sFontFile_7;
		static char* const sFontFile_8;
	};

	struct GamePlay {
		static bool* const bAllowHavokGrabTheLiving;
		static bool* const bEssentialTakeNoDamage;
		static bool* const bHealthBarShowing;
		static bool* const bInstantLevelUp;
		static bool* const bSetDemigodMode;
		static bool* const bShowChallengeUpdates;
		static bool* const bTrackProgress;

		static SInt32* const iActorsDismemberedPerFrame;
		static SInt32* const iDetectionPicks;

		static char* const sTrackProgressPath;

		static float* const fHealthBarEmittanceFadeTime;
		static float* const fHealthBarEmittanceTime;
		static float* const fHealthBarFadeOutSpeed;
		static float* const fHealthBarHeight;
		static float* const fHealthBarSpeed;
		static float* const fHealthBarWidth;
	};

	struct General {
		static bool* const bActivateAllQuestScripts;
		static bool* const bActorLookWithHavok;
		static bool* const bAlwaysActive;
		static bool* const bAnimateDoorPhysics;
		static bool* const bBorderRegionsEnabled;
		static bool* const bCRTMemoryChecks;
		static bool* const bChangeTimeMultSlowly;
		static bool* const bCheckCellOffsetsOnInit;
		static bool* const bCheckPurgedTextureList;
		static bool* const bCreateMapsEnable;
		static bool* const bCreateShaderPackage;
		static bool* const bDebugSpectatorThreats;
		static bool* const bDefaultCOCPlacement;
		static bool* const bDisableAllGore;
		static bool* const bDisableAutoVanityMode;
		static bool* const bDisableDuplicateReferenceCheck;
		static bool* const bDisplayBoundingVolumes;
		static bool* const bDrawSpellContact;
		static bool* const bEnableBoundingVolumeOcclusion;
		static bool* const bExternalLODDataFiles;
		static bool* const bFaceGenTexturing;
		static bool* const bFaceMipMaps;
		static bool* const bFadeInScreenShot;
		static bool* const bFixAIPackagesOnLoad;
		static bool* const bFixFaceNormals;
		static bool* const bKeepPluginWhenMerging;
		static bool* const bLoadFaceGenHeadEGTFiles;
		static bool* const bPreCullActors;
		static bool* const bPreemptivelyUnloadCells;
		static bool* const bPreloadIntroSequence;
		static bool* const bQueueWarnings;
		static bool* const bRenderLocalMapContinually;
		static bool* const bRunActorMovementDuringRendering;
		static bool* const bRunDetectionDuringRendering;
		static bool* const bRunMiddleLowLevelProcess;
		static bool* const bRunVTuneTest;
		static bool* const bShowCheckMemoryOutput;
		static bool* const bShowGunTarget;
		static bool* const bShowLoadingAreaMessage;
		static bool* const bTaskletActorAnimMovementUpdates;
		static bool* const bTaskletActorHavokSync;
		static bool* const bTaskletActorSceneGraphUpdates;
		static bool* const bTaskletCellTransformsUpdate;
		static bool* const bTaskletTempEffectsUpdate;
		static bool* const bTintMipMaps;
		static bool* const bTrackAllDeaths;
		static bool* const bUseEyeEnvMapping;
		static bool* const bUseFaceGenHeads;
		static bool* const bUseHardDriveCache;
		static bool* const bUseMovementBlockedPackage;
		static bool* const bUseMultibounds;
		static bool* const bUseMyGamesDirectory;
		static bool* const bUseOptimizedTextureLoading;
		static bool* const bUseThreadedAI;
		static bool* const bUseThreadedMorpher;
		static bool* const bUseThreadedParticleSystem;
		static bool* const bUseThreadedTempEffects;
		static bool* const bWarnOnMissingFileEntry;

		static SInt32* const iAIThread1HWThread;
		static SInt32* const iAIThread2HWThread;
		static SInt32* const iFPSClamp;
		static SInt32* const iHWThread1;
		static SInt32* const iHWThread2;
		static SInt32* const iHWThread3;
		static SInt32* const iHWThread4;
		static SInt32* const iHWThread5;
		static SInt32* const iHWThread6;
		static SInt32* const iHoursToSleep;
		static SInt32* const iIntroSequencePriority;
		static SInt32* const iNumBitsForFullySeen;
		static SInt32* const iNumHWThreads;
		static SInt32* const iPreloadSizeLimit;
		static SInt32* const iRenderingThread1HWThread;
		static SInt32* const iRenderingThread2HWThread;

		static UInt32* const uExteriorCellBuffer;
		static UInt32* const uGridsToLoad;
		static UInt32* const uInteriorCellBuffer;
		static UInt32* const uiEGTClampSize;
		static UInt32* const uiFaceGenMaxEGMDataSize;
		static UInt32* const uiFaceGenMaxEGTDataSize;

		static char* const sCharGenQuest;
		static char* const sEssentialFileCacheList;
		static char* const sIntroMovie;
		static char* const sIntroSequence;
		static char* const sLanguage;
		static char* const sLocalMasterPath;
		static char* const sLocalSavePath;
		static char* const sMainMenuMovieIntro;
		static char* const sStartingCell;
		static char* const sStartingCellX;
		static char* const sStartingCellY;
		static char* const sStartingWorld;
		static char* const sTestFile10;
		static char* const sTestFile1;
		static char* const sTestFile2;
		static char* const sTestFile3;
		static char* const sTestFile4;
		static char* const sTestFile5;
		static char* const sTestFile6;
		static char* const sTestFile7;
		static char* const sTestFile8;
		static char* const sTestFile9;
		static char* const sUnessentialFileCacheList;

		static float* const fAnimationDefaultBlend;
		static float* const fAnimationMult;
		static float* const fDegradeTexturesDistance;
		static float* const fLODActorAvoidanceDistance;
		static float* const fUpgradeTexturesDistance;
		static float* const fZoom3rdPersonSnapDist;
	};

	struct GeneralWarnings {
		static char* const sGeneralMasterMismatchWarning;
		static char* const sMasterMismatchWarning;
	};

	struct Grass {
		static bool* const bDrawShaderGrass;
		static bool* const bGrassPointLighting;

		static SInt32* const iGrassDensityEvalSize;
		static SInt32* const iMaxGrassTypesPerTexure;
		static SInt32* const iMinGrassSize;

		static float* const fGrassDefaultStartFadeDistance;
		static float* const fGrassFadeRange;
		static float* const fGrassMaxStartFadeDistance;
		static float* const fGrassMinStartFadeDistance;
		static float* const fGrassWindMagnitudeMax;
		static float* const fGrassWindMagnitudeMin;
		static float* const fTexturePctThreshold;
		static float* const fWaveOffsetRange;
	};

	struct GrabIK {
		static float* const fDriveGain;
	};

	struct GethitShader {
		static float* const fBlockedTexOffset;
		static float* const fBlurAmmount;
		static float* const fHitTexOffset;
	};

	struct HAVOK {
		static bool* const bAddBipedWhenKeyframed;
		static bool* const bAutoWaterUsesHavokShapes;
		static bool* const bDisablePlayerCollision;
		static bool* const bFindContactPointsOnAdd;
		static bool* const bHavokDebug;
		static bool* const bPreventHavokAddAll;
		static bool* const bPreventHavokAddClutter;
		static bool* const bTreeTops;
		static bool* const bWONameSync;

		static SInt32* const iEntityBatchRemoveRate;
		static SInt32* const iMinNumSubSteps;
		static SInt32* const iSimType;
		static SInt32* const iUpdateType;
		static SInt32* const iHavokSkipFrameCountTEST;

		static float* const fCameraCasterFadeSittingRadius;
		static float* const fCameraCasterPlayerSize;
		static float* const fCameraCasterSize;
		static float* const fCharControllerWarpDistSqr;
		static float* const fChaseDeltaMult;
		static float* const fJumpAnimDelay;
		static float* const fMaxPickTime;
		static float* const fMaxPickTimeDebug;
		static float* const fMaxPickTimeDebugVATS;
		static float* const fMaxPickTimeVATS;
		static float* const fMaxTime;
		static float* const fMoveLimitMass;
		static float* const fOD;
		static float* const fQuadrupedPitchMult;
		static float* const fRF;
		static float* const fSD;
		static float* const fSE;
		static float* const fTimePerSubStep;
	};

	struct Hacking {
		static float* const fHackingMinSkillAverage;
		static float* const fHackingMinSkillEasy;
		static float* const fHackingMinSkillHard;
		static float* const fHackingMinSkillVeryEasy;
		static float* const fHackingMinSkillVeryHard;
	};

	struct HeadTracking {
		static bool* const bDisableHeadTracking;

		static SInt32* const iUpdateActorsPerFrame;

		static float* const fUpdateDelayNewTargetSecondsMax;
		static float* const fUpdateDelayNewTargetSecondsMin;
		static float* const fUpdateDelaySecondsMax;
		static float* const fUpdateDelaySecondsMin;
	};

	struct Imagespace {
		static bool* const bDoMotionBlur;
		static bool* const bDoRadialBlur;

		static float* const fRenderDepthMaxDepth;
	};

	struct InterfaceFX {
		static bool* const bEnableMenuRenderDebugMode;
		static bool* const bUseImageSpaceMenuFX;
		static bool* const bUseOptimizedMenuRendering;

		static SInt32* const iDistortMaxInterval;

		static char* const sScanlineTexture;

		static float* const fBlurIntensityHUD;
		static float* const fBlurIntensityMenus;
		static float* const fBlurRadiusHUD;
		static float* const fBlurRadiusMenus;
		static float* const fDefaultBlurIntensity;
		static float* const fDefaultBlurRadius;
		static float* const fDefaultBurstDuration;
		static float* const fDefaultBurstIntensity;
		static float* const fDefaultScanlineFrequency;
		static float* const fDefaultShudderDuration;
		static float* const fDefaultShudderFrequency;
		static float* const fDefaultShudderIntensity;
		static float* const fDefaultVertHoldDuration;
		static float* const fDefaultVertHoldSpeed;
		static float* const fDistortDuration;
		static float* const fDistortHorizontalScale;
		static float* const fDistortVerticalScale;
		static float* const fHUDShudderDefaultDuration;
		static float* const fHUDShudderDefaultFrequencyX;
		static float* const fHUDShudderDefaultFrequencyY;
		static float* const fHUDShudderDefaultIntensity;
		static float* const fMenuPackerBufferOverflowZone;
		static float* const fMenuPackerBufferSize;
		static float* const fMiniBurstDuration;
		static float* const fMiniBurstIntensity;
		static float* const fPulseBrightenIntensity;
		static float* const fPulseRadiusIntensity;
		static float* const fPulseRate;
		static float* const fScanlineFrequencyHUD;
		static float* const fScanlineFrequencyMenus;
		static float* const fScreenLightBaseIntensity;
		static float* const fScreenLightBurstModifier;
		static float* const fScreenLightPulseModifier;
		static float* const fScreenLightRadius;
		static float* const fScreenLightVertHoldRate;
		static float* const fShudderChance;
		static float* const fVertHoldChance;
	};

	struct Interface {
		static bool* const bActivatePickUseGamebryoPick;
		static bool* const bAllowConsole;
		static bool* const bHideUnavailablePerks;
		static bool* const bUseFuzzyPicking;

		static SInt32* const iMaxViewCasterPicksFuzzy;
		static SInt32* const iMaxViewCasterPicksGamebryo;
		static SInt32* const iMaxViewCasterPicksHavok;
		static SInt32* const iSafeZoneX;
		static SInt32* const iSafeZoneXWide;
		static SInt32* const iSafeZoneY;
		static SInt32* const iSafeZoneYWide;

		static float* const fActivatePickSphereRadius;
		static float* const fDlgFocus;
		static float* const fDlgHeadingDegStart;
		static float* const fDlgHeadingDegStop;
		static float* const fDlgLookAdj;
		static float* const fDlgLookDegStart;
		static float* const fDlgLookDegStop;
		static float* const fDlgLookMult;
		static float* const fHUDShakeMult;
		static float* const fInterfaceTintB;
		static float* const fInterfaceTintG;
		static float* const fInterfaceTintR;
		static float* const fKeyRepeatInterval;
		static float* const fKeyRepeatTime;
		static float* const fMenuBGBlurRadius;
		static float* const fMenuBackgroundOpacity;
		static float* const fMenuModeAnimBlend;
		static float* const fMenuPlayerLightAmbientBlue;
		static float* const fMenuPlayerLightAmbientGreen;
		static float* const fMenuPlayerLightAmbientRed;
		static float* const fMenuPlayerLightDiffuseBlue;
		static float* const fMenuPlayerLightDiffuseGreen;
		static float* const fMenuPlayerLightDiffuseRed;
		static float* const fPopUpBackgroundOpacity;
		static float* const fRSMFaceSliderDefaultMax;
		static float* const fRSMFaceSliderDefaultMin;
		static float* const fRSMStartingZoom;
	};

	struct LOD {
		static bool* const bDisplayLODLand;
		static bool* const bUseFaceGenLOD;

		static SInt32* const iFadeNodeMinNearDistance;

		static float* const fActorLODDefault;
		static float* const fActorLODMax;
		static float* const fActorLODMin;
		static float* const fDistanceMultiplier;
		static float* const fFadeInThreshold;
		static float* const fFadeInTime;
		static float* const fFadeOutThreshold;
		static float* const fFadeOutTime;
		static float* const fItemLODDefault;
		static float* const fItemLODMax;
		static float* const fItemLODMin;
		static float* const fLODBoundRadiusMult;
		static float* const fLODFadeOutActorMultCity;
		static float* const fLODFadeOutActorMultComplex;
		static float* const fLODFadeOutActorMultInterior;
		static float* const fLODFadeOutItemMultCity;
		static float* const fLODFadeOutItemMultComplex;
		static float* const fLODFadeOutItemMultInterior;
		static float* const fLODFadeOutObjectMultCity;
		static float* const fLODFadeOutObjectMultComplex;
		static float* const fLODFadeOutObjectMultInterior;
		static float* const fLODFadeOutPercent;
		static float* const fLODLandDropAmount;
		static float* const fLODLandVerticalBias;
		static float* const fLODMultTrees;
		static float* const fLodDistance;
		static float* const fObjectLODDefault;
		static float* const fObjectLODMax;
		static float* const fObjectLODMin;
		static float* const fTalkingDistance;
	};

	struct Landscape {
		static bool* const bCurrentCellOnly;
		static bool* const bPreventSafetyCheck;

		static SInt32* const iLandBorder1B;
		static SInt32* const iLandBorder1G;
		static SInt32* const iLandBorder1R;
		static SInt32* const iLandBorder2B;
		static SInt32* const iLandBorder2G;
		static SInt32* const iLandBorder2R;

		static char* const sDefaultLandDiffuseTexture;
		static char* const sDefaultLandNormalTexture;

		static float* const fLandFriction;
		static float* const fLandTextureTilingMult;
	};

	struct LANGUAGE {
		static char* const sDPad;
		static char* const sFailureMessage;
		static char* const sSysUtil_AutoSaveWarning;
		static char* const sSysUtil_DirtyDisc;
		static char* const sSysUtil_DiscEject;
		static char* const sSysUtil_GameDataCorrupt;
		static char* const sSysUtil_GameDataInsufficientSpace;
		static char* const sSysUtil_Retry;
		static char* const sSysUtil_SaveDataCreateNew;
		static char* const sSysUtil_SaveDataInsufficientSpace;
		static char* const sThumbstick;
	};

	struct Loading {
		static SInt32* const iMaxScreens;
		static SInt32* const iMaxScreens_MainMenu;
		static SInt32* const iNumLocationSpecificScreens;

		static char* const sInitialSound;
		static char* const sMainMenuBackground;
		static char* const sTitleMusic;
		static char* const sWelcomeScreen1;
		static char* const sWelcomeScreen2;
		static char* const sWelcomeScreen3;
		static char* const sWelcomeScreen4;

		static float* const fLoadingBkgdUpdateInterval;
		static float* const fLoadingInitUpdateInterval;
		static float* const fLoadingTextUpdateInterval;
		static float* const fMainMenuBkgdUpdateInterval;
	};

	struct LookIK {
		static float* const fAngleMax;
		static float* const fAngleMaxEase;
		static float* const fEaseAngleShutOff;
		static float* const fMaxTrackingDist;
		static float* const fMinTrackingDist;
	};

	struct MAIN {
		static float* const fQuestScriptDelayTime;
	};

	struct MESSAGES {
		static bool* const bAllowYesToAll;
		static bool* const bBlockMessageBoxes;
		static bool* const bSkipInitializationFlows;
		static bool* const bSkipProgramFlows;
		static bool* const bUseWindowsMessageBox;

		static SInt32* const iFileLogging;
	};

	struct Menu {
		static SInt32* const iConsoleFont;
		static SInt32* const iConsoleHistorySize;
		static SInt32* const iConsoleLineSpacing;
		static SInt32* const iConsoleTextXPos;
		static SInt32* const iConsoleTextYPos;
		static SInt32* const iConsoleVisibleLines;
		static SInt32* const iDebugTextFont;

		static float* const fCreditsScrollSpeed;

		static void* const rDebugTextColor;
	};

	struct NavMeshGeneration {
		static bool* const bGlobalNavMeshCheck;
	};

	struct Online {
		static bool* const bXenonLoadDownloadedContent;

		static UInt32* const uXenonPresenceUpdateInterval;
	};

	struct Pathfinding {
		static bool* const bBackgroundNavmeshUpdate;
		static bool* const bBackgroundPathing;
		static bool* const bCreateDebugInfo;
		static bool* const bCutDoors;
		static bool* const bDrawPathBounds;
		static bool* const bRebuildPathIfSmootherFailed;
		static bool* const bSelectedPathDrawAvoidNodes;
		static bool* const bUseActorAvoidance;
		static bool* const bUseAlternateSmoothingForPrime;
		static bool* const bUseFartherCheckForAvoidance;
		static bool* const bUseObstacleAvoidance;
		static bool* const bUseOldPathSmoothing;
		static bool* const bUsePathSmoothing;
		static bool* const bUseRayCasts;
		static bool* const bUseStraightLineCheckFirst;

		static float* const fAICombatTurnSpeedScale;
		static float* const fAITurnSpeedScale;
		static float* const fAvoidanceAvoidAllRadius;
		static float* const fAvoidanceConeAngle;
		static float* const fAvoidanceDefaultWaitTime;
		static float* const fAvoidanceDetectionTime;
		static float* const fAvoidanceIgnoreMinTime;
		static float* const fAvoidanceIgnoreTime;
		static float* const fAvoidanceMinWaitTime;
		static float* const fAvoidanceTimeCheck;
		static float* const fAvoidanceWiderConeAngle;
		static float* const fCreatureCombatTurnSpeedScale;
		static float* const fCreatureTurnSpeedScale;
		static float* const fObstacleUpdateDeltaWhenMoving;
		static float* const fObstacleUpdateDeltaWhenUnknown;
		static float* const fPOVSmootherAvoidNodeCost;
		static float* const fRepathingWaitDistance;
		static float* const fRotationSpeedIncrease;
		static float* const fRunningWiderConeAngle;
	};

	struct Pathing {
		static float* const fFindClosestEdgesRadius;
		static float* const fINIDetectDoorsForPathingTime;
	};

	struct Pipboy {
		static bool* const bEnableFlickerPipboy;
		static bool* const bUsePipboyMode;

		static float* const fBlurIntensityPipboy;
		static float* const fBlurRadiusPipboy;
		static float* const fLightEffectFadeDuration;
		static float* const fLightEffectLightFadeDuration;
		static float* const fScanlineFrequencyPipboy;
		static float* const fScrollKnobIncrement;
		static float* const fScrollKnobRate;
		static float* const fTabKnobMaxPosition;
		static float* const fTabKnobMinPosition;
		static float* const fTabKnobMoveRate;
	};

	struct RagdollAnim {
		static bool* const bFootIK;
		static bool* const bGrabIK;
		static bool* const bLookIK;
		static bool* const bPoseMatching;
		static bool* const bRagdollAnim;
		static bool* const bRagdollFeedback;

		static float* const fAccelerationGain;
		static float* const fCameraDist;
		static float* const fDesiredVel;
		static float* const fDetectionUpdateTimeSec;
		static float* const fHierarchyGain;
		static float* const fImpulseLimit;
		static float* const fPositionGain;
		static float* const fPositionMaxAngularVelocity;
		static float* const fPositionMaxLinearVelocity;
		static float* const fSnapGain;
		static float* const fSnapMaxAngularDistance;
		static float* const fSnapMaxAngularVelocity;
		static float* const fSnapMaxLinearDistance;
		static float* const fSnapMaxLinearVelocity;
		static float* const fVelocityDamping;
		static float* const fVelocityGain;
	};

	struct RagdollFeedback {
		static UInt32* const uiFeedbackTimeout;
	};

	struct RenderedMenus {
		static float* const fRaceSexMenuDistortDuration;
	};

	struct RenderedTerminal {
		static bool* const bUseRenderedTerminals;

		static float* const fRaceSexMenuHPos;
		static float* const fRaceSexMenuScale;
		static float* const fRaceSexMenuVPos;
		static float* const fRaceSexMenuZoom;
		static float* const fRenderedTerminalFOV;
		static float* const fRenderedTerminalHPos;
		static float* const fRenderedTerminalScanlineScale;
		static float* const fRenderedTerminalVPos;
		static float* const fRenderedTerminalZoom;
		static float* const fScreenLightBaseIntensity;
		static float* const fScreenLightColorB;
		static float* const fScreenLightColorG;
		static float* const fScreenLightColorR;
		static float* const fScreenLightRadius;
	};

	struct SaveGame {
		static bool* const bAllowProfileTransfer;
		static bool* const bAllowScriptedAutosave;
		static bool* const bAllowScriptedForceSave;
		static bool* const bCopySaveGameToHostOrMemStick;
		static bool* const bDisplayMissingContentDialogue;
		static bool* const bOutputSaveGameScreenshot;
		static bool* const bUseSaveGameHistory;

		static SInt32* const iSaveGameBackupCount;

		static char* const sSaveGameSafeMarkerID;
	};

	struct ScreenSplatter {
		static bool* const bBloodSplatterEnabled;
		static bool* const bCustomSplatterEnabled;
	};

	struct SpeedTree {
		static bool* const bEnableTrees;
		static bool* const bForceFullLOD;

		static SInt32* const iCanopyShadowScale;

		static float* const fCanopyShadowGrassMult;
		static float* const fLODTreeMipMapLODBias;
		static float* const fLocalTreeMipMapLODBias;
		static float* const fTreeForceBranchDimming;
		static float* const fTreeForceCS;
		static float* const fTreeForceLLA;
		static float* const fTreeForceLeafDimming;
		static float* const fTreeForceMaxBudAngle;
		static float* const fTreeForceMinBudAngle;
		static float* const fTreeLODExponent;
	};

	struct Surgery {
		static float* const fSurgeryPlayerDefaultHPos3D;
		static float* const fSurgeryPlayerDefaultScale3D;
		static float* const fSurgeryPlayerDefaultVPos3D;
		static float* const fSurgeryPlayerZoom3D;
		static float* const fSurgerySkinHPos3D;
		static float* const fSurgerySkinScale3D;
		static float* const fSurgerySkinVPos3D;
		static float* const fSurgerySkinZoom3D;
	};

	struct TerrainManager {
		static bool* const bKeepLowDetailTerrain;
		static bool* const bUseDistantObjectBlocks;
		static bool* const bUseDistantTrees;
		static bool* const bUseNewTerrainSystem;

		static UInt32* const uDistantTreeBlockCacheSizePerCell;
		static UInt32* const uTerrainTextureFadeTime;

		static float* const fDefaultBlockLoadDistanceLow;
		static float* const fDefaultTreeLoadDistance;
		static float* const fDetailTextureScale;
		static float* const fHighBlockLoadDistanceLow;
		static float* const fHighTreeLoadDistance;
		static float* const fLowBlockLoadDistanceLow;
		static float* const fLowTreeLoadDistance;
		static float* const fMorphEndDistanceMult;
		static float* const fMorphStartDistanceMult;
	};

	struct TestAllCells {
		static bool* const bFileCheckModelCollision;
		static bool* const bFileControllerOnRoot;
		static bool* const bFileGoneMessage;
		static bool* const bFileNeededMessage;
		static bool* const bFileShowIcons;
		static bool* const bFileShowTextures;
		static bool* const bFileSkipIconChecks;
		static bool* const bFileSkipModelChecks;
		static bool* const bFileTestLoad;
		static bool* const bFileUnusedObject;
	};

	struct VATS {
		static bool* const bVATSSmartCameraCheckDebug;

		static SInt32* const iDistortMaxInterval;

		static char* const sScanlineTexture;

		static float* const fBlurIntensity;
		static float* const fBlurRadius;
		static float* const fBurstDuration;
		static float* const fBurstIntensity;
		static float* const fDistortDuration;
		static float* const fDistortHorizontalScale;
		static float* const fDistortVerticalScale;
		static float* const fPulseIntensity;
		static float* const fPulseRadius;
		static float* const fPulseRate;
		static float* const fScanModeDuration;
		static float* const fScanModeScanlineFrequency;
		static float* const fScanModeVerticalScale;
		static float* const fScanlineFrequency;
		static float* const fVATSColorB;
		static float* const fVATSColorG;
		static float* const fVATSColorR;
		static float* const fVATSLightAngle;
		static float* const fVATSLightDistance;
		static float* const fVATSLightElevation;
		static float* const fVATSLightLevelMax;
		static float* const fVATSLightLevelMin;
		static float* const fVATSTargetPulseRate;
		static float* const fVatsLightColorB;
		static float* const fVatsLightColorG;
		static float* const fVatsLightColorR;
	};

	struct Voice {
		static char* const sFileTypeGame;
		static char* const sFileTypeLTF;
		static char* const sFileTypeLip;
		static char* const sFileTypeSource;
	};

	struct Water {
		static bool* const bForceHighDetailLandReflections;
		static bool* const bForceLowDetailReflections;
		static bool* const bReflectExplosions;
		static bool* const bUseBulletWaterDisplacements;
		static bool* const bUsePerWorldSpaceWaterNoise;
		static bool* const bUseWater;
		static bool* const bUseWaterHiRes;
		static bool* const bUseWaterLOD;
		static bool* const bUseWaterShader;

		static SInt32* const iWaterNoiseResolution;

		static UInt32* const uMaxExteriorWaterReflections;
		static UInt32* const uMaxInteriorWaterReflections;
		static UInt32* const uNearWaterPoints;
		static UInt32* const uNearWaterRadius;
		static UInt32* const uSurfaceFPS;

		static char* const sSurfaceTexture;

		static float* const fExteriorWaterReflectionThreshold;
		static float* const fInteriorWaterReflectionThreshold;
		static float* const fNearWaterIndoorTolerance;
		static float* const fNearWaterOutdoorTolerance;
		static float* const fNearWaterUnderwaterFreq;
		static float* const fNearWaterUnderwaterVolume;
		static float* const fRefractionWaterPlaneBias;
		static float* const fSurfaceTileSize;
		static float* const fTileTextureDivisor;
		static float* const fWadingWaterQuadSize;
		static float* const fWadingWaterTextureRes;
		static float* const fWaterGroupHeightRange;
	};

	struct Weather {
		static bool* const bPrecipitation;

		static char* const sBumpFadeColor;
		static char* const sEnvReduceColor;
		static char* const sLerpCloseColor;

		static float* const fAlphaReduce;
		static float* const fSunBaseSize;
		static float* const fSunGlareSize;
	};

	static bool* const bPrimitivesOn;
	static bool* const bUseWaterHDR;

	static SInt32* const iConcentricLengthMaxAverage;
	static SInt32* const iConcentricLengthMaxEasy;
	static SInt32* const iConcentricLengthMaxHard;
	static SInt32* const iConcentricLengthMaxVeryEasy;
	static SInt32* const iConcentricLengthMaxVeryHard;
	static SInt32* const iConcentricLengthMinAverage;
	static SInt32* const iConcentricLengthMinEasy;
	static SInt32* const iConcentricLengthMinHard;
	static SInt32* const iConcentricLengthMinVeryEasy;
	static SInt32* const iConcentricLengthMinVeryHard;
	static SInt32* const iDetectionHighNumPicks;
	static SInt32* const iLastHDRSetting;
	static SInt32* const iLowProcessingMilliseconds;
	static SInt32* const iSweetSpotLengthMaxAverage;
	static SInt32* const iSweetSpotLengthMaxEasy;
	static SInt32* const iSweetSpotLengthMaxHard;
	static SInt32* const iSweetSpotLengthMaxVeryEasy;
	static SInt32* const iSweetSpotLengthMaxVeryHard;
	static SInt32* const iSweetSpotLengthMinAverage;
	static SInt32* const iSweetSpotLengthMinEasy;
	static SInt32* const iSweetSpotLengthMinHard;
	static SInt32* const iSweetSpotLengthMinVeryEasy;
	static SInt32* const iSweetSpotLengthMinVeryHard;

	static char* const sDefaultTexAtlas;

	static float* const fAvoidanceAvoidNodeCost;
	static float* const fLowPerfCombatantVoiceDistance;
	static float* const fPC360ControllerMouseXMult;
	static float* const fPC360ControllerMouseYMult;
	static float* const fWeaponInteriorFarAttenuationMod;
	static float* const fWeaponInteriorFarVolumeMod;
	static float* const fWeaponInteriorNearAttenuationMod;
	static float* const fWeaponInteriorNearVolumeMod;

};
static_assert(sizeof(INISettingCollection) == 0x114);