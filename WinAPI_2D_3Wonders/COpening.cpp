#include "pch.h"
#include "COpening.h"
#include "CCollider.h"
#include "CAnimator.h"

COpening::COpening()
{
	SetName(L"Opening");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"OpeningImg", L"texture\\Animation\\Animation_Opening.png");
	SetPos(Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f));
	SetScale(Vec2(600.f, 450.f));
	CreateCollider();
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	
	CreateAnimator();


	GetAnimator()->CreateAnimation(L"Opening", m_pImg, Vec2(0.f, 0.f), Vec2(600.f, 450.f), Vec2(600.f, 0.f), 2.f, 2, false,false);

	GetAnimator()->Play(L"Opening");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

COpening::~COpening()
{
}

void COpening::update()
{
	GetAnimator()->update();
}

void COpening::render()
{
	component_render();
}


