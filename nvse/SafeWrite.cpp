#include <Safewrite.hpp>

#include <unordered_map>
#include <vector>

std::unordered_map<UInt32, std::vector<std::byte>> originalData;

void SafeWrite8(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect(reinterpret_cast<void*>(addr), 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<UInt8*>(addr) = data;
	VirtualProtect(reinterpret_cast<void*>(addr), 4, oldProtect, &oldProtect);
}

void SafeWrite16(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect(reinterpret_cast<void*>(addr), 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<UInt16*>(addr) = data;
	VirtualProtect(reinterpret_cast<void*>(addr), 4, oldProtect, &oldProtect);
}

void SafeWrite32(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect(reinterpret_cast<void*>(addr), 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<UInt32*>(addr) = data;
	VirtualProtect(reinterpret_cast<void*>(addr), 4, oldProtect, &oldProtect);
}

void SafeWriteBuf(UInt32 addr, const char* data, UInt32 len)
{
	UInt32	oldProtect;

	auto patch = std::vector(reinterpret_cast<std::byte*>(addr), reinterpret_cast<std::byte*>(addr) + len);
	if (!originalData.contains(addr)) originalData.emplace(addr, patch);

	VirtualProtect(reinterpret_cast<void*>(addr), len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(reinterpret_cast<void*>(addr), data, len);
	VirtualProtect(reinterpret_cast<void*>(addr), len, oldProtect, &oldProtect);
}

void WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jmp rel32
	auto patch = std::vector(reinterpret_cast<std::byte*>(jumpSrc), reinterpret_cast<std::byte*>(jumpSrc) + sizeof jumpTgt + 1);
	if (!originalData.contains(jumpSrc)) originalData.emplace(jumpSrc, patch);
	
	SafeWrite8(jumpSrc, 0xE9);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// call rel32
	auto patch = std::vector(reinterpret_cast<std::byte*>(jumpSrc), reinterpret_cast<std::byte*>(jumpSrc) + sizeof jumpTgt + 1);
	if (!originalData.contains(jumpSrc)) originalData.emplace(jumpSrc, patch);
	
	SafeWrite8(jumpSrc, 0xE8);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void WriteVirtualCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	SafeWrite8(jumpSrc - 6, 0xB8); // mov eax
	SafeWrite32(jumpSrc - 5, jumpTgt);
	SafeWrite8(jumpSrc - 1, 0x90); // nop
}

void WriteRelJnz(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jnz rel32
	SafeWrite16(jumpSrc, 0x850F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

void WriteRelJle(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jle rel32
	SafeWrite16(jumpSrc, 0x8E0F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

void UndoSafeWrite(UInt32 addr)
{
	if (!originalData.contains(addr)) return;
	UInt32	oldProtect;
	VirtualProtect(reinterpret_cast<void*>(addr), originalData[addr].size(), PAGE_EXECUTE_READWRITE, &oldProtect);
	for (UInt32 addroffset = 0; const auto iter : originalData[addr])
	{
		*(reinterpret_cast<UInt8*>(addr) + addroffset) = std::to_integer<UInt8>(iter);
		addroffset++;
	}
	VirtualProtect(reinterpret_cast<void*>(addr), originalData[addr].size(), oldProtect, &oldProtect);
}

void PatchMemoryNop(ULONG_PTR Address, SIZE_T Size)
{
	DWORD d = 0;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

	for (SIZE_T i = 0; i < Size; i++)
		*(volatile BYTE*)(Address + i) = 0x90; //0x90 == opcode for NOP

	VirtualProtect((LPVOID)Address, Size, d, &d);

	FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
}

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst)
{
	UInt32 originalFunction = *(UInt32*)addr;
	SafeWrite32(addr, dst);
	return originalFunction;
}

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	UInt32 originalFunction = *(UInt32*)(jumpSrc + 1) + jumpSrc + 5;
	WriteRelCall(jumpSrc, jumpTgt);
	return originalFunction;
}
