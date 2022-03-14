#include "pch.h"
#include "CCollider.h"
#include "CGameObject.h"

#include "SelectGDI.h"

UINT CCollider::s_iID = 0;

CCollider::CCollider()
{
	m_pOwner = nullptr;
	m_fptOffsetPos = {};
	m_fptFinalPos = {};
	m_fptScale = {};
	m_iColCount = 0;
	m_iID = s_iID++;
}

CCollider::CCollider(const CCollider& other)
{
	m_pOwner = nullptr;
	m_fptOffsetPos = other.m_fptOffsetPos;
	m_fptFinalPos = other.m_fptFinalPos;
	m_fptScale = other.m_fptScale;
	m_iColCount = other.m_iColCount;
	m_iID = s_iID++;
}

CCollider::~CCollider()
{
}

CGameObject* CCollider::GetObj()
{
	return m_pOwner;
}

Vec2 CCollider::GetOffsetPos()
{
	return m_fptOffsetPos;
}

Vec2 CCollider::GetFinalPos()
{
	return m_fptFinalPos;
}

Vec2 CCollider::GetScale()
{
	return m_fptScale;
}

void CCollider::SetOffsetPos(Vec2 offsetPos)
{
	m_fptOffsetPos = offsetPos;
}

void CCollider::SetFinalPos(Vec2 finalPos)
{
	m_fptFinalPos = finalPos;
}

void CCollider::SetScale(Vec2 scale)
{
	m_fptScale = scale;
}

UINT CCollider::GetID()
{
	return m_iID;
}

void CCollider::finalupdate()
{
	Vec2 fptObjectPos = m_pOwner->GetPos();
	m_fptFinalPos = fptObjectPos + m_fptOffsetPos;
}

void CCollider::render(HDC hDC)
{
	TYPE_PEN typePen;
	if (m_iColCount)
		typePen = TYPE_PEN::RED;
	else
		typePen = TYPE_PEN::GREEN;

	SelectGDI brush(hDC, TYPE_BRUSH::HOLLOW);
	SelectGDI pen(hDC, typePen);

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(m_fptFinalPos);

	Rectangle(hDC,
		(int)(fptRenderPos.x - m_fptScale.x / 2.f),
		(int)(fptRenderPos.y - m_fptScale.y / 2.f),
		(int)(fptRenderPos.x + m_fptScale.x / 2.f),
		(int)(fptRenderPos.y + m_fptScale.y / 2.f));
}

void CCollider::OnCollision(CCollider* pOther)
{
	m_pOwner->OnCollision(pOther);
}

void CCollider::OnCollisionEnter(CCollider* pOther)
{
	m_pOwner->OnCollisionEnter(pOther);
	m_iColCount++;
}

void CCollider::OnCollisionExit(CCollider* pOther)
{
	m_pOwner->OnCollisionExit(pOther);
	m_iColCount--;
}
