#include <events.h>
#include <GameAPI.h>

#include <GameObjects.h>

void EventLayer()
{
	CallFunctionAlt(CompileExpression(R"(SetOnHitEventHandler ({} => (this).DispatchEventAlt "yJAM:JIP:OnHit") 1)"), nullptr, 0);
}
