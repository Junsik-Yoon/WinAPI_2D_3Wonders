#include "pch.h"
#include "CShelter.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CEffect.h"
#include "CD2DImage.h"

#include <iostream>
#include <random>

CShelter::CShelter()
{
	isExploding = false;
	SetName(L"Tile");
	SetHP(4);
	SetGroup(GROUP_TILE::WALL);
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"ShelterImg", L"texture\\Animation\\Animation_Shelter.png");
	CreateCollider();
	SetScale(Vec2(200.f, 200.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"HP4", m_pImg, Vec2(0.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP3", m_pImg, Vec2(384.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP2", m_pImg, Vec2(768.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP1", m_pImg, Vec2(1152.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"HP0", m_pImg, Vec2(1536.f, 0.f), Vec2(384.f, 384.f), Vec2(384.f, 0.f), 0.5f, 1, false);


	GetAnimator()->Play(L"HP4");

	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CShelter::~CShelter()
{
}


void CShelter::update()
{
	int hp = GetHP();
	switch (hp)
	{
	case 4:
		GetAnimator()->Play(L"HP4");
		break;
	case 3:
		GetAnimator()->Play(L"HP3");
		break;
	case 2:
		GetAnimator()->Play(L"HP2");
		break;
	case 1:
		GetAnimator()->Play(L"HP1");
		break;
	case 0:
		GetAnimator()->Play(L"HP0");
		break;
	}
	if (0 >= GetHP())
	{
		if(isExploding)
		//GetCollider()->SetScale(Vec2(0.f, 0.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, -650.f));
		
	}
	GetAnimator()->update();
}

void CShelter::Explode()
{
	int iHeight = m_pImg->GetHeight();
	int iWidth = m_pImg->GetWidth();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, iHeight/2);


	//////////////////////ÀÌÆåÆ®///////////////
	CEffect* effectExplode = new CEffect(L"Effect_Explode_Small", L"texture\\Animation\\Effect_Explode_Small.png",
		L"Effect_Explode_Small", Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.05f, 10, false, 1.f);
	effectExplode->SetPos(Vec2(GetPos().x-GetScale().x/2.f + dis(gen), GetPos().y - GetScale().y / 2.f + dis(gen)));
	effectExplode->SetDuration(1.f);
	CreateObj(effectExplode, GROUP_GAMEOBJ::EFFECT);
	///////////////////////////////////////////
}

void CShelter::render()
{
	component_render();
	render_information();
}

void CShelter::render_information()
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
		wstring state = {};
		////////////////////////
		if (GetHP() <= 0)
		{
			state = L"Destroyed";
		}
		else
		{
			state = L"NONE";
		}
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + state + L"\n" +
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

void CShelter::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if(pOther->GetName()== L"Missile_Player")
	{
		Explode();
		int hp = GetHP();
		if (0 < hp) SetHP(--hp);
	}

}

void CShelter::OnCollision(CCollider* _pOther)
{
}

void CShelter::OnCollisionExit(CCollider* _pOther)
{
}
