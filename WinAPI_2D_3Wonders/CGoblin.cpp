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
	SetHP(2);
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
	GetAnimator()->CreateAnimation(L"Create_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6,false);
	GetAnimator()->CreateAnimation(L"Create_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6,false);
	
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	
	GetAnimator()->CreateAnimation(L"Laugh_Left", m_pImg, Vec2(128.f, 896.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);
	GetAnimator()->CreateAnimation(L"Laugh_Right", m_pImg, Vec2(128.f, 1152.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);

	GetAnimator()->CreateAnimation(L"Die_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);
	GetAnimator()->CreateAnimation(L"Die_Right", m_pImg, Vec2(512.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);


	GetAnimator()->Play(L"Die_Left");
	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CGoblin::~CGoblin()
{
}


void CGoblin::update()
{
	Vec2 vPos = GetPos();

	vector<CGameObject*> pPlayer=CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	if (pPlayer[0]->GetPos().x >= GetPos().x)
	{
		vPos.x += 50.f * fDT;
		GetAnimator()->Play(L"Move_Right");
	}
	else
	{
		vPos.x -= 50.f * fDT;
		GetAnimator()->Play(L"Move_Left");
	}
	//TODO: 플레이어와 좌표 x차이(절대값)가 일정 이하이고 y축이 위나 아래인 경우 고블린이 점프해서 플레이어를 찌르거나 점프해서 내려오는 것 구현

	SetPos(vPos);

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
	if (pOther->GetName() == L"Lou")
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Laugh_Right");
		}//TODO:타이머나 다른 것을 사용해서 다 웃고 다음행동하게하기
		else
		{
			GetAnimator()->Play(L"Laugh_Left");
		}
	}
	//if (pOther->GetName() == L"Missile_Player");
	//{
	//	int hp = GetHP();
	//	SetHP(--hp);
	//}
	if (GetHP() <= 0)
	{
		DeleteObj(this);
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





