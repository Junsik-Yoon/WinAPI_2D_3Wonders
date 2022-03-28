#include "pch.h"
#include "CChestnut.h"
#include "CAnimator.h"
#include "CCollider.h"

#define GRAVITY 400
#define VELOCITY 20
#define UPFORCE 600
CChestnut::CChestnut()
{
	SetHP(1);
	SetName(L"Chestnut");
	m_gravity = GRAVITY;
	m_velocity = VELOCITY;
	m_upforce = UPFORCE;
	isShaking = false;
	m_floor = 0;
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"ChestnutImg", L"texture\\chestnut.png");

	CreateCollider();
	SetScale(Vec2(20.f, 20.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));


	CreateAnimator();
	GetAnimator()->CreateAnimation(L"ChestNut", m_pImg, Vec2(0.f, 0.f), Vec2(48.f, 48.f), Vec2(48.f, 0.f), 10.f, 1, false);


	GetAnimator()->Play(L"ChestNut");
}

CChestnut::~CChestnut()
{
}

void CChestnut::update()
{
	Vec2 vPos = GetPos();

	if (!onTree)
	{
		vPos.y += m_gravity * fDT;

		if (m_floor == 0)
		{
			m_gravity = GRAVITY;
			m_upforce -= m_gravity * fDT;
			vPos.y -= m_upforce * fDT;
			vPos.x -= m_velocity * fDT;
		}
		else
		{
			m_upforce = UPFORCE;
			m_gravity = 0.f;
			vPos.y -= 2.f;

		}
	}


	SetPos(vPos);
	GetAnimator()->update();
	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(vPos);
	if (fptRenderPos.x < 0)
	{
		DeleteObj(this);
	}
}

void CChestnut::render()
{
	component_render();
}

void CChestnut::Shake()
{
	Vec2 vPos = GetPos();
	if (isShaking)
	{
		isShaking = !isShaking;
		vPos.x+=2.f;
		vPos.y+=2.f;
	}
	else
	{
		isShaking = !isShaking;
		vPos.x-=2.f;
		vPos.y-=2.f;
	}


}

void CChestnut::MoveForward()
{
}



void CChestnut::OnCollisionEnter(CCollider* pOther)
{
	if (pOther->GetObj()->GetName() == L"Tile")
	{
		++m_floor;
	}
	if (pOther->GetObj()->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
}

void CChestnut::OnCollision(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
		int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
		int sum = abs(a - b);
		if (1 < sum)
			--vPos.y;
	}
	SetPos(vPos);
}

void CChestnut::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Tile")
	{
			--m_floor;
	}
}
