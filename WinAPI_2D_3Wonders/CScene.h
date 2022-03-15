#pragma once

class CGameObject;

class CScene
{
	vector<CGameObject*> m_arrObj[(UINT)GROUP_GAMEOBJ::SIZE];
	wstring m_strName;

	UINT m_iTileX;
	UINT m_iTileY;
public:
	virtual void update();
	virtual void finalupdate();
	virtual void render();

	void render_tile();
	void render_background();

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	void SetName(const wstring& strName) { m_strName = strName; }
	wstring GetName() { return m_strName; }

	UINT GetTileX() { return m_iTileX; }
	UINT GetTileY() { return m_iTileY; }

	void AddObject(CGameObject* pObj, GROUP_GAMEOBJ type);
	const vector<CGameObject*>& GetGroupObject(GROUP_GAMEOBJ group) { return m_arrObj[(UINT)group]; }
	void DeleteGroup(GROUP_GAMEOBJ group);
	void DeleteAll();

	void CreateTile(UINT xSize, UINT ySize);
	void LoadTile(const wstring& strPath);

	vector<CGameObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_GAMEOBJ::UI]; }

public:
	CScene();
	virtual ~CScene();
};

