#include "pch.h"
#include "CTile.h"
#include "CD2DImage.h"
#include "CCollider.h"

CTile::CTile()
{
	m_pImg = nullptr;
	m_iIdx = 0;
	SetScale(Vec2(SIZE_TILE, SIZE_TILE));

}

CTile::~CTile()
{
}


void CTile::update()
{
}

void CTile::render()
{
	if (nullptr == m_pImg)
	{
		return;
	}
	UINT iWidth = m_pImg->GetWidth();
	UINT iHeight = m_pImg->GetHeight();

	UINT iMaxRow = iHeight / SIZE_TILE;
	UINT iMaxCol = iWidth / SIZE_TILE;
	UINT iCurRow = (m_iIdx / iMaxCol) % iMaxRow;
	UINT iCurCol = (m_iIdx % iMaxCol);

	Vec2 fptRenderPos = CCameraManager::getInst()->GetRenderPos(GetPos());
	Vec2 fptScale = GetScale();

	CRenderManager::getInst()->RenderFrame(
		m_pImg,
		fptRenderPos.x,
		fptRenderPos.y,
		fptScale.x,
		fptScale.y,
		iCurCol * fptScale.x,
		iCurRow * fptScale.y,
		fptScale.x,
		fptScale.y);
	//if (iCurCol == 0 && iCurRow == 0)
	//{

	//}
	//else
	//{
	//	BitBlt(hDC,
	//		(int)(fptRenderPos.x),
	//		(int)(fptRenderPos.y),
	//		(int)(fptScale.x),
	//		(int)(fptScale.y),
	//		m_pTex->GetDC(),
	//		iCurCol * SIZE_TILE,
	//		iCurRow * SIZE_TILE,
	//		SRCCOPY);
	//}

	//Rectangle(hDC,
	//	(int)(fptRenderPos.x),
	//	(int)(fptRenderPos.y),
	//	(int)(fptRenderPos.x + fptScale.x),
	//	(int)(fptRenderPos.y + fptScale.y));

	component_render();
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



