#pragma once

__declspec(noinline) void __stdcall SafeWrite8(UInt32 addr, UInt32 data);
__declspec(noinline) void __stdcall SafeWrite16(UInt32 addr, UInt32 data);
__declspec(noinline) void __stdcall SafeWrite32(UInt32 addr, UInt32 data);
__declspec(noinline) void __stdcall SafeWriteBuf(UInt32 addr, void * data, UInt32 len);
__declspec(noinline) UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst);
__declspec(noinline) UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt);

// 5 bytes
__declspec(noinline) void __stdcall WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt);
__declspec(noinline) void __stdcall WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt);
void NopFunctionCall(UInt32 addr, UInt32 numArgs);
void NopFunctionCall(UInt32 addr);
void NopIndirectCall(UInt32 addr);
void NopIndirectCall(UInt32 addr, UInt32 numArgs);


// 6 bytes
__declspec(noinline) void WriteRelJne(UInt32 jumpSrc, UInt32 jumpTgt);
__declspec(noinline) void WriteRelJle(UInt32 jumpSrc, UInt32 jumpTgt);
__declspec(noinline) void WriteRelJe(UInt32 jumpSrc, UInt32 jumpTgt);

// replace a memory range (inclusive) with NOP
#define NopMemoryRange(start, end) PatchMemoryNop(start, end-start + 1)

void PatchMemoryNop(ULONG_PTR Address, SIZE_T Size);
void PatchMemoryInt3(ULONG_PTR Address, SIZE_T Size);

UInt32 __stdcall GetRelJumpAddr(UInt32 jumpSrc);
