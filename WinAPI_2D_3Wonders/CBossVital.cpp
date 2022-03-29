#include "pch.h"
#include "CBossVital.h"
#include "CAnimator.h"
#include "CGolemWood.h"

CBossVital::CBossVital()
	:CUI(false)
{


}

CBossVital::CBossVital(CGolemWood* _boss)
	:CUI(false)
{
	m_boss = _boss;
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"BossVitalImg", L"texture\\Animation\\Animation_BossVital.png");

	SetPos(Vec2(8700.f, 50.f));
	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Vital_1", m_pImg, Vec2(0.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_2", m_pImg, Vec2(260.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_3", m_pImg, Vec2(520.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_4", m_pImg, Vec2(780.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_5", m_pImg, Vec2(1040.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_6", m_pImg, Vec2(1300.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_7", m_pImg, Vec2(1560.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_8", m_pImg, Vec2(1820.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_9", m_pImg, Vec2(2080.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Vital_10", m_pImg, Vec2(2340.f, 0.f), Vec2(260.f, 37.f), Vec2(260.f, 0.f), 1.f, 1, false);


	GetAnimator()->Play(L"Vital_10");
}

CBossVital::~CBossVital()
{
	m_boss = nullptr;
}


void CBossVital::update()
{
	int bossHP = m_boss->GetHP();
	m_vital = bossHP / 5;
	switch (m_vital)
	{
	case 1:GetAnimator()->Play(L"Vital_1");
		break;
	case 2:GetAnimator()->Play(L"Vital_2");
		break;
	case 3:GetAnimator()->Play(L"Vital_3");
		break;
	case 4:GetAnimator()->Play(L"Vital_4");
		break;
	case 5:GetAnimator()->Play(L"Vital_5");
		break;
	case 6:GetAnimator()->Play(L"Vital_6");
		break;
	case 7:GetAnimator()->Play(L"Vital_7");
		break;
	case 8:GetAnimator()->Play(L"Vital_8");
		break;
	case 9:GetAnimator()->Play(L"Vital_9");
		break;
	case 10:GetAnimator()->Play(L"Vital_10");
		break;
	}
	GetAnimator()->update();
}

void CBossVital::render()
{
	component_render();
}


