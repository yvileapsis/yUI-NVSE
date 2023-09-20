#pragma once

namespace CrashLogger
{
	void ApplyNVSE();
	void ApplyFOSE();
	void ApplyOBSE();
	namespace VirtualTables
	{
		void FillNVSELabels();
		void FillFOSELabels();
		void FillOBSELabels();
	}
}