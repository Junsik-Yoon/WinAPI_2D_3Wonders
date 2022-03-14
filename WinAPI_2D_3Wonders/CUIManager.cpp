
#include "pch.h"
#include "CUIManager.h"
#include "CScene.h"
#include "CUI.h"


CUIManager::CUIManager()
{
	m_pFocusedUI = nullptr;
}
CUIManager::~CUIManager()
{

}

void CUIManager::update()
{
	//TODO: 마우스가 올려진 UI, 눌린 ,떼진, 클릭된
	//CScene* pCurScene = CSceneManager::getInst()->GetCurScene();
	//const vector<CGameObject*>& vecUI = pCurScene->GetGroupObject(GROUP_GAMEOBJ::UI);

	m_pFocusedUI = GetFocusedUI();
	if (nullptr == m_pFocusedUI)
		return;
	CUI* pUI = GetTargetUI(m_pFocusedUI);

	//for (UINT i = 0; i < vecUI.size(); ++i)
	//{
	//	CUI* pUI = (CUI*)vecUI[i];

	//	pUI = GetTargetUI(pUI);

	if (nullptr != pUI)//마우스가 올려져있을 때
	{
		pUI->IsMouseOn();
		if (KEYDOWN(VK_LBUTTON)) //마우스가 올려져있으면서 버튼이 눌림
		{
			pUI->MouseLbtnDown();
			pUI->m_bLbtnDown = true;
		}
		else if (KEYUP(VK_LBUTTON))
		{
			pUI->MouseLbtnUp();
			if (pUI->m_bLbtnDown)
			{
				pUI->MouseLbtnClicked();
			}
			pUI->m_bLbtnDown = false;
		}
		//	}
	}

}

void CUIManager::SetFocusedUI(CUI* pUI)
{
	m_pFocusedUI = pUI;
	CScene* pCurScene = CSceneManager::getInst()->GetCurScene();
	vector<CGameObject*>& vecUI = pCurScene->GetUIGroup();

	vector<CGameObject*>::iterator iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			break;
		}
	}
	if (vecUI.end() == iter)
	{
		return;
	}
	vecUI.erase(iter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIManager::GetTargetUI(CUI* pParentUI)
{
	//자식을 전부 순회
	//너비우선탐색 queue로
	static list<CUI*> queue;
	static vector<CUI*> vecNoneTarget;
	CUI* pTargetUI = nullptr;



	queue.push_back(pParentUI);

	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			vecNoneTarget.push_back(pUI);
		}

		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

	}

	if (KEYUP(VK_LBUTTON))
	{
		for (size_t i = 0; i < vecNoneTarget.size(); i++)
		{
			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}

CUI* CUIManager::GetFocusedUI()
{
	CScene* pCurScene = CSceneManager::getInst()->GetCurScene();
	vector<CGameObject*>& vecUI = pCurScene->GetUIGroup();
	CUI* pFocusedUI = m_pFocusedUI;
	if (!KEYDOWN(VK_LBUTTON))
	{
		return pFocusedUI;
	}
	vector<CGameObject*>::iterator targetIter = vecUI.end();
	vector<CGameObject*>::iterator iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}
	pFocusedUI = (CUI*)*targetIter;
	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}
