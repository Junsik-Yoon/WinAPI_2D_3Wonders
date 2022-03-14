#pragma once

class CTexture;
class CSound;

class CResourceManager
{
	SINGLETON(CResourceManager);
private:
	map<wstring, CTexture*> m_mapTex;//키값,텍스처
	map<wstring, CSound*> m_mapSound; //sound 리소스의 저장 자료구조
	CSound* m_pBGM;
public:
	CTexture* LoadTexture(const wstring& strKey, const wstring& strPath);
	CTexture* FindTexture(const wstring& strKey);

	CTexture* CreateTexture(const wstring& strKey, UINT width, UINT height);//경로로 사진을 불러오지 않고 직접 만듬

	CSound* FindSound(const wstring& strKey);
	CSound* LoadSound(const wstring& strKey, const wstring& strRelativePath);

	CSound LoadBGM(const wstring& strKey, const wstring& strRelativePath); // stream으로

};
