#include "pch.h"
#include "CTile.h"
#include "CTexture.h"
#include "CCollider.h"

CTile::CTile()
{
	m_pTex = nullptr;
	m_iIdx = 0;
	SetScale(Vec2(SIZE_TILE, SIZE_TILE));

}

CTile::~CTile()
{
}

iPoint CTile::CheckCurrentIndex()
{
	UINT iWidth = m_pTex->GetBmpWidth();
	UINT iHeight = m_pTex->GetBmpHeight();

	UINT iMaxRow = iHeight / SIZE_TILE;
	UINT iMaxCol = iWidth / SIZE_TILE;
	UINT iCurRow = (m_iIdx / iMaxCol) % iMaxRow;
	UINT iCurCol = (m_iIdx % iMaxCol);
	return iPoint((int)iCurRow, (int)iCurCol);
}

void CTile::update()
{
}

void CTile::render(HDC hDC)
{
	if (nullptr == m_pTex)
	{
		return;
	}
	UINT iWidth = m_pTex->GetBmpWidth();
	UINT iHeight = m_pTex->GetBmpHeight();

	UINT iMaxRow = iHeight / SIZE_TILE;
	UINT iMaxCol = iWidth / SIZE_TILE;
	UINT iCurRow = (m_iIdx / iMaxCol) % iMaxRow;
	UINT iCurCol = (m_iIdx % iMaxCol);

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(GetPos());
	Vec2 fptScale = GetScale();
	if (iCurCol == 0 && iCurRow == 0)
	{

	}
	else
	{
		BitBlt(hDC,
			(int)(fptRenderPos.x),
			(int)(fptRenderPos.y),
			(int)(fptScale.x),
			(int)(fptScale.y),
			m_pTex->GetDC(),
			iCurCol * SIZE_TILE,
			iCurRow * SIZE_TILE,
			SRCCOPY);
	}

	//Rectangle(hDC,
	//	(int)(fptRenderPos.x),
	//	(int)(fptRenderPos.y),
	//	(int)(fptRenderPos.x + fptScale.x),
	//	(int)(fptRenderPos.y + fptScale.y));

	component_render(hDC);
}

void CTile::OnCollisionEnter(CCollider* _pOther)
{
}

void CTile::OnCollision(CCollider* _pOther)
{
}

void CTile::OnCollisionExit(CCollider* _pOther)
{
}



void CTile::Save(FILE* pFile)
{
	//wstring str = GetName();
	fwrite(&m_iIdx, sizeof(int), 1, pFile);
	fwrite(&isCollision, sizeof(bool), 1, pFile);
	//fwrite(&str, str.size(), 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	//wstring str = GetName();
	fread(&m_iIdx, sizeof(int), 1, pFile);
	fread(&isCollision, sizeof(bool), 1, pFile);
	//fread(&str, str.size(), 1, pFile);
	//SetName(str);
	if (isCollision)
	{
		CreateCollider();
		GetCollider()->SetOffsetPos(Vec2(GetScale().x / 2.f, GetScale().y / 2.f));
		GetCollider()->SetScale(Vec2(GetScale().x, GetScale().y));
	}
}



