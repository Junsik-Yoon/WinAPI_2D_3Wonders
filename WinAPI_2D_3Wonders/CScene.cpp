#include "pch.h"
#include "CScene.h"
#include "CGameObject.h"
#include "CTile.h"
#include "CCollider.h"
#include "CTexture.h"

CScene::CScene()
{
	m_strName = L"";

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
	for (UINT i = 0; i < vecTile.size(); ++i)
	{
		vecTile[i]->render();
	}

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

void CScene::LoadTile(const wstring& strPath)
{
	DeleteGroup(GROUP_GAMEOBJ::TILE);


	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb"); //r: read, b:binary
	assert(pFile);


	UINT xCount = 0;
	UINT yCount = 0;
	UINT tileCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);
	fread(&tileCount, sizeof(UINT), 1, pFile);

	CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"Tile", L"texture\\Tile\\tilemap.bmp");
	
	for (UINT i = 0; i < tileCount; i++)
	{
		CTile* newTile = new CTile;
		newTile->Load(pFile);
		newTile->SetD2DImage(pImg);
		newTile->SetPos(Vec2((float)(newTile->GetX() * CTile::SIZE_TILE), (float)(newTile->GetY() * CTile::SIZE_TILE)));

		if (GROUP_TILE::NONE != newTile->GetGroup())
		{
			newTile->CreateCollider();
			newTile->GetCollider()->SetScale(Vec2(CTile::SIZE_TILE, CTile::SIZE_TILE));
			newTile->GetCollider()->SetOffsetPos(Vec2(CTile::SIZE_TILE / 2.f, CTile::SIZE_TILE / 2.f));
		}

		AddObject(newTile, GROUP_GAMEOBJ::TILE);
	}

	fclose(pFile);
}
