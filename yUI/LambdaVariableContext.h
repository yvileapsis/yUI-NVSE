#pragma once
#include <deque>
#include <functional>
extern std::deque<std::function<void()>> g_executionQueue;

class Script;

typedef void (*_CaptureLambdaVars)(Script* scriptLambda);
typedef void (*_UncaptureLambdaVars)(Script* scriptLambda);

_CaptureLambdaVars CaptureLambdaVars;
_UncaptureLambdaVars UncaptureLambdaVars;

class LambdaVariableContext
{
	Script* scriptLambda;
public:
	LambdaVariableContext(const LambdaVariableContext& other) = delete;
	LambdaVariableContext& operator=(const LambdaVariableContext& other) = delete;
	LambdaVariableContext(Script* scriptLambda);
	LambdaVariableContext(LambdaVariableContext&& other) noexcept;
	LambdaVariableContext& operator=(LambdaVariableContext&& other) noexcept;
	~LambdaVariableContext();

	Script*& operator*();
};