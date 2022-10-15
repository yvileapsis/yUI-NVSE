#include <definitions.h>

#define INIT_MODULE(mod) namespace mod { extern void Init(); }

INIT_MODULE(SortingIcons)

INIT_MODULE(Fix::DroppedItems)
INIT_MODULE(Fix::TablineSelected)
INIT_MODULE(Fix::ReorderMCM)

INIT_MODULE(Patch::MatchedCursor)
INIT_MODULE(Patch::TimeMult)

void Inits()
{
	SortingIcons::Init();

	Fix::DroppedItems::Init();
	Fix::TablineSelected::Init();
	Fix::ReorderMCM::Init();

	Patch::MatchedCursor::Init();
	Patch::TimeMult::Init();
}