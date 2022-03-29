#include "pch.h"
#include "CCameraManager.h"
#include "CGameObject.h"
#include "CD2DImage.h"
#include "CScene.h"
CCameraManager::CCameraManager()
{
	m_fTime = 0.1f;
	m_fptLookAt = Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f); // ���� �ִ� ��ġ
	m_fptCurLookAt = m_fptLookAt;						// ī�޶� ���� ���� ��ġ
	m_fptPrevLookAt = m_fptLookAt;						// ī�޶� ������ ���� ��ġ
	m_pTargetObj = nullptr;								// Ʈ��ŷ �� ������Ʈ
	m_fAccTime = m_fTime;								// Ÿ���� ���� �ҿ�ð�
	m_fSpeed = 0.f;										// Ÿ���� ���󰡴� �ӵ�
	

}

CCameraManager::~CCameraManager()
{

}

void CCameraManager::init()
{
}

void CCameraManager::update()
{
	if (m_pTargetObj)
	{
		m_pTargetX = nullptr;
		m_pTargetY = nullptr;
		if (m_pTargetObj->isDead())
		{
			m_pTargetObj = nullptr;
		}
		else
		{
			SetLookAt(m_pTargetObj->GetPos());
		}
	}
	if (m_pTargetX)
	{
		m_pTargetObj=nullptr;
		m_pTargetY=nullptr;
		if (m_pTargetX->isDead())
		{
			m_pTargetX = nullptr;
		}
		else
		{
			Vec2 tempPos = m_pTargetX->GetPos();
			tempPos.y = m_fptPrevLookAt.y;
			SetLookAt(tempPos);
		}

	}
	if (m_pTargetY)
	{
		m_pTargetObj = nullptr;
		m_pTargetX = nullptr;
		if (m_pTargetY->isDead())
		{
			m_pTargetY = nullptr;
		}
		else
		{
			Vec2 tempPos = m_pTargetY->GetPos();
			tempPos.x = m_fptPrevLookAt.x;
			SetLookAt(tempPos);
		}
	}

	// ȭ�� �߾Ӱ� ī�޶� LookAt ��ǥ ������ ���� ���
	CalDiff();

	CD2DImage* pStage1BG = CResourceManager::getInst()->FindD2DImage(L"BGImg");
	
	//ī�޶� ���ȭ�� �ٱ��� ������ �ʵ���
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	if (pPlayer.size() > 0)
	{
		if (m_fptCurLookAt.x - WINSIZEX / 2 < 0)
		{
			m_fptCurLookAt.x = WINSIZEX / 2;
		}
		if (m_fptCurLookAt.x + WINSIZEX / 2 > pStage1BG->GetWidth())
		{
			m_fptCurLookAt.x = pStage1BG->GetWidth() - WINSIZEX / 2;
		}
		m_fptDiff = m_fptCurLookAt - Vec2(WINSIZEX / 2, WINSIZEY / 2);
	}	
}

void CCameraManager::render()
{
	if (m_listCamEffect.empty())
	{
		return;
	}
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;

	float fRatio = 0.f;
	fRatio = effect.fCurTime / effect.fDuration;
	if (fRatio < 0.f)
		fRatio = 0.f;
	else if (fRatio > 1.f)
		fRatio = 1.f;


	else if (CAM_EFFECT::FADE_IN == effect.m_eEffect)
		fRatio = 1.f - fRatio;

	CRenderManager::getInst()->RenderFillRectangle(0, 0, WINSIZEX, WINSIZEY, RGB(0, 0, 0), fRatio);
	if (effect.fDuration < effect.fCurTime)
	{
		m_listCamEffect.pop_front();
	}
}


void CCameraManager::SetLookAt(Vec2 lookAt)
{
	m_fptLookAt = lookAt;
	float fMoveDist = (m_fptLookAt - m_fptPrevLookAt).Length();

	m_fSpeed = fMoveDist / m_fTime;
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
	tCamEffect ef = {};
	ef.m_eEffect = CAM_EFFECT::FADE_IN;
	ef.fDuration = duration;
	ef.fCurTime = 0.f;

	m_listCamEffect.push_back(ef);

	if (0.f == duration)
	{
		assert(nullptr);
	}
}

void CCameraManager::FadeOut(float duration)
{
	tCamEffect ef = {};
	ef.m_eEffect = CAM_EFFECT::FADE_OUT;
	ef.fDuration = duration;
	ef.fCurTime = 0.f;

	m_listCamEffect.push_back(ef);

	if (0.f == duration)
	{
		assert(nullptr);
	}
}


void CCameraManager::CalDiff()
{
	m_fAccTime += fDT;

	// �ð��� ������, �����Ѱ����� ����
	if (m_fTime <= m_fAccTime)
	{
		m_fptCurLookAt = m_fptLookAt;
	}
	else
	{
		Vec2 fptCenter = Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f);

		m_fptCurLookAt = m_fptPrevLookAt + (m_fptLookAt - m_fptPrevLookAt).Normalize() * m_fSpeed * fDT;
		m_fptDiff = m_fptCurLookAt - fptCenter;
		m_fptPrevLookAt = m_fptCurLookAt;
	}
}