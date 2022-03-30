#include "pch.h"
#include "CScene_Stage1.h"
#include "CScene_Tool.h"

#include "CSound.h"
#include "CCollider.h"
#include "CAnimator.h"

#include "CGameObject.h"
#include "CBossTrigger.h"

#include "CBG1.h"
#include "CBG2.h"

#include "CShelter.h"
#include "CMovingTile.h"
#include "CChest.h"
#include "CFlyingChest.h"

#include "CCard.h"
#include "CCoin.h"
#include "COptionFire.h"
#include "CMissileItem.h"
#include "CLamp.h"

#include "CScoreUI.h"
#include "CStatusUI.h"

#include "CLou.h"
#include "CGoblin.h"
#include "CGreen.h"
#include "CHalfMoon.h"
#include "CBug.h"
#include "CGolemWood.h"
#include "CSlopeTile.h"


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
	path += L"tile\\stage1-1.tile";
	LoadTile(path);
	
	CBG1* pBG1 = new CBG1();
	pBG1->SetPos(Vec2(0.f, 0.f));
	pBG1->SetScale(Vec2(1.f, 1.f));
	AddObject(pBG1, GROUP_GAMEOBJ::MAP);

	CBG2* pBG_back = new CBG2();
	pBG_back->setIndex(2);
	AddObject(pBG_back, GROUP_GAMEOBJ::BACKGROUND);

	CBG2* pBG2 = new CBG2();
	pBG2->setIndex(1);
	AddObject(pBG2, GROUP_GAMEOBJ::BACKGROUND);

	CScoreUI* pScoreUI = new CScoreUI();
	AddObject(pScoreUI, GROUP_GAMEOBJ::UI);

	CStatusUI* pStatusUI = new CStatusUI();
	AddObject(pStatusUI, GROUP_GAMEOBJ::UI);



	CLou* pLou = new CLou();
	pLou->SetPos(Vec2(200.f, 100.f));
	AddObject(pLou, GROUP_GAMEOBJ::PLAYER);

	CHalfMoon* pHalfMoon1 = new CHalfMoon();
	pHalfMoon1->SetPos(Vec2(2985.f, 70.f));
	pHalfMoon1->SetRight(true);
	pHalfMoon1->GetCollider()->SetOffsetPos(Vec2(-20.f, 0.f));
	AddObject(pHalfMoon1, GROUP_GAMEOBJ::MONSTER);

	CHalfMoon* pHalfMoon2 = new CHalfMoon();
	pHalfMoon2->SetPos(Vec2(4590.f, 110.f));
	pHalfMoon2->SetRight(false);
	pHalfMoon2->GetAnimator()->Play(L"Idle_Left");
	pHalfMoon2->GetCollider()->SetOffsetPos(Vec2(20.f, 0.f));
	AddObject(pHalfMoon2, GROUP_GAMEOBJ::MONSTER);

	CGreen* pGreen1 = new CGreen();
	pGreen1->SetPos(Vec2(4480.f, 260.f));
	AddObject(pGreen1, GROUP_GAMEOBJ::MONSTER);

	CGreen* pGreen2 = new CGreen();
	pGreen2->SetPos(Vec2(5050.f, 175.f));
	AddObject(pGreen2, GROUP_GAMEOBJ::MONSTER);

	CShelter* pShelter1 = new CShelter();
	pShelter1->SetPos(Vec2(1570.f, 230.f));
	AddObject(pShelter1, GROUP_GAMEOBJ::TILE);

	CShelter* pShelter2 = new CShelter();
	pShelter2->SetPos(Vec2(5350.f, -35.f));
	AddObject(pShelter2, GROUP_GAMEOBJ::TILE);

	CShelter* pShelter3 = new CShelter();
	pShelter3->SetPos(Vec2(7260.f, 280.f));
	AddObject(pShelter3, GROUP_GAMEOBJ::TILE);

	CChest* pChest1 = new CChest();
	pChest1->SetPos(Vec2(1570.f, 300.f));
	pChest1->Set_Item(new CCard(5));
	AddObject(pChest1, GROUP_GAMEOBJ::ITEM);

	CChest* pChest2 = new CChest();
	pChest2->SetPos(Vec2(2250.f, 140.f));
	pChest2->Set_Item(new CCoin);
	AddObject(pChest2, GROUP_GAMEOBJ::ITEM);

	CChest* pChest3 = new CChest();
	pChest3->SetPos(Vec2(2800.f, 300.f));
	pChest3->SetVisibility(false);
	pChest3->Set_Item(new CMissileItem());
	CMissileItem* pMissileItem = (CMissileItem*)pChest3->Get_Item();
	//pMissileItem->SetRandomMissile();//TODO::미사일 다 만들면 바꾸기
	pMissileItem->SetMissileType(eType::HYPER);
	AddObject(pChest3, GROUP_GAMEOBJ::ITEM);

	CChest* pChest4 = new CChest();
	pChest4->SetPos(Vec2(4130.f, 215.f));
	pChest4->SetVisibility(false);
	pChest4->Set_Item(new COptionFire());
	AddObject(pChest4, GROUP_GAMEOBJ::ITEM);

	CChest* pChest5 = new CChest();
	pChest5->SetPos(Vec2(4450.f, 400.f));
	pChest5->Set_Item(new CLamp()); //TODO:Bound총알로 변경하기
	AddObject(pChest5, GROUP_GAMEOBJ::ITEM);

	CChest* pChest6 = new CChest();
	pChest6->SetPos(Vec2(5350.f, 55.f));
	pChest6->Set_Item(new CCoin());
	AddObject(pChest6, GROUP_GAMEOBJ::ITEM);

	CChest* pChest6_2 = new CChest();
	pChest6_2->SetPos(Vec2(5250.f, 225.f));
	pChest6_2->SetVisibility(false);
	pChest6_2->Set_Item(new CLamp());//TODO:바운드미사일넣기
	AddObject(pChest6_2, GROUP_GAMEOBJ::ITEM);

	CChest* pChest7 = new CChest();
	pChest7->SetPos(Vec2(5910.f, 75.f));
	//pChest7->SetVisibility(false);
	pChest7->Set_Item(new CLamp());
	AddObject(pChest7, GROUP_GAMEOBJ::ITEM);

	CChest* pChest8 = new CChest();
	pChest8->SetPos(Vec2(7250.f, 365.f));
	pChest8->Set_Item(new CCoin());
	AddObject(pChest8, GROUP_GAMEOBJ::ITEM);

	CFlyingChest* pFChest1 = new CFlyingChest();
	pFChest1->SetPos(Vec2(5000.f, 100.f));
	for (int i = 1; i < 6; ++i)
	{
		if (3 == i)
			continue;
		pFChest1->Set_Item(new CCard(i));
	}
	AddObject(pFChest1, GROUP_GAMEOBJ::ITEM);

	CGolemWood* pGolemWood = new CGolemWood();
	pGolemWood->SetPos(Vec2(9300.f, 240.f));
	AddObject(pGolemWood, GROUP_GAMEOBJ::MONSTER);
	
	CMovingTile* pMovingTile = nullptr;
	for (int i = 0; i < 46; ++i)
	{
		pMovingTile = new CMovingTile();
		pMovingTile->SetPos(Vec2(7537.f + (32.f * i), 430.f));
		pMovingTile->SetRight(false);
		AddObject(pMovingTile, GROUP_GAMEOBJ::TILE);
		pGolemWood->SetMovingTiles(pMovingTile);
	}

	CBossTrigger* pBossTrigger = new CBossTrigger();
	pBossTrigger->SetPos(Vec2(7600.f, WINSIZEY / 2.f));
	AddObject(pBossTrigger, GROUP_GAMEOBJ::TILE);

	CBossTrigger* pTileTrigger = new CBossTrigger();
	pTileTrigger->SetPos(Vec2(6600.f, WINSIZEY / 2.f));
	pTileTrigger->SetName(L"TileTrigger");
	AddObject(pTileTrigger, GROUP_GAMEOBJ::TILE);


	//CSlopeTile::create(1312.f, 300.f, 1344.f, 332.f,GROUP_TILE::GROUND);
	//CSlopeTile::create(1344.f, 308.f, 1376.f, 340.f, GROUP_TILE::GROUND);
	//CSlopeTile::create(1376.f, 316.f, 1408.f, 348.f, GROUP_TILE::GROUND);
	//CSlopeTile::create(1408.f, 324.f, 1440.f, 356.f, GROUP_TILE::GROUND);

	CSlopeTile::create(1312.f, 288.f, 1440.f, 320.f, GROUP_TILE::SLOPEDOWN);

	//for (int i = 0; i < 128; ++i)
	//{
	//	CSlopeTile::create(1312.f+i, 288.f +i/4.f, 1313.f + i, 289.f+i / 4.f, GROUP_TILE::GROUND);
	//}


	CCameraManager::getInst()->SetTargetX(pLou);



	CSoundManager::getInst()->AddSound(L"item_change", L"sound\\select_change.wav", false);
	CSoundManager::getInst()->AddSound(L"stage1_1_bgm", L"sound\\stage1_1_bgm.wav", true);
	CSoundManager::getInst()->AddSound(L"monster_die", L"sound\\monster_die.wav", false);
	CSoundManager::getInst()->AddSound(L"explodeSound", L"sound\\explode.wav", false);
	CSoundManager::getInst()->AddSound(L"stage1_1_boss_bgm", L"sound\\stage1_1_boss_bgm.wav", true);

	CSoundManager::getInst()->Play(L"stage1_1_bgm",0.3f);
	
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::MISSILE_PLAYER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::TILE, GROUP_GAMEOBJ::MISSILE_PLAYER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::TILE, GROUP_GAMEOBJ::MISSILE_MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::MISSILE_MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::FIRE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::FIRE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MISSILE_PLAYER, GROUP_GAMEOBJ::ITEM);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::ITEM);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::TILE, GROUP_GAMEOBJ::ITEM);

	CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
	//CCameraManager::getInst()->FadeOut(5.f);
	CCameraManager::getInst()->FadeIn(1.f);


	//위치확인용
	//CCameraManager::getInst()->SetTargetX(pLou);


}

void CScene_Stage1::Exit()
{
	DeleteAll();
	CSoundManager::getInst()->Stop(L"stage1_1_bgm");
	CCollisionManager::getInst()->Reset();
}
