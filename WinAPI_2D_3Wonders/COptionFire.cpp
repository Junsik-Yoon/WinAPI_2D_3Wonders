#include "pch.h"
#include "COptionFire.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CLou.h"

COptionFire::COptionFire()
{
	isFacedRight = {};
	m_owner = nullptr;
	SetName(L"Option_Fire");
	m_state = eState_OpFire::NO_OWNER;
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"FireOpImg", L"texture\\Animation\\Animation_FireOp.png");
	CreateAnimator();

	CreateCollider();
	SetScale(Vec2(20.f, 20.f));
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));

	GetAnimator()->CreateAnimation(L"FireOpIdle_Right", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 1, false);
	GetAnimator()->CreateAnimation(L"FireOpIdle_Left", m_pImg, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 1, true);

	GetAnimator()->CreateAnimation(L"FireOpMove_Right", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 4, false);
	GetAnimator()->CreateAnimation(L"FireOpMove_Left", m_pImg, Vec2(128.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 4, true);

	GetAnimator()->CreateAnimation(L"FireOpAttack_Right", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 8, false, false);
	GetAnimator()->CreateAnimation(L"FireOpAttack_Left", m_pImg, Vec2(640.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 8, true , false);

	GetAnimator()->CreateAnimation(L"FireOpUnborn", m_pImg, Vec2(1664.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.15f, 6, false);
}

COptionFire::~COptionFire()
{
}

void COptionFire::update()
{
	Vec2 vPlayerPos = {};
	if (nullptr == m_owner)
	{
		m_state == eState_OpFire::NO_OWNER;
	}
	else
	{
		isFacedRight = m_owner->GetRight();
		vPlayerPos = m_owner->GetPos();
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
		
	}
		break;
	case eState_OpFire::ATTACK:
	{
		if (isFacedRight)
		{
			GetAnimator()->Play(L"FireOpAttack_Right");
		}
		else
		{
			GetAnimator()->Play(L"FireOpAttack_Left");
		}

	}
		break;
	}

	GetAnimator()->update();
}

void COptionFire::render()
{
	component_render();
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


