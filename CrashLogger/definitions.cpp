#include <definitions.h>

#define INIT_MODULE(mod) namespace mod { extern void Init(); }

INIT_MODULE(Patch::NameThreads)

void Inits()
{
	Patch::NameThreads::Init();
}