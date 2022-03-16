#include "pch.h"
#include "CGoblin.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400
CGoblin::CGoblin()
{
	m_floor=0;
	m_wall=0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;

	SetName(L"Goblin");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GoblinImg", L"texture\\Animation\\Animation_Goblin.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 13.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Create_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6);
	GetAnimator()->CreateAnimation(L"Create_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6);
	
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6);
	
	GetAnimator()->CreateAnimation(L"Laugh_Left", m_pImg, Vec2(128.f, 896.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2);
	GetAnimator()->CreateAnimation(L"Laugh_Right", m_pImg, Vec2(128.f, 1152.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2);

	GetAnimator()->CreateAnimation(L"Die_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5);
	GetAnimator()->CreateAnimation(L"Die_Right", m_pImg, Vec2(512.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5);


	GetAnimator()->Play(L"Die_Left");

	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CGoblin::~CGoblin()
{
}


void CGoblin::update()
{
	Vec2 pos = GetPos();

	SetPos(pos);

	GetAnimator()->update();
}

void CGoblin::render()
{
	component_render();
}

void CGoblin::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
}

void CGoblin::OnCollision(CCollider* _pOther)
{
}

void CGoblin::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}





