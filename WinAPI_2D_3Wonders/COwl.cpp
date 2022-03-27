#include "pch.h"
#include "COwl.h"
#include "CAnimator.h"
#include "CScene.h"

COwl::COwl()
{

}

COwl::COwl(Vec2 _pos)
{
	SetPos(_pos);
	SetName(L"Owl");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"OwlImg", L"texture\\Animation\\Animation_Owl.png");

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Fly_Owl", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 6, false);

	GetAnimator()->Play(L"Fly_Owl");
}

COwl::~COwl()
{
}

void COwl::update()
{
	FollowPlayer();
	GetAnimator()->update();
}
void COwl::FollowPlayer()
{
	vector<CGameObject*>pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPos = pPlayer[0]->GetPos();
	vPos.y -= (pPlayer[0]->GetScale().y / 2.f + GetScale().y/2.f);
	SetPos(vPos);
}

void COwl::AttackEntireWindow()
{
	Vec2 vPos = GetPos();
	vector<CGameObject*>pMonsters = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::MONSTER);
	for (int i = 0; i < pMonsters.size(); ++i)
	{
		if (pMonsters[i]->GetHP() >0)
		{
			if (abs(vPos.x - pMonsters[i]->GetPos().x) <= 400.f)
			{
				int iHp = pMonsters[i]->GetHP();
				iHp -= 2;
				pMonsters[i]->SetHP(iHp);
			}
		}
	}
}
void COwl::render()
{
	component_render();
}


