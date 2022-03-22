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
	m_pImgPlayer = CResourceManager::getInst()->LoadD2DImage(L"PlayerMissileImg", L"texture\\Animation\\Animation_PlayerMissile.png");
	m_pImgGW = CResourceManager::getInst()->LoadD2DImage(L"GWMissileImg", L"texture\\Animation\\Animation_Missile_GW.png");
	CreateCollider();
	GetCollider()->SetScale(Vec2(GetScale().x,GetScale().y));
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

	CreateAnimator();

	GetAnimator()->CreateAnimation(L"Missile_GW", m_pImgGW, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 0.f), 0.1f, 4, false);

	GetAnimator()->CreateAnimation(L"N_Shoot_Right", m_pImgPlayer, Vec2(0.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"N_Shoot_Left", m_pImgPlayer, Vec2(0.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, true);

	GetAnimator()->CreateAnimation(L"N_Shoot_Up", m_pImgPlayer, Vec2(0.f, 192.f), Vec2(48.f, 192.f), Vec2(48.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"N_Shoot_Down", m_pImgPlayer, Vec2(96.f, 192.f), Vec2(48.f, 192.f), Vec2(48.f, 0.f), 0.5f, 1, false);


	GetAnimator()->CreateAnimation(L"N_Hit_Right", m_pImgPlayer, Vec2(192.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, false);
	GetAnimator()->CreateAnimation(L"N_Hit_Left", m_pImgPlayer, Vec2(192.f, 0.f), Vec2(192.f, 48.f), Vec2(192.f, 0.f), 0.5f, 1, true);

	

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
	GetAnimator()->update();
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
		GetAnimator()->Play(L"NULL"); //맞으면 이미지 없앰
	}//TODO:getgroupobj구현하기 오브젝트 클래스에
}

void CMissile::OnCollision(CCollider* _pOther)
{
}

void CMissile::OnCollisionExit(CCollider* _pOther)
{
}


