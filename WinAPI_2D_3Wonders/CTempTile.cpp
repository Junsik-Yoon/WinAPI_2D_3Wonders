#include "pch.h"
#include "CTempTile.h"
#include "CCollider.h"

CTempTile::CTempTile()
{
	CreateCollider();
	SetScale(Vec2(32.f, 32.f));
	SetName(L"Tile");
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
}

CTempTile::~CTempTile()
{
}


void CTempTile::OnCollisionEnter(CCollider* _pOther)
{
}

void CTempTile::OnCollision(CCollider* _pOther)
{
}

void CTempTile::OnCollisionExit(CCollider* _pOther)
{
}

void CTempTile::update()
{
}

void CTempTile::render()
{
	component_render();
}
