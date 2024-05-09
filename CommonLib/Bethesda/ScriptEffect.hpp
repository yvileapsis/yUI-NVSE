#pragma once
#include "ActiveEffect.hpp"

class ScriptEffect : public ActiveEffect
{
public:
	ScriptEffect();
	~ScriptEffect();

	virtual void Unk_14();
	virtual void Unk_15();
	virtual void RunScriptAndDestroyEventList();

	Script* pkScript;
	ScriptEventList* pkEventList;
};
