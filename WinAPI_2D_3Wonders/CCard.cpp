#include "pch.h"
#include "CCard.h"
#include "CAnimator.h"
#include "CCollider.h"

CCard::CCard()
{
}

CCard::CCard(int cardValue)
{
	m_dropDir = 0.f;
	m_gravity = 100.f;

	m_cardValue = cardValue;

	SetName(L"Card");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"CardsImg", L"texture\\Animation\\Animation_Cards.png");
	CreateCollider();
	SetScale(Vec2(30.f, 30.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Spin_1", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 6, false);
	GetAnimator()->CreateAnimation(L"Spin_2", m_pImg, Vec2(768.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 6, false);
	GetAnimator()->CreateAnimation(L"Spin_4", m_pImg, Vec2(1536.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 6, false);
	GetAnimator()->CreateAnimation(L"Spin_5", m_pImg, Vec2(2304.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 6, false);

	switch (m_cardValue)
	{
	case 1:GetAnimator()->Play(L"Spin_1");
		break;
	case 2:GetAnimator()->Play(L"Spin_2");
		break;
	case 4:GetAnimator()->Play(L"Spin_4");
		break;
	case 5:GetAnimator()->Play(L"Spin_5");
		break;
	default: GetAnimator()->Play(L"null_card");
		break;
	}

}

CCard::~CCard()
{
}


void CCard::update()
{
	Vec2 vPos = GetPos();
	
	vPos.x += m_dropDir * fDT;
	vPos.y += m_gravity * fDT;


	SetPos(vPos);
	GetAnimator()->update();
}

void CCard::render()
{
	component_render();
}

void CCard::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Lou")
	{
		DeleteObj(this);
	}
	if (_pOther->GetObj()->GetName() == L"Tile")
	{
		m_dropDir = 0.f;
		m_gravity = 0.f;
	}
}

void CCard::OnCollision(CCollider* _pOther)
{
}

void CCard::OnCollisionExit(CCollider* _pOther)
{
}
