#pragma once

class CGameObject;

class CScene
{
	vector<CGameObject*> m_arrObj[(UINT)GROUP_GAMEOBJ::SIZE];
	wstring m_strName;

public:
	virtual void update();
	virtual void finalupdate();
	virtual void render();

	void render_tile();

	virtual void Enter() = 0;
	virtual void Exit() = 0;

public:

	void SetName(const wstring& strName) { m_strName = strName; }
	wstring GetName() { return m_strName; }

	const vector<CGameObject*>& GetGroupObject(GROUP_GAMEOBJ group) { return m_arrObj[(UINT)group]; }
	vector<CGameObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_GAMEOBJ::UI]; }

public:

	void AddObject(CGameObject* pObj, GROUP_GAMEOBJ type);
	
	void DeleteGroup(GROUP_GAMEOBJ group);
	void DeleteAll();
	void LoadTile(const wstring& strPath);

public:
	CScene();
	virtual ~CScene();
};

