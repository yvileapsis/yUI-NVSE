#pragma once

#include "NiThread.hpp"
#include "NiSearchPath.hpp"
#include "NiTexturePalette.hpp"
#include "NiBinaryStream.hpp"
#include "NiTArray.hpp"
#include "NiTPointerMap.hpp"
#include "NiTLargeArray.hpp"
#include "NiObjectGroup.hpp"
#include "NiTSet.hpp"
#include "NiFixedString.hpp"

class NiObject;

NiSmartPointer(NiStream);

class BackgroundLoadProcedure : public NiThreadProcedure {
public:
	NiStream* m_pkStream;
};

class NiStream {
public:
	NiStream();

	enum ErrorMessages
	{
		STREAM_OKAY = 0x0,
		FILE_NOT_LOADED = 0x1,
		NOT_NIF_FILE = 0x2,
		OLDER_VERSION = 0x3,
		LATER_VERSION = 0x4,
		NO_CREATE_FUNCTION = 0x5,
		ENDIAN_MISMATCH = 0x6,
	};

	enum ThreadStatus
	{
		IDLE = 0x0,
		LOADING = 0x1,
		CANCELLING = 0x2,
		PAUSING = 0x3,
		PAUSED = 0x4,
	};

	virtual			~NiStream();
	virtual bool	Load(NiBinaryStream* pkIstr);
	virtual bool	Load1(char* pcBuffer, SInt32* iBufferSize);
	virtual bool	Load2(const char* pcName);
	virtual bool	Save(NiBinaryStream* pkOstr);
	virtual bool	Save1(char** pcBuffer, SInt32* iBufferSize);
	virtual bool	Save2(const char* pcName);
	virtual void	Unk_07();
	virtual void	RegisterFixedString();
	virtual bool	RegisterSaveObject(NiObject* pkObject);
	virtual void	ChangeObject(NiObject* pkNewObject);
	virtual UInt32	GetLinkIDFromObject(const NiObject* pkObject);
	virtual void	SaveLinkID(NiObject* pkObject);
	virtual bool	LoadHeader();
	virtual void	SaveHeader();
	virtual bool	LoadStream();
	virtual bool	SaveStream();
	virtual void	RegisterObjects();
	virtual void	LoadTopLevelObjects();
	virtual void	SaveTopLevelObjects();
	virtual bool	LoadObject();
	virtual UInt32	PreSaveObjectSizeTable();
	virtual bool	SaveObjectSizeTable(UInt32 uiStartOffset);
	virtual bool	LoadObjectSizeTable();

	struct BSStreamHeader {
		UInt32	m_uiNifBSVersion;
		char	cAuthor[64];
		char	cProcessScript[64];
		char	cExportScript[64];
	};

	BSStreamHeader							m_BSStreamHeader;
	NiTPrimitiveArray<NiObjectGroup*>		m_kGroups;
	UInt32									m_uiNifFileVersion;
	UInt32									m_uiNifFileUserDefinedVersion;
	char									m_acFileName[260];
	bool									m_bSaveLittleEndian;
	bool									m_bSourceIsLittleEndian;
	NiSearchPath*							m_pkSearchPath;
	NiTLargeObjectArray<NiObjectPtr>		m_kObjects;
	NiTLargePrimitiveArray<UInt32>			m_kObjectSizes;
	NiTLargeObjectArray<NiObjectPtr>		m_kTopObjects;
	NiTLargeObjectArray<NiFixedString>		m_kFixedStrings;
	NiBinaryStream*							m_pkIstr;
	NiBinaryStream*							m_pkOstr;
	NiUnsignedIntSet						m_kLinkIDs;
	UInt32									m_uiLinkIndex;
	NiUnsignedIntSet						m_kLinkIDBlocks;
	UInt32									m_uiLinkBlockIndex;
	NiTPointerMap<const NiObject*, UInt32>	m_kRegisterMap;
	NiPointer<NiTexturePalette>				m_spTexturePalette;
	float									a;
	NiStream::ThreadStatus					m_eBackgroundLoadStatus;
	bool									m_bBackgroundLoadExitStatus;
	UInt32									m_uiLoad;
	UInt32									m_uiLink;
	UInt32									m_uiPostLink;
	NiThread*								m_pkThread;
	BackgroundLoadProcedure*				m_pkBGLoadProc;
	NiThread::Priority						m_ePriority;
	NiProcessorAffinity						m_kAffinity;
	char									unkString_2B4[260];
	NiStream::ErrorMessages					m_uiLastError;
	char									m_acLastErrorMessage[260];
	char									m_acReferencePath[260];
	NiObject*								unk5C4;

	static void RegisterLoader(const char* pcName, void* pfnFunc);
	static NiStream* Create(NiStream* apThis);

	NiObject* GetObjectAt(UInt32 uiIndex);
	UInt32 GetObjectCount();
	void InsertObject(NiObject* apObject);

	void ReadLinkID();
	NiObject* GetObjectFromLinkID();
};

ASSERT_SIZE(NiStream, 0x5C8)