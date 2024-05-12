#pragma once

#include "MemoryManager.hpp"

template <typename T_Data>
class BSSimpleArray {
public:
	BSSimpleArray() { InitialSetup(0, 0); }
	BSSimpleArray(UInt32 auiSize) { InitialSetup(auiSize, auiSize); }

	virtual			~BSSimpleArray() { Clear(true); };
	virtual T_Data* Allocate(UInt32 auiCount) { return MemoryManager::Allocate<T_Data>(auiCount); };
	virtual void	Deallocate(T_Data* apData) { MemoryManager::Deallocate(apData); };
	virtual T_Data* Reallocate(T_Data* apData, UInt32 auiCount) { return (T_Data*)MemoryManager::Reallocate(apData, sizeof(T_Data) * auiCount); };

	T_Data*	pBuffer;
	UInt32	uiSize;
	UInt32	uiAllocSize;

	// this only compiles for pointer types
	T_Data operator[](UInt32 idx) {
		return pBuffer[idx];
	}

	UInt32 GetSize() { return uiSize; }
	UInt32 GetAllocSize() { return uiAllocSize; }
	bool IsEmpty() { return uiSize == 0; }
	bool IsFull() { return uiSize == uiAllocSize; }
	T_Data* GetAt(UInt32 idx) { return &pBuffer[idx]; }
	T_Data* GetLast() { return &pBuffer[uiSize - 1]; }

	void ConstructDefaultItems(UInt32 auiCount) {
		for (UInt32 i = 0; i < auiCount; ++i)
			new (&pBuffer[i]) T_Data;
	}

	// 0x822780
	void ConstructItems(T_Data* apData, UInt32 auiCount) {
		for (UInt32 i = 0; i < auiCount; i++)
			new (&apData[i]) T_Data;
	}

	// 0x822820
	void DestructItems(T_Data* apData, UInt32 auiCount) {
		for (UInt32 i = 0; i < auiCount; ++i)
			apData[i].~T_Data();
	}

	void InitialSetup(UInt32 auiSize, UInt32 auiAllocSize) {
		pBuffer = nullptr;
		uiSize = 0;
		uiAllocSize = 0;
		if (auiSize > auiAllocSize)
			auiAllocSize = auiSize;

		if (auiAllocSize) {
			pBuffer = Allocate(auiAllocSize);
			uiAllocSize = auiAllocSize;
		}

		if (auiAllocSize) {
			ConstructDefaultItems(auiAllocSize);
			uiSize = auiAllocSize;
		}
	}

	// 0x6A8500
	void FreeArray() {
		Deallocate(pBuffer);
		pBuffer = nullptr;
	}

	// 0x6C6200
	void Clear(bool abFree) {
		if (!pBuffer)
			return;

		DestructItems(pBuffer, uiSize);

		if (abFree) {
			FreeArray();
			pBuffer = nullptr;
			uiAllocSize = 0;
		}

		uiSize = 0;
	}

	void CopyItems(T_Data* apNewBuffer, T_Data* apOldBuffer, UInt32 auiSize) {
		if (!auiSize)
			return;

		if (apNewBuffer >= apOldBuffer) {
			if (apNewBuffer > apOldBuffer) {
				for (UInt32 i = auiSize - 1; i >= 0; --i)
					memcpy(&apNewBuffer[i], &apOldBuffer[i], sizeof(T_Data));
			}
		}
		else {
			for (UInt32 j = 0; j < auiSize; ++j)
				memcpy(&apNewBuffer[j], &apOldBuffer[j], sizeof(T_Data));
		}
	}

	// 0x42FC00
	void ReallocateBuffer(UInt32 auiNewSize, UInt32 auiSize) {
		if (!pBuffer) {
			pBuffer = Allocate(auiNewSize);
			uiAllocSize = auiNewSize;
		}
		else if (auiSize == uiAllocSize) {
			pBuffer = Reallocate(pBuffer, auiNewSize);
		}
		else {
			T_Data* pNewBuffer = Allocate(auiNewSize);
			CopyItems(pNewBuffer, pBuffer, auiSize);
			FreeArray();
			pBuffer = pNewBuffer;
		}
	}

	// 0x695230
	void SetSize(UInt32 auiSize, bool abFree) {
		if (!auiSize) {
			Clear(abFree);
			return;
		}

		if (auiSize > uiAllocSize) {
			if (uiAllocSize)
				ReallocateBuffer(auiSize, uiSize);
			else
				pBuffer = Allocate(auiSize);

			uiAllocSize = auiSize;
			DestructItems(&pBuffer[uiSize], auiSize - uiSize);
		}
		else if (auiSize >= uiSize) {
			ConstructItems(&pBuffer[uiSize], auiSize - uiSize);
		}
		else {
			DestructItems(&pBuffer[auiSize], uiSize - auiSize);

			if (abFree && auiSize <= uiAllocSize >> 2) {
				ReallocateBuffer(auiSize, auiSize);
				uiAllocSize = auiSize;
			}
		}
		uiSize = auiSize;
	}

	// 0x6F9790
	void SetAllocSize(UInt32 auiAllocSize) {
		if (auiAllocSize == uiAllocSize)
			return;

		if (auiAllocSize < uiSize) {
			uiSize = auiAllocSize;
		}
		ReallocateBuffer(auiAllocSize, uiSize);
		uiAllocSize = auiAllocSize;
	}

	// 0x9A3910
	UInt32 GetResizeSize() {
		if (uiAllocSize <= 1024)
			return 2 * uiAllocSize;
		else
			return uiAllocSize + 1024;
	}

	// 0x761540
	UInt32 GetNextIndex() {
		if (!IsFull())
			return uiSize++;

		if (uiAllocSize) {
			UInt32 a2 = GetResizeSize();
			ReallocateBuffer(a2, uiSize);
			uiAllocSize = a2;
		}
		else {
			pBuffer = Allocate(4);
			uiAllocSize = 4;
		}

		return uiSize++;
	}

	// 0x726BF0
	void AddNewSlots(T_Data* apBuffer, UInt32 auiCount) {

	}

	// 0x7CB2E0
	void Append(T_Data& aItem) {
		UInt32 uiNext = GetNextIndex();
		pBuffer[uiNext] = aItem;
	}
};

ASSERT_SIZE(BSSimpleArray<UInt32>, 0x10);