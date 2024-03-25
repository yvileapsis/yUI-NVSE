#pragma once

class TESFile;

class TESPackageData
{
public:
	TESPackageData();
	virtual ~TESPackageData();
	virtual void	CopyFrom(TESPackageData* apkPackageData);
	virtual void	Unk_02();
	virtual void	Save(TESFile* apkModInfo);
	virtual void	Unk_04();
	virtual void	Unk_05();
	virtual void	Unk_06();
	virtual void	Unk_07();
};