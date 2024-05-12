#pragma once

class BSTaskletGroupData {
public:
	virtual ~BSTaskletGroupData();
	virtual void WaitForCompletion(bool);
	virtual void SetMaxConcurrent(UInt32);
};