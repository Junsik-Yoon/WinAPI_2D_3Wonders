#include "pch.h"
#include "CMissileItem.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include <iostream>
#include <random>

CMissileItem::CMissileItem()
{

	SetName(L"Missile_Item");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"MissileItemImg", L"texture\\Animation\\Animation_Missile_Item.png");
	CreateCollider();
	SetScale(Vec2(30.f, 30.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));

	CreateAnimator();


	GetAnimator()->CreateAnimation(L"Hyper", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 5, false);
	GetAnimator()->CreateAnimation(L"Bound", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 5, false);
	GetAnimator()->CreateAnimation(L"Tail", m_pImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 5, false);

}

CMissileItem::~CMissileItem()
{
}




void CMissileItem::update()
{
	switch (m_missileType)
	{
		case eType::HYPER:	GetAnimator()->Play(L"Hyper"); break;
		case eType::BOUND:	GetAnimator()->Play(L"Bound"); break;
		case eType::TAIL:	GetAnimator()->Play(L"Tail"); break;
	}
	GetAnimator()->update();
}

void CMissileItem::render()
{

	component_render();
}

void CMissileItem::SetRandomMissile()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 3);
	int ranNum = dis(gen);
	if (ranNum == (UINT)eType::BOUND)
	{
		SetMissileType(eType::BOUND);
		SetName(L"Bound");
	}
	else if (ranNum == (UINT)eType::HYPER)
	{
		SetMissileType(eType::HYPER);
		SetName(L"Hyper");
	}
	else if (ranNum == (UINT)eType::TAIL)
	{
		SetMissileType(eType::TAIL);
		SetName(L"Tail");
	}	
}
void CMissileItem::SetMissileType(eType _missileType)
{
	m_missileType = _missileType; 
	switch (m_missileType)
	{
	case eType::HYPER: SetName(L"Hyper");
		break;
	case eType::BOUND:	SetName(L"Bound");
		break;
	case eType::TAIL:	SetName(L"Tail");
		break;
	}
}

void CMissileItem::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Lou")
	{
		DeleteObj(this);
		//TODO: 없어지며 해당 미사일으로 변경하도록
	}
}

void CMissileItem::OnCollision(CCollider* _pOther)
{
}

void CMissileItem::OnCollisionExit(CCollider* _pOther)
{
}

