#include "pch.h"
#include "CCameraManager.h"
#include "CGameObject.h"
#include "CTexture.h"

CCameraManager::CCameraManager()
{
	m_fptLookAt = Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f); // ���� �ִ� ��ġ
	m_fptCurLookAt = m_fptLookAt;						// ī�޶� ���� ���� ��ġ
	m_fptPrevLookAt = m_fptLookAt;						// ī�޶� ������ ���� ��ġ
	m_pTargetObj = nullptr;								// Ʈ��ŷ �� ������Ʈ

	m_fptDiff = {};										// �ػ� �߽ɰ� ī�޶� LookAt ������ ����
	m_fTime = 2;										// Ÿ���� ���󰡴� �ѽð�
	m_fAccTime = m_fTime;								// Ÿ���� ���� �ҿ�ð�
	m_fSpeed = 0;										// Ÿ���� ���󰡴� �ӵ�
	m_fPreSpeed = m_fSpeed;								// Ÿ���� ���󰡴� �ӵ�
	m_fAccel = 0;										// Ÿ���� ���󰡴� ��ӵ�
	m_fAccDir = 1.f;									// ��ӵ��� ����

	m_pImg = nullptr;
}

CCameraManager::~CCameraManager()
{

}

void CCameraManager::init()
{
	m_pImg = CResourceManager::getInst()->CreateTexture(L"CameraTex", WINSIZEX, WINSIZEY);
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

	// ȭ�� �߾Ӱ� ī�޶� LookAt ��ǥ ������ ���� ���
	CalDiff();
}

void CCameraManager::render(HDC hDC)
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

	int iAlpha = 0;
	if (CAM_EFFECT::FADE_OUT == effect.m_eEffect)
		iAlpha = (int)(255.f * fRatio);
	else if (CAM_EFFECT::FADE_IN == effect.m_eEffect)
		iAlpha = (int)(255.f * (1.f - fRatio));

	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(hDC
		, 0, 0
		, (int)(m_pImg->GetBmpWidth())
		, (int)(m_pImg->GetBmpHeight())
		, m_pImg->GetDC()
		, 0, 0
		, (int)(m_pImg->GetBmpWidth())
		, (int)(m_pImg->GetBmpHeight())
		, bf);

	if (effect.fDuration < effect.fCurTime)
	{
		m_listCamEffect.pop_front();
	}
}


void CCameraManager::SetLookAt(Vec2 lookAt)
{
	m_fptLookAt = lookAt;
	float fMoveDist = (m_fptLookAt - m_fptPrevLookAt).Length();
	m_fAccel = fMoveDist / m_fTime;		// ��ӵ� ���
	m_fAccel *= m_fAccDir;

	if (m_fAccDir < 0) // �ʱ� �ӵ� ����
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

	Vec2 fvLookDir = m_fptLookAt - m_fptPrevLookAt;

	// �ð��� �����ų� ���� ������ ũ�Ⱑ 0�̶��, �����Ѱ����� ����. �ӵ��� 0����
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

	// ��ӵ���ŭ �����ӵ��� ������.
	m_fSpeed = m_fPreSpeed += m_fAccel * fDT;
}