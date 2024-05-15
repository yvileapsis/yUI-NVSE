#include "NiTList.hpp"

class TESAnimGroup;
class BSAnimGroupSequence;

class AnimSequenceBase
{
public:
	virtual void Destroy(bool doFree);
	virtual void AddAnimGroupSequence(BSAnimGroupSequence* sequence, int unused);
	virtual bool RemoveAnimGroupSequence(BSAnimGroupSequence* sequence, int unused);
	virtual bool IsSingle();
	virtual BSAnimGroupSequence* GetSequenceByIndex(int index);
	virtual BSAnimGroupSequence* GetSequenceByAnimGroup(TESAnimGroup* group);
	virtual signed int GetIndexOfSequenceName(const char* name);
};

class AnimSequenceSingle : public AnimSequenceBase
{
public:
	BSAnimGroupSequence* pkAnim;
};

// 08
class AnimSequenceMultiple : public AnimSequenceBase
{
public:
	NiTList<BSAnimGroupSequence*>* pkAnims; // 04
};