#include "pch.h"
#include "CBG1.h"
#include "CTexture.h"
#include "CScene.h"


CBG1::CBG1()
{
	m_pTex = CResourceManager::getInst()->LoadTexture(L"BGTex", L"texture\\map_24bit.bmp");
}

CBG1::~CBG1()
{
}
void CBG1::update()
{

}

void CBG1::render(HDC hDC)
{
	m_pTex;
	int iWidth = (int)m_pTex->GetBmpWidth();
	int iHeight = (int)m_pTex->GetBmpHeight();

	Vec2 fptCamLook = CCameraManager::getInst()->GetLookAt();

	Vec2 vPos = GetPos();
	vPos = CCameraManager::getInst()->GetRenderPos(vPos);

	////////////////////////////////
	//transparent로 배경을 그릴 시 //
	////////////////////////////////
	//TransparentBlt(hDC,
	//	vPos.x  + fptCamLook.x - WINSIZEX / 2,
	//	vPos.y  + fptCamLook.y - WINSIZEY / 2,
	//	WINSIZEX,
	//	iHeight,
	//	m_pTex->GetDC(),
	//	fptCamLook.x - WINSIZEX / 2,
	//	fptCamLook.y - WINSIZEY / 2,
	//	WINSIZEX,
	//	iHeight,
	//	RGB(255, 0, 255));

	///////////////////////////
	// bitblt로 통으로 그릴 시//
	///////////////////////////
	BitBlt(hDC,
		(int)vPos.x,
		(int)vPos.y,
		iWidth,
		iHeight,
		m_pTex->GetDC(),
		0, 0,
		SRCCOPY);

}
