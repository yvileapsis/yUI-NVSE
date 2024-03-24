#pragma once

#include "InterfacedClass.hpp"
#include "BSSpinLock.hpp"
#include "BSMemObject.hpp"

// 40
template<typename T_Key, typename T_Data>
class LockFreeMap : public InterfacedClass {
public:
	// 0C
	struct Data004 {
		UInt32	unk000;		// 00
		UInt32	unk004;		// 04
		UInt32* unk008;	// 08
	};

	// 24
	struct TLSValue {
		LockFreeMap*	pMap;				// 00
		UInt32			mapData004Unk000;	// 04
		UInt32			mapData004Unk008;	// 08
		UInt32*			mapData004Unk00C;	// 0C	stores first DWord of bucket during lookup, next pointer is data, next flags bit 0 is status ok/found
		UInt32			unk010;				// 10	stores bucketOffset during lookup
		UInt32*			unk014;			// 14	stores pointer to bucket during lookup
		UInt32*			unk018;			// 18
		UInt32			unk01C;			// 1C
		UInt32			unk020;			// 20
	};

	// 10
	struct Data014 {
		// 08
		struct Data008 {
			UInt32		uiThreadID;	// 00 threadID
			TLSValue*	pTLSValue;	// 04 lpTlsValue obtained from AllocateTLSValue of LockFreeMap
		};

		UInt32		uiAlloc;		// Init'd to arg0, count of array at 008
		UInt32		uiTLSDataIndex;	// Init'd by TlsAlloc
		Data008*	ppDat008;		// array of pair of DWord
		UInt32		uiCount;		// Init'd to 0
	};	// most likely an array or a map

	virtual bool	Lookup(T_Key key, T_Data* destination);
	virtual void	Unk_03(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt8 arg4);
	virtual bool	Insert(T_Key key, T_Data& dataPtr, UInt8 arg3);
	virtual void	EraseKey(T_Key key);
	virtual void	Unk_06(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_07(UInt32 arg); // Calls 06 for every item
	virtual bool	Unk_08(UInt32 arg1, UInt32 arg2);
	virtual UInt32	CalcBucketIndex(T_Key key);
	virtual void	FreeKey(T_Key key);
	virtual T_Key	GenerateKey(T_Key src);
	virtual UInt32	CopyKeyTo(T_Key src, T_Key& destPtr);
	virtual bool	LKeyGreaterOrEqual(T_Key lkey, T_Key rkey);
	virtual bool	KeysEqual(T_Key lkey, T_Key rkey);
	virtual UInt32	IncQueuedCount();
	virtual UInt32	DecQueuedCount();
	virtual UInt32	GetQueuedCount();

	Data004**	dat004;		// 04 array of arg0 12 bytes elements (uninitialized)
	UInt32		uiSize;	// 08 Init'd to arg1, count of DWord to allocate in array at 000C
	UInt32**	ppBuckets;		// 0C array of arg1 DWord elements
	UInt32		unk010;			// 10 Init'd to arg2
	Data014*	dat014;		// 14 Init'd to a 16 bytes structure
	UInt32		uiQueuedCount;			// 18
	UInt32		unk01C;			// 1C
	BSSpinLock	kLock;

	class LockFreeMapIterator : public BSMemObject {
	public:
		LockFreeMapIterator() : uiIndex(0) {};
		virtual ~LockFreeMapIterator() {};
		virtual void ClearKey() { Key = T_Key(0); };

		enum StateFlags {
			STATE_UNK_1 = 1,
			STATE_FINISHED = 2,
		};

		UInt32		uiIndex;
		T_Key		Key;
		Bitfield8	ucFlags;

		bool IsFinished() const { return ucFlags.IsSet(STATE_FINISHED); };
	};

};

template<class T_Data>
class LockFreeStringMap : public LockFreeMap<const char*, T_Data> {
public:
	class LockFreeStringMapIterator : public LockFreeMap<const char*, T_Data>::LockFreeMapIterator {
	public:
		LockFreeStringMapIterator() : LockFreeMap<const char*, T_Data>::LockFreeMapIterator() {
			//Key = &cBuffer;
			cBuffer[0] = 0;
		};
		virtual ~LockFreeStringMapIterator() {};
		virtual void ClearKey() override { cBuffer[0] = 0; };

		char cBuffer[256];
	};
};

template<class T_Data>
class LockFreeCaseInsensitiveStringMap : public LockFreeStringMap<T_Data> {};

STATIC_ASSERT(sizeof(LockFreeMap<char const*, void*>) == 0x40);