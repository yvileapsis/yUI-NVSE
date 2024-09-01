#include "TLSData.hpp"

struct _TEB {
	UInt32 padding[0x2C / 4];
	struct TLSData** ThreadLocalStoragePointer;
};

TLSData* TLSData::GetTLSData() {
	return NtCurrentTeb()->ThreadLocalStoragePointer[*(UInt32*)0x126FD98];
}

UInt32 TLSData::GetHeapIndex() {
	return GetTLSData()->uiHeapIndex;
}

UInt32 TLSData::GetAccumulatorCount() {
	return GetTLSData()->uiAccumulatorCount;
}

void TLSData::SetHeapIndex(UInt32 index) {
	GetTLSData()->uiHeapIndex = index;
}