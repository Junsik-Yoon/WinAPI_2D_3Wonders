#include "pch.h"
#include "CMovingTile.h"
#include "CCollider.h"
#include "CAnimator.h"

#define MINY 450.f
#define MAXY 250.f

CMovingTile::CMovingTile()
{
	isRight = false;
	SetName(L"Tile");
	Y_axis = -1;
	nextTileCounter = 0.f;
	SetGroup(GROUP_TILE::MOVE);
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"MovingTileImg", L"texture\\Animation\\Animation_MovingTile.png");
	CreateCollider();

	SetScale(Vec2(32.f, 82.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"State01", m_pImg, Vec2(0.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State02", m_pImg, Vec2(32.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State03", m_pImg, Vec2(64.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State04", m_pImg, Vec2(96.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State05", m_pImg, Vec2(128.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State06", m_pImg, Vec2(160.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State07", m_pImg, Vec2(192.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State08", m_pImg, Vec2(224.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State09", m_pImg, Vec2(256.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State10", m_pImg, Vec2(288.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State11", m_pImg, Vec2(320.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"State12", m_pImg, Vec2(352.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.5f, 1, false);
	
	GetAnimator()->CreateAnimation(L"PlayAll", m_pImg, Vec2(0.f, 0.f), Vec2(32.f, 82.f), Vec2(32.f, 0.f), 0.2f, 12, false);

	GetAnimator()->Play(L"PlayAll");
	
	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CMovingTile::~CMovingTile()
{
}



void CMovingTile::update()
{
	Vec2 vPos = GetPos();

	float fdiff = (MINY - MAXY)/6.f;
	
	if (vPos.y <= MINY + 1.f
		&& vPos.y > MINY - fdiff
		)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"State12");
			isRight = false;
		}
		else
		{
			GetAnimator()->Play(L"State01");
		}		
	}
	if (vPos.y <= MINY - fdiff
		&& vPos.y > MINY - fdiff * 2.f
		&& !isRight)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"State11");
		}
		else
		{
			GetAnimator()->Play(L"State02");
		}
	}
	if (vPos.y <= MINY - fdiff * 2.f
		&& vPos.y > MINY - fdiff * 3.f
		&& !isRight)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"State10");
		}
		else
		{
			GetAnimator()->Play(L"State03");
		}
	}
	if (GetPos().y <= MINY - fdiff * 3.f
		&& GetPos().y > MINY - fdiff * 4.f
		&& !isRight)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"State09");
		}
		else
		{
			GetAnimator()->Play(L"State04");
		}
	}
	if (GetPos().y <= MINY - fdiff * 4.f
		&& GetPos().y > MINY - fdiff * 5.f
		&& !isRight)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"State08");
		}
		else
		{
			GetAnimator()->Play(L"State05");
		}
	}
	if (GetPos().y <= MINY - fdiff * 5.f
		&& GetPos().y > MINY - fdiff * 6.f
		&& !isRight)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"State07");
		}
		else
		{
			GetAnimator()->Play(L"State06");
			isRight = true;
		}
	}

	SetPos(vPos);
	GetAnimator()->update();
}

void CMovingTile::render()
{
	component_render();
}

void CMovingTile::OnCollisionEnter(CCollider* _pOther)
{
}

void CMovingTile::OnCollision(CCollider* _pOther)
{
}

void CMovingTile::OnCollisionExit(CCollider* _pOther)
{
}

