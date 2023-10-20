#include <definitions.hpp>

#define INIT_MODULE(mod) namespace mod { extern void Init(); }

INIT_MODULE(CrashLogger)
INIT_MODULE(Patch::NameThreads)

void Inits()
{
	CrashLogger::Init();
	Patch::NameThreads::Init();
}