#pragma once

#include "IOTask.hpp"
#include "BSSimpleArray.hpp"
#include "NiSmartPointer.hpp"

class QueuedReference;
class QueuedChildren;
class QueuedParents;
class NiObject;

NiSmartPointer(QueuedFile);

class QueuedFile : public IOTask {
public:
	QueuedFile();
	virtual ~QueuedFile();

	virtual void DoRequestHandles();
	virtual void DoOnChildrenFinished(UInt32 arg0);
	virtual void DoOnTaskFinished();

	UInt32				eContext;
	QueuedReference*	spParent;
	QueuedChildren*		pChildren;
	QueuedParents*		pParents;
};

ASSERT_SIZE(QueuedFile, 0x28);

class QueuedChildren : public BSSimpleArray<NiPointer<QueuedFile>> {
public:
	UInt32	uiCounter;
};