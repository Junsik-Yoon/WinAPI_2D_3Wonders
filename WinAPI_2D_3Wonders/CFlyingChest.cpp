#include "pch.h"
#include "CFlyingChest.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CItem.h"
#include "CScene.h"
#include "CCard.h"
#include "CEffect.h"
#include "CD2DImage.h"

#include <iostream>
#include <random>

CFlyingChest::CFlyingChest()
{
	flySound = 0;
	m_center = { 4500.f,100.f };
	m_appearTimer = 0.f;
	m_fTheta = 0.f;

	m_state = eState_FChest::IDLE;

	SetName(L"Chest");
	SetHP(1);
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"FlyingChestImg", L"texture\\Animation\\Animation_FlyingChest.png");

	CreateCollider();
	SetScale(Vec2(80.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));
	
	CreateAnimator();

	GetAnimator()->CreateAnimation(L"FlyingChest", m_pImg, Vec2(0.f, 0.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.1f, 2, false);

	GetAnimator()->Play(L"FlyingChest");
	
	CSoundManager::getInst()->AddSound(L"FlySound", L"sound\\flyingchest.wav",false);

}

CFlyingChest::~CFlyingChest()
{
	CSoundManager::getInst()->Stop(L"FlySound");
}


void CFlyingChest::update()
{

	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	Vec2 vPos = GetPos();
	switch (m_state)
	{
	case eState_FChest::IDLE:
	{
		if (abs(vPlayerPos.x - vPos.x) <= 1400.f)
		{
			m_state = eState_FChest::FLY;
		}
		if (GetHP() <= 0)
		{
			m_state = eState_FChest::DESTROYED;
		}
	}
		break;
	case eState_FChest::FLY:
	{

		if (m_appearTimer < 2.f)
		{
			m_fTheta += m_fSpd * fDT;
			m_width -= 900.f * fDT;
			vPos.x = m_width * (float)cos(m_fTheta);
			vPos.y = m_height * (float)sin(m_fTheta);

			vPos.x += m_center.x;
			vPos.y += m_center.y;
		}
		else if (m_appearTimer >= 11.f)
		{
			m_appearTimer = 0.f;
			m_state = eState_FChest::FLYAWAY;
		}
		else if (m_appearTimer > 2.f)
		{
			m_fSpd += 0.1f*fDT;
			m_fTheta += m_fSpd * fDT;
			vPos.x = m_width * (float)cos(m_fTheta);
			vPos.y = m_height * (float)sin(m_fTheta);

			vPos.x += m_center.x;
			vPos.y += m_center.y;
		}

		m_appearTimer += fDT;
		m_effectTimer += fDT;
		if (m_effectTimer >= 0.05f)
		{
			++flySound;
			if(flySound>12)
			{
				flySound = 0;
				CSoundManager::getInst()->Play(L"FlySound",0.3f);
			}

			m_effectTimer = 0.f;
			TwinkleEffect();
		}

		if (GetHP() <= 0)
		{
			CSoundManager::getInst()->Play(L"explodeSound");
			m_state = eState_FChest::DESTROYED;
		}
	}
		break;
	case eState_FChest::DESTROYED:
	{
		//////////////////////이펙트///////////////
		CEffect* effectExplode = new CEffect(L"Effect_Explode", L"texture\\Animation\\Effect_Explode.png",
			L"Effect_Explode", Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 9, false, false, L"Effect_Explode");
		effectExplode->SetPos(Vec2(GetPos()));
		effectExplode->SetDuration(0.9f);
		CreateObj(effectExplode, GROUP_GAMEOBJ::EFFECT);
		///////////////////////////////////////////
		Put_Item();
		DeleteObj(this);
	}
		break;
	case eState_FChest::FLYAWAY:
	{
		vPos.x -= 300.f * fDT;
		vPos.y -= 70.f * fDT;

		if (GetPos().y <= -100.f)
		{
			for (int i = 0; i < itemsContain.size(); ++i)
			{
				if (nullptr != itemsContain[i])
				{
					DeleteObj(itemsContain[i]);
				}
			}
			itemsContain.clear();
			DeleteObj(this);
		}
	}
		break;
	}

	SetPos(vPos);
	GetAnimator()->update();
}

void CFlyingChest::render()
{
	component_render();
}

void CFlyingChest::Put_Item()
{
	CItem* pItem = nullptr;
	for (int i = 0; i < itemsContain.size(); ++i)
	{
		pItem = itemsContain[i];
		pItem->SetPos(Vec2(GetPos().x -10.f + i*5.f,GetPos().y));
		if (pItem->GetName() == L"Card")
		{
			CCard* pCard = (CCard*)pItem;
			pCard->SetDropDir(i*30.f);
		}
		//TODO:추후에 수정하기
		CreateObj(pItem, GROUP_GAMEOBJ::ITEM);
	}


}

void CFlyingChest::TwinkleEffect()
{

	int iHeight = m_pImg->GetHeight();
	int iWidth = m_pImg->GetWidth();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, iHeight );

	//////////////////////이펙트///////////////
	CEffect* effectTwinkle = new CEffect(L"Effect_Twinkle", L"texture\\Animation\\Effect_Twinkle.png",
		L"Effect_Twinkle", Vec2(0.f, 0.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 0.05f, 6, false, 1.f);
	effectTwinkle->SetPos(Vec2(GetPos().x - GetScale().x/2.f + dis(gen), GetPos().y - GetScale().y / 2.f + dis(gen)));
	effectTwinkle->SetDuration(1.f);
	CreateObj(effectTwinkle, GROUP_GAMEOBJ::EFFECT);
	///////////////////////////////////////////

}


void CFlyingChest::information_render()
{
}

void CFlyingChest::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
}

void CFlyingChest::OnCollision(CCollider* _pOther)
{
}

void CFlyingChest::OnCollisionExit(CCollider* _pOther)
{
}


