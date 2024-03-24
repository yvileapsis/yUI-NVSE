#pragma once

class NiBinaryStream;
typedef UInt32(__cdecl* NIBINARYSTREAM_READFN)(NiBinaryStream* apThis, void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
typedef UInt32(__cdecl* NIBINARYSTREAM_WRITEFN)(NiBinaryStream* apThis, const void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);

class NiBinaryStream {
public:
	NiBinaryStream();
	virtual ~NiBinaryStream() {};

	virtual 		operator bool() = 0;
	virtual void	Seek(SInt32 aiNumBytes) = 0;
	virtual UInt32	GetPosition() const;
	virtual void	SetEndianSwap(bool abDoSwap) = 0;


	UInt32					m_uiAbsoluteCurrentPos;
	NIBINARYSTREAM_READFN	m_pfnRead;
	NIBINARYSTREAM_WRITEFN	m_pfnWrite;

	static void DoByteSwap(void* apvData, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);


	__forceinline UInt32 Read(void* apvBuffer, UInt32 auiBytes) {
		UInt32 uiSize = 1;
		UInt32 uiBytesRead = BinaryRead(apvBuffer, auiBytes, &uiSize, 1);
		return uiBytesRead;
	}

	__forceinline UInt32 Write(const void* apvBuffer, UInt32 auiBytes) {
		UInt32 uiSize = 1;
		UInt32 uiBytesWritten = BinaryWrite(apvBuffer, auiBytes, &uiSize, 1);
		return uiBytesWritten;
	}

	__forceinline UInt32 BinaryRead(void* apvBuffer, UInt32 auiTotalBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents) {
		UInt32 uiBytesRead = m_pfnRead(this, apvBuffer, auiTotalBytes, apuiComponentSizes, auiNumComponents);
		m_uiAbsoluteCurrentPos += uiBytesRead;
		return uiBytesRead;
	}

	__forceinline UInt32 BinaryWrite(const void* apvBuffer, UInt32 auiTotalBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents) {
		UInt32 uiBytesWritten = m_pfnWrite(this, apvBuffer, auiTotalBytes, apuiComponentSizes, auiNumComponents);
		m_uiAbsoluteCurrentPos += uiBytesWritten;
		return uiBytesWritten;
	}

	template <class T_Data>
	void NiBinaryStreamLoad(NiBinaryStream& is, T_Data* pValue, UInt32 uiNumEls = 1) {
		UInt32 uiSize = sizeof(T_Data);
		is.BinaryRead(pValue, uiSize * uiNumEls, &uiSize, 1);
	}

	template <class T_Data>
	void NiStreamLoadBinary(NiBinaryStream& binstream, T_Data& value) {
		NiBinaryStreamLoad(binstream, &value, 1);
	}

	template <class T_Data>
	void NiStreamLoadBinary(NiBinaryStream& binstream, T_Data* value, UInt32 auiNumEls) {
		NiBinaryStreamLoad(binstream, value, auiNumEls);
	}
};

ASSERT_SIZE(NiBinaryStream, 0x10);