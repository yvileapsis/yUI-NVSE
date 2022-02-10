#include <timemult.h>
#include <GameAPI.h>

void SetGlobalTimeMultiplier(TimeGlobal* timeGlobal, char isImmediateChange)
{
	const auto timeMult = globalTimeMultMin * globalTimeMultMax;
	timeGlobal->Set(timeMult, isImmediateChange);
}

void Time_ModifyMap(const float timeMult, const UInt16 mod) {

	if (timeMult < -1) {}
	else if (timeMult == -1) {
		map_yGTM_LocalTimeMultsMin.clear();
		map_yGTM_LocalTimeMultsMax.clear();
		globalTimeMultMin = 1.0;
		globalTimeMultMax = 1.0;
	}
	else if (timeMult <= 0) {}
	else if (timeMult < 1.0) {
		map_yGTM_LocalTimeMultsMin[mod] = timeMult;
		if (const auto it = map_yGTM_LocalTimeMultsMax.find(mod); it != map_yGTM_LocalTimeMultsMax.end()) {
			map_yGTM_LocalTimeMultsMax.erase(it);
			globalTimeMultMax = 1.0;
			for (const auto iter : map_yGTM_LocalTimeMultsMax)
				if (globalTimeMultMax < iter.second) globalTimeMultMax = iter.second;
		}

		globalTimeMultMin = 1.0;
		for (const auto iter : map_yGTM_LocalTimeMultsMin)
			if (globalTimeMultMin > iter.second) globalTimeMultMin = iter.second;
	}
	else if (timeMult > 1.0) {
		map_yGTM_LocalTimeMultsMax[mod] = timeMult;
		if (const auto it = map_yGTM_LocalTimeMultsMin.find(mod); it != map_yGTM_LocalTimeMultsMin.end()) {
			map_yGTM_LocalTimeMultsMin.erase(it);
			globalTimeMultMin = 1.0;
			for (const auto iter : map_yGTM_LocalTimeMultsMin)
				if (globalTimeMultMin > iter.second) globalTimeMultMin = iter.second;
		}

		globalTimeMultMax = 1.0;
		for (const auto iter : map_yGTM_LocalTimeMultsMax)
			if (globalTimeMultMax < iter.second) globalTimeMultMax = iter.second;
	}
	else {
		globalTimeMultMin = globalTimeMultMax = 1.0;

		auto it = map_yGTM_LocalTimeMultsMin.find(mod);
		if (it != map_yGTM_LocalTimeMultsMin.end()) map_yGTM_LocalTimeMultsMin.erase(it);

		for (const auto iter : map_yGTM_LocalTimeMultsMin)
			if (globalTimeMultMin > iter.second) globalTimeMultMin = iter.second;

		it = map_yGTM_LocalTimeMultsMax.find(mod);
		if (it != map_yGTM_LocalTimeMultsMax.end()) map_yGTM_LocalTimeMultsMax.erase(it);

		for (const auto iter : map_yGTM_LocalTimeMultsMax)
			if (globalTimeMultMax < iter.second) globalTimeMultMax = iter.second;
	}
}