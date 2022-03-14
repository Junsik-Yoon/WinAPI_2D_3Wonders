#pragma once
#include "CResource.h"


class CTexture :public CResource
{
private:
	HDC		m_hDC;
	HBITMAP m_hBMP;
	BITMAP m_bmpInfo;
public:
	void Load(const wstring& strFilePath);
	void Create(UINT width, UINT height);

	HDC GetDC() { return m_hDC; }
	HBITMAP GetBitMap() { return m_hBMP; }
	BITMAP GetBmpInfo() { return m_bmpInfo; }

	int GetBmpWidth();
	int GetBmpHeight();
public:
	CTexture();
	virtual ~CTexture();
};
