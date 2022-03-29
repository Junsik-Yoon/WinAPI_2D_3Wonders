#include "pch.h"
#include "CScoreUI.h"
#include "CD2DImage.h"

CScoreUI::CScoreUI()
	:CUI(false)
{
	m_gameScore = 0;
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"ScoreImg", L"texture\\score.png");
	SetPos(Vec2(WINSIZEX/2.f, 50.f));
}

CScoreUI::~CScoreUI()
{
}

void CScoreUI::update()
{
	m_gameScore = CGameManager::getInst()->GetGameScore();
}

void CScoreUI::render()
{
	
	CRenderManager::getInst()->RenderImage(
		m_pImg
		, 0.f
		, 0.f
		, m_pImg->GetWidth()/2.f
		, m_pImg->GetHeight()/2.f
		
	);

	//CRenderManager::getInst()->RenderText(
	//	 std::to_wstring(m_gameScore)
	//	, 250.f
	//	, 0.f
	//	, 300.f
	//	, 25.f
	//	, 25.f
	//	, RGB(255, 0, 0));
}


