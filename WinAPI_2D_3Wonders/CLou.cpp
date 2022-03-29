#include "pch.h"
#include "CLou.h"

#include "CScene.h"
#include "CEffect.h"
#include "CSound.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CMissile.h"
#include "CHyperMissile.h"
#include "CTile.h"
#include "CGoblin.h"
#include "CD2DImage.h"
#include "COwl.h"

#include <iostream>
#include <random>

///////////////////////
#define D_GRAVITY 400
#define D_DOWNFORCE 700
#define D_VELOCITY 200
#define D_UPFORCE 800
#define D_FROMGOBMAX 400
#define D_FROMGOBMIN 80

#define SETSTATE m_state = eState

CLou::CLou()
{
	m_dir = eDir::STILL;
	m_state = eState::IDLE;
	m_facing = D_FACING::RIGHT;
	pOwl = nullptr;
	missileType = L"NONE";
	blink = 0.f;
	isInvincible = 0.f;
	m_stateCounter = 0.f;
	dash = 0.f;
	isJumpingDownPlatform = false;
	
	m_goblinCounter = 0.f;

	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	m_upforce = D_UPFORCE;
	isFacedRight = true;
	isClothed = true;

	SetHP(2);
	prevHP = GetHP();
	prevPos = GetPos();

	SetName(L"Lou");
	m_pClothedImg = CResourceManager::getInst()->LoadD2DImage(L"PlayerClothedImg", L"texture\\Animation\\Animation_Lou.png");
	m_pUnclothedImg = CResourceManager::getInst()->LoadD2DImage(L"PlayerUnclothedImg", L"texture\\Animation\\Animation_Lou_Naked.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, -10.f));

	CreateAnimator();
	////////////////////////////////CLOTHED 애니메이션////////////////////////////////
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pClothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pClothedImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);

	GetAnimator()->CreateAnimation(L"Still_Right", m_pClothedImg, Vec2(0.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);
	GetAnimator()->CreateAnimation(L"Still_Left", m_pClothedImg, Vec2(128.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 2, false);

	GetAnimator()->CreateAnimation(L"Move_Right", m_pClothedImg, Vec2(0.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.15f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Left", m_pClothedImg, Vec2(128.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.15f, 6, false);
	
	GetAnimator()->CreateAnimation(L"Lookup_Right", m_pClothedImg, Vec2(768.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Lookup_Left", m_pClothedImg, Vec2(896.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Sit_Right", m_pClothedImg, Vec2(768.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Sit_Left", m_pClothedImg, Vec2(896.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);

	GetAnimator()->CreateAnimation(L"pShoot_Right", m_pClothedImg, Vec2(768.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.0666f, 3, false);
	GetAnimator()->CreateAnimation(L"pShoot_Left", m_pClothedImg, Vec2(896.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.0666f, 3, false);
	
	GetAnimator()->CreateAnimation(L"pShoot_Sit_Right", m_pClothedImg, Vec2(768.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"pShoot_Sit_Left", m_pClothedImg, Vec2(896.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"pShoot_Up_Right", m_pClothedImg, Vec2(768.f, 768.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"pShoot_Up_Left", m_pClothedImg, Vec2(896.f, 768.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);


	GetAnimator()->CreateAnimation(L"Jump_Right_U", m_pClothedImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_U", m_pClothedImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_D", m_pClothedImg, Vec2(256.f, 128.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_D", m_pClothedImg, Vec2(384.f, 128.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Onland", m_pClothedImg, Vec2(384.f, 256.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Onland", m_pClothedImg, Vec2(384.f, 384.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Look_D", m_pClothedImg, Vec2(256.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Look_D", m_pClothedImg, Vec2(384.f, 512.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Right_Look_U", m_pClothedImg, Vec2(256.f, 768.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Look_U", m_pClothedImg, Vec2(384.f, 768.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.5f, 1, false);

	GetAnimator()->CreateAnimation(L"Dash_Right", m_pClothedImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.06f, 5, false);
	GetAnimator()->CreateAnimation(L"Dash_Left", m_pClothedImg, Vec2(1408.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.06f, 5, false);
	
	GetAnimator()->CreateAnimation(L"Lou_Hurt_Right", m_pClothedImg, Vec2(1280.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.166f, 3, false);
	GetAnimator()->CreateAnimation(L"Lou_Hurt_Left", m_pClothedImg, Vec2(1408.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.166f, 3, false);

	GetAnimator()->CreateAnimation(L"Lou_Down_Plat", m_pClothedImg, Vec2(1792.f, 640.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 1.f, 1, true);

	GetAnimator()->CreateAnimation(L"Lou_Fly_Right", m_pClothedImg, Vec2(1024.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 1, false);
	GetAnimator()->CreateAnimation(L"Lou_Fly_Left", m_pClothedImg, Vec2(1024.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 1, true);

	GetAnimator()->CreateAnimation(L"Lou_NULL", m_pClothedImg, Vec2(1280.f, 1280.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 1, false);

	
	///////UNCLOTHED 애니메이션///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GetAnimator()->CreateAnimation(L"GetUp_Right_N", m_pUnclothedImg, Vec2(2944.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.125f, 4, false);
	GetAnimator()->CreateAnimation(L"GetUp_Left_N", m_pUnclothedImg, Vec2(2944.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.125f, 4, true);

	GetAnimator()->CreateAnimation(L"Idle_Right_N", m_pUnclothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Idle_Left_N", m_pUnclothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 1, true);

	GetAnimator()->CreateAnimation(L"Still_Right_N", m_pUnclothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 2, false);
	GetAnimator()->CreateAnimation(L"Still_Left_N", m_pUnclothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 2, true);

	GetAnimator()->CreateAnimation(L"Move_Right_N", m_pUnclothedImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Left_N", m_pUnclothedImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 6, true);

	GetAnimator()->CreateAnimation(L"Lookup_Right_N", m_pUnclothedImg, Vec2(1152.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Lookup_Left_N", m_pUnclothedImg, Vec2(1152.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);
	GetAnimator()->CreateAnimation(L"Sit_Right_N", m_pUnclothedImg, Vec2(2048.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Sit_Left_N", m_pUnclothedImg, Vec2(2048.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);

	GetAnimator()->CreateAnimation(L"pShoot_Right_N", m_pUnclothedImg, Vec2(896.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 2, false);
	GetAnimator()->CreateAnimation(L"pShoot_Left_N", m_pUnclothedImg, Vec2(896.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 2, true);

	GetAnimator()->CreateAnimation(L"pShoot_Sit_Right_N", m_pUnclothedImg, Vec2(2176.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"pShoot_Sit_Left_N", m_pUnclothedImg, Vec2(2176.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);
	GetAnimator()->CreateAnimation(L"pShoot_Up_Right_N", m_pUnclothedImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"pShoot_Up_Left_N", m_pUnclothedImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);

	GetAnimator()->CreateAnimation(L"Jump_Right_U_N", m_pUnclothedImg, Vec2(1536.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_U_N", m_pUnclothedImg, Vec2(1536.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);
	GetAnimator()->CreateAnimation(L"Jump_Right_Onland_N", m_pUnclothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Onland_N", m_pUnclothedImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);
	GetAnimator()->CreateAnimation(L"Jump_Right_Look_D_N", m_pUnclothedImg, Vec2(1792.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_Left_Look_D_N", m_pUnclothedImg, Vec2(1792.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.5f, 1, true);
	GetAnimator()->CreateAnimation(L"Jump_R_Up_Shoot_N", m_pUnclothedImg, Vec2(1408.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_L_Up_Shoot_N", m_pUnclothedImg, Vec2(1408.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);
	GetAnimator()->CreateAnimation(L"Jump_R_Shoot_N", m_pUnclothedImg, Vec2(1664.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_L_Shoot_N", m_pUnclothedImg, Vec2(1664.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);
	GetAnimator()->CreateAnimation(L"Jump_R_Dn_Shoot_N", m_pUnclothedImg, Vec2(1920.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);
	GetAnimator()->CreateAnimation(L"Jump_L_Dn_Shoot_N", m_pUnclothedImg, Vec2(1920.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, true);

	GetAnimator()->CreateAnimation(L"Dash_Right_N", m_pUnclothedImg, Vec2(2432.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 1, false);
	GetAnimator()->CreateAnimation(L"Dash_Left_N", m_pUnclothedImg, Vec2(2432.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 1, true);

	GetAnimator()->CreateAnimation(L"Lou_Hurt_Right_N", m_pUnclothedImg, Vec2(2560.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.333f, 3, false);
	GetAnimator()->CreateAnimation(L"Lou_Hurt_Left_N", m_pUnclothedImg, Vec2(2560.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.333f, 3, true);

	GetAnimator()->CreateAnimation(L"Lou_Down_Plat_N", m_pUnclothedImg, Vec2(6912.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, true);

	GetAnimator()->CreateAnimation(L"GetHealed", m_pUnclothedImg, Vec2(3456.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.125f, 8, false);

	GetAnimator()->Play(L"Idle_Right");

	/////////////////////////////////사운드//////////////////////////
	CSoundManager::getInst()->AddSound(L"normalGunSound", L"sound\\normal_gun.wav", false);
	CSoundManager::getInst()->AddSound(L"hyperGunSound", L"sound\\hyper.wav", false);
	CSoundManager::getInst()->AddSound(L"hurtSound", L"sound\\hurt.wav", false);
	CSoundManager::getInst()->AddSound(L"dieSound", L"sound\\die.wav", false);
	CSoundManager::getInst()->AddSound(L"clothingSound", L"sound\\clothingSound.wav", false);

}

CLou::~CLou()
{

}

void CLou::update()
{
	update_move();
	update_animation();
}

void CLou::update_move()
{
	Vec2 vPos = GetPos();
	
	//////////////////////디버그용//////////////////
	//누르고있으면 속도 부스터:테스트용	
	if (KEY('W'))
	{
		m_velocity = 1000.f;
	}
	if (KEYUP('W'))
	{
		m_velocity = D_VELOCITY;
	}
	//카메라 캐릭터 target
	if (KEYDOWN('V'))
	{
		CCameraManager::getInst()->SetTargetX(this);
	}
	if (m_goblinCounter >= 0.f)
	{
		m_goblinCounter += fDT;
	}

	//////////////////////////////////////
	//////////////////////////////////////


	//n초마다 고블린 플레이어 주위에서 리젠
	if (m_goblinCounter >= 4.f)
	{
		m_goblinCounter = 0.f;
		GenerateGoblin();
	}

	if (m_floor > 0)	//충돌on
	{
		m_gravity = 0.f;
		m_upforce = 0.f;
		isJumpingDownPlatform = false;
	}
	else				//충돌off
	{
		m_gravity = D_GRAVITY;
		if (m_state != eState::FLY)
		{
			vPos.y += D_GRAVITY * fDT;
		}
	}

	//enum class로 상태 구분
	switch (m_state)
	{
	case eState::IDLE:
	{
		m_stateCounter += fDT;
		if (KEY(VK_UP))
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
		if (KEY(VK_DOWN))
		{
			m_stateCounter = 0.f;
			SETSTATE::SIT;
		}
		if (KEY(VK_RIGHT))
		{
			m_stateCounter = 0.f;
			isFacedRight = true;
			SETSTATE::LANDMOVE;
		}
		if (KEYUP(VK_RIGHT))
		{
			isFacedRight = true;
		}

		if (KEY(VK_LEFT))
		{
			m_stateCounter = 0.f;
			isFacedRight = false;
			SETSTATE::LANDMOVE;
		}
		if (KEYUP(VK_LEFT))
		{
			isFacedRight = false;
		}

		if (KEYDOWN('X'))
		{
			m_stateCounter = 0.f;
			m_upforce = D_UPFORCE;
			if (moveTileCounter>0)
			{
				vPos.y -= 10.f;
			}
			else
			{
				vPos.y -= 2.f;
			}

			SETSTATE::JUMP;
		}
		if (KEYDOWN('Z'))
		{
			m_stateCounter = 0.f;
			CreateMissile();
			m_state = eState::ATTACK;
		}
		if (prevHP > GetHP())
		{
			m_stateCounter = 0.f;
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}

			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			m_stateCounter = 0.f;
			
			SETSTATE::DEAD;
		}

	}
		break;
	case eState::SIT:
	{
		m_stateCounter += fDT;
		GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y - 30.f));
		GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, -20.f), 0);
		GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, -20.f), 0);
		GetAnimator()->FindAnimation(L"Sit_Right_N")->SetOffset(Vec2(0.f, -10.f), 0);
		GetAnimator()->FindAnimation(L"Sit_Left_N")->SetOffset(Vec2(0.f, -10.f), 0);

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
			m_stateCounter = 0.f;
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			SETSTATE::IDLE;
		}
		if (KEYDOWN(VK_UP))
		{
			m_stateCounter = 0.f;
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			SETSTATE::IDLE;
		}
		if (KEYDOWN('X'))
		{
			m_stateCounter = 0.f;
			if (m_platformCounter == 0)
			{
				SETSTATE::DASH;
			}
			if (m_platformCounter > 0)
			{
				isJumpingDownPlatform = true;
				vPos.y += 30.f;
				m_floor = 0;
				m_platformCounter = 0;
			}
		}
		if (KEYDOWN('Z'))
		{
			m_stateCounter = 0.f;
			CreateMissile();
			SETSTATE::ATTACK;
		}
		if (prevHP > GetHP())
		{
			m_stateCounter = 0.f;
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			
			m_stateCounter = 0.f;
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			SETSTATE::DEAD;
		}
	}
		break;
	case eState::LANDMOVE:
	{
		m_stateCounter += fDT;
		if (KEYDOWN('Z'))
		{
			m_stateCounter = 0.f;
			CreateMissile();
			m_state = eState::ATTACK;
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
			m_stateCounter = 0.f;
			isFacedRight = true;
			m_facing = D_FACING::RIGHT;
			SETSTATE::IDLE;
		}
		if (KEYUP(VK_LEFT))
		{
			m_stateCounter = 0.f;
			isFacedRight = false;
			m_facing = D_FACING::LEFT;
			SETSTATE::IDLE;
		}

		if (KEYDOWN('X'))
		{
			m_stateCounter = 0.f;
			m_upforce = D_UPFORCE;

			if (moveTileCounter > 0)
			{
				vPos.y -= 10.f;
			}
			else
			{
				vPos.y -= 2.f;
			}
			SETSTATE::JUMP;
		}

		if (prevHP > GetHP())
		{
			m_stateCounter = 0.f;
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			
			m_stateCounter = 0.f;
			SETSTATE::DEAD;
		}
	}
		break;
	case eState::ATTACK:
	{
		//GetAnimator()->FindAnimation(L"pShoot_Sit_Right_N")->SetOffset(Vec2(0.f, 50.f), 0);
		//GetAnimator()->FindAnimation(L"pShoot_Sit_Left_N")->SetOffset(Vec2(0.f, 50.f), 0);

		m_stateCounter += fDT;
		if (m_stateCounter >= 0.2f)
		{
			m_stateCounter = 0.f;
			if (KEY(VK_DOWN))
			{
				SETSTATE::SIT;
			}
			else
			{//TODO:앉아서 쏘다가 일어나면 애니메이션 요동치는것 수정
				GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
				GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
				GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
				m_state = eState::IDLE;
			}

		}
	}break;
	case eState::FALL:
	{
		m_stateCounter += fDT;
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
			m_stateCounter = 0.f;
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			
			m_stateCounter = 0.f;
			SETSTATE::DEAD;
		}
		if (0 == m_upforce && m_floor > 0)
		{
			m_stateCounter = 0.f;
			SETSTATE::IDLE;
		}
		if (KEYDOWN('Z'))
		{
			CreateMissile();
		}
	}

		break;
	case eState::JUMP:
	{
		m_stateCounter += fDT;
		m_upforce -= (D_DOWNFORCE * fDT);
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
			m_stateCounter = 0.f;
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			
			m_stateCounter = 0.f;
			SETSTATE::DEAD;
		}
		if (m_upforce <= 0.f)
		{
			m_stateCounter = 0.f;
			SETSTATE::FALL;
		}
	}
		break;
	case eState::DASH:
	{
		m_stateCounter += fDT;
		dash += fDT;
		if (dash >= 0.3f)
		{
			m_stateCounter = 0.f;
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
			m_stateCounter = 0.f;
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			
			m_stateCounter = 0.f;
			GetAnimator()->FindAnimation(L"Sit_Right")->SetOffset(Vec2(0.f, 0.f), 0);
			GetAnimator()->FindAnimation(L"Sit_Left")->SetOffset(Vec2(0.f, 0.f), 0);
			GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
			SETSTATE::DEAD;
		}
	}
		break;
	case eState::GOTHIT:
	{
		if (m_stateCounter == 0.f)
		{
			//////////////////////이펙트///////////////
			CEffect* effectExplode = new CEffect(L"Effect_Explode", L"texture\\Animation\\Effect_Explode.png",
				L"Effect_Explode", Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 9, false, false, L"Effect_Explode");
			effectExplode->SetPos(Vec2(GetPos()));
			effectExplode->SetDuration(0.9f);
			CreateObj(effectExplode, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
		}
		m_stateCounter += fDT;

		if (m_stateCounter >= 1.f)
		{
			isInvincible += fDT;
			m_stateCounter = 0.f;
			if (GetHP() > 0)
			{
				isClothed = false;
				m_state = eState::IDLE;
			}
			else
			{
				
				m_state = eState::DEAD;
			}

		}

	}
		break;
	case eState::DEAD:
	{
		if (m_stateCounter == 0.f)
		{
			//////////////////////이펙트///////////////
			CEffect* effectDie = new CEffect(L"Effect_Lou_Dead", L"texture\\Animation\\Effect_Lou_Dead.png",
				L"Effect_Lou_Dead", Vec2(0.f, 0.f), Vec2(192.f, 512.f), Vec2(192.f, 0.f), 0.03f, 14, false, false, L"Effect_Lou_Dead");
			effectDie->SetPos(Vec2(GetPos().x, GetPos().y - 200.f));
			effectDie->SetDuration(0.42f);
			CreateObj(effectDie, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
		}
		m_stateCounter += fDT;
		if (m_stateCounter >= 3.f)
		{
			pOwl = new COwl(vPos);
			CreateObj(pOwl, GROUP_GAMEOBJ::ITEM);
			m_stateCounter = 0.f;
			//TODO:목숨이 남았다면
			SETSTATE::FLY;
		}
	}
		break;
	case eState::HOLDCLIFF:
	{
		m_stateCounter += fDT;
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
			m_stateCounter = 0.f;
			prevHP = GetHP();
			if (GetHP() > 0)
			{
				CSoundManager::getInst()->Play(L"hurtSound");
			}
			else
			{
				CSoundManager::getInst()->Play(L"dieSound");
			}
			SETSTATE::GOTHIT;
		}
		else if (0 >= GetHP())
		{
			
			m_stateCounter = 0.f;
			SETSTATE::DEAD;
		}
	}
		break;
	case eState::FLY:
	{
		if (KEY(VK_LEFT))
		{
			vPos.x -= m_velocity * fDT;
		}
		if (KEY(VK_RIGHT))
		{
			vPos.x += m_velocity * fDT;
		}
		m_stateCounter += fDT;
		isInvincible += fDT;
		vPos.y = 50.f;
		missileType = L"NONE";
		SetHP(2);
		prevHP = 2;
		isClothed = true;
		if (KEYDOWN('X') || m_stateCounter >= 5.f)
		{ 
			pOwl->AttackEntireWindow();
			DeleteObj(pOwl);
			m_stateCounter = 0.f;
			SETSTATE::FALL;
		}

	}
		break;
	case eState::HEALED:
	{
		//isInvincible += fDT;
		m_stateCounter += fDT;
		SetHP(2);
		isClothed = true;
		if (m_stateCounter >= 1.f)
		{
			SETSTATE::IDLE;
		}
	}break;
	case eState::BOSS_ENCOUNTER:
	{
		if (vPos.x <= 8500.f)
		{
			m_goblinCounter = -1.f;//고블린리젠OFF
					//TODO:고블린이 못따라오도록 죽이던가 방안찾기
			vPos.x += m_velocity * fDT;
			CCameraManager::getInst()->SetTargetX(nullptr);
			CCameraManager::getInst()->Scroll(Vec2(1.f, 0.f), m_velocity + 70.f);
		}
		else
		{
			m_stateCounter += fDT;
		}
			
		if (m_stateCounter >= 2.f)
		{
			m_stateCounter = 0.f;
			SETSTATE::IDLE;
		}

	}break;
	}

///////////////////
	//무적판정관련
	if (isInvincible !=0.f)
	{
		isInvincible += fDT;
	}
	if (isInvincible >= 3.f)
	{
		isInvincible = 0.f;
	}
////////////////////
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
					if (isClothed)
					{
						GetAnimator()->Play(L"Lookup_Right");
					}
					else
					{
						GetAnimator()->Play(L"Lookup_Right_N");
					}
				}
				else
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Idle_Right");
					}
					else
					{
						GetAnimator()->Play(L"Idle_Right_N");
					}
				}
			}
			else
			{
				if (KEY(VK_UP))
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Lookup_Left");
					}
					else
					{
						GetAnimator()->Play(L"Lookup_Left_N");
					}
					
				}
				else
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Idle_Left");
					}
					else
					{
						GetAnimator()->Play(L"Idle_Left_N");
					}
				}
			}
		}break;
		case eState::SIT:
		{
			if (isFacedRight)
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Sit_Right");
				}
				else
				{
					GetAnimator()->Play(L"Sit_Right_N");
				}
			}
			else
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Sit_Left");
				}
				else
				{
					GetAnimator()->Play(L"Sit_Left_N");
				}
			}
		}break;
		case eState::ATTACK:
		{
			if (KEY(VK_DOWN))
			{
				if (isFacedRight)
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"pShoot_Sit_Right");
					}
					else
					{
						GetAnimator()->Play(L"pShoot_Sit_Right_N");
					}
				}
				else
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"pShoot_Sit_Left");
					}
					else
					{
						GetAnimator()->Play(L"pShoot_Sit_Left_N");
					}
				}
				GetAnimator()->GetCurrentAnimation()->SetOffset(Vec2(0.f, -20.f), 0);
			}
			else if (KEY(VK_UP))
			{
				if (isFacedRight)
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"pShoot_Up_Right");
					}
					else
					{
						GetAnimator()->Play(L"pShoot_Up_Right_N");
					}
				}
				else
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"pShoot_Up_Left");
					}
					else
					{
						GetAnimator()->Play(L"pShoot_Up_Left_N");
					}
				}
			}
			else
			{
				if (isFacedRight)
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"pShoot_Right");
					}
					else
					{
						GetAnimator()->Play(L"pShoot_Right_N");
					}
				}
				else
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"pShoot_Left");
					}
					else
					{
						GetAnimator()->Play(L"pShoot_Left_N");
					}
				}
			}

		}break;
		case eState::JUMP:
		{		
			if (isFacedRight)
			{
				if(KEY(VK_DOWN))
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Jump_Right_Look_D");
					}
					else if (KEY('Z') && !isClothed)
					{
						GetAnimator()->Play(L"Jump_R_Dn_Shoot_N");
					}
					else if (!isClothed)
					{
						GetAnimator()->Play(L"Jump_Right_Look_D_N");
					}
				}
				else if (KEY(VK_UP))
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Jump_Right_Look_U");
					}
					else if (KEY('Z') && !isClothed)
					{
						GetAnimator()->Play(L"Jump_R_Up_Shoot_N");
					}
					else if(!isClothed)
					{
						GetAnimator()->Play(L"Jump_Right_U_N");
					}
				}
				else if (KEY('Z') && !isClothed)
				{
					GetAnimator()->Play(L"Jump_R_Shoot_N");
				}
				else
				{
					if (prevPos.y < GetPos().y)
					{
						if (isClothed)
						{
							GetAnimator()->Play(L"Jump_Right_D");
						}
						else
						{
							GetAnimator()->Play(L"Jump_Right_U_N");
						}
					}
					else
					{
						if (isClothed)
						{
							GetAnimator()->Play(L"Jump_Right_U");
						}
						else
						{
							GetAnimator()->Play(L"Jump_Right_U_N");
						}
					}
				}
			}
			else//왼쪽볼때
			{
				if (KEY(VK_DOWN))
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Jump_Left_Look_D");
					}
					else if (KEY('Z') && !isClothed)
					{
						GetAnimator()->Play(L"Jump_R_Dn_Shoot_N");
					}
					else if (!isClothed)
					{
						GetAnimator()->Play(L"Jump_Left_Look_D_N");
					}
				}
				else if (KEY(VK_UP))
				{
					if (isClothed)
					{
						GetAnimator()->Play(L"Jump_Left_Look_U");
					}
					else if (KEY('Z') && !isClothed)
					{
						GetAnimator()->Play(L"Jump_L_Up_Shoot_N");
					}
					else if (!isClothed)
					{
						GetAnimator()->Play(L"Jump_Left_U_N");
					}
				}
				else if (KEY('Z') && !isClothed)
				{
					GetAnimator()->Play(L"Jump_L_Shoot_N");
				}
				else
				{
					if (prevPos.y < GetPos().y)
					{
						if (isClothed)
						{
							GetAnimator()->Play(L"Jump_Left_D");
						}
						else
						{
							GetAnimator()->Play(L"Jump_Left_U_N");
						}
					}
					else
					{
						if (isClothed)
						{
							GetAnimator()->Play(L"Jump_Left_U");
						}
						else
						{
							GetAnimator()->Play(L"Jump_Left_U_N");
						}
					}
				}
			}

		}break;
		case eState::FALL:
		{
			if (isFacedRight)
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Jump_Right_D");
				}
				else
				{
					GetAnimator()->Play(L"Jump_Right_U_N");
				}
			}
			else
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Jump_Left_D");
				}
				else
				{
					GetAnimator()->Play(L"Jump_Left_U_N");
				}
			}
		}break;
			
		case eState::LANDMOVE:
		{
			if (isFacedRight)
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Move_Right");
				}
				else
				{
					GetAnimator()->Play(L"Move_Right_N");
				}
			}
			else
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Move_Left");
				}
				else
				{
					GetAnimator()->Play(L"Move_Left_N");
				}
			}
		}break;
		case eState::HOLDCLIFF:
		{

		}break;
		case eState::GOTHIT:
		{
			if (isClothed)
			{
				if (m_stateCounter < 0.5f)
				{
					if (isFacedRight)
					{
						GetAnimator()->Play(L"Lou_Hurt_Right");
					}
					else
					{
						GetAnimator()->Play(L"Lou_Hurt_Left");
					}
				}
				else if (m_stateCounter >= 0.5f)
				{
					if (isFacedRight)
					{
						GetAnimator()->Play(L"GetUp_Right_N");
					}
					else
					{
						GetAnimator()->Play(L"GetUp_Left_N");
					}
				}
			}
			else
			{
				if (isFacedRight)
				{
					GetAnimator()->Play(L"Lou_Hurt_Right_N");
				}
				else
				{
					GetAnimator()->Play(L"Lou_Hurt_Left_N");
				}
			}


		}break;
		case eState::DASH:
		{
			if (isFacedRight)
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Dash_Right");
				}
				else
				{
					GetAnimator()->Play(L"Dash_Right_N");
				}
			}
			else
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Dash_Left");
				}
				else
				{
					GetAnimator()->Play(L"Dash_Left_N");
				}
			}
		}break;
		case eState::DEAD:
		{

		}break;
		case eState::HEALED:
		{
			GetAnimator()->Play(L"GetHealed");
		}break;
		case eState::FLY:
		{
			if (isFacedRight)
			{
				GetAnimator()->Play(L"Lou_Fly_Right");
			}
			else
			{
				GetAnimator()->Play(L"Lou_Fly_Left");
			}
		}break;
		case eState::BOSS_ENCOUNTER:
		{
			if (GetPos().x <= 8500.f)
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Move_Right");
				}
				else
				{
					GetAnimator()->Play(L"Move_Right_N");
				}
			}
			else
			{
				if (isClothed)
				{
					GetAnimator()->Play(L"Idle_Right");
				}
				else
				{
					GetAnimator()->Play(L"Idle_Right_N");
				}
			}

		}break;
	}

	if (isJumpingDownPlatform)
	{
		if(isClothed)
			GetAnimator()->Play(L"Lou_Down_Plat");
		else
			GetAnimator()->Play(L"Lou_Down_Plat_N");
	}

	if (isInvincible > 0.f)
	{
		blink += fDT;
	}
	if (blink >=0.1f)
	{
		if (isInvincible)
		{
			GetAnimator()->Play(L"Lou_NULL");
		}
		if (blink >= 0.2f)
		{
			blink = 0.f;
		}	
	}

	DirCheck(prevPos);

	prevPos = GetPos();
	GetAnimator()->update();
}

void CLou::CreateMissile()
{

	Vec2 fpMissilePos = GetPos();
	CMissile* pMissile = new CMissile();
	CHyperMissile* pHyper = nullptr;
	// Misiile Object
	if (missileType == L"NONE")
	{
		CSoundManager::getInst()->Play(L"normalGunSound");
	}
	else if (missileType == L"HYPER")
	{
		CSoundManager::getInst()->Play(L"hyperGunSound");
	}

	switch(m_facing)
	{
	case D_FACING::RIGHT:
		{
		if (missileType == L"NONE")
		{
			pMissile->GetAnimator()->Play(L"N_Shoot_Right");
		}
		else if (missileType == L"HYPER")
		{
			pMissile->GetAnimator()->Play(L"N_Hyper_Right");
			pHyper = new CHyperMissile(Vec2(1.f,0.f));
		}
			fpMissilePos.x += 70;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			
			pMissile->GetCollider()->SetOffsetPos(Vec2(-30.f, 0.f));
			pMissile->SetDir(Vec2(1, 0));		
		}break;
	case D_FACING::LEFT:
		{
		if (missileType == L"NONE")
		{
			pMissile->GetAnimator()->Play(L"N_Shoot_Left");
		}
		else if (missileType == L"HYPER")
		{
			pHyper = new CHyperMissile(Vec2(-1.f, 0.f));
			pMissile->GetAnimator()->Play(L"N_Hyper_Left");
		}
			fpMissilePos.x -= 70;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
			pMissile->SetDir(Vec2(-1, 0));
		}break;
	case D_FACING::UP:
		{
		if (0 == m_floor)
		{
			if (isFacedRight)
			{
				fpMissilePos.x += 25.f;
			}
			else
			{
				fpMissilePos.x -= 25.f;
			}
		}
		if (missileType == L"NONE")
		{
			pMissile->GetAnimator()->Play(L"N_Shoot_Up");
		}
		else if (missileType == L"HYPER")
		{
			if (isFacedRight)
			{
				pHyper = new CHyperMissile(Vec2(0.1f, -1.f));

			}
			else
			{
				pHyper = new CHyperMissile(Vec2(-0.1f, -1.f));
			}
			pMissile->GetAnimator()->Play(L"N_Hyper_Up");
		}
			fpMissilePos.y -= 50;
			pMissile->SetPos(fpMissilePos);
			pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
			pMissile->GetCollider()->SetOffsetPos(Vec2(0.f, 30.f));
			pMissile->SetDir(Vec2(0, -1));
		}break;
	case D_FACING::DOWN:
		{
			if (0 == m_floor)
			{
				if (isFacedRight)
				{
					fpMissilePos.x += 15.f;
				}
				else
				{
					fpMissilePos.x -= 15.f;
				}
				if (missileType == L"NONE")
				{
					pMissile->GetAnimator()->Play(L"N_Shoot_Down");
				}
				else if (missileType == L"HYPER")
				{
					pHyper = new CHyperMissile(Vec2(0.f, 1.f));
					pMissile->GetAnimator()->Play(L"N_Hyper_Down");
				}
				fpMissilePos.y += 50;
				pMissile->SetPos(fpMissilePos);
				pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
				pMissile->GetCollider()->SetOffsetPos(Vec2(0.f, -30.f));
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
					if (missileType == L"NONE")
					{
						pMissile->GetAnimator()->Play(L"N_Shoot_Right");
					}
					else if (missileType == L"HYPER")
					{
						pHyper = new CHyperMissile(Vec2(1.f, 0.f));
						pMissile->GetAnimator()->Play(L"N_Hyper_Right");
					}
					pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
					pMissile->SetDir(Vec2(1, 0));
				}
				else
				{
					fpMissilePos.x -= 110;
					fpMissilePos.y += 20;
					pMissile->SetPos(fpMissilePos);
					pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
					if (missileType == L"NONE")
					{
						pMissile->GetAnimator()->Play(L"N_Shoot_Left");
					}
					else if (missileType == L"HYPER")
					{
						pHyper = new CHyperMissile(Vec2(-1.f, 0.f));
						pMissile->GetAnimator()->Play(L"N_Hyper_Left");
					}
					pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));
					pMissile->SetDir(Vec2(-1, 0));
				}
			}
		}break;	

	}

	if (nullptr != pHyper)
	{
		pHyper->SetPos(fpMissilePos);
		CreateObj(pHyper, GROUP_GAMEOBJ::MISSILE_PLAYER);
	}
	CreateObj(pMissile, GROUP_GAMEOBJ::MISSILE_PLAYER);
}

void CLou::DirCheck(Vec2 _prevPos)
{
	if (_prevPos.x < GetPos().x && _prevPos.y ==GetPos().y)
	{
		m_dir = eDir::RIGHT;
	}
	else if (_prevPos.x > GetPos().x && _prevPos.y == GetPos().y)
	{
		m_dir = eDir::LEFT;
	}
	else if (_prevPos.x == GetPos().x && _prevPos.y > GetPos().y)
	{
		m_dir = eDir::UP;
	}
	else if (_prevPos.x == GetPos().x && _prevPos.y < GetPos().y)
	{
		m_dir = eDir::DOWN;
	}
	else if (_prevPos.x == GetPos().x && _prevPos.y == GetPos().y)
	{
		m_dir = eDir::STILL;
	}
	else if (_prevPos.x > GetPos().x && _prevPos.y > GetPos().y)
	{
		m_dir = eDir::LEFTUP;
	}
	else if (_prevPos.x < GetPos().x && _prevPos.y > GetPos().y)
	{
		m_dir = eDir::RIGHTUP;
	}
	else if (_prevPos.x < GetPos().x && _prevPos.y < GetPos().y)
	{
		m_dir = eDir::RIGHTDOWN;
	}
	else if (_prevPos.x > GetPos().x && _prevPos.y < GetPos().y)
	{
		m_dir = eDir::LEFTDOWN;
	}
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
	if (pTiles.size() > 1)
	{
		std::uniform_int_distribution<int> dis(0, pTiles.size() - 1);
		CGameObject* pTile = pTiles[dis(gen)];
		Vec2 tilePos = pTile->GetPos();

		CGoblin* pGoblin = new CGoblin();
		tilePos.y -= (pGoblin->GetScale().y / 2.f + pTile->GetScale().y / 2.f);
		pGoblin->SetPos(tilePos);
		if (pGoblin->GetPos().x < GetPos().x)
		{
			pGoblin->SetFacedRight(true);
		}
		else
		{
			pGoblin->SetFacedRight(false);
		}

		//////////////////////이펙트///////////////
		CEffect* effectGobGen = new CEffect(L"GoblinGenEff", L"texture\\Animation\\Effect_Goblin_Gen.png",
			L"Goblin_Gen_Eff", Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 10, false, false, L"GoblinGenEff");
		effectGobGen->SetPos(Vec2(pGoblin->GetPos().x, pGoblin->GetPos().y + 60.f));
		effectGobGen->SetDuration(2.f);
		CreateObj(effectGobGen, GROUP_GAMEOBJ::EFFECT);
		///////////////////////////////////////////
		CreateObj(pGoblin, GROUP_GAMEOBJ::MONSTER);
	}	

}

void CLou::OnCollisionEnter(CCollider* _pOther)
{
	vector<CGameObject*> pGroupMonsters = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::MONSTER);
	vector<CGameObject*> pGroupEnemyMissiles = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::MISSILE_MONSTER);
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
			++moveTileCounter;
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
			if (m_platformCounter == 0)
			{
				if (m_dir == eDir::DOWN ||
					m_dir == eDir::LEFTDOWN ||
					m_dir == eDir::RIGHTDOWN)
				{
					++m_platformCounter;
					++m_floor;
				}
			}
			else if (m_platformCounter > 0)
			{
				if (m_dir == eDir::DOWN ||
					m_dir == eDir::LEFTDOWN ||
					m_dir == eDir::RIGHTDOWN ||
					m_dir == eDir::LEFT ||
					m_dir == eDir::RIGHT)
				{
					++m_platformCounter;
					++m_floor;
				}
			}

		}break;
		case GROUP_TILE::SLOPE:
		{

		}break;
		}


		SetPos(vPos);

	}

	for (int i = 0; i < pGroupMonsters.size(); ++i)
	{

		if (pGroupMonsters[i] == pOther)
		{
			if (pOther->GetName() == L"Goblin")
			{
				CGoblin* pGoblin = (CGoblin*)pOther;
				if (pGoblin->GetState() != eState_Goblin::BORN)
				{//고블린이 리젠모션 중일 때 충돌처리를 안하도록 예외처리
					if (!(m_state == eState::GOTHIT || m_state == eState::DEAD || isInvincible > 0.f))
					{
						int hp = GetHP();
						SetHP(--hp);
					}
				}
			}
			else
			{
				if (!(m_state == eState::GOTHIT || m_state == eState::DEAD || isInvincible > 0.f))
				{
					int hp = GetHP();
					SetHP(--hp);
				}
			}
		}
	}
	for (int i = 0; i < pGroupEnemyMissiles.size(); ++i)
	{
		if (pGroupEnemyMissiles[i] == pOther)
		{
			if (!(m_state == eState::GOTHIT || m_state == eState::DEAD || isInvincible > 0.f))
			{
				int hp = GetHP();
				SetHP(--hp);
			}
		}
	}
	
	if (pOther->GetName() == L"Hyper")
	{
		missileType = L"HYPER";
	}
	if (pOther->GetName() == L"Lamp"&& 1==GetHP())
	{
		CSoundManager::getInst()->Play(L"clothingSound");	
		m_stateCounter = 0.f;
		SETSTATE::HEALED;
	}
	else if (pOther->GetName() == L"Lamp" && 1 < GetHP())
	{
		CSoundManager::getInst()->Play(L"item_change");
	}
	if (pOther->GetName() == L"BossTrigger")
	{
		SETSTATE::BOSS_ENCOUNTER;
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
				vPos.y-=3;
		}break;
		case GROUP_TILE::WALL:
		{

		}break;
		case GROUP_TILE::PLATFORM:
		{
			if (m_platformCounter > 0)
			{
				int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
				int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
				int sum = abs(a - b);
				if (1 < sum)
					--vPos.y;
			}
			else if (m_platformCounter == 0)
			{
				if (m_dir == eDir::UP ||
					m_dir == eDir::LEFTUP ||
					m_dir == eDir::RIGHTUP)
				{
					if (KEYDOWN('C'))
					{
						m_state = eState::HOLDCLIFF;
					}
				}
			}


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
			--moveTileCounter;
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
			if(m_platformCounter>0)
			{
				--m_platformCounter;
				--m_floor;
			}
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
		wstring dir = {};

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
		case eState::HEALED:stateName = L"HEALED";
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
		switch (m_dir)
		{
		case eDir::RIGHTUP:dir = L"RIGHTUP";
			break;
		case eDir::RIGHT:dir = L"RIGHT";
			break;
		case eDir::RIGHTDOWN:dir = L"RIGHTDOWN";
			break;
		case eDir::DOWN:dir = L"DOWN";
			break;
		case eDir::LEFTDOWN:dir = L"LEFTDOWN";
			break;
		case eDir::LEFT:dir = L"LEFT";
			break;
		case eDir::LEFTUP:dir = L"LEFTUP";
			break;
		case eDir::UP:dir = L"UP";
			break;
		case eDir::STILL:dir = L"STILL";
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
			L" shootDir  : " + direction + L"\n" +
			L" Dir: " + dir + L"\n" +
			L" curAm : " + curAni + L"\n"+
			L" HP:  " + std::to_wstring(GetHP()) + L"\n" +
			L" MsileType: "+ missileType +L"\n"+
			L" wallCount : "  + std::to_wstring(m_floor) + L"\n" +
			L" platformCtr: " +std::to_wstring(m_platformCounter)
			, vPos.x+30.f
			, vPos.y+ -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}

}