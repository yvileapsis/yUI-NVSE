#pragma once

typedef bool(__cdecl* BSParallelTaskCallback)(void*, void*);

struct BSParallelTask {
	BSParallelTask(BSParallelTaskCallback apFunc, void* apArray, UInt32 auiSize);

	BSParallelTaskCallback	pCallBack;

	struct DataType {
		union {
			void* pData;
			UInt32 uiData;
			float fData;
			const char* pcData;
		};
	};

	DataType kData[3];
};

ASSERT_SIZE(BSParallelTask, 0x10);