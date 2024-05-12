#pragma once

struct MobileObjectMessage
{
	UInt32 unk00;
	UInt32 unk04;
	UInt32 unk08;
	UInt32 unk0C;
};

ASSERT_SIZE(MobileObjectMessage, 0x10);