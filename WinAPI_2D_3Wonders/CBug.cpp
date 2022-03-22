#include "pch.h"
#include "CBug.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CBug::CBug()
{
	isHeadedRight = false;
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = 0;
	SetName(L"Bug");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"HalfmoonBugImg", L"texture\\Animation\\Animation_HalfmoonBug.png");

	CreateCollider();
	SetScale(Vec2(50.f, 50.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));


	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Fly_Left", m_pImg, Vec2(0.f, 256.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 2, false);
	GetAnimator()->CreateAnimation(L"Fly_Right", m_pImg, Vec2(0.f, 384.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 2, false);

	GetAnimator()->CreateAnimation(L"Land_Left", m_pImg, Vec2(256.f, 256.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.4f, 2, false);
	GetAnimator()->CreateAnimation(L"Land_Right", m_pImg, Vec2(256.f, 384.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.4f, 2, false);

	GetAnimator()->CreateAnimation(L"Jump_Left", m_pImg, Vec2(384.f, 256.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 5, false);
	GetAnimator()->CreateAnimation(L"Jump_Right", m_pImg, Vec2(384.f, 384.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 5, false);



	GetAnimator()->Play(L"Jump_Right");

	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CBug::~CBug()
{

}


void CBug::update()
{
	Vec2 vPos = GetPos();

	vPos.x += 30.f * fDT;
	
	switch (m_state)
	{
	case eState_Bug::NONE:
	{

	}break;
	case eState_Bug::FLY:
	{
		
		m_gravity = 100.f;

		if (m_floor > 0)
		{
			m_state = eState_Bug::HOP;
		}

		if (GetHP() <= 0)//이 조건은 항상 마지막에 있어야 할 듯
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	case eState_Bug::HOP:
	{
		if (GetHP() <= 0)
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	case eState_Bug::FLYAWAY:
	{
		if (GetHP() <= 0)
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	}

	SetPos(vPos);

	GetAnimator()->update();
}
void CBug::Fly()
{
}

void CBug::Hop()
{
}

void CBug::FlyAway()
{
}



void CBug::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
}

void CBug::OnCollision(CCollider* _pOther)
{
}

void CBug::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}


void CBug::render()
{
	render_information();
	component_render();
}

void CBug::render_information()
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
			L" drctn  : " + L"" + L"\n" +
			L" curAm : " + curAni + L"\n" +
			L" HP:  " + std::to_wstring(GetHP()) + L"\n" +
			L" wallCount : " + std::to_wstring(m_floor)
			, vPos.x + 30.f
			, vPos.y + -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}
}
