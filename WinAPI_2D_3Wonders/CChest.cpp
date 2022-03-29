#include "pch.h"
#include "CChest.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CEffect.h"
#include "CAnimation.h"
#include "CScene.h"
#include "CItem.h"

CChest::CChest()
{
	itemContains = nullptr;
	m_vanishTimer = 0.f;
	isVisible = true;
	m_state = eState_Chest::IDLE;
	SetName(L"Chest");
	SetHP(3);
	prevHP = GetHP();
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"ChestImg", L"texture\\Animation\\Animation_Chest.png");

	CreateCollider();
	SetScale(Vec2(80.f, 50.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, -10.f));
	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Idle_Chest", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Hit_Chest", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Destroyed_Chest", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.066667f, 3, false);
	GetAnimator()->CreateAnimation(L"NULL_Chest", m_pImg, Vec2(0.f, 0.f), Vec2(2.f, 2.f), Vec2(2.f, 0.f), 1.f, 1, false);

	GetAnimator()->Play(L"Idle_Chest");


}

CChest::~CChest()
{

}



void CChest::update()
{
	Vec2 vPos = GetPos();
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	if (!isVisible)
	{
		m_state = eState_Chest::INVISIBLE;
	}
	switch (m_state)
	{
	case eState_Chest::INVISIBLE:
	{
		GetAnimator()->Play(L"NULL_Chest");
		if (abs(vPos.x - vPlayerPos.x) < 150.f)
		{
			isVisible = true;
			m_state = eState_Chest::IDLE;
		}
	}break;
	case eState_Chest::IDLE:
	{
		GetAnimator()->Play(L"Idle_Chest");
		if (prevHP > GetHP())
		{
			m_state = eState_Chest::HIT;
		}
		if (GetHP() <= 0)
		{
			CSoundManager::getInst()->Play(L"explodeSound");
			m_state = eState_Chest::DESTROYED;
		}
	}
		break;
	case eState_Chest::HIT:
	{
		GetAnimator()->Play(L"Hit_Chest");
		m_vanishTimer += fDT;
		if (m_vanishTimer >= 0.1f)
		{
			m_vanishTimer = 0.f;
			m_state = eState_Chest::IDLE;
		}
		if (GetHP() <= 0)
		{		
			CSoundManager::getInst()->Play(L"explodeSound");
			m_vanishTimer = 0.f;
			m_state = eState_Chest::DESTROYED;
		}
	}
		break;
	case eState_Chest::DESTROYED:
	{
		m_vanishTimer += fDT;

		GetAnimator()->Play(L"Destroyed_Chest");
		if (m_vanishTimer >= 0.2f)
		{
			Put_Item();
			DeleteObj(this);
			m_vanishTimer = 0.f;
			//////////////////////ÀÌÆåÆ®///////////////
			CEffect* effectExplode = new CEffect(L"Effect_Explode", L"texture\\Animation\\Effect_Explode.png",
				L"Effect_Explode", Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 9, false, false, L"Effect_Explode");
			effectExplode->SetPos(Vec2(GetPos()));
			effectExplode->SetDuration(0.9f);
			CreateObj(effectExplode, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
		}

	}
		break;
	}
	prevHP = GetHP();
	GetAnimator()->update();
}

void CChest::Put_Item()
{
	if (nullptr != itemContains)
	{
		CItem* pItem = itemContains;
		pItem->SetPos(Vec2(GetPos()));
		CreateObj(pItem, GROUP_GAMEOBJ::ITEM);
	}
}



void CChest::render()
{
	component_render();
	information_render();
}

void CChest::information_render()
{
	if (true == CCore::getInst()->DebugMode())
	{
		CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"BackInfo", L"texture\\BackInfo.png");
		Vec2 vPos = GetPos();
		vPos = CCameraManager::getInst()->GetRenderPos(vPos);

		CRenderManager::getInst()->RenderImage(
			pImg,
			vPos.x + 30.f,
			vPos.y + -40.f,
			vPos.x + 200.f,
			vPos.y + 100.f,
			0.3f);

		////////////////////////
		wstring curAni = {};
		wstring curState = {};
		////////////////////////

		switch (m_state)
		{
		case eState_Chest::INVISIBLE:curState = L"INVISIBLE";
			break;
		case eState_Chest::IDLE:curState = L"IDLE";
			break;
		case eState_Chest::HIT:curState = L"HIT";
			break;
		case eState_Chest::DESTROYED:curState = L"DESTROYED";
			break;
		}
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + curState + L"\n" +
			L" curAm : " + curAni + L"\n" +
			L" HP:  " + std::to_wstring(GetHP()) + L"\n"
			, vPos.x + 30.f
			, vPos.y + -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}
}

void CChest::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Missile_Player"
		&& m_state != eState_Chest::INVISIBLE)
	{
		int hp = GetHP();
		SetHP(--hp);
	}
}

void CChest::OnCollision(CCollider* _pOther)
{
}

void CChest::OnCollisionExit(CCollider* _pOther)
{
}

