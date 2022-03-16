#include "pch.h"
#include "CLou.h"
#include "CScene.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_GRAVITY 800
#define D_VELOCITY 200
#define D_UPFORCE 400

float CLou::sCountTime = 0.f;

CLou::CLou()
{

	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	m_upforce = D_UPFORCE;
	isFacedRight = true;
	m_counter_toggle = false;

	SetName(L"Lou");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"PlayerImg", L"texture\\Animation\\Animation_Lou.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 13.f));

	CreateAnimator();
	//¿Ê ÀÔÀ½
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pImg, Vec2(0.f, 0.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.5f, 4);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pImg, Vec2(106.f, 0.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.5f, 4);

	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(0.f, 424.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 6);
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(106.f, 424.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 6);
	
	GetAnimator()->CreateAnimation(L"Jump_Right_U", m_pImg, Vec2(212.f, 0.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 1);
	GetAnimator()->CreateAnimation(L"Jump_Left_U", m_pImg, Vec2(318.f, 0.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 1);
	GetAnimator()->CreateAnimation(L"Jump_Right_D", m_pImg, Vec2(212.f, 106.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 1);
	GetAnimator()->CreateAnimation(L"Jump_Left_D", m_pImg, Vec2(318.f, 106.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 1);
	GetAnimator()->CreateAnimation(L"Jump_Right_Onland", m_pImg, Vec2(318.f, 212.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 1);
	GetAnimator()->CreateAnimation(L"Jump_Left_Onland", m_pImg, Vec2(318.f, 318.f), Vec2(106.f, 106.f), Vec2(0.f, 106.f), 0.2f, 1);
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
		isFacedRight = false;
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
		isFacedRight = true;


		GetAnimator()->Play(L"Move_Right");

	}
	if (KEYUP(VK_RIGHT))
	{
		GetAnimator()->Play(L"Idle_Right");
	}
	if (m_floor > 0)
	{
		if (true == isFacedRight)
		{
			GetAnimator()->Play(L"Idle_Right");
		}
		else
		{
			GetAnimator()->Play(L"Idle_Left");
		}
		isAir = false;
		m_gravity = 0;
		m_upforce = D_UPFORCE;
		if (KEYDOWN(VK_UP))
		{
			--pos.y;
			isUpside = true;
			isAir = true;
		}
	}
	if (m_floor == 0 && true == isUpside)
	{
		if (false == isFacedRight)
		{
			GetAnimator()->Play(L"Jump_Left_U");
		}
		if (true == isFacedRight)
		{
			GetAnimator()->Play(L"Jump_Right_U");
		}
		m_gravity = D_GRAVITY;
		m_upforce -= m_gravity * fDT;
		pos.y -= m_upforce * fDT;

		if (m_upforce <= 0.f)
		{
			isUpside = false;
		}
	}
	else if (m_floor == 0 && false == isUpside)
	{
		if (false == isFacedRight)
		{
			GetAnimator()->Play(L"Jump_Left_D");
		}
		if (true == isFacedRight)
		{
			GetAnimator()->Play(L"Jump_Right_D");
		}
		m_gravity = D_GRAVITY;
		m_upforce += m_gravity * fDT;
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

void CLou::render()
{
	component_render();
}


