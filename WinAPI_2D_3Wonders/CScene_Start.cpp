#include "pch.h"
#include "CScene_Start.h"
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

#include "CTempTile.h"

CScene_Start::CScene_Start()
{
}

CScene_Start::~CScene_Start()
{
}

CSound* pSound = nullptr;

void CScene_Start::update()
{
	CScene::update();
	if (KEYDOWN(VK_ESCAPE))
	{
		CEventManager::getInst()->EventChangeScene(GROUP_SCENE::TOOL);
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

void CScene_Start::Enter()
{
	wstring path = CPathManager::getInst()->GetContentPath();
	path += L"tile\\test4.tile";
	LoadTile(path);

	CBG1* pBG1 = new CBG1();
	pBG1->SetPos(Vec2(0.f, 0.f));
	pBG1->SetScale(Vec2(1.f, 1.f));
	AddObject(pBG1, GROUP_GAMEOBJ::BACKGROUND);

	CLou* pLou = new CLou();
	pLou->SetPos(Vec2(300.f, -100.f));
	AddObject(pLou, GROUP_GAMEOBJ::PLAYER);

	//CSoundManager::getInst()->AddSound(L"test", L"sound\\B4VHMGUN.wav", false);
	//CSoundManager::getInst()->AddSound(L"test2", L"sound\\B4VLASER.wav", false);
	//CSoundManager::getInst()->AddSound(L"test3", L"sound\\B4VHELL.wav", false);

	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::MISSILE_PLAYER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::TILE, GROUP_GAMEOBJ::MISSILE_PLAYER);
	CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
	//CCameraManager::getInst()->FadeOut(5.f);
	CCameraManager::getInst()->FadeIn(1.f);
	
}

void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionManager::getInst()->Reset();
}
