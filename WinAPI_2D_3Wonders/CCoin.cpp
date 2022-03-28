#include "pch.h"
#include "CCoin.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CCoin::CCoin()
{
	create_motion = 0.f;
	SetName(L"Coin");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"CoinImg", L"texture\\Animation\\Animation_Coin.png");
	CreateCollider();
	SetScale(Vec2(30.f, 30.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Spin_Created", m_pImg, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 4, false);
	GetAnimator()->CreateAnimation(L"Spin", m_pImg, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.15f, 4, false);

	
}

CCoin::~CCoin()
{
}

void CCoin::update()
{
	if (create_motion < 0.4f)
	{
		create_motion += fDT;
		
		GetAnimator()->FindAnimation(L"Spin_Created")->SetOffset(Vec2(0.f, -30.f), 0);
		GetAnimator()->FindAnimation(L"Spin_Created")->SetOffset(Vec2(0.f, -50.f), 1);
		GetAnimator()->FindAnimation(L"Spin_Created")->SetOffset(Vec2(0.f, -70.f), 2);
		GetAnimator()->FindAnimation(L"Spin_Created")->SetOffset(Vec2(0.f, -50.f), 3);
		GetAnimator()->Play(L"Spin_Created");

	}
	if (create_motion >= 0.4f)
	{
		create_motion = 1.f;
		GetAnimator()->Play(L"Spin");
	}
	GetAnimator()->update();
}

void CCoin::render()
{
	component_render();
}

void CCoin::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Lou")
	{
		CSoundManager::getInst()->Play(L"item_change");
		DeleteObj(this);
		//TODO: 없어지며 점수숫자 애니메이션이 나오면서 스코어가 오른다
	}
}

void CCoin::OnCollision(CCollider* _pOther)
{
}

void CCoin::OnCollisionExit(CCollider* _pOther)
{
}


