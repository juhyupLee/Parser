#include "Parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

bool Parser::Parser_LoadFile(const wchar_t* fileName)
{
	//���� ������ �̹� ���� �Ҵ� �� ��Ȳ�̶�� 
	//delete �ϰ� ,�ٽ� ���Ӱ� �����Ҵ��Ѵ�.
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
	// ���� ���� ��, ���ϻ���� ���� ���۸� �ϴ� �����.
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
	// ���� ���� ��, ���ڿ��� ���ۿ� ���� ���� ��´�.
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
	//���� ����� ���ؼ�, ���Ϸε��� �޸� ���� null���ڷ� ä���ش�.
	//-------------------------
	

	//-------------------------
	//���� ������ ������Ű������ �纻�� �����
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

	//���� ���ڰ� ""���ƴϰų�, " " �ƴϸ�  SkipNoneCommand ����
	if (!wcscmp(m_CurrentWord, L"") || !wcscmp(m_CurrentWord, L" "))
	{
		return true;
	}
	//
	////����  �ּ�ó�� �߿� // �ܾ ������, 
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
	////����  �ּ�ó�� �߿� /* �ܾ ������, 
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
	//�ϳ��� ���� ������, �ϳ��� ����������
	//���� ����,�����̽� �ǰ� �����͵��� ���ڿ������� ��� �ű⿡ null�� �ڰ�,
	//���� ������~null���� ���ڿ��� chappBuffer�� �ѱ��.
	//�׸��� ���� �����͸� �����Ѵ�.
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
//	//�׷��� �����Ͽ� �� �׷쿡 ���õ� Value �� ���� �����´�.
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
	//Paser�� �޸𸮿� �ε��� �������۸� ������Ű��, �� ���� ���۸� �纻�� �����Ѵ�.
	// ** Restore �� �� ���� �����Ҵ��� ����ؾߵǴ� ������ �ִµ�... ���� �������� ����غ�����.
	//-----------------------------------------------------
	if (nullptr == m_Buffer)
	{
		return;
	}
	if (nullptr != m_CpyBufferStartPointer)
	{
		//�������̳� �̹� �纻���簡 ���� ��� �޸��������ش�.
		delete [] m_CpyBufferStartPointer;
		m_CpyBufferStartPointer = nullptr;
	}
	m_CpyBuffer = new wchar_t[m_FileSize];
	m_CpyBufferStartPointer = m_CpyBuffer;

	memcpy(m_CpyBuffer, m_Buffer, m_FileSize);
	////-----------------------------------------------------
	////���� �ݺ����� ���� ������ ������ pointer�� ������ �д�.
	////-----------------------------------------------------
	//m_EndPointer = m_CpyBuffer + m_FileSize;
	//-----------------------------------------------------
	//���� �ΰ��� �����ͷ� �ݺ����� ���鼭 ���ڿ��� �Ľ��ؾߵǴµ�,
	//���ڿ��� ���� �����͸� �����Ѵ�.
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
