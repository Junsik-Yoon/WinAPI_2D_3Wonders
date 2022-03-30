#include "pch.h"
#include "CGameSelect.h"
#include "CAnimator.h"
#include "CCollider.h"

#define ACTION 1
#define SHOOTING 2
#define PUZZLE 3

CGameSelect::CGameSelect()
{
	isSelected = false;
	select = 1;
	selectCounter = 0.f;

	SetName(L"SelectGame");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GameSelectImg", L"texture\\Animation\\Animation_GameSelect.png");
	SetPos(Vec2(WINSIZEX/2.f,WINSIZEY/2.f ));
	SetScale(Vec2(600.f, 450.f));
	CreateCollider();
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	//TODO: 왜 계속 이미지가 안보이다가 콜라이더렌더까지 하니 보이는지 알아보기
	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Action_Unselected", m_pImg, Vec2(0.f, 0.f), Vec2(600.f, 450.f), Vec2(600.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Action_Selected", m_pImg, Vec2(600.f, 0.f), Vec2(600.f, 450.f), Vec2(600.f, 0.f), 0.5f, 4, false,false);
	GetAnimator()->CreateAnimation(L"Puzzle_Unselected", m_pImg, Vec2(3000.f, 0.f), Vec2(600.f, 450.f), Vec2(600.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Shooting_Unselected", m_pImg, Vec2(3600.f, 0.f), Vec2(600.f, 450.f), Vec2(600.f, 0.f), 1.f, 1, false);

	GetAnimator()->Play(L"Action_Unselected");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CGameSelect::~CGameSelect()
{
}


void CGameSelect::update()
{
	if(selectCounter==0.f)
	{
		CSoundManager::getInst()->Play(L"select_start",0.5f);
		selectCounter += fDT;
	}
	if (false==isSelected)
	{
		if (KEYDOWN(VK_RIGHT))
		{
			CSoundManager::getInst()->Play(L"select_change",0.5f);
			if (3 == select)
				select = 1;
			else
				++select;
		}
		if (KEYDOWN(VK_LEFT))
		{
			CSoundManager::getInst()->Play(L"select_change",0.5f);
			if (1 == select)
				select = 3;
			else
				--select;
		}
		switch (select)
		{
		case ACTION:
			GetAnimator()->Play(L"Action_Unselected");
			break;
		case SHOOTING:
			GetAnimator()->Play(L"Shooting_Unselected");
			break;
		case PUZZLE:
			GetAnimator()->Play(L"Puzzle_Unselected");
			break;
		}
	}
	else
	{
		selectCounter += fDT;
		//CCameraManager::getInst()->FadeOut(3.f);
	}
	if (KEYDOWN('Z')||KEYDOWN('X'))
	{
		if (1 == select)
		{
			CSoundManager::getInst()->Play(L"select_scene_selected",0.5f);			
			GetAnimator()->Play(L"Action_Selected");
			isSelected = true;			
		}
	}

	if (3.f <= selectCounter)
	{
		ChangeScn(GROUP_SCENE::OPENING);
	}
	
	GetAnimator()->update();
}

void CGameSelect::render()
{
	component_render();
}

