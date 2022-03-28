#include "pch.h"
#include "CBossTrigger.h"
#include "CCollider.h"
#include "CGolemWood.h"
#include "CScene.h"

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
	CGolemWood* pGolemWood = nullptr;
	vector<CGameObject*> pObj = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::MONSTER);
	for (int i = 0; i < pObj.size(); ++i)
	{
		if (pObj[i]->GetName() == L"GolemWood")
		{
			pGolemWood = (CGolemWood*)pObj[i];
		}
	}
	
	if (GetName() == L"TileTrigger" &&
		_pOther->GetObj()->GetName() == L"Lou")
	{
		if (nullptr != pGolemWood)
		{
			pGolemWood->ActivateTiles(true);
		}
	}
}

void CBossTrigger::OnCollision(CCollider* _pOther)
{
}

void CBossTrigger::OnCollisionExit(CCollider* _pOther)
{
}



