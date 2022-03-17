#include "pch.h"
#include "CHalfMoon.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CHalfMoon::CHalfMoon()
{
	
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	SetName(L"Halfmoon");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonBugImg", L"texture\\Animation\\Animation_HalfmoonBug.png");

	CreateCollider();
	SetScale(Vec2(60.f, 100.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));


	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pImg, Vec2(0.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);

	GetAnimator()->CreateAnimation(L"Summon_Right", m_pImg, Vec2(128.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 9, false);
	GetAnimator()->CreateAnimation(L"Summon_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 9, false);



	GetAnimator()->Play(L"Summon_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CHalfMoon::~CHalfMoon()
{
}

void CHalfMoon::update()
{
	Vec2 pos = GetPos();

	SetPos(pos);

	GetAnimator()->update();
}

void CHalfMoon::render()
{
	component_render();
}


void CHalfMoon::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
}

void CHalfMoon::OnCollision(CCollider* _pOther)
{
}

void CHalfMoon::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}

