#include <Containers.h>
#include <Utilities.hpp>

String::String() : m_data(NULL), m_dataLen(0), m_bufLen(0) {}

String::~String()
{
	if (m_data)
	{
		GameHeapFree(m_data);
		m_data = NULL;
	}
	m_bufLen = m_dataLen = 0;
}

bool String::Set(const char* src)
{
	if (!src) {
		GameHeapFree(m_data);
		m_data = 0;
		m_bufLen = 0;
		m_dataLen = 0;
		return true;
	}

	UInt32	srcLen = strlen(src);

	// realloc if needed
	if (srcLen > m_bufLen)
	{
		GameHeapFree(m_data);
		m_data = (char*)GameHeapAlloc(srcLen + 1);
		m_bufLen = m_data ? srcLen : 0;
	}

	if (m_data)
	{
		strcpy_s(m_data, m_bufLen + 1, src);
		m_dataLen = srcLen;
	}
	else
	{
		m_dataLen = 0;
	}

	return m_data != NULL;
}

bool String::Includes(const char* toFind) const
{
	if (!m_data || !toFind)		//passing null ptr to std::string c'tor = CRASH
		return false;
	std::string curr(m_data, m_dataLen);
	std::string str2(toFind);
	std::string::iterator currEnd = curr.end();
	return (std::search(curr.begin(), currEnd, str2.begin(), str2.end(), ci_equal) != currEnd);
}

bool String::Replace(const char* _toReplace, const char* _replaceWith)
{
	if (!m_data || !_toReplace)
		return false;

	std::string curr(m_data, m_dataLen);
	std::string toReplace(_toReplace);

	std::string::iterator currBegin = curr.begin();
	std::string::iterator currEnd = curr.end();
	std::string::iterator replaceIt = std::search(currBegin, currEnd, toReplace.begin(), toReplace.end(), ci_equal);
	if (replaceIt != currEnd) {
		std::string replaceWith(_replaceWith);
		// we found the substring, now we need to do the modification
		std::string::size_type replacePos = distance(currBegin, replaceIt);
		curr.replace(replacePos, toReplace.size(), replaceWith);
		Set(curr.c_str());
		return true;
	}
	return false;
}

bool String::Append(const char* toAppend)
{
	std::string curr("");
	if (m_data)
		curr = std::string(m_data, m_dataLen);

	curr += toAppend;
	Set(curr.c_str());
	return true;
}

double String::Compare(const String& compareTo, bool caseSensitive)
{
	if (!m_data)
		return -2;		//signal value if comparison could not be made

	std::string first(m_data, m_dataLen);
	std::string second(compareTo.m_data, compareTo.m_dataLen);

	if (!caseSensitive)
	{
		std::transform(first.begin(), first.end(), first.begin(), tolower);
		std::transform(second.begin(), second.end(), second.begin(), tolower);
	}

	double comp = 0;
	if (first < second)
		comp = -1;
	else if (first > second)
		comp = 1;

	return comp;
}

const char* String::CStr() const
{
	return m_dataLen && m_data ? m_data : "";
}

void String::Init(UInt32 bufSize)
{
	if (m_data) GameHeapFree(m_data);
	m_bufLen = bufSize;
	m_data = (char*)GameHeapAlloc(m_bufLen + 1);
	*m_data = 0;
	m_dataLen = 0;
}


void String::AppendChar(char toAppend)
{
	if (m_bufLen == m_dataLen)
	{
		m_bufLen++;
		char* newStr = (char*)GameHeapAlloc(m_bufLen + 1);
		if (m_data)
		{
			memcpy(newStr, m_data, m_dataLen);
			GameHeapFree(m_data);
		}
		m_data = newStr;
	}
	m_data[m_dataLen++] = toAppend;
	m_data[m_dataLen] = 0;
}

void String::InsertChar(char toInsert, UInt32 index)
{
	if (index > m_dataLen) return;
	if (m_bufLen == m_dataLen)
	{
		m_bufLen++;
		char* newStr = (char*)GameHeapAlloc(m_bufLen + 1);
		if (m_data)
		{
			memcpy(newStr, m_data, m_dataLen);
			GameHeapFree(m_data);
		}
		m_data = newStr;
	}
	if (index < m_dataLen)
		memcpy(m_data + index + 1, m_data + index, m_dataLen - index);
	m_dataLen++;
	m_data[index] = toInsert;
	m_data[m_dataLen] = 0;
}

void String::EraseAt(UInt32 index)
{
	if (!m_data || (index >= m_dataLen)) return;
	m_dataLen--;
	if (m_dataLen)
	{
		UInt32 length = m_dataLen - index;
		if (!length)
			m_data[m_dataLen] = 0;
		else
			memmove(m_data + index, m_data + index + 1, length + 1);
	}
	else *m_data = 0;
}

void String::EraseAfter(UInt32 index)
{
	while (m_dataLen > index)
	{
		EraseAt(index);
	}
}

int String::EraseWord(UInt32 index)
{
	int charsSinceSpace = GetCharsSinceSpace(m_data, index);
	int newIndex = index - charsSinceSpace;
	while (charsSinceSpace--)
	{
		EraseAt(--index);
	}
	return newIndex;
}

void String::EraseNextWord(UInt32 index)
{
	int charsTillSpace = GetCharsTillSpace(m_data, index) + 1;
	while (charsTillSpace--)
	{
		EraseAt(index);
	}
}

void String::RemoveLastChar()
{
	if (!m_data || !m_dataLen) return;
	m_data[--m_dataLen] = '\0';
}