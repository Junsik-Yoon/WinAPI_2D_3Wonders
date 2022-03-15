#include "pch.h"
#include "CUI.h"
#include "CGameObject.h"

CUI::CUI()
{
	m_pParentUI = nullptr;
	m_bCameraAffected = false;
	m_bMouseOn = false;
	m_bLbtnDown = false;
}

CUI::CUI(const CUI& other)
	:CGameObject(other)
{
	m_pParentUI = nullptr;
	m_bCameraAffected = other.m_bCameraAffected;
	m_bMouseOn = false;
	m_bLbtnDown = false;

	m_vecChildUI.clear();

	for (UINT i = 0; i < other.m_vecChildUI.size(); ++i)
	{
		AddChild(other.m_vecChildUI[i]->Clone());
	}
}

CUI::~CUI()
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		if (nullptr != m_vecChildUI[i])
			delete m_vecChildUI[i];//지울때 자식 지우고 자신도 지워야함
	}
}

void CUI::update()
{

	update_child();
}

void CUI::update_child()
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void CUI::finalupdate()
{
	CGameObject::finalupdate();

	m_fptFinalPos = GetPos();
	if (GetParent())//부모가있는경우
	{
		Vec2 fptParentPos = GetParent()->GetFinalPos();
		m_fptFinalPos += fptParentPos;//부모의포지션을 상대포지션만큼 더함


	}
	MouseOnCheck();
	finalupdate_child();
}



void CUI::finalupdate_child()
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->finalupdate();
	}
}


void CUI::render()
{
	Vec2 fptPos = GetFinalPos();
	Vec2 fptScale = GetScale();

	if (m_bCameraAffected)//카메라 영향을 받으면 렌더링 좌표를 카메라 기준으로 하여야 한다
	{
		fptPos = CCameraManager::getInst()->GetRenderPos(fptPos);
	}

	if (m_bLbtnDown)//클릭시색변경
	{
		CRenderManager::getInst()->RenderRectangle(
			fptPos.x,
			fptPos.y,
			fptPos.x + fptScale.x,
			fptPos.y + fptScale.y,
			RGB(0, 255, 0));
	}
	else
	{
		CRenderManager::getInst()->RenderRectangle(
			fptPos.x,
			fptPos.y,
			fptPos.x + fptScale.x,
			fptPos.y + fptScale.y,
			RGB(0, 0, 0));
	}
	render_child();
}


void CUI::render_child()
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->render();
	}
}

void CUI::AddChild(CUI* pUI)
{
	m_vecChildUI.push_back(pUI);
	pUI->m_pParentUI = this;//큰창을부모로
}

void CUI::MouseOnCheck()
{
	Vec2 fptMousePos = MousePos();
	Vec2 fptScale = GetScale();

	if (m_bCameraAffected)
	{
		fptMousePos = CCameraManager::getInst()->GetRealPos(fptMousePos);
	}
	if (m_fptFinalPos.x <= fptMousePos.x && fptMousePos.x <= m_fptFinalPos.x + fptScale.x &&
		m_fptFinalPos.y <= fptMousePos.y && fptMousePos.y <= m_fptFinalPos.y + fptScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}

void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}

const vector<CUI*>& CUI::GetChildUI()
{
	return m_vecChildUI;
}




