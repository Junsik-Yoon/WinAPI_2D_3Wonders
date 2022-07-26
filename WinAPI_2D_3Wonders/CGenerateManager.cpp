#include "pch.h"
#include "CGenerateManager.h"
#include "CTile.h"
#include "CLou.h"
#include "CScene.h"
#include "CGoblin.h"
#include "CEffect.h"

#include <iostream>
#include <random>

#define D_FROMGOBMAX 400
#define D_FROMGOBMIN 80

CGenerateManager::CGenerateManager()
{
	goblinTimer = 0.f;
}
CGenerateManager::~CGenerateManager()
{

}

void CGenerateManager::update()
{
	if (CSceneManager::getInst()->GetCurScene()->GetName() == L"Stage1_Scene")
	{
		goblinTimer += fDT;
		init();
		inited = true;
	}
	if (goblinTimer >= (float)goblinGenTime)
	{
		goblinTimer = 0.f;
		GenerateGoblin();
	}
}

void CGenerateManager::init()
{
	if (inited) return;
	pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	pGroundTile = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::TILE);
}

void CGenerateManager::GenerateGoblin()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	vector<CTile*>pTiles;

	Vec2 playerPos = pPlayer[0]->GetPos();

	
	for (int i = 0; i < pGroundTile.size(); ++i)
	{
		CTile* pTile = (CTile*)pGroundTile[i];
		if (D_FROMGOBMAX > abs(playerPos.x - pTile->GetPos().x) &&
			D_FROMGOBMIN < abs(playerPos.x - pTile->GetPos().x))
		{
			if (pTile->GetGroup() == GROUP_TILE::GROUND ||
				pTile->GetGroup() == GROUP_TILE::PLATFORM)
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
		if (pGoblin->GetPos().x < playerPos.x)
		{
			pGoblin->SetFacedRight(true);
		}
		else
		{
			pGoblin->SetFacedRight(false);
		}

		//////////////////////ÀÌÆåÆ®///////////////
		CEffect* effectGobGen = new CEffect(L"GoblinGenEff", L"texture\\Animation\\Effect_Goblin_Gen.png",
			L"Goblin_Gen_Eff", Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.1f, 10, false, false, L"GoblinGenEff");
		effectGobGen->SetPos(Vec2(pGoblin->GetPos().x, pGoblin->GetPos().y + 60.f));
		effectGobGen->SetDuration(2.f);
		CreateObj(effectGobGen, GROUP_GAMEOBJ::EFFECT);
		///////////////////////////////////////////
		CreateObj(pGoblin, GROUP_GAMEOBJ::MONSTER);
	}
}

