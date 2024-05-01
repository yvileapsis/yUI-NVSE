#pragma once

#include "QueuedFileEntry.hpp"
#include "NiSourceTexture.hpp"

class QueuedTexture : public QueuedFileEntry {
public:
	QueuedTexture();
	~QueuedTexture();

	enum Flags {
		UPGRADE_TO_HIRES = 1 << 1,
	};

	NiSourceTexturePtr	spTexture;
	Bitfield8			ucFlags;
};

ASSERT_SIZE(QueuedTexture, 0x38)