#include "pch.h"
#include "CCameraManager.h"
#include "CGameObject.h"
#include "CTexture.h"

CCameraManager::CCameraManager()
{
	m_fptLookAt = Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f); // 보고 있는 위치
	m_fptCurLookAt = m_fptLookAt;						// 카메라가 지금 보는 위치
	m_fptPrevLookAt = m_fptLookAt;						// 카메라가 이전에 보던 위치
	m_pTargetObj = nullptr;								// 트래킹 할 오브젝트

	m_fptDiff = {};										// 해상도 중심과 카메라 LookAt 사이의 차이
	m_fTime = 2;										// 타겟을 따라가는 총시간
	m_fAccTime = m_fTime;								// 타겟을 따라간 소요시간
	m_fSpeed = 0;										// 타겟을 따라가는 속도
	m_fPreSpeed = m_fSpeed;								// 타겟을 따라가던 속도
	m_fAccel = 0;										// 타겟을 따라가는 등가속도
	m_fAccDir = 1.f;									// 등가속도의 증감


}

CCameraManager::~CCameraManager()
{

}

void CCameraManager::init()
{
	m_pTex = CResourceManager::getInst()->CreateTexture(L"CameraTex", WINSIZEX, WINSIZEY);
}

void CCameraManager::update()
{
	if (m_pTargetObj)
	{
		if (m_pTargetObj->isDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			SetLookAt(m_pTargetObj->GetPos());
		}
	}

	// 화면 중앙과 카메라 LookAt 좌표 사이의 차이 계산
	CalDiff();
}

void CCameraManager::render(HDC hDC)
{
	//Rectangle(m_pTex->GetDC(), 0, 0, 500, 500);

	if (CAM_EFFECT::NONE == m_eEffect)
		return;

	m_fCurTime += fDT;
	if (m_fEffectDuration < m_fCurTime)
	{
		m_eEffect = CAM_EFFECT::NONE;
		return;
	}
	float fRatio = m_fCurTime / m_fEffectDuration;
	int iAlpha = (int)(255.f * fRatio);
	if (CAM_EFFECT::FADE_OUT == m_eEffect)
	{
		iAlpha = (int)(255.f * (1 - fRatio));
	}
	else if (CAM_EFFECT::FADE_IN == m_eEffect)
	{
		iAlpha = (int)(255.f * (1 - fRatio));
	}


	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = 127; //0~255까지 조절 가능

	AlphaBlend(hDC
		, 0, 0
		, (int)(m_pTex->GetBmpWidth())
		, (int)(m_pTex->GetBmpHeight())
		, m_pTex->GetDC()
		, 0, 0
		, (int)(m_pTex->GetBmpWidth())
		, (int)(m_pTex->GetBmpHeight())
		, bf);

	//BitBlt(hDC, 0, 0,
	//	(int)(m_pTex->GetBmpWidth()),
	//	(int)(m_pTex->GetBmpHeight()),
	//	m_pTex->GetDC(),
	//	0,
	//	0,
	//	SRCCOPY);
}


void CCameraManager::SetLookAt(Vec2 lookAt)
{
	m_fptLookAt = lookAt;
	float fMoveDist = (m_fptLookAt - m_fptPrevLookAt).Length();
	m_fAccel = fMoveDist / m_fTime;		// 등가속도 계산
	m_fAccel *= m_fAccDir;

	if (m_fAccDir < 0) // 초기 속도 세팅
	{
		m_fSpeed = fMoveDist;
	}
	else
	{
		m_fSpeed = 0.f;
	}
	m_fAccTime = 0.f;
}

void CCameraManager::Scroll(Vec2 vec, float velocity)
{
	m_fptLookAt = m_fptLookAt + vec * velocity * fDT;
	m_fptCurLookAt = m_fptCurLookAt + vec * velocity * fDT;

	Vec2 fptCenter = Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f);
	m_fptDiff = m_fptCurLookAt - fptCenter;
}

void CCameraManager::FadeIn(float duration)
{
	m_eEffect = CAM_EFFECT::FADE_IN;
	m_fEffectDuration = duration;
	m_fCurTime = 0.f;
}

void CCameraManager::FadeOut(float duration)
{
}


void CCameraManager::CalDiff()
{
	m_fAccTime += fDT;

	Vec2 fvLookDir = m_fptLookAt - m_fptPrevLookAt;

	// 시간이 지났거나 방향 벡터의 크기가 0이라면, 도착한것으로 간주. 속도를 0으로
	if (m_fTime <= m_fAccTime || fvLookDir.Length() == 0)
	{
		m_fptCurLookAt = m_fptLookAt;
		m_fSpeed = 0.f;
	}
	else
	{
		m_fptCurLookAt = m_fptPrevLookAt + fvLookDir.Normalize() * m_fSpeed * fDT;
	}

	Vec2 fptCenter = Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f);

	m_fptDiff = m_fptCurLookAt - fptCenter;
	m_fptPrevLookAt = m_fptCurLookAt;
	m_fPreSpeed = m_fSpeed;

	// 등가속도만큼 이전속도에 더해줌.
	m_fSpeed = m_fPreSpeed += m_fAccel * fDT;
}