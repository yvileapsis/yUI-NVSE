#include <definitions.h>

#define INIT_MODULE(mod) namespace mod { extern void Init(); }

INIT_MODULE(SortingIcons)

INIT_MODULE(Fix::DroppedItems)
INIT_MODULE(Fix::TablineSelected)
INIT_MODULE(Fix::ReorderMCM)
INIT_MODULE(Fix::ProjectileEffects)

INIT_MODULE(Patch::RestoreFO3Spread)
INIT_MODULE(Patch::ArmedUnarmed)
INIT_MODULE(Patch::ExplosionForce)
INIT_MODULE(Patch::MatchedCursor)
INIT_MODULE(Patch::MultiplicativeShots)
INIT_MODULE(Patch::TimeMult)

void Inits()
{
	SortingIcons::Init();

	Fix::DroppedItems::Init();
	Fix::TablineSelected::Init();
	Fix::ReorderMCM::Init();
	Fix::ProjectileEffects::Init();

	Patch::RestoreFO3Spread::Init();
	Patch::ArmedUnarmed::Init();
	Patch::ExplosionForce::Init();
	Patch::MatchedCursor::Init();
	Patch::MultiplicativeShots::Init();
	Patch::TimeMult::Init();

}