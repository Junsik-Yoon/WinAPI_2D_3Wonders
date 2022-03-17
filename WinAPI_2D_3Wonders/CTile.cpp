#include "pch.h"
#include "CTile.h"
#include "CD2DImage.h"

CTile::CTile()
{
	m_pImg = nullptr;
	m_iX = 0;
	m_iY = 0;
	m_iIdx = 0;
	SetName(L"Tile");
	SetScale(Vec2(SIZE_TILE, SIZE_TILE));
	m_eGroup = GROUP_TILE::NONE;
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

	if (0 != m_iIdx)
	{
		CRenderManager::getInst()->RenderFrame(
			m_pImg,
			fptRenderPos.x,
			fptRenderPos.y,
			fptRenderPos.x + fptScale.x,
			fptRenderPos.y + fptScale.y,
			iCurCol * fptScale.x,
			iCurRow * fptScale.y,
			(iCurCol + 1) * fptScale.x,
			(iCurRow + 1) * fptScale.y
		);
	}
	component_render();
}




void CTile::Save(FILE* pFile)
{
	fwrite(&m_iX, sizeof(int), 1, pFile);
	fwrite(&m_iY, sizeof(int), 1, pFile);
	fwrite(&m_iIdx, sizeof(int), 1, pFile);

	int group = (int)m_eGroup;
	fwrite(&group, sizeof(int), 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	fread(&m_iX, sizeof(int), 1, pFile);
	fread(&m_iY, sizeof(int), 1, pFile);
	fread(&m_iIdx, sizeof(int), 1, pFile);

	int group;
	fread(&group, sizeof(int), 1, pFile);
	m_eGroup = (GROUP_TILE)group;
}



