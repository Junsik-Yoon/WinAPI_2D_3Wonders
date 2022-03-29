#include "pch.h"
#include "CStatusUI.h"
#include "CD2DImage.h"

CStatusUI::CStatusUI()
	:CUI(false)
{
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"StatusImg", L"texture\\status.png");
}

CStatusUI::~CStatusUI()
{
}

void CStatusUI::update()
{
}

void CStatusUI::render()
{
	CRenderManager::getInst()->RenderImage(
		m_pImg
		, 0.f
		, WINSIZEY- m_pImg->GetHeight()
		, m_pImg->GetWidth()
		, WINSIZEY
	);
}


