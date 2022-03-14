#pragma once
class CPathManager
{
	SINGLETON(CPathManager)
private:
	const static int MAXSIZE_PATH = 255;
	WCHAR m_szContentPath[MAXSIZE_PATH];
public:
	void init();

	const WCHAR* GetContentPath() { return m_szContentPath; }
	const WCHAR* GetContentRelativePath() { return L"..\\Release\\content\\"; }

};

