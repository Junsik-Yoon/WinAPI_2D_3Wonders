#include "pch.h"
#include "CBG2.h"
#include "CAnimator.h"
#include "CD2DImage.h"

CBG2::CBG2()
{
    index = 0;
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"BG2Img", L"texture\\background1.png");
    m_pImg_back = CResourceManager::getInst()->LoadD2DImage(L"BG3Img", L"texture\\background2.png");
	SetScale(Vec2(1.f, 1.f));
}

CBG2::~CBG2()
{
}

void CBG2::update()
{
}

void CBG2::render()
{
	int iWidth  = m_pImg->GetWidth();
    int iHeight = m_pImg->GetHeight();
    int iWidth_back = m_pImg_back->GetWidth();
    int iHeight_back = m_pImg_back->GetHeight();
    Vec2 vPos = GetPos();
    Vec2 vScale = GetScale();
    Vec2 vRenderPos = CCameraManager::getInst()->GetRenderPos(vPos);

    if (1 == index)
    {
        CRenderManager::getInst()->RenderImage(
            m_pImg,
            vPos.x + (vRenderPos.x - vPos.x) / 3.5f,
            vRenderPos.y,
            vPos.x + (vRenderPos.x - vPos.x) / 3.5f + iWidth,
            vRenderPos.y + iHeight);
    }
    else if (2 == index)
    {
        CRenderManager::getInst()->RenderImage(
            m_pImg_back,
            vPos.x + (vRenderPos.x - vPos.x) / 4.6f,
            vRenderPos.y,
            vPos.x + (vRenderPos.x - vPos.x) / 4.6f + iWidth_back,
            vRenderPos.y + iHeight_back);
    }

}


