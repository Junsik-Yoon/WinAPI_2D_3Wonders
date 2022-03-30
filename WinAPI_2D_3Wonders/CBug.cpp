#include "pch.h"
#include "CBug.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTile.h"
#include "CEffect.h"

#define D_VELOCITY 150
#define D_GRAVITY 500
#define D_UPFORCE 800

CBug::CBug()
{
	isJump = false;
	m_upforce = D_UPFORCE;
	m_highJump = 0.f;
	isGoingRight = false;
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_lifeCycle = 0.f;
	m_gravity = 0.f;
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

	GetAnimator()->CreateAnimation(L"NULL", m_pImg, Vec2(0.f, 1000.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.2f, 1, false);



	GetAnimator()->Play(L"NULL");

}

CBug::~CBug()
{

}


void CBug::update()
{
	update_move();
	update_animation();

	GetAnimator()->update();
}

void CBug::update_move()
{
	Vec2 vPos = GetPos();
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();

	vPos.y += m_gravity * fDT;

	switch (m_state)
	{
	case eState_Bug::NONE:
	{
		m_gravity = 0.f;
		m_velocity = 0.f;
		m_lifeCycle = 0.f;
	}break;
	case eState_Bug::BORN:
	{
		m_lifeCycle += fDT;
		m_gravity = 0.f;
		m_velocity = D_VELOCITY;
		if (isFacingRight)
		{
			vPos.x += m_velocity * fDT;
		}
		else
		{
			vPos.x -= m_velocity * fDT;
		}
		if (m_lifeCycle >= 1.f)
		{
			m_state = eState_Bug::FLY;
		}

		if (m_floor > 0)
		{
			m_state = eState_Bug::IDLE;
		}
		if (GetHP() <= 0)//이 조건은 항상 마지막에 있어야 할 듯
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	case eState_Bug::FLY:
	{
		m_lifeCycle += fDT;

		m_gravity = 100.f;
		if (isGoingRight)
		{
			isFacingRight = true;
			vPos.x += m_velocity * fDT;
		}
		else
		{
			isFacingRight = false;
			vPos.x -= m_velocity * fDT;
		}

		if (m_floor > 0)
		{
			m_state = eState_Bug::IDLE;
		}

		if (m_lifeCycle >= 15.f)
		{
			m_state = eState_Bug::FLYAWAY;
		}
		if (GetHP() <= 0)
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	case eState_Bug::HOP:
	{
		vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
		Vec2 pPlayerPos = pPlayer[0]->GetPos();

		m_velocity = 50.f;
		m_gravity = D_GRAVITY;
		m_upforce -= m_gravity * fDT;
		vPos.y -= m_upforce * fDT;
		if (pPlayerPos.x < vPos.x &&abs(pPlayerPos.x-vPos.x)>10.f)
		{
			isFacingRight = false;
			vPos.x -= m_velocity * fDT;
		}
		else if (pPlayerPos.x > vPos.x && abs(pPlayerPos.x - vPos.x) > 10.f)
		{
			isFacingRight = true;
			vPos.x += m_velocity * fDT;
		}

		if (m_floor > 0)
		{
			m_state = eState_Bug::IDLE;
		}
		m_lifeCycle += fDT;
		if (m_lifeCycle >= 15.f)
		{
			m_state = eState_Bug::FLYAWAY;
		}
		if (GetHP() <= 0)
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	case eState_Bug::IDLE:
	{
		m_gravity = 0.f;
		m_upforce = D_UPFORCE;
		vPos.y -= 5.f;
		if (m_floor == 0)
		{
			m_state = eState_Bug::HOP;
		}
		m_lifeCycle += fDT;
		if (m_lifeCycle >= 15.f)
		{
			m_state = eState_Bug::FLYAWAY;
		}
		if (GetHP() <= 0)
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	case eState_Bug::FLYAWAY:
	{
		m_lifeCycle = 0.f;
		m_gravity = -D_GRAVITY;

		if (vPos.y <= 0.f)
		{
			SetHP(0);
		}

		if (GetHP() <= 0)
		{
			m_state = eState_Bug::NONE;
		}
	}break;
	}

	SetPos(vPos);

}



void CBug::update_animation()
{
	switch (m_state)
	{
	case eState_Bug::NONE:	GetAnimator()->Play(L"Land_Left");
		break;
	case eState_Bug::BORN:	
		if (isFacingRight)
		{
			GetAnimator()->Play(L"Fly_Right");
		}
		else
		{
			GetAnimator()->Play(L"Fly_Left");
		}
		break;
	case eState_Bug::FLY:
		if (isFacingRight)
		{
			GetAnimator()->Play(L"Fly_Right");
		}
		else
		{
			GetAnimator()->Play(L"Fly_Left");
		}
		break;
	case eState_Bug::FLYAWAY:
		if (isFacingRight)
		{
			GetAnimator()->Play(L"Fly_Right");
		}
		else
		{
			GetAnimator()->Play(L"Fly_Left");
		}
		break;

	case eState_Bug::HOP:
		if (isFacingRight)
		{
			GetAnimator()->Play(L"Jump_Right");
		}
		else
		{
			GetAnimator()->Play(L"Jump_Left");
		}
		break;

	}

}





void CBug::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	CTile* pTile = (CTile *)pOther;
	if (pTile->GetName() == L"Tile")
	{
		if (pTile->GetGroup() == GROUP_TILE::GROUND)
		{
			++m_floor;
		}
	}
	if (pOther->GetName() == L"Missile_Player"||
		pOther->GetName() == L"Hyper_Missile_Player"||
		pOther->GetName() == L"Fire_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}
}

void CBug::OnCollision(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	CTile* pTile = (CTile*)pOther;
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		if (pTile->GetGroup() == GROUP_TILE::GROUND)
		{
			int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
			int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
			int sum = abs(a - b);
			if (1 < sum)
				--vPos.y;
		}
	}
	SetPos(vPos);
}

void CBug::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	CTile* pTile = (CTile*)pOther;
	if (pOther->GetName() == L"Tile")
	{
		if (pTile->GetGroup() == GROUP_TILE::GROUND)
		{
			--m_floor;
		}
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
		wstring curState = {};
		////////////////////////
		switch (m_state)
		{
		case eState_Bug::NONE: curState = L"NONE";
			break;
		case eState_Bug::BORN:curState = L"BORN";
			break;
		case eState_Bug::FLY:curState = L"FLY";
			break;
		case eState_Bug::HOP:curState = L"HOP";
			break;
		case eState_Bug::IDLE:curState = L"IDLE";
			break;
		case eState_Bug::FLYAWAY:curState = L"FLYAWAY";
			break;
		}
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + curState + L"\n" +
			L"upforce : "+ std::to_wstring(m_upforce)+L"\n"+
			L"gravity : "+ std::to_wstring(m_gravity)+L"\n"+ 
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
