#include "pch.h"
#include "CHyperMissile.h"
#include "CCollider.h"
#include "CAnimator.h"

CHyperMissile::CHyperMissile()
{


}

CHyperMissile::CHyperMissile(Vec2 _dirVec)
{
	m_fSpeed = 400.f;
	m_fvDir = _dirVec;
	m_gravity = 150.f;
	SetName(L"Hyper_Missile_Player");

	SetScale(Vec2(20.f, 20.f));
	m_pImgHyper = CResourceManager::getInst()->LoadD2DImage(L"PlayerSPMissileImg", L"texture\\Animation\\Animation_Hyper_Missile.png");
	CreateCollider();
	GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Missile_Hyper", m_pImgHyper, Vec2(192.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 0.f), 0.03f, 6, false);

	GetAnimator()->Play(L"Missile_Hyper");
}

CHyperMissile::~CHyperMissile()
{
}


void CHyperMissile::update()
{
	Vec2 vPos = GetPos();

	m_fvDir.y += 0.4f * fDT;

	vPos.x += m_fSpeed * m_fvDir.x * fDT;
	vPos.y += m_fSpeed * m_fvDir.y * fDT;

	if (m_fvDir.y < m_fvDir.x
		&&m_fvDir.y<0.f)
	{
		m_gravity += 100.f *fDT;
		vPos.y += m_gravity * fDT;
	}


	SetPos(vPos);

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(vPos);
	if (fptRenderPos.x < 0 ||
		fptRenderPos.x > WINSIZEX ||
		//fptRenderPos.y < 0 ||
		fptRenderPos.y > WINSIZEY)
		DeleteObj(this);
	GetAnimator()->update();
}

void CHyperMissile::render()
{
	component_render();
}

void CHyperMissile::SetDir(Vec2 vec)
{
	m_fvDir = vec.Normalize();
}

void CHyperMissile::SetDir(float theta)
{
	m_fvDir.x = (float)cos(theta);
	m_fvDir.y = (float)sin(theta);
}

void CHyperMissile::OnCollisionEnter(CCollider* pOther)
{
}

void CHyperMissile::OnCollision(CCollider* _pOther)
{
}

void CHyperMissile::OnCollisionExit(CCollider* _pOther)
{
}

