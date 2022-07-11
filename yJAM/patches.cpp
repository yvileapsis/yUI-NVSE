#include <patches.h>
#include <SafeWrite.h>
#include <functions.h>

void patchSortingTabs(const bool bEnable)
{
	WriteRelCall(0x000000, 0x000000);
}