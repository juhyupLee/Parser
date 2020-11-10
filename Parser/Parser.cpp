#include "Parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

bool Parser::Parser_LoadFile(const wchar_t* fileName)
{
	//만일 파일이 이미 동적 할당 된 상황이라면 
	//delete 하고 ,다시 새롭게 동적할당한다.
	if (nullptr != m_Buffer)
	{
		delete[]m_Buffer;
		m_Buffer = nullptr;
		if (nullptr != m_CpyBuffer)
		{
			delete[]m_CpyBuffer;
			m_CpyBuffer = nullptr;
		}
	}
	FILE* pFile = nullptr;
	//-----------------------------------------------------------
	// 파일 오픈 후, 파일사이즈를 구해 버퍼를 일단 만든다.
	//-----------------------------------------------------------
	_wfopen_s(&pFile,fileName, L"rt,ccs=UTF-8");
	while (pFile == nullptr)
	{
		_wfopen_s(&pFile, fileName, L"rt,ccs=UTF-8");
	}
	fseek(pFile, 0, SEEK_END);
	m_FileSize = ftell(pFile);
	m_Buffer = new wchar_t[m_FileSize];
	fclose(pFile);
	memset(m_Buffer, 0, m_FileSize);

	//-----------------------------------------------------------
	// 파일 오픈 후, 문자열을 버퍼에 한줄 한줄 담는다.
	//-----------------------------------------------------------
	_wfopen_s(&pFile, fileName, L"rt,ccs=UTF-8");
	while (pFile==nullptr)
	{
		_wfopen_s(&pFile, fileName, L"rt,ccs=UTF-8");
	}

	wchar_t tempBuffer[1024];

	int pointer = 0;

	while (nullptr != fgetws(tempBuffer, 1024, pFile))
	{
		int len = wcslen(tempBuffer);
		memcpy(m_Buffer +pointer, tempBuffer, len*2);
		pointer +=len;

	}
	fclose(pFile);
	//-------------------------
	//파일 사이즈를 구해서, 파일로드한 메모리 끝은 null문자로 채워준다.
	//-------------------------
	

	//-------------------------
	//파일 원본을 유지시키기위해 사본을 만든다
	//-------------------------
	RestoreBuffer();
	return true;

}

bool Parser::SkipNoneCommand(void)
{
  	if (m_CpyBuffer == nullptr)
	{
		return false;
	}
	wchar_t* buf;
	int strlen = 0;
	
	GetNextWord(&buf, &strlen);

	memset(m_CurrentWord, 0, WORD_BUFFER);
	memcpy(m_CurrentWord, buf, strlen*2+2);

	//만약 문자가 ""가아니거나, " " 아니면  SkipNoneCommand 실패
	if (!wcscmp(m_CurrentWord, L"") || !wcscmp(m_CurrentWord, L" "))
	{
		return true;
	}
	//
	////만약  주석처리 중에 // 단어를 만나면, 
	if (!wcscmp(m_CurrentWord, L"//"))
	{
		while (true)
		{
			GetNextWord(&buf,&strlen);
			//memset(mCurrentWord, 0, 255);
			//memcpy(mCurrentWord, buf, strlen);
			if (!wcscmp(buf, L""))
			{
				return true;
			}
		}
	}
	////만약  주석처리 중에 /* 단어를 만나면, 
	if (!wcscmp(m_CurrentWord, L"/*"))
	{
		while (true)
		{
			GetNextWord(&buf, &strlen);
			//memset(mCurrentWord, 0, 255);
			//memcpy(mCurrentWord, buf, strlen);
			if (!wcscmp(buf, L"*/"))
			{
				return true;
			}
		}
	}

	return false;
}

void Parser::GetNextWord(wchar_t** chppBuffer, int* ipLength)
{
	while (*m_CpyBuffer !=L'\0')
	{
		if (	*m_CpyBuffer ==L'\n'
			||	*m_CpyBuffer == L'\t'
			||	*m_CpyBuffer ==L' ')
		{
			*m_CpyBuffer = L'\0';
			m_CpyBuffer++;
			break;
		}
		m_CpyBuffer++;
	}
	//-----------------------------------------------------
	//하나는 시작 포인터, 하나는 증가포인터
	//만약 엔터,스페이스 탭과 같은것들이 문자열에있을 경우 거기에 null을 박고,
	//시작 포인터~null까지 문자열을 chappBuffer에 넘긴다.
	//그리고 시작 포인터를 갱신한다.
	//-----------------------------------------------------
	*chppBuffer = m_StringStartPointer;
	*ipLength = wcslen(m_StringStartPointer);
	m_StringStartPointer = m_CpyBuffer;
}

bool Parser::GetValue_Int(const wchar_t* szName, int* value)
{
	wchar_t* buf;
	int strLen = 0;
	RestoreBuffer();
	while (wcscmp(GetCurrentWord(),L"End"))
	{
		while (!SkipNoneCommand())
		{
			if (!wcscmp(GetCurrentWord(), szName))
			{
				GetNextWord(&buf, &strLen);
				if (!wcscmp(buf, L"="))
				{
					GetNextWord(&buf, &strLen);
					*value = _wtoi(buf);
					return true;
				}

			}
		}
	}
	return false;
}

//bool Parser::GetValue_Str(const char* szName, char* value,int size)
//{
//	char* buf;
//	int strLen = 0;
//	RestoreBuffer();
//	while (strcmp(GetCurrentWord(), "End"))
//	{
//		while (!SkipNoneCommand())
//		{
//			if (!strcmp(GetCurrentWord(), szName))
//			{
//				GetNextWord(&buf, &strLen);
//				if (!strcmp(buf, "="))
//				{
//					GetNextWord(&buf, &strLen);
//					strcpy_s(value, size, buf);
//					return true;
//				}
//
//			}
//		}
//	}
//	return false;
//}
//
//bool Parser::GetValue_Group(const char* szGroupName, char* value, int size)
//{
//	//------------------------------------
//	//그룹을 지정하여 그 그룹에 관련된 Value 를 몽땅 가져온다.
//	//------------------------------------
//	int strLen = 0;
//	bool bFirst = true;
//	RestoreBuffer();
//	while (strcmp(GetCurrentWord(), "End"))
//	{
//		while (SkipNoneCommand());
//			if (!strcmp(GetCurrentWord(), szGroupName))
//			{
//				while (strcmp(GetCurrentWord(), "}"))
//				{
//					while (SkipNoneCommand());
//					
//						if (!strcmp(GetCurrentWord(), "="))
//						{
//							while (SkipNoneCommand());
//							if (bFirst)
//							{
//								sprintf_s(value, size, "%s",GetCurrentWord());
//								bFirst = false;
//							}
//							sprintf_s(value, size, "%s,%s", value, GetCurrentWord());
//						}
//				}
//				return true;
//			}
//		
//	}
//	return false;
//}


void Parser::RestoreBuffer()
{
	//-----------------------------------------------------
	//Paser가 메모리에 로드한 원본버퍼를 유지시키고, 그 원본 버퍼를 사본에 복사한다.
	// ** Restore 할 때 마다 동적할당을 계속해야되는 문제가 있는데... 추후 수정할지 고민해봐야함.
	//-----------------------------------------------------
	if (nullptr == m_Buffer)
	{
		return;
	}
	if (nullptr != m_CpyBufferStartPointer)
	{
		//전파일이나 이미 사본복사가 있을 경우 메모리해제해준다.
		delete [] m_CpyBufferStartPointer;
		m_CpyBufferStartPointer = nullptr;
	}
	m_CpyBuffer = new wchar_t[m_FileSize];
	m_CpyBufferStartPointer = m_CpyBuffer;

	memcpy(m_CpyBuffer, m_Buffer, m_FileSize);
	////-----------------------------------------------------
	////추후 반복문을 위해 버퍼의 마지막 pointer를 저장해 둔다.
	////-----------------------------------------------------
	//m_EndPointer = m_CpyBuffer + m_FileSize;
	//-----------------------------------------------------
	//추후 두개의 포인터로 반복문을 돌면서 문자열을 파싱해야되는데,
	//문자열의 시작 포인터를 갱신한다.
	//-----------------------------------------------------
	m_StringStartPointer = m_CpyBuffer;
	
}

wchar_t* Parser::GetCurrentWord()
{
	return m_CurrentWord;
}

Parser::Parser()
	:m_Buffer(nullptr),
     m_StringStartPointer(nullptr)
{
	memset(m_CurrentWord, 0, 255);
}

Parser::~Parser()
{
	if (nullptr!=m_Buffer)
	{
		delete[] m_Buffer;
	}
	if (nullptr != m_CpyBufferStartPointer)
	{
		delete[] m_CpyBufferStartPointer;
	}
}
