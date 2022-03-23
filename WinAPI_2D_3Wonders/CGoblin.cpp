#include "pch.h"
#include "CGoblin.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CEffect.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CGoblin::CGoblin()
{
	isBackflipRight = true;
	m_timeCounter = 0.f;
	m_state = eState_Goblin::BORN;
	SetHP(2);
	m_floor=0;
	m_wall=0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;

	SetName(L"Goblin");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GoblinImg", L"texture\\Animation\\Animation_Goblin.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 13.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Create_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6,false);
	GetAnimator()->CreateAnimation(L"Create_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6,false);
	
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	
	GetAnimator()->CreateAnimation(L"Laugh_Left", m_pImg, Vec2(128.f, 896.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);
	GetAnimator()->CreateAnimation(L"Laugh_Right", m_pImg, Vec2(128.f, 1152.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);

	GetAnimator()->CreateAnimation(L"Die_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);
	GetAnimator()->CreateAnimation(L"Die_Right", m_pImg, Vec2(512.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);

	CCameraManager::getInst()->GetRenderPos(GetPos());
}

CGoblin::~CGoblin()
{
}


void CGoblin::update()
{
	update_move();
	update_animation();
	GetAnimator()->update();
}

void CGoblin::render()
{
	component_render();
	render_information();
}

void CGoblin::update_move()
{
	Vec2 vPos = GetPos();

	if (m_floor == 0)
	{
		m_gravity = D_GRAVITY;
		vPos.y += D_GRAVITY * fDT;
	}
	else
	{
		m_gravity = 0;
	}

	switch (m_state)
	{
	case eState_Goblin::BORN:
	{
		m_timeCounter += fDT;
		if (m_timeCounter >= 2.f)
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::MOVE;
		}
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::MOVE:
	{
		vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
		if (pPlayer[0]->GetPos().x >= GetPos().x)
		{
			vPos.x += 50.f * fDT;
			GetAnimator()->Play(L"Move_Right");
		}
		else
		{
			vPos.x -= 50.f * fDT;
			GetAnimator()->Play(L"Move_Left");
		}

		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::LAUGH:
	{

	}break;
	case eState_Goblin::JUMP:
	{

	}break;
	case eState_Goblin::JUMPUP:
	{

	}break;
	case eState_Goblin::JUMPDOWN:
	{

	}break;
	case eState_Goblin::TELEPORT:
	{

	}break;
	case eState_Goblin::DEAD:
	{
		m_timeCounter += fDT;

		if (isBackflipRight)
		{
			vPos.x += m_velocity * 2.f;
		}
		else
		{
			vPos.x -= m_velocity * 2.f;
		}

		if (m_timeCounter >= 100.f)
		{
			m_timeCounter = 0.f;
			DeleteObj(this);
		}

	}break;
	}

	SetPos(vPos);
}

void CGoblin::update_animation()
{
	switch (m_state)
	{
	case eState_Goblin::BORN:
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Create_Right");
		}
		else
		{
			GetAnimator()->Play(L"Create_Left");
		}
	}
		break;
	case eState_Goblin::MOVE:
		break;
	case eState_Goblin::LAUGH:
		break;
	case eState_Goblin::JUMP:
		break;
	case eState_Goblin::JUMPUP:
		break;
	case eState_Goblin::JUMPDOWN:
		break;
	case eState_Goblin::TELEPORT:
		break;
	case eState_Goblin::DEAD: 
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Die_Right");
		}
		else
		{
			GetAnimator()->Play(L"Die_Left");
		}

	}break;
	}
}

void CGoblin::render_information()
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
		case eState_Goblin::BORN:curState = L"BORN";
			break;
		case eState_Goblin::MOVE:curState = L"BORN";
			break;
		case eState_Goblin::LAUGH:curState = L"BORN";
			break;
		case eState_Goblin::JUMP:curState = L"BORN";
			break;
		case eState_Goblin::JUMPUP:curState = L"BORN";
			break;
		case eState_Goblin::JUMPDOWN:curState = L"BORN";
			break;
		case eState_Goblin::TELEPORT:curState = L"BORN";
			break;
		case eState_Goblin::DEAD:curState = L"DEAD";
		}
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + curState + L"\n" +
			L" drtnLR  : " + std::to_wstring(isFacedRight) + L"\n" +//TODO:수정
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

void CGoblin::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		++m_floor;
	}
	if (pOther->GetName() == L"Lou")
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Laugh_Right");
		}//TODO:타이머나 다른 것을 사용해서 다 웃고 다음행동하게하기
		else
		{
			GetAnimator()->Play(L"Laugh_Left");
		}
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();

		if(pOther->GetPos().x>=GetPos().x)
			SetBackflipDir(false);
		if (pOther->GetPos().x < GetPos().x)
			SetBackflipDir(true);

		SetHP(--hp);
	}

}

void CGoblin::OnCollision(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		int a = abs((int)(GetCollider()->GetFinalPos().y - _pOther->GetFinalPos().y));
		int b = (int)(GetCollider()->GetScale().y / 2.f + _pOther->GetScale().y / 2.f);
		int sum = abs(a - b);
		if(1<sum)
			--vPos.y;
	}
	
	SetPos(vPos);
}

void CGoblin::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (pOther->GetName() == L"Tile")
	{
		--m_floor;
	}
}





