#include "pch.h"
#include "CScene_Opening.h"
#include "COpening.h"

CScene_Opening::CScene_Opening()
{

}

CScene_Opening::~CScene_Opening()
{

}



void CScene_Opening::update()
{
	CScene::update();
	if (KEYDOWN('Z') || KEYDOWN('X'))
	{
		ChangeScn(GROUP_SCENE::STAGE1);
	}
}

void CScene_Opening::Enter()
{
	COpening* pOpening = new COpening();
	AddObject(pOpening, GROUP_GAMEOBJ::BACKGROUND);

	CSoundManager::getInst()->AddSound(L"opening_demo", L"sound\\opening_demo.wav", false);
	CSoundManager::getInst()->Play(L"opening_demo");
	CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
}

void CScene_Opening::Exit()
{
	DeleteAll();
	CSoundManager::getInst()->Stop(L"opening_demo");
}
