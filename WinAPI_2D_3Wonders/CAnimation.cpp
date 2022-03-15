#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CGameObject.h"
#include "CTexture.h"
#include "CD2DImage.h"


CAnimation::CAnimation()
{
    m_strName = L"";
    m_pAnimator = nullptr;
    m_pImg = nullptr;
    m_iCurFrm = 0;
    m_fAccTime = 0;
}

CAnimation::~CAnimation()
{
}


void CAnimation::update()
{
    m_fAccTime += fDT;

    if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
    {
        m_iCurFrm++;
        m_iCurFrm %= m_vecFrm.size();
        m_fAccTime -= m_vecFrm[m_iCurFrm].fDuration;
    }
}

void CAnimation::render()
{
    CGameObject* pObj = m_pAnimator->GetObj();
    Vec2 fptPos = pObj->GetPos();
    tAniFrm frm = m_vecFrm[m_iCurFrm];

    fptPos = fptPos + frm.fptOffset;
    fptPos = CCameraManager::getInst()->GetRenderPos(fptPos);


    if (m_bReverse)
    {
        CRenderManager::getInst()->RenderRevFrame(
            m_pImg,
            fptPos.x - frm.fptSlice.x / 2.f,
            fptPos.y - frm.fptSlice.y / 2.f,
            fptPos.x + frm.fptSlice.x / 2.f,
            fptPos.y + frm.fptSlice.y / 2.f,
            frm.fptLT.x,
            frm.fptLT.y,
            frm.fptLT.x + frm.fptSlice.x,
            frm.fptLT.y + frm.fptSlice.y
        );
    }
    else
    {
        CRenderManager::getInst()->RenderFrame(
            m_pImg,
            fptPos.x - frm.fptSlice.x / 2.f,
            fptPos.y - frm.fptSlice.y / 2.f,
            fptPos.x + frm.fptSlice.x / 2.f,
            fptPos.y + frm.fptSlice.y / 2.f,
            frm.fptLT.x,
            frm.fptLT.y,
            frm.fptLT.x + frm.fptSlice.x,
            frm.fptLT.y + frm.fptSlice.y
        );
    }
}

void CAnimation::Create(CD2DImage* Img,      // 애니메이션의 이미지
    Vec2 lt,           // 애니메이션 시작 프레임의 좌상단 좌표
    Vec2 slice,        // 애니메이션 프레임의 크기
    Vec2 step,         // 애니메이션 프레임의 반복 위치
    float duration,    // 애니메이션 프레임 지속시간
    UINT frmCount)     // 애니메이션 프레임 갯수
{
    m_pImg = Img;

    tAniFrm frm = {};
    for (UINT i = 0; i < frmCount; i++)
    {
        frm.fDuration = duration;
        frm.fptSlice = slice;
        frm.fptLT = lt + step * i;

        m_vecFrm.push_back(frm);
    }
}
