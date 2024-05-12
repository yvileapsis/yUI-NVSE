#pragma once

class MagicCaster {
public:
	MagicCaster();
	~MagicCaster();

	UInt32 vtabl;
	UInt32 unk004;	// 004
	UInt32 unk008;	// 008
};

ASSERT_SIZE(MagicCaster, 0x00C);