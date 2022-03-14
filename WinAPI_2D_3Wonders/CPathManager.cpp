#include "pch.h"
#include "CPathManager.h"

CPathManager::CPathManager()
	:m_szContentPath{}
{

}
CPathManager::~CPathManager()
{
}

void CPathManager::init()
{
	GetCurrentDirectory(MAXSIZE_PATH, m_szContentPath);//프로그램 경로를 확인
	int size = (int)wcslen(m_szContentPath);
	for (int i = size - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}
	wcscat_s(m_szContentPath, MAXSIZE_PATH, L"\\Release\\content\\");

}