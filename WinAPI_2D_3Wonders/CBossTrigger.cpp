#include "pch.h"
#include "CBossTrigger.h"
#include "CCollider.h"

CBossTrigger::CBossTrigger()
{
	SetName(L"BossTrigger");
	CreateCollider();
	SetScale(Vec2(100.f, WINSIZEY));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
}

CBossTrigger::~CBossTrigger()
{
}


void CBossTrigger::update()
{
}

void CBossTrigger::render()
{
	component_render();
}

void CBossTrigger::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Lou")
	{

	}
}

void CBossTrigger::OnCollision(CCollider* _pOther)
{
}

void CBossTrigger::OnCollisionExit(CCollider* _pOther)
{
}



