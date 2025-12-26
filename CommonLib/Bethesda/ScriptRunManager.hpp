#pragma once

class ScriptRunner;

class ScriptRunManager {
public:
	ScriptRunner* pScriptRunner;
	ScriptRunner* pCurrentRunner;

	static ScriptRunManager* GetSingleton();
};
