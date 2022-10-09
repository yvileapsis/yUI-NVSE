#define yUI_VERSION 1.5
#define yUI_VERSION_STR "1.5"
#define yUI_STR "yUI"

#define yUI_LOG "yUI.log"
#define yUI_INI R"(\Data\NVSE\Plugins\yUI.ini)"

#define MO2_VFS_DLL_NAME "usvfs_x86.dll"

namespace SortingIcons { extern void Init(); }
namespace Fix::DroppedItems { extern void Init(); }
namespace Fix::TablineSelected { extern void Init(); }
namespace Fix::ReorderMCM { extern void Init(); }
namespace Patch::MatchedCursor { extern void Init(); }

inline void Inits()
{
	SortingIcons::Init();
	Fix::DroppedItems::Init();
	Fix::TablineSelected::Init();
	Fix::ReorderMCM::Init();
	Patch::MatchedCursor::Init();
}