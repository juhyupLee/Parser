#pragma once


class Parser
{
	enum
	{
		WORD_BUFFER=255
	};
public:
	Parser();
	virtual ~Parser();

	bool Parser_LoadFile(const wchar_t* fileName);
	bool SkipNoneCommand(void);

	void GetNextWord(wchar_t** chppBuffer, int* ipLength);
	bool GetValue_Int(const wchar_t* szName, int* value);
	//bool GetValue_Str(const char* szName, char* value, int size);
	//bool GetValue_Group(const char* szGroupName, char* value, int size);
	void RestoreBuffer();
	wchar_t* GetCurrentWord();

private:
	wchar_t* m_Buffer;
	wchar_t* m_CpyBuffer;
	wchar_t* m_CpyBufferStartPointer;
	wchar_t* m_StringStartPointer;
	wchar_t m_CurrentWord[WORD_BUFFER];
	wchar_t* m_EndPointer;
	int m_FileSize;
};