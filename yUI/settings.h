#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

inline int g_logLevel				= 0;

inline int g_FixIndefiniteSorting	= 1;
inline int g_FixDroppedItems		= 1;
inline int g_FixTablineSelected		= 1;
inline int g_FixReorderMCM			= 1;

inline int g_ySI					= 0;

inline int g_ySI_Sort				= 1;
inline int g_ySI_Icons				= 1;
inline int g_ySI_Hotkeys			= 1;
inline int g_ySI_Categories			= 1;
inline int g_ySI_JustInTime			= 1;

inline int g_yCM					= 0;

inline int g_yMC					= 0;


//extern HMODULE yUIHandle;
void handleINIOptions();