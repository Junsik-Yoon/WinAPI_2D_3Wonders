#include "pch.h"
#include "CEventManager.h"
#include "CGameObject.h"
#include "CScene.h"

CEventManager::CEventManager()
{
}
CEventManager::~CEventManager()
{
}

void CEventManager::Execute(const tEvent& event)
{
	switch (event.eEven)
	{
	case TYPE_EVENT::CREATE_OBJECT:
	{
		// lParam : 오브젝트 주소
		// wParam : 그룹 
		CGameObject* pObj = (CGameObject*)event.lParam;
		GROUP_GAMEOBJ group = (GROUP_GAMEOBJ)event.wParam;

		CSceneManager::getInst()->GetCurScene()->AddObject(pObj, group);
	}
	break;
	case TYPE_EVENT::DELETE_OBJECT:
	{
		// lParam : 오브젝트 주소
		// object를 삭제대기상태로 변경
		CGameObject* pObj = (CGameObject*)event.lParam;
		pObj->SetDead();
		m_vecDead.push_back(pObj);
	}
	break;
	case TYPE_EVENT::CHANGE_SCENE:
	{
		// lParam : 씬 그룹
		GROUP_SCENE scene = (GROUP_SCENE)event.lParam;
		CUIManager::getInst()->SetFocusedUI(nullptr);
		CSceneManager::getInst()->ChangeScene(scene);
	}
	break;
	}
}

void CEventManager::update()
{
	// 삭제 예정 오브젝트들을 삭제
	for (int i = 0; i < m_vecDead.size(); i++)
	{
		delete m_vecDead[i];//TODO:미사일이 타일 2개이상을 동시에 타격시 삭제에러가난다
	}
	m_vecDead.clear();

	// 이벤트 실행
	for (int i = 0; i < m_vecEvent.size(); i++)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventManager::AddEvent(const tEvent& event)
{
	m_vecEvent.push_back(event);
}

void CEventManager::EventCreateObject(CGameObject* pObj, GROUP_GAMEOBJ group)
{
	tEvent event = {};
	event.eEven = TYPE_EVENT::CREATE_OBJECT;
	event.lParam = (DWORD_PTR)pObj;
	event.wParam = (DWORD_PTR)group;

	AddEvent(event);
}

void CEventManager::EventDeleteObject(CGameObject* pObj)
{
	tEvent event = {};
	event.eEven = TYPE_EVENT::DELETE_OBJECT;
	event.lParam = (DWORD_PTR)pObj;

	AddEvent(event);
}

void CEventManager::EventChangeScene(GROUP_SCENE scene)
{
	tEvent event = {};
	event.eEven = TYPE_EVENT::CHANGE_SCENE;
	event.lParam = (DWORD_PTR)scene;

	AddEvent(event);
}
