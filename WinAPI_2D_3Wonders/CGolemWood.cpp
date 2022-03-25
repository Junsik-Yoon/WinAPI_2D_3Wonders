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

#define D_GRAVITY 400
#define D_TILESPEED 200

#define MINY 400.f
#define MAXY 320.f

CGolemWood::CGolemWood()
{
	m_floor = 0;
	SetHP(30);
	m_gravity = D_GRAVITY;
	m_tileSpeed = D_TILESPEED;
	m_missileCounter = 0.f;
	m_stopCounter = 0.f;
	SetName(L"GolemWood");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GolemWoodImg", L"texture\\Animation\\Animation_GolemWood.png");
	CreateCollider();
	SetScale(Vec2(10.f, 300.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(15.f, -20.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Idle", m_pImg, Vec2(0.f, 0.f), Vec2(512.f, 512.f), Vec2(512.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Shoot", m_pImg, Vec2(512.f, 0.f), Vec2(512.f, 512.f), Vec2(512.f, 0.f), 0.5f, 3, false);
	GetAnimator()->CreateAnimation(L"Damaged", m_pImg, Vec2(2048.f, 0.f), Vec2(512.f, 512.f), Vec2(512.f, 0.f), 0.5f, 1, false);

	GetAnimator()->Play(L"Idle");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CGolemWood::~CGolemWood()
{
}


void CGolemWood::update()
{
	Vec2 vPos = GetPos();

	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	if (abs(pPlayer[0]->GetPos().x - GetPos().x) <= 600.f
		|| KEY('Q'))
	{

		//TODO:플레이어가 일정구역 안에 들어오면 동적할당
		if (m_stopCounter == 0.f)
		{
			m_missileCounter += fDT;
			MoveTiles();
		}
		else
		{
			m_stopCounter += fDT;
		}
		if (m_missileCounter >= 3.f)
		{
			CreateMissile();
			GetAnimator()->Play(L"Shoot");
			m_missileCounter = 0.f;
			m_stopCounter += fDT;

		}
		if (m_stopCounter >= 2.f)
		{
			GetAnimator()->Play(L"Idle");
			m_stopCounter = 0.f;
			m_missileCounter += fDT;
		}


		if (m_floor == 0)
		{
			m_gravity = D_GRAVITY;
			vPos.y += D_GRAVITY * fDT;
		}
		else
		{
			m_gravity = 0;
		}
	}
	SetPos(vPos);

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
		if (vPos.y <= MAXY)			
		{
			pMovingTiles[i]->Y_axis = 1;
		}
		if (vPos.y >= MINY)
		{
			pMovingTiles[i]->Y_axis = -1;
		}

		vPos.y += m_tileSpeed * fDT * pMovingTiles[i]->Y_axis;
		pMovingTiles[i]->SetPos(vPos);
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
	new CChestnut();
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
	if (GetHP() <= 0)
	{
		DeleteObj(this);
	}
}

void CGolemWood::OnCollision(CCollider* _pOther)
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

void CGolemWood::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}


