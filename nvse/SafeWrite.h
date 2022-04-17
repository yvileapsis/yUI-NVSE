#pragma once
#include <prefix.h>
#include <unordered_map>
#include <vector>
#include <span>

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx

inline std::unordered_map <UInt32, std::vector<std::byte>> g_SafeWriteData;

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