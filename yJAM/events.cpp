#include <events.h>
#include <GameAPI.h>

#include <GameObjects.h>

#include "PluginAPI.h"

void EventLayer()
{
	RegisterEvent("yJAM:MainLoop", 0, nullptr, 0);

	RegisterEvent("yJAM:JG:OnRender", 0, nullptr, NVSEEventManagerInterface::kFlag_IsUserDefined);
	auto expression = CompileExpression(R"(SetOnRenderUpdateEventHandler 1 ({} => DispatchEventAlt "yJAM:JG:OnRender") 4)");
	if (!expression) Log("MISSING REQUIREMENTS - JG WAAAAAAAAAAAAA");
	CallFunctionAlt(expression, nullptr, 0);

	RegisterEvent("yJAM:JIP:OnHit", 0, nullptr, NVSEEventManagerInterface::kFlag_IsUserDefined);
	expression = CompileExpression(R"(SetOnHitEventHandler ({} => (this).DispatchEventAlt "yJAM:JIP:OnHit") 1)");
	if (!expression) Log("MISSING REQUIREMENTS - JIP LN WAAAAAAAAAAAAA");
	CallFunctionAlt(expression, nullptr, 0);

//	CallFunctionAlt(expression, nullptr, 0);
}
