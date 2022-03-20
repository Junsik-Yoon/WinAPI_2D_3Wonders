#include "pch.h"
#include "CScene_Stage1.h"
#include "CGameObject.h"
#include "CScene_Tool.h"
#include "CSound.h"
#include "CBG1.h"
#include "CLou.h"
#include "CGoblin.h"
#include "CGreen.h"
#include "CHalfMoon.h"
#include "CBug.h"
#include "CCollider.h"
#include "CShelter.h"
#include "CGolemWood.h"
#include "CMovingTile.h"

CScene_Stage1::CScene_Stage1()
{
}

CScene_Stage1::~CScene_Stage1()
{
}

CSound* pSound = nullptr;

void CScene_Stage1::update()
{
	CScene::update();
	if (KEYDOWN(VK_ESCAPE))
	{
		CEventManager::getInst()->EventChangeScene(GROUP_SCENE::TOOL);
	}
	if (KEY('A'))
	{
		CCameraManager::getInst()->SetTargetX(nullptr);
	}

	if (KEY('A'))
	{
		CCameraManager::getInst()->Scroll(Vec2(-1, 0), m_velocity);
	}
	if (KEY('D'))
	{
		CCameraManager::getInst()->Scroll(Vec2(1, 0), m_velocity);
	}
	if (KEY('W'))
	{
		CCameraManager::getInst()->Scroll(Vec2(0, -1), m_velocity);
	}
	if (KEY('S'))
	{
		CCameraManager::getInst()->Scroll(Vec2(0, 1), m_velocity);
	}

}

void CScene_Stage1::Enter()
{
	wstring path = CPathManager::getInst()->GetContentPath();
	path += L"tile\\stage1.tile";

	LoadTile(path);
	
	CBG1* pBG1 = new CBG1();
	pBG1->SetPos(Vec2(0.f, 0.f));
	pBG1->SetScale(Vec2(1.f, 1.f));
	AddObject(pBG1, GROUP_GAMEOBJ::BACKGROUND);

	CLou* pLou = new CLou();
	pLou->SetPos(Vec2(300.f, -100.f));
	AddObject(pLou, GROUP_GAMEOBJ::PLAYER);

	CGolemWood* pGolemWood = new CGolemWood();
	pGolemWood->SetPos(Vec2(8200.f, 150.f));
	AddObject(pGolemWood, GROUP_GAMEOBJ::MONSTER);

	CShelter* pShelter1 = new CShelter();
	pShelter1->SetPos(Vec2(1570.f, 230.f));
	AddObject(pShelter1, GROUP_GAMEOBJ::TILE);

	CShelter* pShelter2 = new CShelter();
	pShelter2->SetPos(Vec2(5350.f, -35.f));
	AddObject(pShelter2, GROUP_GAMEOBJ::TILE);

	CShelter* pShelter3 = new CShelter();
	pShelter3->SetPos(Vec2(7270.f, 280.f));
	AddObject(pShelter3, GROUP_GAMEOBJ::TILE);

	CMovingTile* pMovingTile = nullptr;
	for (int i = 0; i < 24; ++i)
	{
		pMovingTile = new CMovingTile();
		pMovingTile->SetPos(Vec2(7600.f+(32.f*i), 450.f+(float)i*-7.f));
		AddObject(pMovingTile, GROUP_GAMEOBJ::TILE);		
	}

	vector<CGameObject*> pTiles = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::TILE);
	for (int i = 0; i < pTiles.size(); ++i)
	{
		CMovingTile* pTile = (CMovingTile*)pTiles[i];
		if (pTile->GetGroup() == GROUP_TILE::MOVE)
		{
			pGolemWood->pMovingTiles.push_back(pTile);
		}
	}


	CSoundManager::getInst()->AddSound(L"stage1_1_bgm", L"sound\\stage1_1_bgm.wav", true);
	//CSoundManager::getInst()->Play(L"stage1_1_bgm");//TODO:사운드되살리기
	
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::MISSILE_PLAYER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::TILE, GROUP_GAMEOBJ::MISSILE_PLAYER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::TILE, GROUP_GAMEOBJ::MISSILE_MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::MISSILE_MONSTER);

	CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
	//CCameraManager::getInst()->FadeOut(5.f);
	CCameraManager::getInst()->FadeIn(1.f);
	CCameraManager::getInst()->SetTargetX(pGolemWood);
}

void CScene_Stage1::Exit()
{
	DeleteAll();
	//CSoundManager::getInst()->Stop(L"stage1_1_bgm");
	CCollisionManager::getInst()->Reset();
}
