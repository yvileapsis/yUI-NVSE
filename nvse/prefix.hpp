#pragma once
// 4018 - signed/unsigned mismatch
// 4244 - loss of data by assignment
// 4267 - possible loss of data (truncation)
// 4305 - truncation by assignment
// 4288 - disable warning for crap microsoft extension screwing up the scope of variables defined in for loops
// 4311 - pointer truncation
// 4312 - pointer extension
// 4584 - many Bethesda classes double-include some base-classes
// credits to Ian Patterson
#pragma warning(disable: 4018 4244 4267 4305 4288 4312 4311 4584 26812)
#include "CommonPrefix.hpp"

#ifndef __NVSE_VERSION_H__
#define __NVSE_VERSION_H__

// these have to be macros so they can be used in the .rc
#define NVSE_VERSION_INTEGER		6
#define NVSE_VERSION_INTEGER_MINOR	3
#define NVSE_VERSION_INTEGER_BETA	0
#define NVSE_VERSION_VERSTRING		"0, 6, 3, 0"
#define NVSE_VERSION_PADDEDSTRING	"0006"

// build numbers do not appear to follow the same format as with oblivion
#define MAKE_NEW_VEGAS_VERSION_EX(major, minor, build, sub)	(((major & 0xFF) << 24) | ((minor & 0xFF) << 16) | ((build & 0xFFF) << 4) | (sub & 0xF))
#define MAKE_NEW_VEGAS_VERSION(major, minor, build)			MAKE_NEW_VEGAS_VERSION_EX(major, minor, build, 0)

// assume the major version number is 1.x
#define RUNTIME_VERSION_1_0_0_240	MAKE_NEW_VEGAS_VERSION(0, 0, 240)		// 0x00000F00
#define RUNTIME_VERSION_1_4_0_525	MAKE_NEW_VEGAS_VERSION(4, 0, 525)		// 0x040020D0

#define OBLIVION_VERSION_1_2_416	MAKE_NEW_VEGAS_VERSION(1, 2, 416)		// 0x010201A0

#define CS_VERSION_1_1_0_262		MAKE_NEW_VEGAS_VERSION(1, 0, 262)		// 0x01001060
#define CS_VERSION_1_4_0_518		MAKE_NEW_VEGAS_VERSION(4, 0, 518)		// 0x04002060

#define PACKED_NVSE_VERSION			MAKE_NEW_VEGAS_VERSION(NVSE_VERSION_INTEGER, NVSE_VERSION_INTEGER_MINOR, NVSE_VERSION_INTEGER_BETA)

#endif /* __NVSE_VERSION_H__ */

#define VERSION_CODE(primary, secondary, sub)	(((primary & 0xFFF) << 20) | ((secondary & 0xFFF) << 8) | ((sub & 0xFF) << 0))
#define VERSION_CODE_PRIMARY(in)				((in >> 20) & 0xFFF)
#define VERSION_CODE_SECONDARY(in)				((in >> 8) & 0xFFF)
#define VERSION_CODE_SUB(in)					((in >> 0) & 0xFF)

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <type_traits>
#include <chrono>

#include <ranges>
#include <string>
#include <vector>
#include <span>
#include <unordered_set>
#include <unordered_map>

#include <functional>

#include "classes.hpp"

#include "Utilities.hpp"