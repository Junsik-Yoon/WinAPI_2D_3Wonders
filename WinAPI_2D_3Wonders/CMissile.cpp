#include "pch.h"
#include "CMissile.h"
#include "CCollider.h"
#include "CAnimator.h"


CMissile::CMissile()
{
	m_fSpeed = 400.f;
	m_fvDir = Vec2(0, 0);
	SetName(L"Missile_Player");

	SetScale(Vec2(20.f, 20.f));
	m_pImg = CResourceManager::getInst()->LoadD2DImage(L"PlayerMissileImg", L"texture\\Animation\\Animation_PlayerMissile.png");
	
	CreateCollider();
	GetCollider()->SetScale(Vec2(GetScale().x,GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Shoot_Right", m_pImg, Vec2(0.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Shoot_Left", m_pImg, Vec2(0.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, true);

	GetAnimator()->CreateAnimation(L"Hit_Right", m_pImg, Vec2(192.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"Hit_Left", m_pImg, Vec2(192.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, true);

	

	CCameraManager::getInst()->GetRenderPos(GetPos());

}

CMissile::~CMissile()
{
}

void CMissile::update()
{
	Vec2 vPos = GetPos();

	vPos.x += m_fSpeed * m_fvDir.x * fDT;
	vPos.y += m_fSpeed * m_fvDir.y * fDT;

	SetPos(vPos);

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(vPos);
	if (fptRenderPos.x < 0			||
		fptRenderPos.x > WINSIZEX	||
		fptRenderPos.y < 0			||
		fptRenderPos.y > WINSIZEY)
		DeleteObj(this);
}

void CMissile::render()
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(vPos);
	
	CRenderManager::getInst()->RenderRectangle(
		fptRenderPos.x-vScale.x/2.f,
		fptRenderPos.y-vScale.y/2.f,
		fptRenderPos.x+vScale.x / 2.f,
		fptRenderPos.y-vScale.y / 2.f);

	component_render();
}

void CMissile::SetDir(Vec2 vec)
{
	m_fvDir = vec.Normalize();
}

void CMissile::SetDir(float theta)
{
	m_fvDir.x = (float)cos(theta);
	m_fvDir.y = (float)sin(theta);
}

void CMissile::OnCollisionEnter(CCollider* pOther)
{
	CGameObject* pOtherObj = pOther->GetObj();
	if (pOtherObj->GetName() == L"Monster"||
		pOtherObj->GetName() == L"Tile")
	{
		GetAnimator()->Play(L"Shoot_Right");
		DeleteObj(this);
	}
}

void CMissile::OnCollision(CCollider* _pOther)
{
}

void CMissile::OnCollisionExit(CCollider* _pOther)
{
}


