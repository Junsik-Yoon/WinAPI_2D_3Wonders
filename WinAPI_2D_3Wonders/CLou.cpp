#include "pch.h"
#include "CLou.h"
#include "CScene.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_GRAVITY 200
#define D_VELOCITY 200
#define D_UPFORCE 200
CLou::CLou()
{

	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	m_upforce = D_UPFORCE;

	SetName(L"Lou");
	m_pTex = CResourceManager::getInst()->LoadTexture(L"PlayerTex", L"texture\\Animation\\Animation_Lou.bmp");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 13.f));

	CreateAnimator();
	//¿Ê ÀÔÀ½
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pTex, Vec2(0.f, 0.f), Vec2(106.f, 106.f), Vec2(106.f, 0.f), 0.5f, 6);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pTex, Vec2(0.f, 106.f), Vec2(106.f, 106.f), Vec2(106.f, 0.f), 0.5f, 6);

	GetAnimator()->CreateAnimation(L"Move_Right", m_pTex, Vec2(0.f, 212.f), Vec2(106.f, 106.f), Vec2(106.f, 0.f), 0.2f, 6);
	GetAnimator()->CreateAnimation(L"Move_Left", m_pTex, Vec2(0.f, 318.f), Vec2(106.f, 106.f), Vec2(106.f, 0.f), 0.2f, 6);
	//Å»ÀÇ

	GetAnimator()->Play(L"Idle_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CLou::~CLou()
{
}


void CLou::update()
{
	Vec2 pos = GetPos();

	if (KEY(VK_LEFT))//¿ÞÅ°´©¸£´Ù¶¼±â
	{
		pos.x -= m_velocity * fDT;
		GetAnimator()->Play(L"Move_Left");
	}
	if (KEYUP(VK_LEFT))
	{
		GetAnimator()->Play(L"Idle_Left");
	}
	if (KEY(VK_RIGHT))//¿À¸¥Å°´©¸£´Ù¶¼±â
	{
		pos.x += m_velocity * fDT;
		GetAnimator()->Play(L"Move_Right");
	}
	if (KEYUP(VK_RIGHT))
	{
		GetAnimator()->Play(L"Idle_Right");
	}
	if (m_floor > 0)
	{
		if (KEYDOWN(VK_UP))
		{
			isJump = true;
		}
		m_gravity = 0;
		m_upforce = D_UPFORCE;
	}
	if (m_floor == 0 && true == isJump)
	{
		m_gravity = D_GRAVITY;
		m_upforce -= m_gravity * fDT;
		pos.y -= m_upforce * fDT;
		if (m_upforce <= 0.f)
		{
			isJump = false;
		}
	}
	else if (m_floor == 0 && false == isJump)
	{
		m_gravity = D_GRAVITY;
		m_upforce -= m_gravity * fDT;
		pos.y += m_upforce * fDT;
	}





	SetPos(pos);

	GetAnimator()->update();
}


void CLou::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
}

void CLou::OnCollision(CCollider* _pOther)
{
}

void CLou::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}

void CLou::render(HDC hDC)
{
	component_render(hDC);
}


