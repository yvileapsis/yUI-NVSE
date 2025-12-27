#pragma once

class Script;
class ScriptLocals;
class TESObjectREFR;
class TESObject;
class TESTopicInfo;
class TESGlobal;

class ScriptRunner {
public:
	enum StackFlags {
		IF		= 1u << 0,
		ELSEIF	= 1u << 1,
	};

	TESObjectREFR*  pCurrentContainer;
	TESObject*		pCurrentObject;
	ScriptLocals*	pCurrentVars;
	TESTopicInfo*	pCurrentInfo;
	TESGlobal*		pGlobal;
	Script*			pRunningScript;
	uint32_t		eError;
	uint32_t		eExpressionError;
	uint32_t		uiIfNextDepth;
	uint32_t		uiIfFlags[10];
	uint32_t		uiWhileNestDepth;
	uint32_t		uiWhileFlags[10];
	uint32_t		uiWhileOffset[10];
	bool			bScriptErrorFound;
	bool			bStopObjectScriptExecution;
};

ASSERT_SIZE(ScriptRunner, 0xA4);