#pragma once

class TESFile;

class TESPackageData {
public:
	TESPackageData();
	~TESPackageData();
	virtual void	Destroy(bool free);
	virtual void	CopyFrom(TESPackageData* packageData);
	virtual void	Unk_02(void);
	virtual void	Save(TESFile* modInfo);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
};