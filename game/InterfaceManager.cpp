#include <InterfaceManager.h>
#include <TESObjectREFR.h>
#include <Safewrite.hpp>

const _QueueUIMessage QueueUIMessage = reinterpret_cast<_QueueUIMessage>(0x007052F0);	// Called from Cmd_AddSpell_Execute

const _ShowMessageBox ShowMessageBox = reinterpret_cast<_ShowMessageBox>(0x00703E80);
const _ShowMessageBox_Callback ShowMessageBox_Callback = reinterpret_cast<_ShowMessageBox_Callback>(0x005B4A70);
const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID = reinterpret_cast<_ShowMessageBox_pScriptRefID>(0x011CAC64);
const _ShowMessageBox_button ShowMessageBox_button = reinterpret_cast<_ShowMessageBox_button>(0x0118C684);

int* (__thiscall* Tile_A01000)(Tile*, enum TileValues) = (int* (__thiscall*)(Tile*, enum TileValues))0xA01000;
void(__thiscall* TileValueSetString)(int*, char*, char) = (void(__thiscall*)(int*, char*, char))0xA0A300;
void FontTextReplaced::GetVariableEscapedText(const char* input) { Font__CheckForVariablesInText(FontManager::GetSingleton()->fontInfos[0], input, this); }

FontInfo* FontInfo::Load(const char* path, UInt32 ID)
{
	FontInfo* info = (FontInfo*)GameHeapAlloc(sizeof(FontInfo));
	return (FontInfo*)ThisStdCall(0x00A12020, info, ID, path, 1);
}

bool FontInfo::GetName(char* out)
{
	UInt32 len = strlen(filePath);
	len -= 4;					// '.fnt'
	UInt32 start = len;
	while (filePath[start - 1] != '\\') {
		start--;
	}

	len -= start;

	memcpy(out, filePath + start, len);
	out[len] = 0;

	return true;
}

std::string FontManager::StringShorten(const std::string& str, const UInt32 font, const Float32 max) const
{
	if (str.empty() || font < 1) return "";

	const auto charDimensions = fontInfos[font - 1]->fontData->charDimensions;

	const auto dotDims = charDimensions['.'];
	const auto dotWidth = (dotDims.width + dotDims.widthMod + dotDims.flt2C) * 3.0;
	Float64 accumulator = 0;
	UInt32 length = 0;

	for (const auto iterChar : str)
	{
		if (accumulator + dotWidth < max) length++;
		if (accumulator >= max) return str.substr(0, length) + "...";
		const auto charDims = charDimensions[iterChar];
		// < 100 is a sanity check because cyrillic fonts for whatever reason have this value ruined
		accumulator += charDims.width + charDims.flt2C + charDims.widthMod < 100 ? charDims.widthMod : 0;
	}
	return str;
}

void InterfaceManager::VATSHighlightData::AddRef(TESObjectREFR* ref)
{
	if (ref->GetNiNode())
	{
		ThisCall(0x800D50, this, ref);
		this->mode = 1;
	}
};

void InterfaceManager::VATSHighlightData::AddRefAndSetFlashing(TESObjectREFR* ref)
{
	if (ref->GetNiNode())
	{
		ThisCall(0x800E50, this, ref);
		this->mode = 1;
	}
};

/*
 * Lifted from JIP LN
 */
__declspec(naked) UInt32 InterfaceManager::GetTopVisibleMenuID()
{
	__asm
	{
		cmp		byte ptr[ecx + 0xC], 2
		jb		retn0
		mov		eax, [ecx + 0xD0]
		add		ecx, 0x114
		test	eax, eax
		jz		stackIter
		mov		eax, [eax + 0x20]
		retn
		ALIGN 16
	stackIter:
		add		ecx, 4
		cmp[ecx], 0
		jnz		stackIter
		mov		eax, [ecx - 4]
		cmp		eax, 1
		jnz		done
		mov		ecx, 0x11F3479
		mov		eax, 0x3EA
		cmp[ecx], 0
		jnz		done
		mov		al, 0xFF
		cmp[ecx + 0x15], 0
		jnz		done
		mov		al, 0xEB
		cmp[ecx + 1], 0
		jnz		done
		mov		eax, 0x40B
		cmp[ecx + 0x21], 0
		jnz		done
		mov		al, 0x25
		cmp[ecx + 0x3B], 0
		jnz		done
	retn0 :
		xor eax, eax
	done :
		retn
	}
}

Tile* InterfaceManager::GetActiveTile()
{
	return activeTile ? activeTile : activeTileAlt;
}