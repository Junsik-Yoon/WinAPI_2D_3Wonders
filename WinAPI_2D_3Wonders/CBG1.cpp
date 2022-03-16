#include "pch.h"
#include "CBG1.h"
#include "CTexture.h"
#include "CScene.h"
#include "CD2DImage.h"

CBG1::CBG1()
{
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"BGImg", L"texture\\map_24bit.png");
}

CBG1::~CBG1()
{
}
void CBG1::update()
{

}

void CBG1::render()
{
	int iWidth = m_pImg->GetWidth();
	int iHeight = m_pImg->GetHeight();

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
	//BitBlt(hDC,
	//	(int)vPos.x,
	//	(int)vPos.y,
	//	iWidth,
	//	iHeight,
	//	m_pTex->GetDC(),
	//	0, 0,
	//	SRCCOPY);
	CRenderManager::getInst()->RenderImage(m_pImg,
		(int)vPos.x+0,
		(int)vPos.y+0,
		(int)vPos.x + iWidth,
		(int)vPos.y+ iHeight
		);

}
