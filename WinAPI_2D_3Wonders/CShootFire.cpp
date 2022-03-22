#include "pch.h"
#include "CShootFire.h"
#include "CCollider.h"
#include "CAnimator.h"

CShootFire::CShootFire()
{
	shootRange = 200.f;
	vanishToggle = 0.f;
	m_fSpeed = 70.f;
	m_fvDir = Vec2(-1.f, 0.f);

	

	SetName(L"Missile_Player");
	SetScale(Vec2(20.f, 20.f));
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"GreenFireImg", L"texture\\Animation\\Animation_Green_Fire.png");
	
	CreateCollider();
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"GreenFire", m_pImg, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.2f, 13, false);

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CShootFire::~CShootFire()
{
}


void CShootFire::update()
{
	Vec2 vPos = GetPos();
	m_fvDir.y -= 0.3f * fDT;
	vPos.x += m_fSpeed * m_fvDir.x * fDT;
	vPos.y += m_fSpeed * m_fvDir.y * fDT;

	if (abs(startPos.x - GetPos().x) >= shootRange)
	{
		m_fSpeed = 0.f;
	}

	vanishToggle += fDT;
	if (vanishToggle >= 5.f)
	{
		DeleteObj(this);
		vanishToggle = 0.f;
	}

	SetPos(vPos);

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(vPos);
	if (fptRenderPos.x < 0 ||
		fptRenderPos.x > WINSIZEX ||
		fptRenderPos.y < 0 ||
		fptRenderPos.y > WINSIZEY)
		DeleteObj(this);
	GetAnimator()->update();
}

void CShootFire::render()
{
	component_render();
}

void CShootFire::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOther = _pOther->GetObj();
	if (GetName() == L"GreenFire" &&
		pOther->GetName() == L"Lou")
	{
		DeleteObj(this);
	}
}

void CShootFire::OnCollision(CCollider* _pOther)
{
}

void CShootFire::OnCollisionExit(CCollider* _pOther)
{
}


