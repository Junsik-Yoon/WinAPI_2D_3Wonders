#include "pch.h"
#include "CScene.h"
#include "CGameObject.h"
#include "CTile.h"
#include "CCollider.h"
#include "CTexture.h"

CScene::CScene()
{
	m_strName = L"";
	m_iTileX = 0;
	m_iTileY = 0;
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_GAMEOBJ::SIZE; ++i)
	{
		for (int j = 0; j < m_arrObj[i].size(); ++j)
		{
			delete m_arrObj[i][j];
		}
	}
}

void CScene::update()
{
	//씬이 가지고 있는 모든 오브젝트 업데이트
	for (int i = 0; i < (int)GROUP_GAMEOBJ::SIZE; i++)
	{
		for (int j = 0; j < m_arrObj[i].size(); j++)
		{
			if (!m_arrObj[i][j]->isDead())
				m_arrObj[i][j]->update();
		}
	}
}

void CScene::finalupdate()
{
	for (int i = 0; i < (int)GROUP_GAMEOBJ::SIZE; i++)
	{
		for (int j = 0; j < m_arrObj[i].size(); j++)
		{
			m_arrObj[i][j]->finalupdate();
		}
	}
}

void CScene::render()
{
	//씬이 가지고 있는 모든 오브젝트 렌더
	for (int i = 0; i < (int)GROUP_GAMEOBJ::SIZE; i++)
	{
		if ((UINT)GROUP_GAMEOBJ::TILE == i)
		{
			//보이는 타일만 그리기
			render_tile();
			continue;
		}

		for (vector<CGameObject*>::iterator iter = m_arrObj[i].begin();
			iter != m_arrObj[i].end(); )
		{
			if (!(*iter)->isDead())
			{
				(*iter)->render();
				iter++;
			}
			else
			{
				iter = m_arrObj[i].erase(iter);
			}
		}
	}
}

void CScene::render_tile()
{
	const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
	//보이는 영역 타일만 그려주기
	Vec2 fptCamLook = CCameraManager::getInst()->GetLookAt();
	Vec2 fptLeftTop = fptCamLook - Vec2(WINSIZEX, WINSIZEY) / 2.f;

	int iLTCol = (int)fptLeftTop.x / CTile::SIZE_TILE;
	int iLTRow = (int)fptLeftTop.y / CTile::SIZE_TILE;
	int iLTIdx = m_iTileX * iLTRow + iLTCol;

	int iClientWidth = (int)WINSIZEX / CTile::SIZE_TILE;
	int iClientHeight = (int)WINSIZEY / CTile::SIZE_TILE;
	for (int iCurRow = iLTRow; iCurRow <= (iLTRow + iClientHeight); ++iCurRow)
	{
		for (int iCurCol = iLTCol; iCurCol <= (iLTCol + iClientWidth); ++iCurCol)
		{
			if (iCurCol < 0 || m_iTileX <= iCurCol || iCurRow < 0 || m_iTileY <= iCurRow)
			{
				continue;
			}
			int iIdx = (m_iTileX * iCurRow) + iCurCol;

			vecTile[iIdx]->render();
		}
	}

}

void CScene::render_background()
{

}

void CScene::AddObject(CGameObject* pObj, GROUP_GAMEOBJ type)
{
	m_arrObj[(UINT)type].push_back(pObj);
}

void CScene::DeleteGroup(GROUP_GAMEOBJ group)
{
	for (int i = 0; i < m_arrObj[(UINT)group].size(); i++)
	{
		delete m_arrObj[(UINT)group][i];
	}
	m_arrObj[(UINT)group].clear();
}

void CScene::DeleteAll()
{
	for (int i = 0; i < (UINT)GROUP_GAMEOBJ::SIZE; i++)
	{
		DeleteGroup((GROUP_GAMEOBJ)i);
	}
}

void CScene::CreateTile(UINT xSize, UINT ySize)
{
	DeleteGroup(GROUP_GAMEOBJ::TILE);

	m_iTileX = xSize;
	m_iTileY = ySize;

	CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"Tile", L"texture\\Tile\\tilemap.bmp");
	for (int i = 0; i < m_iTileY; ++i)
	{
		for (int j = 0; j < m_iTileX; ++j)
		{
			CTile* pTile = new CTile();
			pTile->SetPos(Vec2((float)(j * CTile::SIZE_TILE), (float)(i * CTile::SIZE_TILE)));
			pTile->SetTexture(pImg);
			//pTile->CreateCollider();
			pTile->SetName(L"Tile");
			AddObject(pTile, GROUP_GAMEOBJ::TILE);
		}
	}

}

void CScene::LoadTile(const wstring& strPath)
{
	//wstring strFilePath = CPathManager::getInst()->GetContentPath();
	//strFilePath += strPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb"); //r: read, b:binary
	assert(pFile);


	UINT xCount = 0;
	UINT yCount = 0;
	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	CreateTile(xCount, yCount);

	const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
	for (UINT i = 0; i < vecTile.size(); ++i)
	{
		((CTile*)vecTile[i])->Load(pFile);
	}

	fclose(pFile);
}
