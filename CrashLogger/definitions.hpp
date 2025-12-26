#define CrashLogger_VERSION 5.15
#define CrashLogger_VERSION_STR "5.15"
#define CrashLogger_STR "CrashLogger"
#define CrashLogger_LOG "CrashLogger.log"
#define CrashLogger_FLD "Crash Logs"
#define CrashLogger_INI R"(\Data\NVSE\Plugins\CrashLogger.ini)"

#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

void Inits();
void InitEarlyPatches();
void InitPatches();
