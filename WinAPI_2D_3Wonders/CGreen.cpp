#include "pch.h"
#include "CGreen.h"
#include "CScene.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CShootFire.h"
#include "CLou.h"
#include "CEffect.h"
#include "CTile.h"

#define D_VELOCITY 150
#define D_GRAVITY 400
CGreen::CGreen()
{
	m_state = eState_Green::IDLE;
	SetHP(20);
	m_stateTimer = 0.f;
	isShootingFire = 0;
	isRight = false;
	m_state = eState_Green::IDLE;
	m_floor = 0;
	m_wall = 0;
	m_velocity = D_VELOCITY;
	m_gravity = D_GRAVITY;
	prevHP = GetHP();
	SetName(L"Green");
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GreenImg", L"texture\\Animation\\Animation_Green.png");
	CreateCollider();
	SetScale(Vec2(120.f, 120.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 20.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Move_Left", m_pImg, Vec2(0.f, 0.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 5,false);
	GetAnimator()->CreateAnimation(L"Move_Right", m_pImg, Vec2(0.f, 384.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 5,false);

	GetAnimator()->CreateAnimation(L"Fire_Right", m_pImg, Vec2(0.f, 576.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 2, false,false);
	GetAnimator()->CreateAnimation(L"Fire_Left", m_pImg, Vec2(0.f, 192.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.2f, 2, false,false);

	GetAnimator()->CreateAnimation(L"Damaged_Right", m_pImg, Vec2(384.f, 192.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.05f, 2, false);
	GetAnimator()->CreateAnimation(L"Damaged_Left", m_pImg, Vec2(384.f, 576.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.05f, 2, false);



	GetAnimator()->Play(L"Move_Left");

	CSoundManager::getInst()->AddSound(L"fireshootingSound", L"sound\\fireshootingSound.wav", false);
	

	
}

CGreen::~CGreen()
{
}


void CGreen::update()
{
	Vec2 vPos = GetPos();
	vector<CGameObject*> pPlayer = CSceneManager::getInst()->GetCurScene()->GetGroupObject(GROUP_GAMEOBJ::PLAYER);
	CLou* pLou = (CLou*)pPlayer[0];
	int hp = GetHP();
	

	if (pPlayer[0]->GetPos().x >= GetPos().x)
	{
		isRight = true;
	}
	else
	{
		isRight = false;
	}

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
	case eState_Green::IDLE:
	{
		if (abs(pPlayer[0]->GetPos().x - GetPos().x) < 400.f
			&& abs(pPlayer[0]->GetPos().x - GetPos().x) > 200.f)
		{
			m_state = eState_Green::TRACE;
		}	
	}
		break;
	case eState_Green::TRACE:
	{
		if (abs(pPlayer[0]->GetPos().x - GetPos().x) >= 400.f)
		{
			m_state = eState_Green::IDLE;
		}
		else if (abs(pPlayer[0]->GetPos().x - GetPos().x) <= 200.f
			&& (pLou->GetState() != eState::DEAD ||
				pLou->GetState() != eState::GOTHIT))
		{
			m_state = eState_Green::SHOOT;
		}

		if(isRight)
		{
			vPos.x += 50.f * fDT;
			GetAnimator()->Play(L"Move_Right");
		}
		else
		{
			vPos.x -= 50.f * fDT;
			GetAnimator()->Play(L"Move_Left");
		}
		if (prevHP > GetHP())
		{
			m_stateTimer = 0.f;
			m_state = eState_Green::DAMAGED;
		}
	}
		break;
	case eState_Green::SHOOT:
	{
		m_stateTimer += fDT;

		if (abs(pPlayer[0]->GetPos().x - GetPos().x) >= 200.f)
		{
			m_stateTimer = 0.f;
			m_state = eState_Green::TRACE;
		}

		if (m_stateTimer >= 0.2f)
		{
			++isShootingFire;
			if (isShootingFire >= 5)
			{
				isShootingFire = 0;
			}
			ShootFire();
			CSoundManager::getInst()->Play(L"fireshootingSound", 0.2f);
			m_stateTimer = 0.f;
		}
		if (prevHP > GetHP())
		{
			m_stateTimer = 0.f;
			m_state = eState_Green::DAMAGED;
		}
	}
		break;
	case eState_Green::DAMAGED:
	{
		m_stateTimer += fDT;
		if (m_stateTimer >= 0.1f)
		{
			m_stateTimer = 0.f;			
			m_state = eState_Green::TRACE;
		}
		if (isRight)
		{
			GetAnimator()->Play(L"Damaged_Left");
			vPos.x -= 100.f * fDT;
		}
		else
		{
			GetAnimator()->Play(L"Damaged_Right");
			vPos.x += 100.f * fDT;
		}
		if (GetHP() <= 0)
		{
			CSoundManager::getInst()->Play(L"monster_die");
			CSoundManager::getInst()->Play(L"halfmoon_die");
			//////////////////////이펙트///////////////
			CEffect* effectHMDie = new CEffect(L"Effect_Die_Big", L"texture\\Animation\\Effect_Die_Big.png",
				L"Effect_Die_Big", Vec2(0.f, 0.f), Vec2(192.f, 192.f), Vec2(192.f, 0.f), 0.15f, 10, false, false, L"Effect_Die_Big");
			effectHMDie->SetPos(Vec2(GetPos()));
			effectHMDie->SetDuration(1.5f);
			CreateObj(effectHMDie, GROUP_GAMEOBJ::EFFECT);
			///////////////////////////////////////////
			m_stateTimer = 0.f;
			m_state = eState_Green::DEAD;
		}
	}
		break;
	case eState_Green::DEAD:
	{
		m_stateTimer += fDT;
		if (m_stateTimer >= 0.5f)
		{
			DeleteObj(this);
		}
	}
		break;
	}
	
	prevHP = GetHP();
	SetPos(vPos);
	GetAnimator()->update();
}

void CGreen::render()
{
	component_render();
	render_information();
}

void CGreen::render_information()
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
			L" drctn  : " + std::to_wstring(isRight) + L"\n" +//TODO:수정
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


void CGreen::ShootFire()
{
	CShootFire* pShootFire = new CShootFire();
	Vec2 fptFirePos = GetPos();
	pShootFire->SetName(L"GreenFire");
	pShootFire->SetStartPos(fptFirePos);
	pShootFire->SetPos(fptFirePos);
	pShootFire->SetScale(Vec2(64.f, 64.f));
	pShootFire->GetCollider()->SetScale(Vec2(pShootFire->GetScale().x, pShootFire->GetScale().y));
	
	if (isRight)
	{
		GetAnimator()->Play(L"Fire_Right");
		pShootFire->SetRight(true);
		pShootFire->SetPos(Vec2(fptFirePos.x + 55.f, fptFirePos.y + 20.f));
		pShootFire->SetDir(Vec2(1.f, 0.f));
	}
	else
	{
		//TODO:방향안먹는거수정
		GetAnimator()->Play(L"Fire_Left");
		pShootFire->SetRight(false);
		pShootFire->SetPos(Vec2(fptFirePos.x-55.f, fptFirePos.y + 20.f));
		pShootFire->SetDir(Vec2(-1.f, 0.f));
	}
	pShootFire->GetAnimator()->Play(L"GreenFire");
	CreateObj(pShootFire, GROUP_GAMEOBJ::FIRE);
}

void CGreen::OnCollisionEnter(CCollider* _pOther)
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
	}
	if (pOther->GetName() == L"Missile_Player")
	{
		int hp = GetHP();
		hp -= 2;
		SetHP(hp);
		m_state = eState_Green::DAMAGED;
	}
	else if (pOther->GetName() == L"Hyper_Missile_Player"
		|| pOther->GetName() == L"Fire_Player")
	{
		int hp = GetHP();
		SetHP(--hp);
	}

}

void CGreen::OnCollision(CCollider* _pOther)
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

void CGreen::OnCollisionExit(CCollider* _pOther)
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
	}
}





