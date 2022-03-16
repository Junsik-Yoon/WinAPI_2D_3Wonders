#include "pch.h"
#include "CScene_Start.h"
#include "CGameObject.h"
#include "CScene_Tool.h"
#include "CSound.h"
#include "CBG1.h"
#include "CLou.h"
#include "CGoblin.h"
#include "CGreen.h"

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
	path += L"tile\\test2.tile";
	LoadTile(path);
	CBG1* pBG1 = new CBG1();
	pBG1->SetPos(Vec2(0.f, 0.f));
	pBG1->SetScale(Vec2(1.f, 1.f));
	AddObject(pBG1, GROUP_GAMEOBJ::BACKGROUND);
	//wstring path = CPathManager::getInst()->GetContentPath();
	//파일이름유의 -> 불러올파일이름
	//path += L"Tile\\test.tile";
	//LoadTile(path);

	CLou* pLou = new CLou();
	pLou->SetPos(Vec2(300.f, -100.f));
	AddObject(pLou, GROUP_GAMEOBJ::PLAYER);

	CGoblin* pGoblin1 = new CGoblin();
	pGoblin1->SetPos(Vec2(700.f, 100.f));
	AddObject(pGoblin1, GROUP_GAMEOBJ::MONSTER);

	CGreen* pGreen1 = new CGreen();
	pGreen1->SetPos(Vec2(850.f, 100.f));
	AddObject(pGreen1, GROUP_GAMEOBJ::MONSTER);

	//CSoundManager::getInst()->AddSound(L"test", L"sound\\B4VHMGUN.wav", false);
	//CSoundManager::getInst()->AddSound(L"test2", L"sound\\B4VLASER.wav", false);
	//CSoundManager::getInst()->AddSound(L"test3", L"sound\\B4VHELL.wav", false);

	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::TILE);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::PLAYER, GROUP_GAMEOBJ::MONSTER);
	CCollisionManager::getInst()->CheckGroup(GROUP_GAMEOBJ::MONSTER, GROUP_GAMEOBJ::TILE);
	CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
	//CCameraManager::getInst()->FadeOut(5.f);
	CCameraManager::getInst()->FadeIn(1.f);
}

void CScene_Start::Exit()
{
	DeleteAll();

	CCollisionManager::getInst()->Reset();
}
