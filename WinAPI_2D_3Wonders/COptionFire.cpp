#include "pch.h"
#include "COptionFire.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CLou.h"
#include "CAnimation.h"
#include "CShootFire.h"


COptionFire::COptionFire()
{
	attackTimer = 0.f;
	fireTimer = 0.f;
	isFacedRight = {};
	m_owner = nullptr;
	SetName(L"Option_Fire");
	m_state = eState_OpFire::NO_OWNER;
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"FireOpImg", L"texture\\Animation\\Animation_FireOp.png");
	CreateAnimator();

	CreateCollider();
	SetScale(Vec2(20.f, 20.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));

	GetAnimator()->CreateAnimation(L"FireOpIdle_Right", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 1, true);
	GetAnimator()->CreateAnimation(L"FireOpIdle_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 1, false);

	GetAnimator()->CreateAnimation(L"FireOpMove_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.3f, 4, true);
	GetAnimator()->CreateAnimation(L"FireOpMove_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.3f, 4, false);

	GetAnimator()->CreateAnimation(L"FireOpAttack_Right", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.25f, 8, true);
	GetAnimator()->CreateAnimation(L"FireOpAttack_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.25f, 8, false);

	GetAnimator()->CreateAnimation(L"FireOpUnborn", m_pImg, Vec2(1664.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 6, false);
}

COptionFire::~COptionFire()
{
}



void COptionFire::update()
{
	Vec2 vPlayerPos = {};
	Vec2 vPlayerScale = {};
	Vec2 prevPos = {};
	if (nullptr == m_owner)
	{
		m_state == eState_OpFire::NO_OWNER;
	}
	else
	{
		isFacedRight = m_owner->GetRight();
		vPlayerPos = m_owner->GetPos();
		vPlayerScale = m_owner->GetScale();
		int iPlayerHP = m_owner->GetHP();
		if (isFacedRight)
		{
			SetPos(Vec2(vPlayerPos.x - vPlayerScale.x , vPlayerPos.y - vPlayerScale.y/2.f ));
		}
		else
		{
			SetPos(Vec2(vPlayerPos.x + vPlayerScale.x , vPlayerPos.y - vPlayerScale.y/2.f ));
		}
		if (iPlayerHP <= 0)
		{
			DeleteObj(this);
		}
	}
	///////////==========ป๓ลย===========//////////////

	switch (m_state)
	{
	case eState_OpFire::NO_OWNER:
	{
		GetAnimator()->Play(L"FireOpUnborn");
	}
		break;
	case eState_OpFire::IDLE:
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"FireOpIdle_Right");
		}
		else
		{
			GetAnimator()->Play(L"FireOpIdle_Left");
		}
		if (abs(prevPos.x-GetPos().x) >1.f)
		{
			m_state = eState_OpFire::MOVE;
		}
		if (KEYDOWN('Z'))
		{
			m_state = eState_OpFire::ATTACK;
		}
	}
		break;
	case eState_OpFire::MOVE:
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"FireOpMove_Right");
		}
		else
		{
			GetAnimator()->Play(L"FireOpMove_Left");
		}
		if (abs(prevPos.x - GetPos().x) <= 1.f)
		{
			m_state = eState_OpFire::IDLE;
		}
		if (KEYDOWN('Z'))
		{
			m_state = eState_OpFire::ATTACK;
		}
	}
		break;
	case eState_OpFire::ATTACK:
	{
		attackTimer += fDT;
		if (isFacedRight)
		{
			GetAnimator()->Play(L"FireOpAttack_Right");
		}
		else
		{
			GetAnimator()->Play(L"FireOpAttack_Left");
		}
		if (attackTimer >= 1.f)
		{
			fireTimer +=fDT;
			if (fireTimer >= 0.2f)
			{
				fireTimer = 0.f;
				FireBreathing();
			}
			if (attackTimer >= 2.f)
			{
				attackTimer = 0.f;
				fireTimer = 0.f;
				m_state = eState_OpFire::IDLE;
			}
		}


	}
		break;
	}
	prevPos = GetPos();
	GetAnimator()->update();
}

void COptionFire::FireBreathing()
{
	CShootFire* pShootFire = new CShootFire();
	Vec2 fptFirePos = GetPos();
	pShootFire->SetPos(fptFirePos);
	pShootFire->SetStartPos(fptFirePos);
	pShootFire->SetName(L"Missile_Player");

	pShootFire->SetScale(Vec2(32.f, 32.f));
	pShootFire->GetCollider()->SetScale(Vec2(pShootFire->GetScale().x, pShootFire->GetScale().y));

	if (isFacedRight)
	{
		pShootFire->SetRight(true);
		pShootFire->SetPos(Vec2(fptFirePos.x + 55.f, fptFirePos.y + 20.f));
		pShootFire->SetDir(Vec2(1.f, 0.f));
	}
	else
	{
		pShootFire->SetRight(false);
		pShootFire->SetPos(Vec2(fptFirePos.x - 55.f, fptFirePos.y + 20.f));
		pShootFire->SetDir(Vec2(-1.f, 0.f));
	}
	pShootFire->GetAnimator()->Play(L"OptionFire");
	CreateObj(pShootFire,GROUP_GAMEOBJ::MISSILE_PLAYER);
}

void COptionFire::render()
{
	component_render();
	render_information();
}

void COptionFire::render_information()
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
		case eState_OpFire::NO_OWNER: curState = L"NO_OWNER";
			break;
		case eState_OpFire::IDLE:curState = L"IDLE";
			break;
		case eState_OpFire::MOVE:curState = L"MOVE";
			break;
		case eState_OpFire::ATTACK:curState = L"ATTACK";
			break;
		}
		curAni = GetAnimator()->GetCurrentAnimation()->GetName();
		CRenderManager::getInst()->RenderText(
			L" pos X : " + std::to_wstring(GetPos().x) + L"\n" +
			L" pos Y : " + std::to_wstring(GetPos().y) + L"\n" +
			L" state  : " + curState + L"\n" +
			L" curAm : " + curAni + L"\n" 
			, vPos.x + 30.f
			, vPos.y + -40.f
			, vPos.x + 200.f
			, vPos.y + 100.f
			, 16.f
			, RGB(255, 255, 255));
	}
}

void COptionFire::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetObj()->GetName() == L"Lou"
		&& nullptr == m_owner)
	{
		m_owner = (CLou*)_pOther->GetObj();
		m_state = eState_OpFire::IDLE;
	}
}

void COptionFire::OnCollision(CCollider* _pOther)
{
}

void COptionFire::OnCollisionExit(CCollider* _pOther)
{
}


