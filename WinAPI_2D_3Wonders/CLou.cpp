#include "pch.h"
#include "CLou.h"
#include "CScene.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CMissile.h"
#include "CTile.h"
#include "CGoblin.h"
#include "CD2DImage.h"

#include <iostream>
#include <random>
//1:800,400 / 2:900,500 / 3: 1000,600
#define D_GRAVITY 400
#define D_DOWNFORCE 700
#define D_VELOCITY 200
#define D_UPFORCE 800
#define D_FROMGOBMAX 600
#define D_FROMGOBMIN 100


#define SETSTATE m_state = eState
float CLou::sCountTime = 0.f;

CLou::CLou()
{

	dash = 0.f;
	bGravity = true;
	m_state = eState::IDLE;
	m_goblinCounter = 0.f;
	m_facing = D_FACING::RIGHT;
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	m_upforce = D_UPFORCE;
	isFacedRight = true;
	m_counter_toggle = false;


	SetHP(2);
	prevHP = GetHP();
	prevY = GetPos().y;

	SetName(L"Lou");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"PlayerImg", L"texture\\Animation\\Animation_Lou.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, -10.f));

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

	GetAnimator()->CreateAnimation(L"pShoot_Right", m_pImg, Vec2(768.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.05f, 3, false,false);
	GetAnimator()->CreateAnimation(L"pShoot_Left", m_pImg, Vec2(896.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.05f, 3, false,false);


	GetAnimator()->CreateAnimation(L"Jump_Right_U", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_U", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_D", m_pImg, Vec2(256.f, 128.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_D", m_pImg, Vec2(384.f, 128.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Onland", m_pImg, Vec2(384.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Onland", m_pImg, Vec2(384.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Look_D", m_pImg, Vec2(256.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Look_D", m_pImg, Vec2(384.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Look_U", m_pImg, Vec2(256.f, 768.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Look_U", m_pImg, Vec2(384.f, 768.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);


	GetAnimator()->CreateAnimation(L"Dash_Right", m_pImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.06f, 5, false);
	GetAnimator()->CreateAnimation(L"Dash_Left", m_pImg, Vec2(1408.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.06f, 5, false);



	//탈의

	GetAnimator()->Play(L"Idle_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CLou::~CLou()
{
}


void CLou::update()
{

	wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"state : %d \t FaceRight : %d \t Direc : %d", m_state,isFacedRight,m_facing);
	SetWindowText(hWnd, szBuffer);

	update_move();
	update_animation();
}


void CLou::update_move()
{


	Vec2 vPos = GetPos();
	

	//////////////////////////////////////
	//////////////////////////////////////
	//모션,기능 시험용
	if (KEYDOWN('Q'))
	{
		//GenerateGoblin();
		GetAnimator()->Play(L"pShoot_Right");
		//m_floor = 0;
	}
	//////////////////////////////////////
	//카메라 캐릭터 target
	if (KEYDOWN('V'))
	{
		CCameraManager::getInst()->SetTargetX(this);
	}
	m_goblinCounter += fDT;
	//n초마다 고블린 리젠
	if (m_goblinCounter >= 5.f)
	{
		m_goblinCounter = 0.f;
		//GenerateGoblin();
	}
	//////////////////////////////////////
	//////////////////////////////////////

	if (m_floor > 0)	//충돌on
	{
		m_gravity = 0.f;
		m_upforce = 0.f;
	}
	else				//충돌off
	{
		m_gravity = D_GRAVITY;
		vPos.y += D_GRAVITY * fDT;
	}

	if (m_state == eState::IDLE)
	{
		//if (m_upforce <= 0.f)
			//&&0==m_floor)
		//{
		//	SETSTATE::FALL;
		//}
		if (KEY(VK_UP))
		{
			m_facing = D_FACING::UP;
		}
		if (KEYUP(VK_UP))
		{
			if (isFacedRight)
			{
				m_facing=D_FACING::RIGHT;
			}
			else
			{
				m_facing=D_FACING::LEFT;
			}
		}
		if (KEY(VK_DOWN))
		{
			SETSTATE::SIT;
		}
		if (KEY(VK_RIGHT))
		{
			isFacedRight = true;
			SETSTATE::LANDMOVE;
		}
		if (KEYUP(VK_RIGHT))
		{
			isFacedRight = true;
		}

		if (KEY(VK_LEFT))
		{
			isFacedRight = false;
			SETSTATE::LANDMOVE;
		}
		if (KEYUP(VK_LEFT))
		{
			isFacedRight = false;
		}

		if (KEYDOWN('X'))
		{
			m_upforce = D_UPFORCE;
			vPos.y -= 2.f;
			SETSTATE::JUMP;
		}
		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}
		if (prevHP > GetHP())
		{
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			SETSTATE::DEAD;
		}

	}
	if (m_state == eState::SIT)
	{

		GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y-30.f));
		GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, -20.f),0);
		GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, -20.f), 0);
	
		if (KEYDOWN(VK_RIGHT))
		{
			isFacedRight = true;
		}
		if (KEYDOWN(VK_LEFT))
		{
			isFacedRight = false;
		}
		if (KEYUP(VK_DOWN))
		{
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			SETSTATE::IDLE;
		}
		if (KEYDOWN(VK_UP))
		{
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			SETSTATE::IDLE;
		}
		if (KEYDOWN('X'))
		{
			SETSTATE::DASH;
		}
		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}
		if (prevHP > GetHP())
		{
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			SETSTATE::DEAD;
		}
	}
	if (m_state == eState::LANDMOVE)
	{
		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}

		if (KEY(VK_RIGHT))
		{
			isFacedRight = true;
			m_facing = D_FACING::RIGHT;
			vPos.x += m_velocity * fDT;
		}
		if (KEY(VK_LEFT))
		{
			isFacedRight = false;
			m_facing = D_FACING::LEFT;
			vPos.x -= m_velocity * fDT;
		}
		if (KEYUP(VK_RIGHT))
		{
			isFacedRight = true;
			m_facing = D_FACING::RIGHT;
			SETSTATE::IDLE;
		}
		if (KEYUP(VK_LEFT))
		{
			isFacedRight = false;
			m_facing = D_FACING::LEFT;
			SETSTATE::IDLE;
		}

		if (KEYDOWN('X'))
		{
			m_upforce = D_UPFORCE;
			vPos.y -= 2.f;
			SETSTATE::JUMP;
		}

		if (prevHP > GetHP())
		{
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			SETSTATE::DEAD;
		}
	}
	if (m_state == eState::JUMP)
	{
		m_upforce -= (D_DOWNFORCE*fDT);
		vPos.y -= m_upforce * fDT;

		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}
		if (KEY(VK_RIGHT))
		{
			if (KEY(VK_DOWN))
			{
				m_facing = D_FACING::DOWN;
			}
			else if (KEY(VK_UP))
			{
				m_facing = D_FACING::UP;
			}
			else
			{
				isFacedRight = true;
				m_facing = D_FACING::RIGHT;
				vPos.x += m_velocity * fDT;
			}
		}
		if (KEY(VK_LEFT))
		{
			if (KEY(VK_DOWN))
			{
				m_facing = D_FACING::DOWN;
			}
			else if (KEY(VK_UP))
			{
				m_facing = D_FACING::UP;
			}
			else
			{
				isFacedRight = false;
				m_facing = D_FACING::LEFT;
				vPos.x -= m_velocity * fDT;
			}

		}
		if (KEYUP(VK_RIGHT))
		{
			isFacedRight = true;
			//m_facing = D_FACING::RIGHT;
		}
		if (KEYUP(VK_LEFT))
		{
			isFacedRight = false;
			//m_facing = D_FACING::LEFT;
		}

		if (KEYDOWN(VK_UP))
		{
			m_facing = D_FACING::UP;
		}
		if (KEYUP(VK_UP))
		{
			if (isFacedRight)
			{
				m_facing = D_FACING::RIGHT;
			}
			else
			{
				m_facing = D_FACING::LEFT;
			}
		}
		if (KEYDOWN(VK_DOWN))
		{
			m_facing = D_FACING::DOWN;
		}
		if (KEYUP(VK_DOWN))
		{
			if (isFacedRight)
			{
				m_facing = D_FACING::RIGHT;
			}
			else
			{
				m_facing = D_FACING::LEFT;
			}
		}


		if (prevHP > GetHP())
		{
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			SETSTATE::DEAD;
		}
		if (m_upforce <= 0.f)
		{
			SETSTATE::FALL;
		}
	}
	if (m_state == eState::FALL)
	{
		if (KEY(VK_RIGHT))
		{
			isFacedRight = true;
			m_facing = D_FACING::RIGHT;
			vPos.x += m_velocity * fDT;
		}
		if (KEY(VK_LEFT))
		{
			isFacedRight = false;
			m_facing = D_FACING::LEFT;
			vPos.x -= m_velocity * fDT;
		}
		if (KEYUP(VK_RIGHT))
		{
			isFacedRight = true;
			m_facing = D_FACING::RIGHT;
		}
		if (KEYUP(VK_LEFT))
		{
			isFacedRight = false;
			m_facing = D_FACING::LEFT;
		}
		if (prevHP > GetHP())
		{
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			SETSTATE::DEAD;
		}
		if (0 == m_upforce && m_floor >0)
		{
			SETSTATE::IDLE;
		}
		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}
	}

	if (m_state == eState::HOLDCLIFF)
	{
		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}
		if (KEYDOWN('X'))
		{
			//TODO:타고올라가게
		}
		if (prevHP > GetHP())
		{
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			SETSTATE::DEAD;
		}
	}
	if (m_state == eState::DASH)
	{
		dash += fDT;
		if (dash >= 0.3f)
		{
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			SETSTATE::IDLE;
			dash = 0.f;
		}
		if (isFacedRight)
		{
			vPos.x += (D_VELOCITY * 1.5f * fDT);
		}
		else
		{
			vPos.x -= (D_VELOCITY * 1.5f * fDT);
		}
		if (prevHP > GetHP())
		{
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			prevHP = GetHP();
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			SETSTATE::DEAD;
		}
	}
	if (m_state == eState::GOTHIT)
	{
		int hp = GetHP();
		SetHP(--hp);
		if (0 >= hp)
		{
			SETSTATE::DEAD;
		}
	}
	if (m_state == eState::DEAD)
	{

	}


	SetPos(vPos);

}
void CLou::update_animation()
{

	switch (m_state)
	{
		case eState::IDLE:
		{
			if (isFacedRight)
			{
				if (KEY(VK_UP))
				{
					GetAnimator()->Play(L"Lookup_Right");
				}
				else
				{
					GetAnimator()->Play(L"Idle_Right");
				}
			}
			else
			{
				if (KEY(VK_UP))
				{
					GetAnimator()->Play(L"Lookup_Left");
				}
				else
				{
					GetAnimator()->Play(L"Idle_Left");
				}
			}
		}break;
		case eState::SIT:
		{
			if (isFacedRight)
			{
				GetAnimator()->Play(L"Sit_Right");
			}
			else
			{
				GetAnimator()->Play(L"Sit_Left");
			}
		}break;
		case eState::JUMP:
		{
			if (isFacedRight)
			{
				if(KEY(VK_DOWN))
				{
					GetAnimator()->Play(L"Jump_Right_Look_D");
				}
				else if (KEY(VK_UP))
				{
					GetAnimator()->Play(L"Jump_Right_Look_U");
				}
				else
				{
					if (prevY < GetPos().y)
						GetAnimator()->Play(L"Jump_Right_D");
					else
						GetAnimator()->Play(L"Jump_Right_U");
				}
			}
			else
			{
				if (KEY(VK_DOWN))
				{
					GetAnimator()->Play(L"Jump_Left_Look_D");
				}
				else if (KEY(VK_UP))
				{
					GetAnimator()->Play(L"Jump_Left_Look_U");
				}
				else
				{
					if (prevY < GetPos().y)
						GetAnimator()->Play(L"Jump_Left_D");
					else
						GetAnimator()->Play(L"Jump_Left_U");
				}
			}

		}break;
		case eState::FALL:
		{
			if (isFacedRight)
			{
				GetAnimator()->Play(L"Jump_Right_D");
			}
			else
			{
				GetAnimator()->Play(L"Jump_Left_D");
			}
		}break;
			
		case eState::LANDMOVE:
		{
			if (true == isFacedRight)
			{
				GetAnimator()->Play(L"Move_Right");
			}
			else if (false == isFacedRight)
			{
				GetAnimator()->Play(L"Move_Left");
			}
		}break;
		case eState::HOLDCLIFF:
		{

		}break;
		case eState::DASH:
		{
			if (true == isFacedRight)
			{
				GetAnimator()->Play(L"Dash_Right");
			}
			else if (false == isFacedRight)
			{
				GetAnimator()->Play(L"Dash_Left");
			}
		}break;
	}
	
	prevY = GetPos().y;
	GetAnimator()->update();
}



void CLou::CreateMissile()
{
	Vec2 fpMissilePos = GetPos();

	// Misiile Object
	CMissile* pMissile = new CMissile;
	
	switch(m_facing)
	{
	case D_FACING::RIGHT:
		{
			fpMissilePos.x += 50;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetAnimator()->Play(L"N_Shoot_Right");
			pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
			pMissile->SetDir(Vec2(1, 0));
			
		}break;
	case D_FACING::LEFT:
		{//TODO:충돌체offset때문에 조금만 피격체가 가까이있으면 애니메이션이 안보임
			fpMissilePos.x -= 50;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetAnimator()->Play(L"N_Shoot_Left");
			pMissile->GetCollider()->SetOffsetPos(Vec2(-30.f, 0.f));
			pMissile->SetDir(Vec2(-1, 0));

		}break;
	case D_FACING::UP:
		{
			fpMissilePos.y -= 50;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetAnimator()->Play(L"N_Shoot_Up");
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
				pMissile->GetAnimator()->Play(L"N_Shoot_Down");
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
					pMissile->GetAnimator()->Play(L"N_Shoot_Right");
					pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
					pMissile->SetDir(Vec2(1, 0));
				}
				else
				{
					fpMissilePos.x -= 110;
					fpMissilePos.y += 20;
					pMissile->SetPos(fpMissilePos);
					pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
					pMissile->GetAnimator()->Play(L"N_Shoot_Left");
					pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
					pMissile->SetDir(Vec2(-1, 0));
				}
			}
		}break;		
	}
	pMissile->SetName(L"Missile_Player");

	CreateObj(pMissile, GROUP_GAMEOBJ::MISSILE_PLAYER);
}

void CLou::GenerateGoblin()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	vector<CTile*>pTiles;

	Vec2 vPos = GetPos();
	
	vector<CGameObject*>pObj = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::TILE);
	for (int i = 0; i < pObj.size(); ++i)
	{
		CTile* pTile = (CTile*)pObj[i];
		if (D_FROMGOBMAX > abs(vPos.x - pTile->GetPos().x)&&
			D_FROMGOBMIN < abs(vPos.x - pTile->GetPos().x))
		{
			if (pTile->GetGroup() == GROUP_TILE::GROUND)
			{
				pTiles.push_back(pTile);
			}		
		}
	}
	std::uniform_int_distribution<int> dis(0, pTiles.size()-1);
	CGameObject* pTile = pTiles[dis(gen)];
	Vec2 tilePos = pTile->GetPos();
	
	CGoblin* pGoblin = new CGoblin();
	tilePos.y -= (pGoblin->GetScale().y / 2.f + pTile->GetScale().y / 2.f);
	pGoblin->SetPos(tilePos);
	if (pGoblin->GetPos().x < GetPos().x)
	{
		pGoblin->GetAnimator()->Play(L"Create_Right");
		pGoblin->SetFacedRight(true);
	}
	else
	{
		pGoblin->GetAnimator()->Play(L"Create_Left");
		pGoblin->SetFacedRight(false);
	}
	CreateObj(pGoblin, GROUP_GAMEOBJ::MONSTER);
}

void CLou::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();

	if (pOther->GetName() == L"Tile")
	{
		Vec2 vPos = GetPos();

		CTile* pTile = (CTile*)pOther;
		GROUP_TILE tileType =pTile->GetGroup();

		switch (tileType)
		{
		case GROUP_TILE::GROUND:
		{
			++m_floor;
		}break;
		case GROUP_TILE::MOVE:
		{
			++m_floor;
		}break;
		case GROUP_TILE::WALL:
		{
			if (abs(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y) + 2.f
				>= GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f)
			{
				++m_floor;
				++m_wall;
			}
			else
			{
				if (GetCollider()->GetFinalPos().x < _pOther->GetFinalPos().x)
				{
					vPos.x -= 2.f;
				}
				else if (GetCollider()->GetFinalPos().x > _pOther->GetFinalPos().x)
				{
					vPos.x += 2.f;
				}
			}

		}break;
		case GROUP_TILE::PLATFORM:
		{

		}break;
		case GROUP_TILE::SLOPE:
		{

		}break;
		}

		//if (plColSize.x / 2.f + oColSize.x / 2.f >= abs(GetPos().x + pOther->GetPos().x))
		//{
			//Vec2 tPos = GetPos();
			//if (true == isFacedRight)
			//{
			//	tPos.x -= 2.f;
			//}
			//else
			//{
			//	tPos.x += 2.f;
			//}
		SetPos(vPos);

	}
	//else if (pOther->GetName() == L"Tile" && (plColSize.y + oColSize.y - 2) <= abs(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
	//{
	//	++m_floor;
	//}
}

void CLou::OnCollision(CCollider* _pOther)
{

	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();

	CTile* pTile = (CTile*)pOther;
	GROUP_TILE tileType = pTile->GetGroup();

	if (pOther->GetName() == L"Tile")
	{

		switch (tileType)
		{
		case GROUP_TILE::GROUND:
		{
			int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
			int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
			int sum = abs(a - b);
			if (1 < sum)
				--vPos.y;
		}break;
		case GROUP_TILE::MOVE:
		{
			int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
			int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
			int sum = abs(a - b);
			if (1 < sum)
				--vPos.y;
		}break;
		case GROUP_TILE::WALL:
		{

		}break;
		case GROUP_TILE::PLATFORM:
		{

		}break;
		case GROUP_TILE::SLOPE:
		{

		}break;
		}
	}

	SetPos(vPos);
}

void CLou::OnCollisionExit(CCollider* _pOther)
{

	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();

	CTile* pTile = (CTile*)pOther;
	GROUP_TILE tileType = pTile->GetGroup();

	if (pOther->GetName() == L"Tile")
	{
		switch (tileType)
		{
		case GROUP_TILE::GROUND:
		{
			--m_floor;
		}break;
		case  GROUP_TILE::MOVE:
		{
			--m_floor;
		}break;
		case GROUP_TILE::WALL:
		{
			if (m_wall)
			{
				--m_floor;
				--m_wall;
			}
		}break;
		case GROUP_TILE::PLATFORM:
		{

		}break;
		case GROUP_TILE::SLOPE:
		{

		}break;
		}
	}
}

void CLou::render()
{
	component_render();
	render_information();
}

void CLou::render_information()
{
	if (true == CCore::getInst()->DebugMode())
	{
		CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"BackInfo", L"texture\\BackInfo.png");
		Vec2 vPos = GetPos();
		vPos = CCameraManager::getInst()->GetRenderPos(vPos);
		
		////////////////////////
		wstring stateName = {};
		wstring direction = {};
		wstring curAni = {};

		////////////////////////
		switch (m_state)
		{
		case eState::IDLE:stateName = L"IDLE";
			break;
		case eState::SIT:stateName = L"SIT";
			break;
		case eState::LANDMOVE:stateName = L"LANDMOVE";
			break;
		case eState::FALL:stateName = L"FALL";
			break;
		case eState::JUMP:stateName = L"JUMP";
			break;
		case eState::DASH:stateName = L"DASH";
			break;
		case eState::GOTHIT:stateName = L"GOTHIT";
			break;
		case eState::DEAD:stateName = L"DEAD";
			break;
		case eState::ATTACK:stateName = L"ATTACK";
			break;
		case eState::HOLDCLIFF:stateName = L"HOLDCLIFF";
			break;
		case eState::INVINCIBLE:stateName = L"INVINCIBLE";
			break;
		case eState::FLY:stateName = L"FLY";
			break;
		}
		switch (m_facing)
		{
		case D_FACING::UP:direction = L"UP";
			break;
		case D_FACING::DOWN:direction = L"DOWN";
			break;
		case D_FACING::LEFT:direction = L"LEFT";
			break;
		case D_FACING::RIGHT:direction = L"RIGHT";
			break;
		}
		CRenderManager::getInst()->RenderImage(
			pImg,
			vPos.x + 30.f,
			vPos.y + -40.f,
			vPos.x + 200.f,
			vPos.y + 100.f,
			0.3f);

		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + stateName + L"\n" +
			L" drctn  : " + direction + L"\n" +
			L" curAm : " + curAni + L"\n"+
			L" HP:  " + std::to_wstring(GetHP()) + L"\n" +
			L" wallCount : "  + std::to_wstring(m_floor)
			, vPos.x+30.f
			, vPos.y+ -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}

}