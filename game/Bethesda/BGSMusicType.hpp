#include "TESForm.hpp"
#include "TESSoundFile.hpp"

class BGSMusicType : public TESForm {
public:
	BGSMusicType();
	~BGSMusicType();

	TESSoundFile		soundFile;	// 018
	UInt32	unk024;
	UInt32	unk028;
	UInt32	unk02C;
};