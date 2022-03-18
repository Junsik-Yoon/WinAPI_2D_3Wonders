#include "pch.h"
#include "CGreen.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400
CGreen::CGreen()
{
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;

	SetName(L"Green");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GreenImg", L"texture\\Animation\\Animation_Green.png");
	CreateCollider();
	SetScale(Vec2(120.f, 120.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(0.f, 0.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 5,false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(0.f, 384.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 5,false);



	GetAnimator()->Play(L"Move_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

	
}

CGreen::~CGreen()
{
}


void CGreen::update()
{
	Vec2 pos = GetPos();

	SetPos(pos);

	GetAnimator()->update();
}

void CGreen::render()
{
	component_render();
}

void CGreen::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
}

void CGreen::OnCollision(CCollider* _pOther)
{
}

void CGreen::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}





