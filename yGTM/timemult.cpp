#include <timeMult.h>
#include <GameAPI.h>
#include <GameData.h>
#include <settings.h>

extern DataHandler* g_dataHandler;

namespace TimeMult
{
	void FillMaps()
	{
		for (auto iter = g_dataHandler->scriptList.Begin(); !iter.End(); ++iter)
		{
			if (g_specialMods.contains(iter.Get()->modIndex)) return;
			if (HasScriptCommand(iter.Get(), GetByOpcode(0x1186), nullptr)) g_specialMods.emplace(iter.Get()->modIndex);
		}
	}

	void Set(TimeGlobal* timeGlobal, char isImmediateChange)
	{
		timeGlobal->Set(g_timeMult, isImmediateChange);
	}

	void ModifyMap(const Float64 timeMult, const UInt16 mod)
	{
		g_localMults.erase(mod);
		if (timeMult != 1) g_localMults.emplace(mod, timeMult);
		if (timeMult == 0) g_localMults.clear();

		g_timeMult = 1;

		if (g_yTM_MinMax == 0)
			for (const auto it : g_localMults) g_timeMult *= it.second;
		else if (g_yTM_MinMax == 1) {
			Float64 min, max;
			min = max = 1.0;
			for (const auto it : g_localMults) {
				if (it.second < min) min = it.second;
				if (it.second > max) max = it.second;
			}
			g_timeMult = min * max;
		}
	}
}