#include "pch.h"
#include "CSceneManager.h"
#include "CScene_Stage1.h"
#include "CScene_Tool.h"
#include "CScene_Selection.h"
#include "CScene_Opening.h"
#include "CScene_Ending.h"
#include "CScene_Intro.h"

CSceneManager::CSceneManager()
{
	for (UINT i = 0; i < (UINT)GROUP_SCENE::SIZE; ++i)
	{
		m_arrScene[i] = nullptr;
	}
	m_pCurScene = nullptr;
}
CSceneManager::~CSceneManager()
{
	for (UINT i = 0; i < (UINT)GROUP_SCENE::SIZE; ++i)
	{
		if (nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void CSceneManager::ChangeScene(GROUP_SCENE group)
{
	m_pCurScene->Exit();
	m_pCurScene = m_arrScene[(UINT)group];
	m_pCurScene->Enter();
}

void CSceneManager::update()
{
	m_pCurScene->update(); //현재 선택된 씬 업데이트
	m_pCurScene->finalupdate();
}

void CSceneManager::render()
{
	m_pCurScene->render(); //현재 선택된 씬 렌더 
}

void CSceneManager::init()
{
	//TODO: 모든 씬 생성

	m_arrScene[(UINT)GROUP_SCENE::INTRO] = new CScene_Intro;
	m_arrScene[(UINT)GROUP_SCENE::INTRO]->SetName(L"Intro_Scene");

	m_arrScene[(UINT)GROUP_SCENE::SELECT] = new CScene_Selection;
	m_arrScene[(UINT)GROUP_SCENE::SELECT]->SetName(L"Selection_Scene");

	m_arrScene[(UINT)GROUP_SCENE::OPENING] = new CScene_Opening;
	m_arrScene[(UINT)GROUP_SCENE::OPENING]->SetName(L"Opening_Scene");

	m_arrScene[(UINT)GROUP_SCENE::STAGE1] = new CScene_Stage1;
	m_arrScene[(UINT)GROUP_SCENE::STAGE1]->SetName(L"Stage1_Scene");

	m_arrScene[(UINT)GROUP_SCENE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)GROUP_SCENE::TOOL]->SetName(L"Tool_Scene");

	m_arrScene[(UINT)GROUP_SCENE::ENDING] = new CScene_Ending;
	m_arrScene[(UINT)GROUP_SCENE::ENDING]->SetName(L"Ending_Scene");


	

	m_pCurScene = m_arrScene[(UINT)GROUP_SCENE::STAGE1];//현재씬지정
	m_pCurScene->Enter();
}
