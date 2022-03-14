#include "pch.h"
#include "CTexture.h"


CTexture::CTexture()
{
	m_hDC = 0;
	m_hBMP = 0;
	m_bmpInfo = {};
}

CTexture::~CTexture()
{
	DeleteDC(m_hDC);
	DeleteObject(m_hBMP);
}

void CTexture::Load(const wstring& strFilePath)
{
	m_hBMP = (HBITMAP)LoadImage(
		nullptr,
		strFilePath.c_str(),						//파일의 경로
		IMAGE_BITMAP,								//이미지 타입
		0, 0,										//이미지의 xy크기, 0==default
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);		//이미지의 속성 

	if (0 == m_hBMP)//로드실패
		assert(nullptr);


	m_hDC = CreateCompatibleDC(CCore::getInst()->GetMainDC());

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBMP);

	GetObject(m_hBMP, sizeof(BITMAP), &m_bmpInfo);
}

void CTexture::Create(UINT width, UINT height)//불러오지 않고 비어있는 texture 생성
{
	HDC mainDC = CCore::getInst()->GetMainDC();
	m_hBMP = CreateCompatibleBitmap(mainDC, width, height);
	m_hDC = CreateCompatibleDC(mainDC);

	HBITMAP hOldBMP = (HBITMAP)SelectObject(m_hDC, m_hBMP);
	DeleteObject(hOldBMP);
	GetObject(m_hBMP, sizeof(BITMAP), &m_bmpInfo);
}


int CTexture::GetBmpWidth()
{
	return m_bmpInfo.bmWidth;
}

int CTexture::GetBmpHeight()
{
	return m_bmpInfo.bmHeight;
}

