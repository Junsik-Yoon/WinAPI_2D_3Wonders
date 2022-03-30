#include "pch.h"
#include "CSlopeTile.h"
#include "CCollider.h"

CSlopeTile::CSlopeTile()
{
	CreateCollider();
	SetName(L"Tile");
}

CSlopeTile::~CSlopeTile()
{
}


void CSlopeTile::create(float left, float top, float right, float bottom, GROUP_TILE group)
{
	// ÁÂÇ¥°ªÀ¸·Î ¶¥ »ý¼º
	CSlopeTile* pNewGrd = new CSlopeTile;
	pNewGrd->SetPos(Vec2((left + right) / 2.f, (top + bottom) / 2.f));
	pNewGrd->GetCollider()->SetScale(Vec2(right - left, bottom - top));
	pNewGrd->SetGroup(group);
	CreateObj(pNewGrd, GROUP_GAMEOBJ::TILE);
}

void CSlopeTile::update()
{
}

void CSlopeTile::render()
{
	component_render();
}

void CSlopeTile::OnCollisionEnter(CCollider* _pOther)
{
}

void CSlopeTile::OnCollision(CCollider* _pOther)
{
}

void CSlopeTile::OnCollisionExit(CCollider* _pOther)
{
}


