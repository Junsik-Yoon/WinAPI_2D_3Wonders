#include "pch.h"
#include "CGolemWood.h"
#include "CChestnut.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CMissile.h"
#include "CScene.h"
#include "CTile.h"
#include "CMovingTile.h"
#include "CAnimation.h"
#include "CBossVital.h"
#include "CEffect.h"
#include "CD2DImage.h"

#include <iostream>
#include <random>

#define D_GRAVITY 400
#define D_TILESPEED 500

#define MINY 420.f
#define MAXY 300.f

CGolemWood::CGolemWood()
{
	m_bossVital = nullptr;
	prevVital = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 200); 
	CChestnut* pChestnut = nullptr;
	for (int i = 0; i < 10; ++i)
	{
		pChestnut = new CChestnut();
		pChestnut->SetOnTree(true);
		pChestnut->SetOffset(Vec2((float)dis(gen), (float)(dis(gen)) - 100));
		pChestnut->SetPos(Vec2(GetPos().x - 200.f + pChestnut->GetOffset().x
			, GetPos().y - 150.f + pChestnut->GetOffset().y));
		pChestnuts.push_back(pChestnut);
		CreateObj(pChestnut, GROUP_GAMEOBJ::MONSTER);
	}

	pMovingTiles = {};
	m_state = eState_GW::IDLE;
	m_floor = 0;
	SetHP(50);
	m_gravity = D_GRAVITY;
	m_dTileSpeed = D_TILESPEED;
	m_bossTimer = 0.f;
	m_shootTimer = 0.f;
	SetName(L"GolemWood");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GolemWoodImg", L"texture\\Animation\\Animation_GolemWood.png");
	CreateCollider();
	SetScale(Vec2(10.f, 285.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(15.f, 0.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Idle", m_pImg, Vec2(0.f, 0.f), Vec2(512.f, 512.f), Vec2(512.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Shoot", m_pImg, Vec2(512.f, 0.f), Vec2(512.f, 512.f), Vec2(512.f, 0.f), 0.333f, 3, false);
	GetAnimator()->CreateAnimation(L"Damaged", m_pImg, Vec2(2048.f, 0.f), Vec2(512.f, 512.f), Vec2(512.f, 0.f), 0.5f, 1, false);

	GetAnimator()->Play(L"Idle");

	CSoundManager::getInst()->AddSound(L"boss_shoot", L"sound\\boss_shoot.wav", false);
	CSoundManager::getInst()->AddSound(L"boss_damaged", L"sound\\boss_damaged.wav", false);
	CSoundManager::getInst()->AddSound(L"boss_die", L"sound\\boss_die.wav", false);

}

CGolemWood::~CGolemWood()
{

}


void CGolemWood::update()
{
	Vec2 vPos = GetPos();


	switch (m_state)
	{
	case eState_GW::IDLE:
	{
		vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
		if (abs(pPlayer[0]->GetPos().x - GetPos().x) <= 1000.f)
		{
			m_dMoveTimer = 0;
			CSoundManager::getInst()->Stop(L"stage1_1_bgm");
			CSoundManager::getInst()->Play(L"stage1_1_boss_bgm");
			m_state = eState_GW::PREPARE;
		}
		if (tileActivate)
		{
			m_dMoveTimer += DT;
			if (m_dMoveTimer >= 0.05)
			{
				m_dMoveTimer = 0;
				MoveTiles();
			}
		}

	}
		break;
	case eState_GW::PREPARE:
	{
		if (positionAdjust)
		{
			if (m_floor == 0)
			{
				m_gravity = D_GRAVITY;
				vPos.y += D_GRAVITY * fDT;
			}
			else
			{
				m_gravity = 0;
			}
			m_bossTimer += fDT;
			vPos.x += 25.f * fDT;
			if (tileActivate)
			{
				m_dMoveTimer += DT;
				if (m_dMoveTimer >= 0.05)
				{
					m_dMoveTimer = 0;
					MoveTiles();
				}
			}
		}
		if (vPos.x <= 8810.f)
		{
			positionAdjust = true;
		}
		else if (vPos.x>8810.f)
		{
			if (!positionAdjust)
			{
				vPos.x -= 300.f * fDT;
			}
		}

		if (m_bossTimer >= 2.f)
		{
			m_bossVital = new CBossVital(this);
			prevVital = m_bossVital->GetVital();
			CreateObj(m_bossVital, GROUP_GAMEOBJ::UI);
			m_bossTimer = 0.f;
			m_state = eState_GW::TILEMOVE;		
		}


	}
		break;
	case eState_GW::TILEMOVE:
	{
		
		if (m_floor == 0)
		{
			m_gravity = D_GRAVITY;
			vPos.y += D_GRAVITY * fDT;
		}
		else
		{
			m_gravity = 0;
		}

		m_bossTimer += fDT;
		m_chestnutTimer += fDT;
		m_dMoveTimer += DT;
		if (m_dMoveTimer >= 0.10)
		{
			for (int i = 0; i < pChestnuts.size(); ++i)
			{
				pChestnuts[i]->Shake();
			}
			m_dMoveTimer = 0;
			MoveTiles();
		}

		GetAnimator()->Play(L"Idle");
		if (m_chestnutTimer >= 1.5f)
		{
			DropChestnut();
			m_chestnutTimer = 0.f;
		}
		if (m_bossTimer >= 3.f)
		{
			m_bossTimer = 0.f;
			m_state = eState_GW::ATTACK;
		}
		if (prevVital > m_bossVital->GetVital())
		{
			CSoundManager::getInst()->Play(L"boss_damaged");
			m_bossTimer = 0.f;
			m_state = eState_GW::HURT;
		}
		if (GetHP() <= 0)
		{
			DeleteObj(m_bossVital);
			m_bossTimer = 0.f;
			CSoundManager::getInst()->Play(L"boss_die");
			m_state = eState_GW::DEAD;
		}
	
	}
		break;
	case eState_GW::ATTACK:
	{
		m_shootTimer += fDT;
		m_bossTimer += fDT;

		if (m_shootTimer >= 0.66f && canShoot)
		{
			CSoundManager::getInst()->Play(L"boss_shoot");
			CreateMissile();
			canShoot = false;
		}
		if (m_shootTimer >= 1.f)
		{
			canShoot = true;
			GetAnimator()->Play(L"Shoot");
			m_shootTimer = 0.f;
		}
		if (m_bossTimer >= 5.f)
		{
			m_state = eState_GW::TILEMOVE;
			m_bossTimer = 0.f;
		}
		if (prevVital > m_bossVital->GetVital())
		{
			CSoundManager::getInst()->Play(L"boss_damaged");
			m_bossTimer = 0.f;
			m_state = eState_GW::HURT;
		}
		if (GetHP() <= 0)
		{
			DeleteObj(m_bossVital);
			m_bossTimer = 0.f;
			CSoundManager::getInst()->Play(L"boss_die");
			m_state = eState_GW::DEAD;
		}
	}
		break;
	case eState_GW::HURT:
	{
		m_bossTimer += fDT;
		if (m_bossTimer>=0.5f)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(1, 2);
			if (dis(gen) == 1)
			{
				m_state = eState_GW::ATTACK;
			}
			else if (dis(gen) == 2)
			{
				m_state = eState_GW::TILEMOVE;
			}
		}
		GetAnimator()->Play(L"Damaged");
		if (GetHP() <= 0)
		{
			DeleteObj(m_bossVital);
			m_bossTimer = 0.f;
			CSoundManager::getInst()->Play(L"boss_die");
			m_state = eState_GW::DEAD;
		}

	}break;
	case eState_GW::DEAD:
	{
		GetAnimator()->Play(L"Damaged");
		m_bossTimer += fDT;
		m_shootTimer += fDT;
		if (m_shootTimer >= 0.1f)
		{
			m_shootTimer = 0.f;
			Explode();
		}

		if (m_bossTimer >= 3.f)
		{
			m_bossTimer = 0.f;
			for (int i = 0; i < pChestnuts.size(); ++i)
			{
				if (pChestnuts[i]->GetHP() > 0)
				{
					DeleteObj(pChestnuts[i]);
				}
			}pChestnuts.clear();
			DeleteObj(this);
		}


	}
		break;
	}



	for (int i = 0; i < pChestnuts.size(); ++i)
	{
		pChestnuts[i]->SetPos(
			Vec2(GetPos().x - 200.f + pChestnuts[i]->GetOffset().x
			, GetPos().y - 200.f + pChestnuts[i]->GetOffset().y));
	}


	SetPos(vPos);
	if (nullptr != m_bossVital)
	{
		prevVital = m_bossVital->GetVital();
	}
	GetAnimator()->update();
}

void CGolemWood::render()
{
	component_render();
	render_information();
}

void CGolemWood::render_information()
{
	if (true == CCore::getInst()->DebugMode())
	{
		CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"BackInfo", L"texture\\BackInfo.png");
		Vec2 vPos = GetPos();
		vPos = CCameraManager::getInst()->GetRenderPos(vPos);

		CRenderManager::getInst()->RenderImage(
			pImg
			, vPos.x - 200.f
			, vPos.y + -40.f
			, vPos.x + 100.f
			, vPos.y + 100.f
			,0.3f);

		////////////////////////
		wstring curAni = {};
		////////////////////////
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + L"" + L"\n" +
			L" curAm : " + curAni + L"\n" +
			L" HP:  " + std::to_wstring(GetHP()) + L"\n" +
			L" wallCount : " + std::to_wstring(m_floor)
			, vPos.x - 200.f
			, vPos.y + -40.f
			, vPos.x + 100.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}
}


void CGolemWood::CreateMissile()
{
	Vec2 fpMissilePos = GetPos();
	CMissile* pMissile = new CMissile;


	pMissile->SetScale(Vec2(30.f, 30.f));
	pMissile->SetPos(Vec2(fpMissilePos.x + 3.f, fpMissilePos.y + 13.f));
	pMissile->GetCollider()->SetFinalPos(pMissile->GetPos());
	pMissile->GetCollider()->SetScale(Vec2(pMissile->GetScale().x, pMissile->GetScale().y));
	pMissile->GetAnimator()->Play(L"Missile_GW");
	//pMissile->GetCollider()->SetOffsetPos(Vec2(30.f, 0.f));

	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	Vec2 vPos = vPlayerPos - fpMissilePos;
	pMissile->SetDir(Vec2(vPos.x, vPos.y));
	pMissile->SetSpeed(300.f);
	pMissile->SetName(L"Missile_GolemWood");
	//if(abs(vPos.x)<=700.f)

	CreateObj(pMissile, GROUP_GAMEOBJ::MISSILE_MONSTER);
}

void CGolemWood::MoveTiles()
{
	Vec2 vFirstTilePos = pMovingTiles[pMovingTiles.size()-4]->GetPos();
	pMovingTiles[pMovingTiles.size() - 4]->prevTilePos = vFirstTilePos;

	m_fTheta += m_fSpd * fDT;
	vFirstTilePos.y = m_radius * (float)sin(m_fTheta);
	vFirstTilePos.y += y_center;


	pMovingTiles[pMovingTiles.size() - 4]->SetPos(vFirstTilePos);

	for (int i = 0; i < pMovingTiles.size(); ++i)
	{
		pMovingTiles[i]->SetMaxHeight(430.f - 20.f * (float)i);
	}

	for (int i = pMovingTiles.size() - 4; i > 0; --i)
	{
		if (pMovingTiles[i - 1]->GetMaxheight() >= pMovingTiles[i]->prevTilePos.y)
		{
			pMovingTiles[i - 1]->prevTilePos = pMovingTiles[i - 1]->GetPos();
			pMovingTiles[i - 1]->SetPos(Vec2(pMovingTiles[i - 1]->GetPos().x, pMovingTiles[i - 1]->GetMaxheight()));
		}
		else
		{
			pMovingTiles[i - 1]->prevTilePos = pMovingTiles[i - 1]->GetPos();
			pMovingTiles[i - 1]->SetPos(Vec2(pMovingTiles[i - 1]->GetPos().x, pMovingTiles[i]->prevTilePos.y));
		}		
	}
	for (int i = pMovingTiles.size() - 3; i < pMovingTiles.size() ; ++i)
	{
		pMovingTiles[i]->SetPos(Vec2(pMovingTiles[i]->GetPos().x, pMovingTiles[pMovingTiles.size() - 4]->GetPos().y));
	}


	/// ///////////////////////////////////////////////////

	int minTileY = 9999;
	int minIndex=-1;
	int maxTileY = 0;
	int maxIndex=-1;
	for (int i = 1; i <pMovingTiles.size()-1; ++i) 
	{
		if (pMovingTiles[i]->GetPos().y > maxTileY)
		{
			maxTileY = pMovingTiles[i]->GetPos().y;
			maxIndex = i;
		}
	}
	for (int i = 0; i < pMovingTiles.size(); ++i)
	{
		if (pMovingTiles[i]->GetPos().y < minTileY)
		{
			minTileY = pMovingTiles[i]->GetPos().y;
			minIndex = i;
		}
	}

	for (int i = 0; i < pMovingTiles.size(); ++i)
	{
		Vec2 vPos = pMovingTiles[i]->GetPos();


		if (0 == i|| pMovingTiles.size()-1==i)
		{
			continue;
		}
		if (i == maxIndex || i==minIndex)
		{
			pMovingTiles[i]->SetState(1);
			pMovingTiles[i]->isEndline = true;
		}
		else
		{
			pMovingTiles[i]->isEndline = false;
		}

		if (pMovingTiles[i]->GetPos().y > pMovingTiles[i - 1]->GetPos().y)
		{
			if(pMovingTiles[i]->GetPos().y<pMovingTiles[i + 1]->GetPos().y)
				pMovingTiles[i]->SetRight(true);
		}
		else if (pMovingTiles[i]->GetPos().y <  pMovingTiles[i - 1]->GetPos().y)
		{
			if (pMovingTiles[i]->GetPos().y > pMovingTiles[i + 1]->GetPos().y)
				pMovingTiles[i]->SetRight(false);
		}
	}

	
}

void CGolemWood::DropChestnut()
{
	CChestnut* pChestnut = new CChestnut();
	pChestnut->SetPos(Vec2(GetPos().x-30.f,GetPos().y-300.f));
	CreateObj(pChestnut, GROUP_GAMEOBJ::MONSTER);
}

void CGolemWood::Explode()
{

	int iHeight = m_pImg->GetHeight();
	int iWidth = m_pImg->GetWidth();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, iHeight / 2);


	//////////////////////ÀÌÆåÆ®///////////////
	CEffect* effectExplode = new CEffect(L"Effect_Explode_Small", L"texture\\Animation\\Effect_Explode_Small.png",
		L"Effect_Explode_Small", Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.05f, 10, false, 1.f);
	effectExplode->SetPos(Vec2(GetPos().x - GetScale().x / 2.f + dis(gen), GetPos().y - GetScale().y / 2.f + dis(gen)));
	effectExplode->SetDuration(1.f);
	CreateObj(effectExplode, GROUP_GAMEOBJ::EFFECT);
	///////////////////////////////////////////

}

void CGolemWood::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
	if (pOther->GetName() == L"Lou")
	{
		
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
}

void CGolemWood::OnCollision(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")// && m_state!=eState_GW::PREPARE)
	{
		int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
		int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
		int sum = abs(a - b);
		if (1 < sum)
			vPos.y-=3.f;
	}

	SetPos(vPos);
}

void CGolemWood::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}


