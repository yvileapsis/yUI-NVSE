#pragma once

#include "BipedAnim.hpp"
#include "QueuedFile.hpp"

class Actor;

class QueuedHelmet : public QueuedFile {
public:
	QueuedHelmet();
	virtual ~QueuedHelmet();

	BipedAnim*		pAnim;
	NiRefObjectPtr	spObjects2C[20];
	NiRefObjectPtr	spObjects7C[20];
	NiRefObjectPtr	spObjectsCC[20];
	Actor*			pActor;
	char			unk120;
	int				unk124;
};

ASSERT_SIZE(QueuedHelmet, 0x128)