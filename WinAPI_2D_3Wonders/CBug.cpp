#include "pch.h"
#include "CBug.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CBug::CBug()
{
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	SetName(L"Bug");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonBugImg", L"texture\\Animation\\Animation_HalfmoonBug.png");

	CreateCollider();
	SetScale(Vec2(50.f, 50.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));


	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Fly_Left", m_pImg, Vec2(0.f, 256.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 2, false);
	GetAnimator()->CreateAnimation(L"Fly_Right", m_pImg, Vec2(0.f, 384.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 2, false);

	GetAnimator()->CreateAnimation(L"Land_Left", m_pImg, Vec2(256.f, 256.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.4f, 2, false);
	GetAnimator()->CreateAnimation(L"Land_Right", m_pImg, Vec2(256.f, 384.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.4f, 2, false);

	GetAnimator()->CreateAnimation(L"Jump_Left", m_pImg, Vec2(384.f, 256.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 5, false);
	GetAnimator()->CreateAnimation(L"Jump_Right", m_pImg, Vec2(384.f, 384.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 5, false);



	GetAnimator()->Play(L"Jump_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CBug::~CBug()
{

}


void CBug::update()
{
	Vec2 pos = GetPos();

	SetPos(pos);

	GetAnimator()->update();
}

void CBug::render()
{
	component_render();
}


void CBug::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
}

void CBug::OnCollision(CCollider* _pOther)
{
}

void CBug::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}

