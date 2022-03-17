#include "pch.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CSound.h"
#include "CD2DImage.h"

CResourceManager::CResourceManager()
{
	m_mapSound = {};
}
CResourceManager::~CResourceManager()
{
	for (map<wstring, CTexture*>::iterator iter = m_mapTex.begin(); iter != m_mapTex.end(); ++iter)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
		}
	}
	m_mapTex.clear();
	//자료구조에 저장된 모든 sound 삭제
	for (map<wstring, CSound*>::iterator iter = m_mapSound.begin(); iter != m_mapSound.end(); ++iter)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
		}
	}
	m_mapSound.clear();
	// 자료구조에 저장된 모든 D2DImage 삭제
	for (map<wstring, CD2DImage*>::iterator iter = m_mapD2DImg.begin(); iter != m_mapD2DImg.end(); iter++)
	{
		if (nullptr != iter->second)
		{
			delete iter->second;
		}
	}
	m_mapD2DImg.clear();
}




CTexture* CResourceManager::LoadTexture(const wstring& strKey, const wstring& strPath)
{
	//이미 키값이 있을 시
	CTexture* pTex = FindTexture(strKey);
	if (nullptr != pTex)
		return pTex;

	//없을 시 동적할당
	wstring strFilePath = CPathManager::getInst()->GetContentPath();
	strFilePath += strPath;

	pTex = new CTexture();
	pTex->Load(strFilePath);
	pTex->SetKey(strKey);
	pTex->SetPath(strPath);

	m_mapTex.insert(make_pair(strKey, pTex));
	return pTex;
}

CTexture* CResourceManager::FindTexture(const wstring& strKey)
{
	map<wstring, CTexture*>::iterator iter = m_mapTex.find(strKey);

	if (m_mapTex.end() == iter)
	{
		return nullptr;//못찾음
	}
	else
	{
		return iter->second;
	}
}

CTexture* CResourceManager::CreateTexture(const wstring& strKey, UINT width, UINT height)
{
	CTexture* pTex = FindTexture(strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}
	pTex = new CTexture();
	pTex->Create(width, height);
	pTex->SetKey(strKey);
	m_mapTex.insert(make_pair(strKey, pTex));
	return pTex;
}

CSound* CResourceManager::FindSound(const wstring& strKey)
{
	map<wstring, CSound*>::iterator iter = m_mapSound.find(strKey);

	if (m_mapSound.end() == iter)
	{
		return nullptr;//못찾음
	}
	else
		return iter->second;
}

CSound* CResourceManager::LoadSound(const wstring& strKey, const wstring& strRelativePath)
{
	//이미 키값이 있을 시
	CSound* pSound = FindSound(strKey);
	if (nullptr != pSound)
		return pSound;

	//없을 시 동적할당
	wstring strFilePath = CPathManager::getInst()->GetContentRelativePath();
	strFilePath += strRelativePath;

	pSound = new CSound();
	pSound->Load(strFilePath);
	pSound->SetKey(strKey);
	pSound->SetPath(strRelativePath);

	m_mapSound.insert(make_pair(strKey, pSound));
	return pSound;
}

CD2DImage* CResourceManager::FindD2DImage(const wstring& strKey)
{
	// CD2DImage 키 값을 통해 탐색
	map<wstring, CD2DImage*>::iterator iter = m_mapD2DImg.find(strKey);

	if (m_mapD2DImg.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}

CD2DImage* CResourceManager::LoadD2DImage(const wstring& strKey, const wstring& strRelativePath)
{
	CD2DImage* pD2DImg = FindD2DImage(strKey);
	if (nullptr != pD2DImg)
		return pD2DImg;

	wstring strFilePath = CPathManager::getInst()->GetContentRelativePath();
	strFilePath += strRelativePath;

	CD2DImage* img = new CD2DImage;

	IWICBitmapDecoder* p_decoder;		// 압축된 이미지를 해제할 객체
	IWICBitmapFrameDecode* p_frame;		// 특정 그림을 선택한 객체
	IWICFormatConverter* p_converter;	// 이미지 변환 객체

	// WIC용 Factory 객체를 사용하여 이미지 압축 해제를 위한 객체를 생성
	if (S_OK != CRenderManager::getInst()->GetImageFactory()->CreateDecoderFromFilename(strFilePath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder))
	{
		assert(nullptr);
	}
	// 파일을 구성하는 이미지 중에서 첫번째 이미지를 선택한다.
	if (S_OK != p_decoder->GetFrame(0, &p_frame))
	{
		assert(nullptr);
	}
	// IWICBitmap형식의 비트맵을 ID2D1Bitmap. 형식으로 변환하기 위한 객체 생성
	if (S_OK != CRenderManager::getInst()->GetImageFactory()->CreateFormatConverter(&p_converter))
	{
		assert(nullptr);
	}
	// 선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정
	if (S_OK != p_converter->Initialize(p_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom))
	{
		assert(nullptr);
	}
	// IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체를 생성
	ID2D1Bitmap* bitmap = CRenderManager::getInst()->GetBitmap();
	if (S_OK != CRenderManager::getInst()->GetRenderTarget()->CreateBitmapFromWicBitmap(p_converter, NULL, &bitmap))
	{
		assert(nullptr);
	}

	// 성공적으로 생성한 경우
	img->SetImage(bitmap);
	img->SetKey(strKey);
	img->SetPath(strRelativePath);
	m_mapD2DImg.insert(make_pair(strKey.c_str(), img));

	p_converter->Release();		// 이미지 변환 객체 제거
	p_frame->Release();			// 그림파일에 있는 이미지를 선택하기 위해 사용한 객체 제거
	p_decoder->Release();		// 압축을 해제하기 위해 생성한 객체 제거

	return img;
}

CD2DImage* CResourceManager::LoadToolD2DImage(const wstring& strPath)
{
	if (nullptr != m_pToolImage)
	{
		delete m_pToolImage;
	}

	m_pToolImage = new CD2DImage;

	IWICBitmapDecoder* p_decoder;		// 압축된 이미지를 해제할 객체
	IWICBitmapFrameDecode* p_frame;		// 특정 그림을 선택한 객체
	IWICFormatConverter* p_converter;	// 이미지 변환 객체

	// WIC용 Factory 객체를 사용하여 이미지 압축 해제를 위한 객체를 생성
	if (S_OK != CRenderManager::getInst()->GetImageFactory()->CreateDecoderFromFilename(strPath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decoder))
	{
		assert(nullptr);
	}
	// 파일을 구성하는 이미지 중에서 첫번째 이미지를 선택한다.
	if (S_OK != p_decoder->GetFrame(0, &p_frame))
	{
		assert(nullptr);
	}
	// IWICBitmap형식의 비트맵을 ID2D1Bitmap. 형식으로 변환하기 위한 객체 생성
	if (S_OK != CRenderManager::getInst()->GetImageFactory()->CreateFormatConverter(&p_converter))
	{
		assert(nullptr);
	}
	// 선택된 그림을 어떤 형식의 비트맵으로 변환할 것인지 설정
	if (S_OK != p_converter->Initialize(p_frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom))
	{
		assert(nullptr);
	}
	// IWICBitmap 형식의 비트맵으로 ID2D1Bitmap 객체를 생성
	ID2D1Bitmap* bitmap = CRenderManager::getInst()->GetBitmap();
	if (S_OK != CRenderManager::getInst()->GetRenderTarget()->CreateBitmapFromWicBitmap(p_converter, NULL, &bitmap))
	{
		assert(nullptr);
	}

	// 성공적으로 생성한 경우
	m_pToolImage->SetImage(bitmap);

	p_converter->Release();		// 이미지 변환 객체 제거
	p_frame->Release();			// 그림파일에 있는 이미지를 선택하기 위해 사용한 객체 제거
	p_decoder->Release();		// 압축을 해제하기 위해 생성한 객체 제거

	return m_pToolImage;
}
