#pragma once

// 0x18
template <typename T_Size>
class BSTask {
public:
	BSTask();
	enum BS_TASK_STATE {
		BS_TASK_STATE_PENDING = 0x0,
		BS_TASK_STATE_QUEUED = 0x1,
		BS_TASK_STATE_MOVING = 0x2,
		BS_TASK_STATE_RUNNING = 0x3,
		BS_TASK_STATE_FINISHED = 0x4,
		BS_TASK_STATE_COMPLETED = 0x5,
		BS_TASK_STATE_CANCELED = 0x6,
		BS_TASK_STATE_POST_PROCESSING = 0x7,
	};

	virtual			~BSTask();
	virtual void	Run();
	virtual void	AddTask();
	virtual void	Cancel(BS_TASK_STATE aeState, void* arg1);
	virtual bool	GetDescription(const char* apDescription, size_t aiBufferSize);

	BSTask* unk04;
	UInt32 uiRefCount;
	BS_TASK_STATE eState;
#pragma pack(push, 4)
	mutable T_Size iKey;
#pragma pack(pop)

	static UInt32* GetCounterSingleton();

	// 0x92C870
	void IncRefCount() {
		InterlockedIncrement(&uiRefCount);
	}

	// 0x44DD60
	void DecRefCount() {
		if (!InterlockedDecrement(&uiRefCount))
			this->~BSTask();
	}
};
static_assert(sizeof(BSTask<SInt64>) == 0x18);