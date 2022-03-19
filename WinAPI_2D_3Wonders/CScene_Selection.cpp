#include "pch.h"
#include "CScene_Selection.h"
#include "CGameSelect.h"

CScene_Selection::CScene_Selection()
{
}

CScene_Selection::~CScene_Selection()
{
}

void CScene_Selection::update()
{
	CScene::update();

}

void CScene_Selection::Enter()
{

	CGameSelect* pGameSelect = new CGameSelect();
	AddObject(pGameSelect, GROUP_GAMEOBJ::BACKGROUND);
	CSoundManager::getInst()->AddSound(L"select_start", L"sound\\select_start.wav", true);
	CSoundManager::getInst()->AddSound(L"select_change", L"sound\\select_change.wav", false); 
	CSoundManager::getInst()->AddSound(L"select_scene_selected", L"sound\\select_scene_selected.wav", false);
	CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));
}

void CScene_Selection::Exit()
{
	DeleteAll();
	CSoundManager::getInst()->Stop(L"select_start");
	CSoundManager::getInst()->Stop(L"select_change");
	CSoundManager::getInst()->Stop(L"select_scene_selected");
}
