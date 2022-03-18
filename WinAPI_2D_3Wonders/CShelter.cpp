#include "pch.h"
#include "CShelter.h"
#include "CCollider.h"
#include "CAnimator.h"

CShelter::CShelter()
{
	SetName(L"Tile");
	SetHP(4);
	SetGroup(GROUP_TILE::WALL);
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"ShelterImg", L"texture\\Animation\\Animation_Shelter.png");
	CreateCollider();
	SetScale(Vec2(200.f, 200.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	CreateAnimator();

	GetAnimator()->CreateAnimation(L"HP4", m_pImg, Vec2(0.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP3", m_pImg, Vec2(384.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP2", m_pImg, Vec2(768.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP1", m_pImg, Vec2(1152.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP0", m_pImg, Vec2(1536.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);


	GetAnimator()->Play(L"HP4");

	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CShelter::~CShelter()
{
}


void CShelter::update()
{
	int hp = GetHP();
	switch (hp)
	{
	case 4:
		GetAnimator()->Play(L"HP4");
		break;
	case 3:
		GetAnimator()->Play(L"HP3");
		break;
	case 2:
		GetAnimator()->Play(L"HP2");
		break;
	case 1:
		GetAnimator()->Play(L"HP1");
		break;
	case 0:
		GetAnimator()->Play(L"HP0");
		break;
	}
	GetAnimator()->update();
}

void CShelter::render()
{
	component_render();
}

void CShelter::OnCollisionEnter(CCollider* _pOther)
{
	int hp = GetHP();
	if(0<hp) SetHP(--hp);
}

void CShelter::OnCollision(CCollider* _pOther)
{
}

void CShelter::OnCollisionExit(CCollider* _pOther)
{
}
