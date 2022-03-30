#include "pch.h"
#include "CGoblin.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CEffect.h"
#include "CTile.h"

#define D_VELOCITY 150
#define D_GRAVITY 400

CGoblin::CGoblin()
{
	prevPlayerHP = 0;
	isBackflipRight = true;
	m_timeCounter = 0.f;
	m_state = eState_Goblin::BORN;
	SetHP(2);
	m_floor=0;
	m_wall=0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	isEffectOn = true;
	SetName(L"Goblin");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GoblinImg", L"texture\\Animation\\Animation_Goblin.png");
	CreateCollider();
	SetScale(Vec2(60.f, 80.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 13.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Create_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.166f, 6,false);
	GetAnimator()->CreateAnimation(L"Create_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.166f, 6,false);
	
	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(256.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(384.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.2f, 6, false);
	
	GetAnimator()->CreateAnimation(L"Laugh_Left", m_pImg, Vec2(128.f, 896.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);
	GetAnimator()->CreateAnimation(L"Laugh_Right", m_pImg, Vec2(128.f, 1152.f), Vec2(256.f, 256.f), Vec2(256.f, 0.f), 0.3f, 2, false);

	GetAnimator()->CreateAnimation(L"Die_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);
	GetAnimator()->CreateAnimation(L"Die_Right", m_pImg, Vec2(512.f, 0.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 5, false);

	GetAnimator()->CreateAnimation(L"NULL", m_pImg, Vec2(0.f, 1100.f), Vec2(128.f, 128.f), Vec2(0.f, 128.f), 0.1f, 1, false);

	GetAnimator()->Play(L"NULL");

	CSoundManager::getInst()->AddSound(L"goblin_laugh", L"sound\\goblin_laugh.wav", false);
	
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
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	Vec2 vPlayerPos = pPlayer[0]->GetPos();
	int curPlayerHP = pPlayer[0]->GetHP();

	if (m_floor == 0)
	{
		m_gravity = D_GRAVITY;
		vPos.y += D_GRAVITY * fDT;
	}
	else
	{
		m_gravity = 0;
	}

	if (abs(vPlayerPos.x - vPos.x) > 600.f)
	{
		DeleteObj(this);
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
		if (isFacedRight)
		{
			vPos.x += 50.f * fDT;
		}
		else
		{
			vPos.x -= 50.f * fDT;
		}
		if (prevPlayerHP > curPlayerHP && abs(vPlayerPos.x-vPos.x)<100.f)
		{
			CSoundManager::getInst()->Play(L"goblin_laugh",0.1f);
			m_state = eState_Goblin::LAUGH;
		}
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::LAUGH:
	{
		m_timeCounter += fDT;
		if (m_timeCounter >= 1.5f)
		{
			m_state = eState_Goblin::MOVE;
		}
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::JUMP:
	{
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::JUMPUP:
	{
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::JUMPDOWN:
	{
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::TELEPORT:
	{
		if (0 >= GetHP())
		{
			m_timeCounter = 0.f;
			m_state = eState_Goblin::DEAD;
		}
	}break;
	case eState_Goblin::DEAD:
	{
		m_timeCounter += fDT;

		if (isBackflipRight)
		{
			vPos.x += m_velocity * 3.f * fDT;
		}
		else
		{
			vPos.x -= m_velocity * 3.f * fDT;
		}

		if (m_timeCounter >= 0.25f)
		{
			CSoundManager::getInst()->Play(L"monster_die");
			m_timeCounter = 0.f;
			DeleteObj(this);
		}

	}break;
	}

	prevPlayerHP = curPlayerHP;
	SetPos(vPos);
}

void CGoblin::update_animation()
{
	switch (m_state)
	{
	case eState_Goblin::BORN:
	{
		if (isFacedRight&& m_timeCounter>=1.f)
		{
			GetAnimator()->Play(L"Create_Right");
		}
		else if (!isFacedRight && m_timeCounter >= 1.f)
		{
			GetAnimator()->Play(L"Create_Left");
		}
	}
		break;
	case eState_Goblin::MOVE:
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Move_Right");
		}
		else
		{
			GetAnimator()->Play(L"Move_Left");
		}
	}
		break;
	case eState_Goblin::LAUGH:
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Laugh_Right");
			GetAnimator()->GetCurrentAnimation()->SetOffset(Vec2(0.f, -60.f), 0);
			GetAnimator()->GetCurrentAnimation()->SetOffset(Vec2(0.f, -60.f), 1);
		}
		else
		{
			GetAnimator()->Play(L"Laugh_Left");
			GetAnimator()->GetCurrentAnimation()->SetOffset(Vec2(0.f, -60.f), 0);
			GetAnimator()->GetCurrentAnimation()->SetOffset(Vec2(0.f, -60.f), 1);
		}

	}
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
		if (m_timeCounter >= 0.23f && isEffectOn)
		{
			isEffectOn = false;
			//////////////////////ÀÌÆåÆ®///////////////
			CEffect* effectDie = new CEffect(L"Die_Effect_Small", L"texture\\Animation\\Effect_Die_Small.png",
				L"Die_Effect_Small", Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 10, false, false, L"Die_Effect_Small");
			effectDie->SetPos(Vec2(GetPos()));
			effectDie->SetDuration(1.f);
			CreateObj(effectDie, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
		}
		if (isFacedRight)
		{
			GetAnimator()->Play(L"Die_Left");
		}
		else
		{
			GetAnimator()->Play(L"Die_Right");
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
		case eState_Goblin::MOVE:curState = L"MOVE";
			break;
		case eState_Goblin::LAUGH:curState = L"LAUGH";
			break;
		case eState_Goblin::JUMP:curState = L"JUMP";
			break;
		case eState_Goblin::JUMPUP:curState = L"JUMPUP";
			break;
		case eState_Goblin::JUMPDOWN:curState = L"JUMPDOWN";
			break;
		case eState_Goblin::TELEPORT:curState = L"TELEPORT";
			break;
		case eState_Goblin::DEAD:curState = L"DEAD";
		}
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + curState + L"\n" +
			L" drtnLR  : " + std::to_wstring(isFacedRight) + L"\n" +//TODO:¼öÁ¤
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
	CTile* pTile = (CTile*)pOther;
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		if (pTile->GetGroup() == GROUP_TILE::GROUND)
		{
			++m_floor;
		}
		else if (pTile->GetGroup() == GROUP_TILE::PLATFORM)
		{
			++m_floor;
		}
	}
	if (pOther->GetName() == L"Lou")
	{

	}
	if (pOther->GetName() == L"Missile_Player")
	{
		if (m_state != eState_Goblin::BORN)
		{
			int hp = GetHP();

			if (pOther->GetPos().x >= GetPos().x)
				SetBackflipDir(false);
			if (pOther->GetPos().x < GetPos().x)
				SetBackflipDir(true);
			hp -= 2;
			SetHP(hp);
		}
	}
	else if (pOther->GetName() == L"Hyper_Missile_Player"
		||pOther->GetName() == L"Fire_Player")
	{
		if (m_state != eState_Goblin::BORN)
		{
			int hp = GetHP();

			if (pOther->GetPos().x >= GetPos().x)
				SetBackflipDir(false);
			if (pOther->GetPos().x < GetPos().x)
				SetBackflipDir(true);
			SetHP(--hp);
		}
	}

}

void CGoblin::OnCollision(CCollider* _pOther)
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

void CGoblin::OnCollisionExit(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	CTile* pTile = (CTile*)pOther;
	Vec2 vPos = GetPos();
	if (pOther->GetName() == L"Tile")
	{
		if (pTile->GetGroup() == GROUP_TILE::GROUND)
		{
			--m_floor;
		}
		else if (pTile->GetGroup() == GROUP_TILE::PLATFORM)
		{
			--m_floor;
		}
	}
}





