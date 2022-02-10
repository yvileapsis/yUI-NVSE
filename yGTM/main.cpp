#pragma warning( disable : 4996 4800 4102 4100 4725 4201 4610 4510)
#include <CommandTable.h>
#include <main.h>

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

static ParamInfo kParams_OneOptionalInt[1] =
{
	{	"int", kParamType_Integer, 1 },
};

static ParamInfo kParams_OneOptionalFloat_OneOptionalInt[2] =
{
	{	"float", kParamType_Float, 1 },
	{	"int", kParamType_Integer, 1 },
};

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	gLog.Create("yGTM.log");
	PrintLog("yGTM: Query");

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yGTM";
	info->version = 2;

	if (nvse->isNogore)
	{
		PrintLog("NoGore is not supported");
		return false;
	}

	return true;
}

typedef void (*_HasScriptCommand)(Script* script, CommandInfo* info, CommandInfo* eventBlock);
_HasScriptCommand HasScriptCommand;

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{
	g_pluginHandle = nvse->GetPluginHandle();

	g_nvseInterface = const_cast<NVSEInterface*>(nvse);

	auto* cmdTable = static_cast<NVSECommandTableInterface*>(nvse->QueryInterface(kInterface_CommandTable));

	CommandInfo* cmdInfo = cmdTable->GetByOpcode(0x22B0);
	cmdInfo->numParams = 1;
	cmdInfo->params = kParams_OneOptionalInt;

	cmdInfo = cmdTable->GetByOpcode(0x1186);
	cmdInfo->numParams = 2;
	cmdInfo->params = kParams_OneOptionalFloat_OneOptionalInt;
	
	if (nvse->isEditor)
	{
		return true;
	}

	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));
	HasScriptCommand = static_cast<_HasScriptCommand>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaSaveVariableList));
	
	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
	ExtractArgsEx = g_scriptInterface->ExtractArgsEx;
	
	cmdInfo = cmdTable->GetByOpcode(0x22B0);
	cmdInfo->execute = Cmd_GetGlobalTimeMultiplierAlt_Execute;

	cmdInfo = cmdTable->GetByOpcode(0x1186);
	cmdInfo->execute = Cmd_SetGlobalTimeMultiplierAlt_Execute;

	writePatch();

	return true;
}