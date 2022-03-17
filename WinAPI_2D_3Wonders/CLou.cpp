#include "pch.h"
#include "CLou.h"
#include "CScene.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CMissile.h"

#define D_GRAVITY 800
#define D_VELOCITY 200
#define D_UPFORCE 400

float CLou::sCountTime = 0.f;

CLou::CLou()
{
	m_facing = D_FACING::RIGHT;
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
	//옷 입음
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);

	GetAnimator()->CreateAnimation(L"Still_Right", m_pImg, Vec2(0.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);
	GetAnimator()->CreateAnimation(L"Still_Left", m_pImg, Vec2(128.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);

	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(0.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(128.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	
	GetAnimator()->CreateAnimation(L"Lookup_Right", m_pImg, Vec2(768.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Lookup_Left", m_pImg, Vec2(896.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Sit_Right", m_pImg, Vec2(768.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Sit_Left", m_pImg, Vec2(896.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);

	GetAnimator()->CreateAnimation(L"pShoot_Right", m_pImg, Vec2(768.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.05f, 3, false);
	GetAnimator()->CreateAnimation(L"pShoot_Left", m_pImg, Vec2(896.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.05f, 3, false,false);


	GetAnimator()->CreateAnimation(L"Jump_Right_U", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_U", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_D", m_pImg, Vec2(256.f, 128.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_D", m_pImg, Vec2(384.f, 128.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Onland", m_pImg, Vec2(384.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Onland", m_pImg, Vec2(384.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	//탈의

	GetAnimator()->Play(L"Idle_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CLou::~CLou()
{
}


void CLou::update()
{
	if (KEY('C'))
	{
		CCameraManager::getInst()->SetTargetObj(this);
	}
	Vec2 pos = GetPos();
	if (KEY(VK_UP))
	{
		m_facing = D_FACING::UP;
		if (true == isFacedRight)
		{
			GetAnimator()->Play(L"Lookup_Right");
		}
		else
		{
			GetAnimator()->Play(L"Lookup_Left");
		}
	}
	if (KEYUP(VK_UP) && true == isFacedRight)
	{
		GetAnimator()->Play(L"Idle_Right");
		m_facing = D_FACING::RIGHT;
	}
	else if(KEYUP(VK_UP) && false == isFacedRight)
	{
		GetAnimator()->Play(L"Idle_Left");
		m_facing = D_FACING::LEFT;
	}//TODO:keyup 다 묶기
	if (KEY(VK_DOWN))
	{
		m_facing = D_FACING::DOWN;
		if (true == isFacedRight)
		{
			GetAnimator()->Play(L"Sit_Right");
		}
		else
		{
			GetAnimator()->Play(L"Sit_Left");
		}
	}
	if (KEYUP(VK_DOWN) && true == isFacedRight)
	{
		GetAnimator()->Play(L"Idle_Right");
		m_facing = D_FACING::RIGHT;
	}
	else if(KEYUP(VK_DOWN)&&false==isFacedRight)
	{
		GetAnimator()->Play(L"Idle_Left");
		m_facing = D_FACING::LEFT;
	}
	if (KEYDOWN('Z'))
	{//TODO:모션 한 번 취하고 멈추도록 play 한번만하게(타이머말고)방법생각하기
		//TODO: 이동중에 공격하면 이동애니메이션 멈추고 공격애니메이션으로 전환할 방법

		CreateMissile();
	}
	if (KEY(VK_LEFT))//왼키누르다떼기
	{
		m_facing = D_FACING::LEFT;
		isFacedRight = false;
		pos.x -= m_velocity * fDT;
		GetAnimator()->Play(L"Move_Left");		
	}
	if (KEYUP(VK_LEFT))
	{
		GetAnimator()->Play(L"Idle_Left");
	}

	if (KEY(VK_RIGHT))//오른키누르다떼기
	{
		pos.x += m_velocity * fDT;
		m_facing = D_FACING::RIGHT;
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
			//GetAnimator()->Play(L"Idle_Right");
		}
		else
		{
			//GetAnimator()->Play(L"Idle_Left");
		}
		isAir = false;
		m_gravity = 0;
		m_upforce = D_UPFORCE;
		if (KEYDOWN('X'))
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


void CLou::CreateMissile()
{
	Vec2 fpMissilePos = GetPos();
	fpMissilePos.x += GetScale().x / 2.f;

	// Misiile Object
	CMissile* pMissile = new CMissile;
	
	switch(m_facing)
	{
	case D_FACING::RIGHT:
		{
			fpMissilePos.x += 50;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetAnimator()->Play(L"Shoot_Right");
			pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
			pMissile->SetDir(Vec2(1, 0));
			
		}break;
	case D_FACING::LEFT:
		{//TODO:충돌체offset때문에 조금만 피격체가 가까이있으면 애니메이션이 안보임
			fpMissilePos.x -= 110;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetAnimator()->Play(L"Shoot_Left");
			pMissile->GetCollider()->SetOffsetPos(Vec2(-30.f, 0.f));
			pMissile->SetDir(Vec2(-1, 0));

		}break;
	case D_FACING::UP:
		{
		//TODO:이미지넣고 코드수정
			fpMissilePos.y -= 50;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetAnimator()->Play(L"Shoot_Right");
			pMissile->GetCollider()->SetOffsetPos(Vec2(0.f, -30.f));
			pMissile->SetDir(Vec2(0, -1));
		}break;
	case D_FACING::DOWN:
		{

			if (0 == m_floor)
			{
				fpMissilePos.y += 50;
				pMissile->SetPos(fpMissilePos);
				pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
				pMissile->GetAnimator()->Play(L"Shoot_Right");
				pMissile->GetCollider()->SetOffsetPos(Vec2(0.f, 30.f));
				pMissile->SetDir(Vec2(0, 1));
				
			}
			else
			{

				if (isFacedRight)
				{
					fpMissilePos.x += 50;
					fpMissilePos.y += 20;
					pMissile->SetPos(fpMissilePos);
					pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
					pMissile->GetAnimator()->Play(L"Shoot_Right");
					pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
					pMissile->SetDir(Vec2(1, 0));
				}
				else
				{
					fpMissilePos.x -= 110;
					fpMissilePos.y += 20;
					pMissile->SetPos(fpMissilePos);
					pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
					pMissile->GetAnimator()->Play(L"Shoot_Right");
					pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
					pMissile->SetDir(Vec2(-1, 0));
				}
			}
		}break;		
	}
	pMissile->SetName(L"Missile_Player");

	CreateObj(pMissile, GROUP_GAMEOBJ::MISSILE_PLAYER);
}

void CLou::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();

	Vec2 plColSize = GetCollider()->GetScale();
	Vec2 oColSize = _pOther->GetScale();

	if (pOther->GetName() == L"Tile"&&(plColSize.x + oColSize.x -4)/ 2.f >= abs(GetCollider()->GetFinalPos().x - _pOther->GetFinalPos().x) && (plColSize.y + oColSize.y) > abs(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y))
	{

		
		//if (plColSize.x / 2.f + oColSize.x / 2.f >= abs(GetPos().x + pOther->GetPos().x))
		//{
			Vec2 tPos = GetPos();
			if (true == isFacedRight)
			{
				tPos.x -= 2.f;
			}
			else
			{
				tPos.x += 2.f;
			}
			SetPos(tPos);
	}
	else if (pOther->GetName() == L"Tile" && (plColSize.y + oColSize.y - 2) <= abs(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
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


