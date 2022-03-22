#include "pch.h"
#include "CHalfMoon.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CHalfMoon::CHalfMoon()
{
	SetHP(8);
	isRight = true;
	SetName(L"Halfmoon");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonBugImg", L"texture\\Animation\\Animation_HalfmoonBug.png");

	CreateCollider();
	SetScale(Vec2(60.f, 100.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));


	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Idle_Right", m_pImg, Vec2(0.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Idle_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);

	GetAnimator()->CreateAnimation(L"Summon_Right", m_pImg, Vec2(128.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 9, false);
	GetAnimator()->CreateAnimation(L"Summon_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 9, false);

	GetAnimator()->CreateAnimation(L"Dead_Left" ,m_pImg, Vec2(1280.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);
	GetAnimator()->CreateAnimation(L"Dead_Right", m_pImg, Vec2(1280.f, 128.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 1.f, 1, false);


	GetAnimator()->Play(L"Idle_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CHalfMoon::~CHalfMoon()
{
}

void CHalfMoon::update()
{
	Vec2 pos = GetPos();

	SetPos(pos);

	GetAnimator()->update();
}

void CHalfMoon::render()
{
	component_render();
	render_information();
}

void CHalfMoon::render_information()
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
		////////////////////////
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + L"" + L"\n" +
			L" drctn  : " + std::to_wstring(isRight) + L"\n" +//TODO:¼öÁ¤
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


void CHalfMoon::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPlayerPos = pOther->GetPos();
	int iPlayerHP = pOther->GetHP();
	if (pOther->GetName() == L"Lou")
	{
		if (_pOther->GetFinalPos().x < GetCollider()->GetFinalPos().x)
		{
			vPlayerPos.x -= 2.f;
		}
		if (_pOther->GetFinalPos().x > GetCollider()->GetFinalPos().x)
		{
			vPlayerPos.x += 2.f;
		}
		pOther->SetHP(--iPlayerHP);
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
	if (GetHP() <= 0)
	{
		if (isRight)
		{
			GetAnimator()->Play(L"Dead_Right");
		}
		else
		{
			GetAnimator()->Play(L"Dead_Left");
		}
		GetCollider()->SetOffsetPos(Vec2(0.f, -500.f));
	}

}

void CHalfMoon::OnCollision(CCollider* _pOther)
{
}

void CHalfMoon::OnCollisionExit(CCollider* _pOther)
{

}

