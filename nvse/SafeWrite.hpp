#pragma once
#include <prefix.hpp>

// These are used for 10h aligning segments in ASM code (massive performance gain, particularly with loops).
#define EMIT(bt) __asm _emit bt
#define NOP_0x1 EMIT(0x90)
#define NOP_0x2 EMIT(0x66) EMIT(0x90)
#define NOP_0x3 EMIT(0x0F) EMIT(0x1F) EMIT(0x00)
#define NOP_0x4 EMIT(0x0F) EMIT(0x1F) EMIT(0x40) EMIT(0x00)
#define NOP_0x5 EMIT(0x0F) EMIT(0x1F) EMIT(0x44) EMIT(0x00) EMIT(0x00)
#define NOP_0x6 EMIT(0x66) EMIT(0x0F) EMIT(0x1F) EMIT(0x44) EMIT(0x00) EMIT(0x00)
#define NOP_0x7 EMIT(0x0F) EMIT(0x1F) EMIT(0x80) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00)
#define NOP_0x8 EMIT(0x0F) EMIT(0x1F) EMIT(0x84) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00)
#define NOP_0x9 EMIT(0x66) EMIT(0x0F) EMIT(0x1F) EMIT(0x84) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00)
#define NOP_0xA NOP_0x5 NOP_0x5
#define NOP_0xB NOP_0x5 NOP_0x6
#define NOP_0xC NOP_0x6 NOP_0x6
#define NOP_0xD NOP_0x6 NOP_0x7
#define NOP_0xE NOP_0x7 NOP_0x7
#define NOP_0xF NOP_0x7 NOP_0x8

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx

void SafeWrite8(UInt32 addr, UInt32 data);
void SafeWrite16(UInt32 addr, UInt32 data);
void SafeWrite32(UInt32 addr, UInt32 data);
void SafeWriteBuf(UInt32 addr, const char* data, UInt32 len);

// 5 bytes
void WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt);
void WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt);
//
void WriteVirtualCall(UInt32 jumpSrc, UInt32 jumpTgt);

template <typename T> void WriteRelJump(UInt32 jumpSrc, T jumpTgt) { WriteRelJump(jumpSrc, UInt32(jumpTgt)); }
template <typename T> void WriteRelCall(UInt32 jumpSrc, T jumpTgt) { WriteRelCall(jumpSrc, UInt32(jumpTgt)); }
template <typename T> void WriteVirtualCall(UInt32 jumpSrc, T jumpTgt) { WriteVirtualCall(jumpSrc, UInt32(jumpTgt)); }

void UndoSafeWrite(UInt32 addr);

// 6 bytes
void WriteRelJnz(UInt32 jumpSrc, UInt32 jumpTgt);
void WriteRelJle(UInt32 jumpSrc, UInt32 jumpTgt);

void PatchMemoryNop(ULONG_PTR Address, SIZE_T Size);

UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst);
UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt);